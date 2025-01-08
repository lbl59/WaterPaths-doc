//
// Created by bernardo on 1/26/17.
//

#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include "ContinuityModelROF.h"
#include "../Utils/Utils.h"

ContinuityModelROF::ContinuityModelROF(vector<WaterSource *> water_sources, const Graph &water_sources_graph,
                                       const vector<vector<int>> &water_sources_to_utilities,
                                       vector<Utility *> utilities,
                                       vector<MinEnvFlowControl *> min_env_flow_controls,
                                       vector<double>& utilities_rdm,
                                       vector<double>& water_sources_rdm, unsigned long total_weeks_simulation,
                                       const int use_precomputed_rof_tables, const unsigned long realization_id)
        : ContinuityModel(water_sources, utilities, min_env_flow_controls,
                          water_sources_graph, water_sources_to_utilities, utilities_rdm,
                          water_sources_rdm,
                          realization_id),
          n_topo_sources((int) sources_topological_order.size()),
          use_precomputed_rof_tables(use_precomputed_rof_tables) {
    // Update utilities' total available stored volume
    // Disable financial calculations for continuity model.
    for (Utility *u : this->continuity_utilities) {
        u->updateTotalAvailableVolume();
        u->setNoFinaicalCalculations();
    }

    for (int u = 0; u < n_utilities; ++u) {
        // Initialize storage and ROF-related structures for each utility
        ut_storage_to_rof_rof_realization.emplace_back(
                (unsigned long) ceil(WEEKS_IN_YEAR),
                (unsigned long) NO_OF_INSURANCE_STORAGE_TIERS + 1);
        // Prepare storage and delta capacity data if using pre-computed ROF tables
        if (use_precomputed_rof_tables != IMPORT_ROF_TABLES) {
            ut_storage_to_rof_table.emplace_back(
                    total_weeks_simulation,
                    (unsigned long) NO_OF_INSURANCE_STORAGE_TIERS + 1);
        }
    }

    // Identify water sources without downstream connections.
    storage_wout_downstream = new bool[sources_topological_order.size()];
    for (int ws : sources_topological_order)
        storage_wout_downstream[ws] = downstream_sources[ws] != NON_INITIALIZED;

    // Identify the next online downstream source for each water source.
    online_downstream_sources = getOnlineDownstreamSources();

    // Calculate utilities' base delta storage corresponding to one table
    // tier and status-quo base storage capacity.
    if (use_precomputed_rof_tables == IMPORT_ROF_TABLES) {
        // Calculate base storage capacities and delta storage capacities for utilities.
        for (int u = 0; u < n_utilities; ++u) {
            utility_base_storage_capacity.push_back(
                    continuity_utilities[u]->getTotal_storage_capacity() * BASE_STORAGE_CAPACITY_MULTIPLIER);
            utility_base_delta_capacity_table.push_back(
                    utility_base_storage_capacity[u] /
                    NO_OF_INSURANCE_STORAGE_TIERS);
        }

        // Initialize vectors for tracking storage capacity ratios, table shifts, and demand buffers.
        current_and_base_storage_capacity_ratio =
                vector<double>((unsigned long) n_utilities);
        current_storage_table_shift =
                vector<double>((unsigned long) n_utilities);
        current_base_storage_table_shift =
                vector<double>((unsigned long) n_utilities);
        current_demand_ratio_buffer =
                vector<double>((unsigned long) n_utilities);
    }
}

ContinuityModelROF::~ContinuityModelROF() {
    delete[] storage_wout_downstream;
}

vector<vector<double>> ContinuityModelROF::calculateLongTermROF(int week) {
    // vector where risks of failure will be stored.
    /// OCT 2019: VECTORS NOW 2-DIMENSIONAL TO HOLD ONE ROW FOR LT STORAGE ROF, ANOTHER FOR TREATMENT ROF
    int storage_row = 0;
    int treatment_row = 1;
    vector<vector<double>> risk_of_failure((unsigned long) n_utilities, vector<double>(2, 0.0));
    vector<vector<double>> year_failure((unsigned long) n_utilities, vector<double>(2, 0.0));

    // Checks if new infrastructure became available 
    // If yes, corresponding realization infrastructure online.
    updateOnlineInfrastructure(week);

    for (int u = 0; u < n_utilities; ++u)
        // Calculate the demand estimate projection based on the current week.
        continuity_utilities[u]->calculateDemandEstimateFromProjection(week, REPROJECT_DEMAND);

    // Perform a continuity simulation for NUMBER_REALIZATIONS_ROF (50) yearly realization.
    for (int yr = 0; yr < NUMBER_REALIZATIONS_ROF; ++yr) {
        // For each realization, reset the utilities' and reservoirs' storage to start fresh
        resetUtilitiesAndReservoirs(LONG_TERM_ROF);

        for (int w = 0; w < WEEKS_ROF_LONG_TERM; ++w) {
            // One week continuity time-step.
            // Update the utilities' storage and treatment capacities.
            continuityStep(w + week, yr, APPLY_DEMAND_BUFFER, APPLY_DEMAND_PROJECTION);

            // Check if the storage capacity or treatment capacity exceeds the failure thresholds.
            // Increase the number of failed years of that utility by 1 if exceeds failure threshold.
            /// OCT 2019: ADDED STATEMENT TO COLLECT TREATMENT CAPACITY FAILURES
            for (int u = 0; u < n_utilities; ++u) {
                if (continuity_utilities[u]->getStorageToCapacityRatio() <= STORAGE_CAPACITY_RATIO_FAIL)
                    year_failure[u][storage_row] = FAILURE;
                if (continuity_utilities[u]->getUnrestrictedDemandToTreatmentCapacityRatio() >= TREATMENT_CAPACITY_RATIO_FAIL)
                    year_failure[u][treatment_row] = FAILURE;
            }
        }

        // Count failures and reset failures counter.
        for (int uu = 0; uu < n_utilities; ++uu) {
            risk_of_failure[uu][storage_row] += year_failure[uu][storage_row];
            year_failure[uu][storage_row] = NON_FAILURE;

            risk_of_failure[uu][treatment_row] += year_failure[uu][treatment_row];
            year_failure[uu][treatment_row] = NON_FAILURE;
        }
    }

    // Average the failure counts for each utility across all ROF realizations (50)
    for (int i = 0; i < n_utilities; ++i) {
        risk_of_failure[i][storage_row] /= NUMBER_REALIZATIONS_ROF;
        risk_of_failure[i][treatment_row] /= NUMBER_REALIZATIONS_ROF;
    }

    return risk_of_failure;
}

vector<vector<double>> ContinuityModelROF::calculateShortTermROF(int week, int import_export_rof_tables) {
    vector<double> risk_of_failure;
    if (import_export_rof_tables == IMPORT_ROF_TABLES) {
        return ContinuityModelROF::calculateShortTermROFTable(week, realization_utilities,
                utility_base_storage_capacity, ut_storage_to_rof_table,
                current_storage_table_shift, current_base_storage_table_shift,
                current_demand_ratio_buffer);
    } else {
        return ContinuityModelROF::calculateShortTermROFFullCalcs(week);
    }
}

vector<vector<double>> ContinuityModelROF::calculateShortTermROFTable(int week, vector<Utility *> utilities,
        vector<double> utilities_base_storage_capacity, const vector<Matrix2D<double>> &ut_storage_to_rof_table,
        vector<double> current_storage_table_shift, vector<double> current_base_storage_table_shift,
        vector<double> current_demand_ratio_buffer) {
    
    auto n_utilities = utilities.size();
    int storage_row = 0;
    int treatment_row = 1;
    double m;

    // Initialize vector where risks of failure will be stored.
    vector<vector<double>> risk_of_failure((unsigned long) n_utilities, vector<double>(2, 0.0));

    // Checks if new infrastructure became available and, if so, set the
    // corresponding realization infrastructure online.
    updateOnlineInfrastructure(week);

    for (int u = 0; u < n_utilities; ++u) {
        // Retrieve the current storage volume for utility u and the corresponding ratio to base storage.
        double utility_storage =
                utilities[u]->getTotal_stored_volume();
        // Ratio of current and status-quo utility storage capacities
        //        double m = current_and_base_storage_capacity_ratio[u];
        // Note: THIS RATIO CAN BECOME ENORMOUS IF STORAGE IS GREATLY INCREASED BY INFRASTRUCTURE
        // OCT 2019: NOW INCLUDING BASE STORAGE SHIFT TO COUNTERACT THIS IN CERTAIN CASES
        // FILLED THE SAME WAY THE STORAGE TABLE SHIFT IS WITHIN THE PROBLEM/TRIANGLE.CPP FILE
        m = utilities[u]->getTotal_storage_capacity() /
                (utilities_base_storage_capacity[u] + current_base_storage_table_shift[u]);
        
        // Adjust the `storage_convert` value based on shifts and calculate the corresponding ROF base table tier from the ROF table.
        // This is done by shifting the table around based on new infrastructure:
        double storage_convert = utility_storage +
                                 STORAGE_CAPACITY_RATIO_FAIL *
                                 (utilities_base_storage_capacity[u] + current_base_storage_table_shift[u]) *
                                 (1. - m) + current_storage_table_shift[u];
        int tier = (int) (storage_convert * NO_OF_INSURANCE_STORAGE_TIERS /
                (utilities_base_storage_capacity[u] + current_base_storage_table_shift[u]));

        // If the table shifts or storage levels result in tier too large for the table, cap it to prevent Borg crash
        // (if past the top tier, ROF = 0 anyway)
        if (tier > NO_OF_INSURANCE_STORAGE_TIERS-2)
            tier = NO_OF_INSURANCE_STORAGE_TIERS-2;

        // Check the demand-to-treatment capacity ratio and flag a treatment failure if necessary.

        // NOTE: THIS IS NOT AN EXACT VERSION OF FULL ROF CALCULATION BUT A ROUGH APPROXIMATION OF CONDITIONS
        // BECAUSE STORAGE DOES NOT HAVE AN IMPACT ON TREATMENT CAPACITY OR UNRESTRICTED DEMAND
        // WE ASSUME THAT WHEN DEMAND GROWS TO A CERTAIN THRESHOLD FAILURE IS GUARANTEED
        // WITH RESPECT TO TREATMENT CAPACITY. 
        
        // This is meant to catch failures of un-met demand before infrastructure is made available or storage failures 
        // due to demand increases or hydrologic change appear.
        double demand_to_treatment_capacity_ratio = utilities[u]->getUnrestrictedDemandToTreatmentCapacityRatio();
        if (demand_to_treatment_capacity_ratio + current_demand_ratio_buffer[u] > TREATMENT_CAPACITY_RATIO_FAIL)
            risk_of_failure[u][treatment_row] = 1;

        // Retrieve the ROF for storage from the precomputed storage-to-ROF table.
        // Average the values for the current and next tiers.
        risk_of_failure[u][storage_row] = (ut_storage_to_rof_table[u](week, tier) +
                              ut_storage_to_rof_table[u](week, tier + 1)) / 2;
    }
    // Return the calculated risk of failure for each utility.
    return risk_of_failure;
}

vector<vector<double>> ContinuityModelROF::calculateShortTermROFFullCalcs(int week) {
    // Vector where risks of failure will be stored.
    // JAN 2020: VECTORS NOW 2-DIMENSIONAL TO HOLD ONE ROW FOR LT STORAGE ROF, ANOTHER FOR TREATMENT ROF
    int storage_row = 0;
    int treatment_row = 1;
    vector<vector<double>> risk_of_failure((unsigned long) n_utilities, vector<double>(2, 0.0));
    vector<vector<double>> year_failure((unsigned long) n_utilities, vector<double>(2, 0.0));
    double to_full[n_sources];

    // Empty volumes are later used to update ROF tables.
    calculateEmptyVolumes(realization_water_sources, to_full);

    int week_of_the_year = Utils::weekOfTheYear(week);

    // Check if new infrastructure is available and update online infrastructure.
    // If so, set the corresponding realization infrastructure status as online.
    updateOnlineInfrastructure(week);

    // Perform a continuity simulation for NUMBER_REALIZATIONS_ROF (50) yearly realization.
    for (int yr = 0; yr < NUMBER_REALIZATIONS_ROF; ++yr) {
        // Reset tables and storage values.
        for (auto &t : ut_storage_to_rof_rof_realization)
            t.reset(NON_FAILURE);

        // Reset current reservoirs' and utilities' storage and combined
        // storage, respectively, in the corresponding realization simulation.
        resetUtilitiesAndReservoirs(SHORT_TERM_ROF);

        for (int w = 0; w < WEEKS_ROF_SHORT_TERM; ++w) {
            // Perform a continuity step and check for failures
            continuityStep(w + week, yr, !APPLY_DEMAND_BUFFER);

            // Check total available storage for each utility .
            // If the storage-to-capacity ratio is below a threshold, count a storage failure.
            // If the demand-to-treatment capacity ratio is too high, count a treatment failure.
            for (int u = 0; u < n_utilities; ++u) {
                if (continuity_utilities[u]->getStorageToCapacityRatio() <= STORAGE_CAPACITY_RATIO_FAIL)
                    year_failure[u][storage_row] = FAILURE;
                if (continuity_utilities[u]->getUnrestrictedDemandToTreatmentCapacityRatio() >= TREATMENT_CAPACITY_RATIO_FAIL)
                    year_failure[u][treatment_row] = FAILURE;
            }

            // Update the storage-to-ROF table and record the results of the realization.
            updateStorageToROFTable(INSURANCE_SHIFT_STORAGE_CURVES_THRESHOLD,
                                    week_of_the_year, to_full);
        }

        // Record ROF realization results into final ROF table for that week.
        recordROFStorageTable(ut_storage_to_rof_rof_realization, ut_storage_to_rof_table,
                              n_utilities, week, week_of_the_year);

        // Count failures and reset failures counter.
        for (int uu = 0; uu < n_utilities; ++uu) {
            risk_of_failure[uu][storage_row] += year_failure[uu][storage_row];
            year_failure[uu][storage_row] = NON_FAILURE;

            risk_of_failure[uu][treatment_row] += year_failure[uu][treatment_row];
            year_failure[uu][treatment_row] = NON_FAILURE;
        }
    }

    setInitialTableTier(week, n_utilities, ut_storage_to_rof_table, beginning_tier);

    // Calculate the final risk of failure for each utility by averaging over all realizations.
    // Check for any invalid values (NaN) in the ROF calculations and throw an error if found.
    for (int u = 0; u < n_utilities; ++u) {
        risk_of_failure[u][storage_row] /= NUMBER_REALIZATIONS_ROF;
        risk_of_failure[u][treatment_row] /= NUMBER_REALIZATIONS_ROF;
        if (std::isnan(risk_of_failure[u][storage_row])) {
            string error_m = "nan rof imported tables with storage ROF. Realization " +
                             to_string(realization_id) + ", week " +
                             to_string(week) + ", utility " + to_string(u);
            printf("%s", error_m.c_str());
            throw_with_nested(logic_error(error_m.c_str()));
        }
        if (std::isnan(risk_of_failure[u][treatment_row])) {
            string error_m = "nan rof imported tables with treatment ROF. Realization " +
                             to_string(realization_id) + ", week " +
                             to_string(week) + ", utility " + to_string(u);
            printf("%s", error_m.c_str());
            throw_with_nested(logic_error(error_m.c_str()));
        }
    }

    return risk_of_failure;
}

void ContinuityModelROF::updateStorageToROFTable(
        double storage_percent_decrement, int week_of_the_year,
        const double *to_full) {
    double available_volumes[n_sources];
    for (int ws = 0; ws < n_sources; ++ws) {
        available_volumes[ws] =
                continuity_water_sources[ws]->getAvailableSupplyVolume();
    }

    // Loop over the storage tiers starting from one level above where failure was last observed.
    // This saves a lot of computational time.
    for (int s = beginning_tier; s <= NO_OF_INSURANCE_STORAGE_TIERS; ++s) {
        // Calculate the delta storage and shift the storage levels accordingly
        // Identify array that will receive the shifted storage curves.
        double percent_decrement_storage_level =
                (double) s * storage_percent_decrement;
        double delta_storage[n_sources];
        double available_volumes_shifted[n_sources];
        memcpy(available_volumes_shifted, available_volumes, sizeof(double) * n_sources);

        // Compare the simulated volume of available combined stored water and
        // the one for the table calculated above based on the percent decrement.
        for (int wss = 0; wss < n_sources; ++wss) {
            delta_storage[wss] = to_full[wss] - water_sources_capacities[wss] *
                                                percent_decrement_storage_level;
        }

        // Shift storages.
        shiftStorages(available_volumes_shifted, delta_storage);

        // Checks for utilities failures.
        int count_fails = 0;
        for (int u = 0; u < n_utilities; ++u) {
            double utility_storage = 0;
            // Calculate combined stored volume for each utility based on
            // shifted storages.
            for (int ws : water_sources_online_to_utilities[u])
                utility_storage += available_volumes_shifted[ws] *
                                   continuity_water_sources[ws]->getSupplyAllocatedFraction(u) *
                                   (realization_water_sources[ws]->getAllocatedTreatmentCapacity(u) > 0
                                    && realization_water_sources[ws]->isOnline());
            // Register failures in the storage-to-ROF table if a utility's storage falls below the failure threshold.
            if (utility_storage / utilities_capacities[u] <
                STORAGE_CAPACITY_RATIO_FAIL) {
                ut_storage_to_rof_rof_realization[u](week_of_the_year,
                                                     NO_OF_INSURANCE_STORAGE_TIERS - s) = FAILURE;
                count_fails++;
            }
        }

        // If all utilities fail at a particular storage level, stop further decrementing and label all lower levels as failures.
        if (count_fails == n_utilities) {
            for (int ss = s; ss <= NO_OF_INSURANCE_STORAGE_TIERS; ++ss) {
                for (unsigned long u = 0; u < (unsigned long) n_utilities; ++u) {
                    ut_storage_to_rof_rof_realization[u](week_of_the_year,
                                                         NO_OF_INSURANCE_STORAGE_TIERS - ss)
                            = FAILURE;
                }
            }
            break;
        }
    }
}

//FIXME: MAKE THIS MORE EFFICIENT. THIS METHOD IS THE MOST EXPENSIVE ONE IN THE CODE.
void ContinuityModelROF::shiftStorages(
        double * available_volumes_shifted,
        const double * delta_storage) {
    // Add deltas to all sources following the topological order, so that
    // upstream is calculated before downstream.
    for (int ws : sources_topological_order) {
        // Calculate initial estimate for shifted total available volume for the current source
        available_volumes_shifted[ws] += delta_storage[ws];
        // Calculate the available volume to full for the current source.
        double available_volume_to_full = water_sources_capacities[ws] -
                                          available_volumes_shifted[ws];

        // If the source is not full, calculate the spillage to downstream sources.
        if (available_volume_to_full > 0) {
            // Calculate spilled water. Since the curves are shifted as the
            // weeks of the rof realizations are calculated, the minimum
            // environmental outflows below will be the ones at the time when
            // the storage is being shifted.
            double spillage =
                    continuity_water_sources[ws]->getTotal_outflow() -
                    continuity_water_sources[ws]
                            ->getMin_environmental_outflow();

            double spillage_retrieved = min(available_volume_to_full, spillage);

            available_volumes_shifted[ws] += spillage_retrieved;

            if (online_downstream_sources[ws] > 0)
                available_volumes_shifted[online_downstream_sources[ws]] -=
                        spillage_retrieved;
        } else {
            // If the source is full, pass the negative spillage to downstream sources.
            double spillage = -available_volume_to_full;
            if (online_downstream_sources[ws] > 0) {
                available_volumes_shifted[ws] -= spillage;
                available_volumes_shifted[online_downstream_sources[ws]] +=
                        spillage;
            }
        }
    }
}

void ContinuityModelROF::printROFTable(const string &folder) {
    for (int u = 0; u < n_utilities; ++u) {

        string file_name = folder + "tables_r" + to_string(realization_id) + "_u" + to_string(u) + ".csv";
        ofstream output_file(file_name);

        auto num_weeks = ut_storage_to_rof_table[u].get_i();
        for (int w = 0; w < num_weeks; ++w) {
            auto data = ut_storage_to_rof_table[u].getPointerToElement(w, 0);
	    std::ostringstream week_table;
	    week_table << std::fixed;
	    week_table << std::setprecision(2);
            for (int t = 0; t < NO_OF_INSURANCE_STORAGE_TIERS; ++t) {
                week_table << to_string(data[t]) + ",";
            }

            string line = week_table.str();
	    line.pop_back();
            output_file << line;
            if (w < num_weeks - 1)
                output_file << endl;
        }

        output_file.close();
    }
}

void ContinuityModelROF::resetUtilitiesAndReservoirs(int rof_type) {
    // update water sources info. If short-term rof, return to current
    // storage; if long-term, make them full.
    if (rof_type == SHORT_TERM_ROF)
        // Update each water source's available allocated volumes and outflows to the current week’s values.
        for (int i = 0; i < n_sources; ++i) {   // Current available volume
            continuity_water_sources[i]->setAvailableAllocatedVolumes
                    (realization_water_sources[i]
                             ->getAvailable_allocated_volumes(),
                     realization_water_sources[i]->getAvailableVolume());
            continuity_water_sources[i]->setOutflow_previous_week(
                    realization_water_sources[i]->getTotal_outflow());
        }
    else
        // If long-term ROF, set all water sources to full capacity.
        for (int i = 0; i < n_sources; ++i) {   // Full capacity
            continuity_water_sources[i]->setFull();
            continuity_water_sources[i]->setOutflow_previous_week(
                    realization_water_sources[i]->getTotal_outflow());
        }

    // Update utilities combined storage to reflect the changes in the reservoirs and water sources.
    for (Utility *u : continuity_utilities) {
        u->updateTotalAvailableVolume();
    }
}

void ContinuityModelROF::connectRealizationWaterSources(
        const vector<WaterSource *> &realization_water_sources) {
    ContinuityModelROF::realization_water_sources =
            realization_water_sources;
}

void ContinuityModelROF::connectRealizationUtilities(
        const vector<Utility *> &realization_utilities) {
    ContinuityModelROF::realization_utilities = realization_utilities;
}

void ContinuityModelROF::updateOnlineInfrastructure(int week) {
    for (unsigned long ws = 0; ws < (unsigned long) n_sources; ++ws) {
        // check if any infrastructure has become online in the realization model but not in the ROF model.
        if (realization_water_sources.at(ws)->isOnline() &&
            !continuity_water_sources.at(ws)->isOnline()) {
            // If so, set it online in the ROF calculation model.
            for (int uu : utilities_to_water_sources[ws]) {
                auto u = (unsigned long) uu;
                water_sources_online_to_utilities.at(u).push_back((int) ws);
                // Update the utilities connected to the water source.
                continuity_utilities.at(u)
                        ->setWaterSourceOnline((int) ws, week);
                if (use_precomputed_rof_tables == IMPORT_ROF_TABLES) {
                    current_storage_table_shift.at(u) += table_storage_shift.at(u)
                            .at(ws);
                    current_base_storage_table_shift.at(u) += table_base_storage_shift.at(u)
                            .at(ws);
                    current_demand_ratio_buffer.at(u) += treatment_demand_buffer_shift.at(u).at(ws);
                }
            }

            // Update water source capacities in case of any expansion.
            water_sources_capacities.at(ws) =
                    continuity_water_sources.at(ws)->getSupplyCapacity();
        }
    }

    // If it's the first week of the year or week 0, 
    // update utilities' storage capacities and their ratios to status-quo capacities
    // in case new infrastructure has been built.
    if (Utils::isFirstWeekOfTheYear(week) || week == 0) {
        for (unsigned long u = 0; u < (unsigned long) n_utilities; ++u) {
            utilities_capacities.at(u) =
                    continuity_utilities.at(u)->getTotal_storage_capacity();
        }

        if (use_precomputed_rof_tables == IMPORT_ROF_TABLES) {
            for (unsigned long u = 0; u < (unsigned long) n_utilities; ++u) {
                current_and_base_storage_capacity_ratio.at(u) =
                        utilities_capacities.at(u) /
                        utility_base_storage_capacity.at(u);
            }
        }
    }

    // Update list of downstream sources of each source.
    online_downstream_sources = getOnlineDownstreamSources();
}

void ContinuityModelROF::setROFTablesAndShifts(
        const vector<Matrix2D<double>> &storage_to_rof_table,
        const vector<vector<double>> &table_storage_shift,
        const vector<vector<double>> &table_base_storage_shift,
        const vector<vector<double>> &treatment_demand_buffer_shift) {
    this->ut_storage_to_rof_table = storage_to_rof_table;
    this->table_storage_shift = table_storage_shift;
    this->table_base_storage_shift = table_base_storage_shift;
    this->treatment_demand_buffer_shift = treatment_demand_buffer_shift;
}

void ContinuityModelROF::updateJointWTPTreatmentAllocations(const vector<WaterSource *> &non_rof_water_sources) {
    for (WaterSource *ws : non_rof_water_sources) {
        if (continuity_water_sources.at(ws->id)->id != ws->id)
            cout << "Error in ContinuityModelROF::updateJointWTPTreatmentAllocations, mismatch of source IDs.";

        if (ws->source_type == NEW_JOINT_WATER_TREATMENT_PLANT && ws->isOnline()) {
            // Update the treatment allocations in both continuity and realization water sources
            continuity_water_sources.at(ws->id)->setTreatmentAllocations(ws->getAllocatedTreatmentCapacities());
            realization_water_sources.at(ws->id)->setTreatmentAllocations(ws->getAllocatedTreatmentCapacities());
        }
    }
}

void ContinuityModelROF::updateUtilityTreatmentAllocations(const vector<Utility *> &non_rof_utilities) {
    for (Utility *u : non_rof_utilities) {
        if (continuity_utilities.at(u->id)->id != u->id)
            cout << "Error in ContinuityModelROF::updateUtilityTreatmentAllocations, mismatch of utility IDs.";
        
        // Update the treatment capacity in both continuity and realization utilities.
        continuity_utilities.at(u->id)->setTreatmentCapacity(u->getTotal_treatment_capacity());
        realization_utilities.at(u->id)->setTreatmentCapacity(u->getTotal_treatment_capacity());
    }
}

void ContinuityModelROF::tableROFExceptionHandler(double m, int u, int week) {
    string error;
    if (m > 1.) {
        error = "ROF tables being extrapolated because current "
                "capacity is greater than table base capacity."
                " Utility " + to_string(u) + ", m=" +
                to_string(m) + ". You should try regenerating "
                               "tables with a higher value for constant "
                               "BASE_STORAGE_CAPACITY_MULTIPLIER  and higher"
                               " number of table tiers";
        throw_with_nested(logic_error(error.c_str()));
    } else {
        error = "Exception happened in week " + to_string(week) +
                " for utility " + to_string(u) + "\n";
        throw_with_nested(runtime_error(error.c_str()));
    }
}

vector<Matrix2D<double>> &ContinuityModelROF::getUt_storage_to_rof_table() {
    return ut_storage_to_rof_table;
}

void ContinuityModelROF::setInitialTableTier(int week, const int &n_utilities,
                                             vector<Matrix2D<double>> &ut_storage_to_rof_table, int &beginning_tier) {
    // Set first tier for ROF table calculation close to where the first failure was observed for last week's table
    // to save computational time
    for (int s = 0; s < NO_OF_INSURANCE_STORAGE_TIERS; ++s) {
        int count_failures = 0;
        for (int u = 0; u < n_utilities; ++u) {
            if (ut_storage_to_rof_table[u](week, NO_OF_INSURANCE_STORAGE_TIERS - s) > 0.) {
                ++count_failures;
            }
        }
        if (count_failures == 0)
            beginning_tier = max(0, s - 1);
        else
            break;
    }
}

void ContinuityModelROF::recordROFStorageTable(vector<Matrix2D<double>> &ut_storage_to_rof_rof_realization,
                                               vector<Matrix2D<double>> &ut_storage_to_rof_table,
                                               const int &n_utilities, int &week, int &week_of_the_year) {
    // Records failure results for one ROF realization in overall ROF table for that week.
    // Does this for each utility.
    for (int u = 0; u < n_utilities; ++u) {
        double *rof_data = (ut_storage_to_rof_rof_realization[u] / NUMBER_REALIZATIONS_ROF).
                getPointerToElement(week_of_the_year, 0);
        ut_storage_to_rof_table[u].add_to_position(
                week, 0, rof_data, NO_OF_INSURANCE_STORAGE_TIERS
        );
    }
}

void ContinuityModelROF::calculateEmptyVolumes(vector<WaterSource *> &realization_water_sources, double *to_full) {
    // Calculate empty volume in storage-based water sources. This information is later used for updating the ROF tables.
    for (int ws = 0; ws < n_sources; ++ws) {
        if (realization_water_sources[ws]->isOnline()) {
            to_full[ws] = realization_water_sources[ws]->getSupplyCapacity() -
                          realization_water_sources[ws]->getAvailableSupplyVolume();
        } else {
            to_full[ws] = 0;
        }
    }
}
