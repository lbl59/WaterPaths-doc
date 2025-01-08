//
// Created by bernardo on 1/13/17.
//

#include <iostream>
#include <numeric>
#include <algorithm>
#include "Utility.h"
#include "../WaterSources/ReservoirExpansion.h"
#include "../WaterSources/SequentialJointTreatmentExpansion.h"
#include "../WaterSources/Relocation.h"
#include "../../Utils/Utils.h"
#include "InfrastructureManager.h"

/**
 * Steps: 
 * 1. Initialize member variables, including total storage capacity, available volume, and parameters for financial 
 *    and demand projection models.
 * 2. Assign provided demand and pricing data to corresponding member variables.
 * 3. Invoke the `calculateWeeklyAverageWaterPrices` method to compute weekly average water prices using 
 *    `typesMonthlyDemandFraction` and `typesMonthlyWaterPrice`.
 */

Utility::Utility(
        const char *name, int id,
        vector<vector<double>>& demands_all_realizations,
        vector<double>& annual_demand_projections,
        int number_of_week_demands,
        const double percent_contingency_fund_contribution,
        const double contingency_fund_cap,
        const vector<vector<double>> &typesMonthlyDemandFraction,
        const vector<vector<double>> &typesMonthlyWaterPrice,
        WwtpDischargeRule wwtp_discharge_rule,
        double demand_buffer,
        int demand_projection_forecast_length,
        int demand_projection_historical_period_to_use,
        int demand_projection_reprojection_frequency) :
        total_storage_capacity(NONE),
        total_available_volume(NONE),
        wwtp_discharge_rule(wwtp_discharge_rule),
        demands_all_realizations(demands_all_realizations),
        annual_demand_projections(annual_demand_projections),
        infra_discount_rate(NON_INITIALIZED),
        bond_term_multiplier(NON_INITIALIZED),
        bond_interest_rate_multiplier(NON_INITIALIZED),
        id(id),
        number_of_week_demands(number_of_week_demands),
        name(name),
        percent_contingency_fund_contribution(percent_contingency_fund_contribution),
        contingency_fund_cap(contingency_fund_cap),
        demand_projection_forecast_length(demand_projection_forecast_length),
        demand_projection_historical_period_to_use(demand_projection_historical_period_to_use),
        demand_projection_reprojection_frequency(demand_projection_reprojection_frequency),
        demand_buffer(demand_buffer) {
    calculateWeeklyAverageWaterPrices(typesMonthlyDemandFraction,
                                      typesMonthlyWaterPrice);
}

/**
 * Steps: 
 * 1. Initialize member variables for storage capacity, available volume, demands, pricing, and financial properties.
 * 2. Construct an InfrastructureManager instance to handle infrastructure construction processes, passing in the 
 *      necessary parameters such as construction triggers, discount rates, and construction order vectors.
 * 3. Connect water sources to the infrastructure manager for synchronization.
 * 4. Validate input parameters (construction order vectors and discount rates).
 * 5. Compute weekly average water prices using the `calculateWeeklyAverageWaterPrices` method.
 */
Utility::Utility(const char *name, int id, vector<vector<double>>& demands_all_realizations, vector<double>& annual_demand_projections,
                 int number_of_week_demands, const double percent_contingency_fund_contribution,
                 const double contingency_fund_cap,
                 const vector<vector<double>> &typesMonthlyDemandFraction,
                 const vector<vector<double>> &typesMonthlyWaterPrice,
                 WwtpDischargeRule wwtp_discharge_rule,
                 double demand_buffer, const vector<int> &rof_infra_construction_order,
                 const vector<int> &demand_infra_construction_order,
                 const vector<double> &infra_construction_triggers, double infra_discount_rate,
                 const vector<vector<int>>& infra_if_built_remove, double bond_term, double bond_interest_rate,
                 int demand_projection_forecast_length,
                 int demand_projection_historical_period_to_use,
                 int demand_projection_reprojection_frequency) :
        total_storage_capacity(NONE),
        total_available_volume(NONE),
        wwtp_discharge_rule(wwtp_discharge_rule),
        demands_all_realizations(demands_all_realizations),
        annual_demand_projections(annual_demand_projections),
        infra_discount_rate(infra_discount_rate),
        bond_term_multiplier(bond_term),
        bond_interest_rate_multiplier(bond_interest_rate),
        id(id),
        number_of_week_demands(number_of_week_demands),
        name(name),
        percent_contingency_fund_contribution(percent_contingency_fund_contribution),
        contingency_fund_cap(contingency_fund_cap),
        demand_projection_forecast_length(demand_projection_forecast_length),
        demand_projection_historical_period_to_use(demand_projection_historical_period_to_use),
        demand_projection_reprojection_frequency(demand_projection_reprojection_frequency),
        demand_buffer(demand_buffer) {
    infrastructure_construction_manager =
            InfrastructureManager(id, infra_construction_triggers, infra_if_built_remove,
                                  infra_discount_rate, bond_term, bond_interest_rate,
                                  rof_infra_construction_order, demand_infra_construction_order);

    infrastructure_construction_manager.connectWaterSourcesVectorsToUtilitys(water_sources,
                                                                             priority_draw_water_source,
                                                                             non_priority_draw_water_source);

    if (rof_infra_construction_order.empty() &&
        demand_infra_construction_order.empty())
        throw std::invalid_argument("At least one infrastructure construction "
                                            "order vector  must have at least "
                                            "one water source ID. If there's "
                                            "not infrastructure to be build, "
                                            "use other constructor "
                                            "instead.");
    if (infra_discount_rate <= 0)
        throw std::invalid_argument("Infrastructure discount rate must be "
                                            "greater than 0.");

    calculateWeeklyAverageWaterPrices(typesMonthlyDemandFraction,
                                      typesMonthlyWaterPrice);
}


/**
 * Steps:
 * 1. Initializes member variables related to water storage, demands, and pricing.
 * 2. Creates an InfrastructureManager instance for managing infrastructure construction projects.
 * 3. Links the infrastructure manager to the Utility's water source vectors.
 * 4. Validate input parameters (construction order vectors, discount rates, demand data).
 * 5. Computes weekly average water prices using `calculateWeeklyAverageWaterPrices`.
 */
Utility::Utility(const char *name, int id, vector<vector<double>>& demands_all_realizations, vector<double>& annual_demand_projections,
                 int number_of_week_demands, const double percent_contingency_fund_contribution,
                 const double contingency_fund_cap,
                 const vector<vector<double>> &typesMonthlyDemandFraction,
                 const vector<vector<double>> &typesMonthlyWaterPrice, WwtpDischargeRule wwtp_discharge_rule,
                 double demand_buffer, const vector<int> &rof_infra_construction_order,
                 const vector<int> &demand_infra_construction_order,
                 const vector<double> &infra_construction_triggers, double infra_discount_rate, double bond_term,
                 double bond_interest_rate,
                 int demand_projection_forecast_length,
                 int demand_projection_historical_period_to_use,
                 int demand_projection_reprojection_frequency) :
        total_storage_capacity(NONE),
        total_available_volume(NONE),
        wwtp_discharge_rule(wwtp_discharge_rule),
        demands_all_realizations(demands_all_realizations),
        annual_demand_projections(annual_demand_projections),
        infra_discount_rate(infra_discount_rate),
        bond_term_multiplier(bond_term),
        bond_interest_rate_multiplier(bond_interest_rate),
        id(id),
        number_of_week_demands(number_of_week_demands),
        name(name),
        percent_contingency_fund_contribution(percent_contingency_fund_contribution),
        contingency_fund_cap(contingency_fund_cap),
        demand_projection_forecast_length(demand_projection_forecast_length),
        demand_projection_historical_period_to_use(demand_projection_historical_period_to_use),
        demand_projection_reprojection_frequency(demand_projection_reprojection_frequency),
        demand_buffer(demand_buffer) {
    infrastructure_construction_manager = InfrastructureManager(id, infra_construction_triggers,
                                                                vector<vector<int>>(), infra_discount_rate,
                                                                bond_term, bond_interest_rate,
                                                                rof_infra_construction_order,
                                                                demand_infra_construction_order);

    infrastructure_construction_manager.connectWaterSourcesVectorsToUtilitys(water_sources,
                                                                             priority_draw_water_source,
                                                                             non_priority_draw_water_source);

    if (rof_infra_construction_order.empty() &&
            demand_infra_construction_order.empty())
        throw std::invalid_argument("At least one infrastructure construction "
                                            "order vector must have at least "
                                            "one water source ID. If there's "
                                            "not infrastructure to be build, "
                                            "use other constructor "
                                            "instead.");
    if (infra_discount_rate <= 0)
        throw std::invalid_argument("Infrastructure discount rate must be "
                                            "greater than 0.");

    if (demands_all_realizations.empty()) {
        char error[256];
        sprintf(error, "Empty demand vectors passed to utility %d", id);
        throw std::invalid_argument(error);
    }

    calculateWeeklyAverageWaterPrices(typesMonthlyDemandFraction,
                                      typesMonthlyWaterPrice);
}

Utility::Utility(Utility &utility) :
        weekly_average_volumetric_price(utility.weekly_average_volumetric_price),
        total_storage_capacity(utility.total_storage_capacity),
        total_available_volume(utility.total_available_volume),
        wwtp_discharge_rule(utility.wwtp_discharge_rule),
        demands_all_realizations(utility.demands_all_realizations),
        annual_demand_projections(utility.annual_demand_projections),
        demand_series_realization(utility.demand_series_realization),
        infra_discount_rate(utility.infra_discount_rate),
        bond_term_multiplier(utility.bond_term_multiplier),
        bond_interest_rate_multiplier(utility.bond_interest_rate_multiplier),
        demand_projection_forecast_length(utility.demand_projection_forecast_length),
        demand_projection_historical_period_to_use(utility.demand_projection_historical_period_to_use),
        demand_projection_reprojection_frequency(utility.demand_projection_reprojection_frequency),
        id(utility.id),
        number_of_week_demands(utility.number_of_week_demands),
        name(utility.name),
        percent_contingency_fund_contribution(utility.percent_contingency_fund_contribution),
        contingency_fund_cap(utility.contingency_fund_cap),
        demand_buffer(utility.demand_buffer),
        infrastructure_construction_manager(utility.infrastructure_construction_manager) {
    infrastructure_construction_manager.connectWaterSourcesVectorsToUtilitys(water_sources,
                                                                             priority_draw_water_source,
                                                                             non_priority_draw_water_source);

    // Create copies of sources
    water_sources.clear();
}

Utility::~Utility() {
    water_sources.clear();
}

Utility &Utility::operator=(const Utility &utility) {
    // Reinitializes the `demand_series_realization` vector with the appropriate size 
    // based on the number of weeks in the source `Utility` object.
    demand_series_realization = vector<double>((unsigned long) utility.number_of_week_demands);

    //Connects the `infrastructure_construction_manager` with the current utility's 
    // water sources, ensuring proper linkage of water source vectors.
    infrastructure_construction_manager.connectWaterSourcesVectorsToUtilitys(water_sources,
                                                                             priority_draw_water_source,
                                                                             non_priority_draw_water_source);

    // Clears the current `water_sources` vector to prevent resource conflicts and 
    // prepares it for a fresh copy.
    water_sources.clear();

    return *this;
}

bool Utility::operator<(const Utility *other) {
    return id < other->id;
}

bool Utility::operator>(const Utility *other) {
    return id > other->id;
}

bool Utility::compById(Utility *a, Utility *b) {
    return a->id < b->id;
}

void Utility::calculateWeeklyAverageWaterPrices(
        const vector<vector<double>> &typesMonthlyDemandFraction,
        const vector<vector<double>> &typesMonthlyWaterPrice) {
    // Check the validity of the input data
    priceCalculationErrorChecking(typesMonthlyDemandFraction,
                                  typesMonthlyWaterPrice);

    weekly_average_volumetric_price = vector<double>((int) WEEKS_IN_YEAR + 1, 0.);
    double monthly_average_price[NUMBER_OF_MONTHS] = {};
    int n_tiers = static_cast<int>(typesMonthlyWaterPrice.at(0).size());

    // Calculate the monthly average prices by summing the contributions from each consumer tier.
    for (int m = 0; m < NUMBER_OF_MONTHS; ++m)
        for (int t = 0; t < n_tiers; ++t)
            monthly_average_price[m] += typesMonthlyDemandFraction[m][t] *
                                        typesMonthlyWaterPrice[m][t];

    //Convert the monthly average price into weekly prices by dividing the month’s price 
    // equally across the weeks.
    for (int w = 0; w < (int) (WEEKS_IN_YEAR + 1); ++w)
        weekly_average_volumetric_price[w] =
                monthly_average_price[(int) (w / WEEKS_IN_MONTH)] / 1e6;
}

void Utility::priceCalculationErrorChecking(
        const vector<vector<double>> &typesMonthlyDemandFraction,
        const vector<vector<double>> &typesMonthlyWaterPrice) {

    if (typesMonthlyDemandFraction.size() != NUMBER_OF_MONTHS)
        throw invalid_argument("There must be 12 total_demand fractions per tier.");
    if (typesMonthlyWaterPrice.size() != NUMBER_OF_MONTHS)
        throw invalid_argument("There must be 12 water prices per tier.");
    if ((&typesMonthlyWaterPrice)[0].size() !=
        (&typesMonthlyDemandFraction)[0].size())
        throw invalid_argument("There must be Demand fractions and water "
                                         "prices for the same number of tiers.");
}

void Utility::updateTotalAvailableVolume() {
    // Initialize local variables
    total_available_volume = 0.0;
    total_stored_volume = 0.0;
    net_stream_inflow = 0.0;

    // For each priority water source,
    for (int ws : priority_draw_water_source) {
        // Oct 2019: available_volume should reflect water present in a given time step for allocating demand. 
        // For intakes and reuse, this should be equal to treatment capacity and/or inflow for the next week
        
        // For each water source, add the available allocated volume to `total_available_volume`.
        total_available_volume +=
                max(1.0e-6,
                    water_sources[ws]->getAvailableAllocatedVolume(id));
	    // Add the allocated inflow to `net_stream_inflow`.
        net_stream_inflow += water_sources[ws]->getAllocatedInflow(id);
	    // Oct 2019: this call operates the same as getAvailableAllocatedVolume except for Intakes and Reuse
	    // For Intakes it returns the volume after week's demands processed
	    // For Reuse it returns zero
	    total_stored_volume += max(1.0e-6,
                                   water_sources[ws]->getPrioritySourcePotentialVolume(id));
    }

    // For each non-priority water source,
    for (int ws : non_priority_draw_water_source) {
        // For each water source, add the available allocated volume and allocated inflow to `total_available_volume` and `net_stream_inflow`.
        // Then add the stored volume to `total_stored_volume`.
        double stored_volume = max(1.0e-6,
                                   water_sources[ws]->getAvailableAllocatedVolume(id));
        total_available_volume += stored_volume;
        total_stored_volume += stored_volume;
	    net_stream_inflow += water_sources[ws]->getAllocatedInflow(id);
    }
}

void Utility::clearWaterSources() {
    water_sources.clear();
}

void Utility::addWaterSource(WaterSource *water_source) {
    // Check for errors before adding the source.
    checkErrorsAddWaterSourceOnline(water_source);

    // Add the water source to the `water_sources` vector at the appropriate index.
    if (water_source->id > (int) water_sources.size() - 1) {
        water_sources.resize((unsigned int) water_source->id + 1);
    }

    // Add water source
    water_sources[water_source->id] = water_source;

    // Add the water source to the infrastructure construction manager
    infrastructure_construction_manager.addWaterSource(water_source);

    // f the water source is online and has certain conditions (e.g., positive treatment capacity or intake type),
    // add it to the online list in the construction manager using `addWaterSourceToOnlineLists()`.
    
    // MAR 2020: removed need to have positive allocated treatment capacity for a source
    //           to be made online (now there are sources with variable capacities where
    //           they can start at zero and change) IF THAT SOURCE IS AN INTAKE
    if (water_source->isOnline() && (
            water_source->getAllocatedTreatmentCapacity(id) > 0 ||
            water_source->source_type == INTAKE ||
            water_source->source_type == ALLOCATED_INTAKE)) {
        infrastructure_construction_manager.addWaterSourceToOnlineLists(
                water_source->id, total_storage_capacity,
                total_treatment_capacity, total_available_volume,
                total_stored_volume);
    }
    // Increment the count of sources
    n_sources++;
    // update the total capacity metrics (`max_capacity`)
    max_capacity += water_source->getAllocatedCapacity(id);
}

void Utility::checkErrorsAddWaterSourceOnline(WaterSource *water_source) {
    for (WaterSource *ws : water_sources) {
        // Check for duplicate water sources.
        if ((ws != nullptr) && ws->id == water_source->id) {
            cout << "Water source ID: " << water_source->id << endl <<
                 "Utility ID: " << id << endl;
            throw invalid_argument("Attempt to add water source with "
                                     "duplicate ID to utility.");
        }
    }
}

void Utility::calculateDemandEstimateFromProjection(int week, bool reproject_demand) {
    // Record year's actual average demand for re-projection in this or later years
    int year = round(week/WEEKS_IN_YEAR_ROUND);
    current_year_recorded_demand = annual_average_weekly_demand[year];

    // Estimate demand in future year for LTROF calculation
    // Check if the forecast length is within bounds of the available projection data.
    if (year + demand_projection_forecast_length >= annual_demand_projections.size()) {
        cout << "ERROR in Utility::calculateDemandEstimateFromProjection, Utility: "
             << name << ", Year: " << year << ", Forecast Length: "
             << demand_projection_forecast_length << ", Projection Vector Length: "
             << annual_demand_projections.size() << endl;
        __throw_logic_error("Error in Utility::calculateDemandEstimateFromProjection: "
                            "input data of annual demand projections is too short for chosen "
                            "demand projection forecast length in re-projection.");
    }

    // Set final demand projection estimate for the ltROF calculation
    // If at least 5 years since start of realization, re-project demand by determining annual average growth rate 
    //      of last five years
    // In the future, the look-ahead period for projection does not have to be the same
    //      as the past period used to calculate a new growth rate projection (both set to 5 now)
    double average_growth_rate = 0;

    // If conditions are met, calculate the average annual growth rate based on historical demand data.
    if (year >= demand_projection_historical_period_to_use && reproject_demand && (year % demand_projection_reprojection_frequency == 0)) {
        // Calculate average annual growth rate over recent past
        average_growth_rate =
                (annual_average_weekly_demand[year] - annual_average_weekly_demand[year-demand_projection_historical_period_to_use]) /
                        demand_projection_historical_period_to_use;

        future_demand_estimate = current_year_recorded_demand + (average_growth_rate * demand_projection_forecast_length);

        // Update future demand estimates by applying the growth rate to the current demand.
        // Overwrite annual demand projections for future years to use reprojected demands until next reprojection occurs
        int i = 0;
        for (int yr = year; yr <= year + demand_projection_reprojection_frequency; yr++) {
            annual_demand_projections[yr] = current_year_recorded_demand + average_growth_rate * i;
            i += 1;
        }
    } else {
        // If re-projection does not occur (between years of re-projection or before re-projections begin at all)
        //      use the forecast length + current projections to set future demand estimate
        future_demand_estimate = annual_demand_projections[year+demand_projection_forecast_length];
    }
}

void Utility::splitDemands(
        int week, vector<vector<double>> &demands,
        bool apply_demand_buffer, bool apply_demand_projection) {
    // Oct 2019: Get demand projection from 5 years in future to set unrestricted demand
    // if projection is being used, don't use actual demand from the year
    // Calculate the unrestricted demand based on the week, projection, and buffer values.
    unrestricted_demand = !apply_demand_projection * demand_series_realization[week] +
            (apply_demand_buffer * demand_buffer + apply_demand_projection * future_demand_estimate) *
            weekly_peaking_factor[Utils::weekOfTheYear(week)];
    // Calculate the restricted demand by applying the demand multiplier and offset.
    restricted_demand = unrestricted_demand * demand_multiplier - demand_offset;
    unfulfilled_demand = max(max(restricted_demand - total_available_volume,
                                 restricted_demand - total_treatment_capacity), 0.);
    restricted_demand -= unfulfilled_demand;

    // Allocate demand to the utility's priority water sources based on their available allocated volume.
    for (int &ws : priority_draw_water_source) {
        double source_demand =
                min(restricted_demand,
                    water_sources[ws]->getAvailableAllocatedVolume(id));
        demands[ws][this->id] = source_demand;
    }

    // Allocate remaining demand to the utility's non-priority water sources based on their available volume.
    unsigned short over_allocated_sources = 0;
    double over_allocated_volume = 0;
    double demand_fraction[water_sources.size()];
    int not_over_allocated_ids[water_sources.size()];
    double sum_not_alloc_demand_fraction = 0;
    unsigned short not_over_allocated_sources = 0;
    for (int &ws : non_priority_draw_water_source) {
        auto source = water_sources[ws];

        // Calculate allocation based on sources' available volumes.
        demand_fraction[ws] =
                max(1.0e-6,
                    source->getAvailableAllocatedVolume(id) /
                    total_available_volume);

        // Calculate demand allocated to a given source.
        double source_demand = restricted_demand * demand_fraction[ws];
        demands[ws][id] = source_demand;

        // Check if allocated demand was greater than treatment capacity.
        double over_allocated_demand_ws =
                source_demand - source->getAllocatedTreatmentCapacity(id);

        // Set reallocation variables for the sake of reallocating demand.
        if (over_allocated_demand_ws > 0.) {
            over_allocated_sources++;
            over_allocated_volume += over_allocated_demand_ws;
            demands[ws][id] = source_demand - over_allocated_demand_ws;
        } else {
            not_over_allocated_ids[not_over_allocated_sources] = ws;
            sum_not_alloc_demand_fraction += demand_fraction[ws];
            not_over_allocated_sources++;
        }
    }

    // Check for over-allocation and reallocate demand if necessary.
    // Do one iteration of demand reallocation among sources whose treatment
    // capacities have not yet been exceeded if there is an instance of
    // overallocation.
    if (over_allocated_sources > 0) {		            
        for (int i = 0; i < not_over_allocated_sources; ++i) {
            int ws = not_over_allocated_ids[i];
            demands[ws][id] += over_allocated_volume *
                               demand_fraction[ws] / sum_not_alloc_demand_fraction;
        }
    }

    // If used for realization, update the contingency fund and debt service
    if (used_for_realization) {
        updateContingencyFundAndDebtService(unrestricted_demand,
                                            demand_multiplier,
                                            demand_offset,
                                            unfulfilled_demand,
                                            week);
    }
}

void Utility::updateContingencyFundAndDebtService(
        double unrestricted_demand, double demand_multiplier,
        double demand_offset, double unfulfilled_demand, int week) {
    int week_of_year = Utils::weekOfTheYear(week);
    double unrestricted_price = weekly_average_volumetric_price[week_of_year];
    double current_price;

    // Clear yearly updated data collecting variables.
    if (week_of_year == 0) {
        insurance_purchase = 0.;
    } else if (week_of_year == 1) {
        infra_net_present_cost = 0.;
        current_debt_payment = 0.;
    }

    // Set current water price, contingent on restrictions being enacted.
    if (restricted_price == NON_INITIALIZED)
        current_price = unrestricted_price;
    else
        current_price = restricted_price;

    if (current_price < unrestricted_price)
        throw logic_error("Prices under surcharge cannot be smaller than "
                                    "prices w/o restrictions enacted.");

    // Calculate the projected fund contribution and gross revenue.
    double projected_fund_contribution = percent_contingency_fund_contribution *
                                         unrestricted_demand *
                                         unrestricted_price;

    // Calculate actual gross revenue.
    gross_revenue = restricted_demand * current_price;

    // Compute the losses due to restrictions and transfers.
    double lost_demand_vol_sales =
            (unrestricted_demand * (1 - demand_multiplier) +
             unfulfilled_demand);
    double revenue_losses = lost_demand_vol_sales * unrestricted_price;
    double transfer_costs = demand_offset * (offset_rate_per_volume -
                                             unrestricted_price);
    double recouped_loss_price_surcharge =
            restricted_demand * (current_price - unrestricted_price);

    // Update the contingency fund while ensuring it is non-negative and capped.
    double previous_fund_level = contingency_fund;
    contingency_fund = max(
            min(contingency_fund + projected_fund_contribution - revenue_losses -
                    transfer_costs + recouped_loss_price_surcharge,
                contingency_fund_cap), 0.0);

    // Update variables for data collection and next iteration.
    drought_mitigation_cost = max(revenue_losses + transfer_costs -
                                  insurance_payout -
                                  recouped_loss_price_surcharge,
                                  0.0);

    // Calculate the drought mitigation costs and fund contributions for the week.
    // Reduce actual contribution based on revenue losses and transfer costs or cap on fund
    fund_contribution =
            min(projected_fund_contribution - revenue_losses - transfer_costs + recouped_loss_price_surcharge,
                    contingency_fund_cap - previous_fund_level);

    resetDroughtMitigationVariables();

    // Calculate current debt payment to be made on that week (if first week of year), if any.
    current_debt_payment = updateCurrent_debt_payment(week);

    // Update the current debt payment and present value debt payment if applicable.
    current_present_valued_debt_payment = updateCurrent_present_value_debt_payment(week);
}

void Utility::resetDroughtMitigationVariables() {
    restricted_price = NON_INITIALIZED;
    offset_rate_per_volume = NONE;
    this->demand_offset = NONE;
}

void Utility::setWaterSourceOnline(unsigned int source_id, int week) {
    infrastructure_construction_manager.setWaterSourceOnline(source_id, week, total_storage_capacity,
                                                             total_treatment_capacity, total_available_volume,
                                                             total_stored_volume);
}

double Utility::updateCurrent_debt_payment(int week) {
    double current_debt_payment = 0;

    // For each bond, check if they are variable interest bonds
    // (aka are any tied to VariableJointWTP projects with changing allocations)
    for (Bond *bond : issued_bonds) {
        if (bond->type == VARIABLE_INTEREST) {
            // June 2020: make the value passed to setDebtService function not just
            // a utility's allocated fraction, but the quantity [utility fraction / sum of utility fractions]
            // because it is possible allocations do not sum to 100% of wtp capacity
            // the getAllocatedTreatmentFraction function override in JointWTP class does this,
            // which isn't the same as the definition in WaterSource

            // Adjust the debt service based on the utility's allocated treatment fraction for the water source.
            bond->setDebtService(water_sources.at(bond->getWaterSourceID())->getAllocatedTreatmentFraction(id))
        }
    }

    // If the week corresponds to the first week of the year, 
    // calculate the total debt service for that week.
    // If any debt service is NaN, output diagnostic information to the console
    for (Bond *bond : issued_bonds) {
        current_debt_payment += bond->getDebtService(week);
        if (isnan(current_debt_payment)) {
            cout << "DEBT SERVICE FOR UTILITY " << name << " IN WEEK " << week << "IS NAN" << endl;
            cout << name << ": " << water_sources.at(bond->getWaterSourceID())->name
                 << " allocated treatment fraction is "
                 << water_sources.at(bond->getWaterSourceID())->getAllocatedTreatmentFraction(id) << endl;
        }
    }

    return current_debt_payment;
}

double Utility::updateCurrent_present_value_debt_payment(int week) {
    double current_pv_debt_payment = 0;

    // Checks if it's the first week of the year, when outstanding debt payments should be made.
    // This version of calculating debt payment determines present-valued payment for current year and adds that
    //      to handle cases where debt service allocated to a utility can
    //      change over the course of repayment (aka a capacity-sharing agreement)
    for (Bond *bond : issued_bonds) {
        // Get the present value of its debt service
        // Add the present value of the debt service to the total present value debt payment
        current_pv_debt_payment += bond->getPresentValueDebtService(week, infra_discount_rate);
    }

    return current_pv_debt_payment;
}

void Utility::issueBond(int new_infra_triggered, int week) {
    if (new_infra_triggered != NON_INITIALIZED) {
        Bond &bond = water_sources.at((unsigned long) new_infra_triggered)
                ->getBond(id);
        // If the bond has not been issued yet:
        if (!bond.isIssued()) {
            // Retrieve the infrastructure's construction time.
            double construction_time = water_sources
                    .at((unsigned long) new_infra_triggered)->construction_time;
            // Issue the bond with the appropriate parameters (week, construction time, etc.).
            bond.issueBond(week, (int) construction_time, bond_term_multiplier,
                           bond_interest_rate_multiplier);
            // Add the bond to the list of issued bonds
            issued_bonds.push_back(&bond);
            // Update the infrastructure's net present cost with the bond's NPV.
            infra_net_present_cost += bond.getNetPresentValueAtIssuance(
                    infra_discount_rate, week);
        }
    }
}

void Utility::forceInfrastructureConstruction(int week, vector<int> new_infra_triggered) {
    // Build all triggered infrastructure
    infrastructure_construction_manager.forceInfrastructureConstruction(week, new_infra_triggered);

    // Issue bonds for triggered infrastructure
    auto under_construction = infrastructure_construction_manager.getUnder_construction();
    for (int ws : new_infra_triggered) {
        if (under_construction.size() > ws && under_construction.at((unsigned long) ws)) {
            // For each triggered infrastructure, check if it is under construction and issue bonds accordingly.
            // If the project is part of a sequence, adjust capital costs based on previous projects.
            // (i.e. if the previous project in the sequence has already started, reduce capital cost
            //  of upcoming project by that amount)
            infrastructure_construction_manager.checkForSequenceProjects(ws);

            // issue the bond after capital cost adjustment
            issueBond(ws, week);
        }
    }
}

int Utility::infrastructureConstructionHandler(double long_term_rof, int week) {
    double past_year_average_demand = 0;
    // Calculate the average demand for the past year (if applicable).
    if (week >= (int) WEEKS_IN_YEAR) {
        for (int w = week - (int) WEEKS_IN_YEAR; w < week; ++w) {
            past_year_average_demand += demand_series_realization.at(w);
        }
    }

    /// OCT 2019: THIS IS NOW SET IN THE setLongTermRisk-of-failures function
    /// and differentiates between (a) storage ROF (b) treatment ROF and (c) actual ROF (max of stor/trmt ROF)
    // Set the long-term actual risk of failure based on the input.
    long_term_actual_risk_of_failure = long_term_rof;

    // Check if any new infrastructure should be triggered.
    int new_infra_triggered = infrastructure_construction_manager.infrastructureConstructionHandler(long_term_rof, week,
                                                                                                    past_year_average_demand,
                                                                                                    total_storage_capacity,
                                                                                                    total_treatment_capacity,
                                                                                                    total_available_volume,
                                                                                                    total_stored_volume);

    // If new infrastructure is triggered, issue the necessary bond and update the outstanding bond list.
    issueBond(new_infra_triggered, week);

    return new_infra_triggered;
}

void Utility::calculateWastewater_releases(int week, double *discharges) {
    double discharge;
    waste_water_discharge = 0;

    // Feb 2020: demand_offset added back for calculating WW releases
    // because transfers are not actually reducing demand, just doing so
    // for the demand splitting calculations
    for (int &id : wwtp_discharge_rule.discharge_to_source_ids) {
        // For each source ID, calculate the discharge using the current week's demand and `demand_offset`.
        discharge = (restricted_demand + demand_offset) * wwtp_discharge_rule
                .get_dependent_variable(id, Utils::weekOfTheYear(week));
        // Update the `discharges` array with the calculated discharge for the source.
        discharges[id] += discharge;
        // Accumulate the discharge value into `waste_water_discharge` for the total wastewater release.
        waste_water_discharge += discharge;
    }
}

void Utility::addInsurancePayout(double payout_value) {
    contingency_fund += payout_value;
    insurance_payout = payout_value;
}

void Utility::purchaseInsurance(double insurance_price) {
    contingency_fund -= insurance_price;
    insurance_purchase = insurance_price;
}

void
Utility::setDemand_offset(double demand_offset, double offset_rate_per_volume) {
    // if a utility has >1 transfer agreement, make this additive
    this->demand_offset += demand_offset; 
    this->offset_rate_per_volume = offset_rate_per_volume;
}

void Utility::setRealization(unsigned long r, vector<double>& rdm_factors) {
    // Determine the number of weeks for the given realization.
    unsigned long n_weeks = demands_all_realizations.at(r).size();
    demand_series_realization = vector<double>(n_weeks);

    // Apply the demand multiplier and adjust demand series based on the realization factors.
    double delta_demand = demands_all_realizations.at(r)[0] * (1. -
            rdm_factors.at(0));

    for (unsigned long w = 0; w < n_weeks; ++w) {
        demand_series_realization[w] = demands_all_realizations.at(r)[w] *
                                               rdm_factors.at(0) + delta_demand;
    }

    // Set the bond-related multipliers and adjust the infrastructure discount rate.
    bond_term_multiplier = rdm_factors.at(1);
    bond_interest_rate_multiplier = rdm_factors.at(2);
    infra_discount_rate *= rdm_factors.at(3);

    // Calculate and set the weekly peaking factor and annual average weekly demand.
    weekly_peaking_factor = calculateWeeklyPeakingFactor
            (&demands_all_realizations.at(r));
    annual_average_weekly_demand = calculateAnnualAverageWeeklyDemand(&demands_all_realizations.at(r));
}

vector<double> Utility::calculateWeeklyPeakingFactor(vector<double> *demands) {
    unsigned long n_weeks = (unsigned long) WEEKS_IN_YEAR + 1;
    int n_years = (int) (demands->size() / WEEKS_IN_YEAR - 1);
    vector<double> year_averages(n_weeks,
                                 0.0);

    double year_average_demand;

    // Calculate the average demand for each year.
    for (int y = 0; y < n_years; ++y) {
        year_average_demand = accumulate(
                demands->begin() + y * WEEKS_IN_YEAR,
                demands->begin() + (y + 1) * WEEKS_IN_YEAR,
                0.0) /
                              ((int) ((y + 1) * WEEKS_IN_YEAR) -
                               (int) (y * WEEKS_IN_YEAR));
        // For each week in the year, calculate the ratio of that week's demand to 
        // the year's average demand.
        // Normalize the values across all years to get a weekly peaking factor.
        for (unsigned long w = 0; w < n_weeks; ++w) {
            year_averages[w] += (*demands)[y * WEEKS_IN_YEAR + w] /
                                year_average_demand / n_years;
        }
    }

    return year_averages;
}

vector<double> Utility::calculateAnnualAverageWeeklyDemand(vector<double> *demands) {
    // Calculate the number of years based on the size of the `demands` vector and the number of weeks per year.
    int n_years = (int) (demands->size() / WEEKS_IN_YEAR + 1);
    vector<double> annual_averages(n_years, 0.0);

    double year_average_demand;
    // Iterate over each year and calculate the average demand for that year.
    for (int y = 0; y < n_years; ++y) {
        year_average_demand = accumulate(
                demands->begin() + y * WEEKS_IN_YEAR_ROUND,
                demands->begin() + (y + 1) * WEEKS_IN_YEAR_ROUND,
                0.0) /
                              ((int) ((y + 1) * WEEKS_IN_YEAR_ROUND) -
                               (int) (y * WEEKS_IN_YEAR_ROUND));
        annual_averages[y] = year_average_demand;
    }

    return annual_averages;
}

//========================= GETTERS AND SETTERS =============================//

double Utility::getStorageToCapacityRatio() const {
    return total_stored_volume / total_storage_capacity;
}

double Utility::getUnrestrictedDemandToTreatmentCapacityRatio() const {
    return unrestricted_demand / total_treatment_capacity;
}

double Utility::getAvailableVolumeToCapacityRatio() const {
    return total_available_volume / total_storage_capacity;
}

double Utility::getTotal_available_volume() const {
    return total_available_volume;
}

double Utility::getTotal_stored_volume() const {
    return total_stored_volume;
}

double Utility::getTotal_storage_capacity() const {
    return total_storage_capacity;
}

double Utility::getCurrent_year_demand_record() const {
    return current_year_recorded_demand;
}

void Utility::setCurrent_year_demand_record(double current_demand) {
    this->current_year_recorded_demand = current_demand;
}

double Utility::getFuture_demand_estimate() const {
    return future_demand_estimate;
}

void Utility::setFuture_demand_estimate(double demand_estimate) {
    this->future_demand_estimate = demand_estimate;
}

double Utility::getRisk_of_failure() const {
    return short_term_risk_of_failure;
}

double Utility::getStorageRisk_of_failure() const {
    return short_term_storage_risk_of_failure;
}

double Utility::getTreatmentRisk_of_failure() const {
    return short_term_treatment_risk_of_failure;
}

void Utility::setRisk_of_failure(double risk_of_failure) {
    this->short_term_risk_of_failure = risk_of_failure;
}

void Utility::setRisk_of_failures(double storage_risk_of_failure, double treatment_risk_of_failure) {
    this->short_term_storage_risk_of_failure = storage_risk_of_failure;
    this->short_term_treatment_risk_of_failure = treatment_risk_of_failure;
}

void Utility::setLongTermRisk_of_failures(double storage_risk_of_failure, double treatment_risk_of_failure) {
    this->long_term_storage_risk_of_failure = storage_risk_of_failure;
    this->long_term_treatment_risk_of_failure = treatment_risk_of_failure;
}

double Utility::getTotal_treatment_capacity() const {
    return total_treatment_capacity;
}

void Utility::setDemand_multiplier(double demand_multiplier) {
    Utility::demand_multiplier = demand_multiplier;
}

double Utility::getContingency_fund() const {
    return contingency_fund;
}

double Utility::getUnrestrictedDemand() const {
    return unrestricted_demand;
}

double Utility::getRestrictedDemand() const {
    return restricted_demand;
}

double Utility::getGrossRevenue() const {
    return gross_revenue;
}

double Utility::getDemand_multiplier() const {
    return demand_multiplier;
}

double Utility::getUnrestrictedDemand(int week) const {
    return demand_series_realization[week];
}

double Utility::getInfrastructure_net_present_cost() const {
    return infra_net_present_cost;
}

double Utility::getCurrent_debt_payment() const {
    return current_debt_payment;
}

double Utility::getCurrent_debt_payment_present_valued() const {
    return current_present_valued_debt_payment;
}

double Utility::getCurrent_contingency_fund_contribution() const {
    return fund_contribution;
}

double Utility::getDrought_mitigation_cost() const {
    return drought_mitigation_cost;
}

double Utility::getInsurance_payout() const {
    return insurance_payout;
}

double Utility::getInsurance_purchase() const {
    return insurance_purchase;
}

const vector<int> &Utility::getRof_infrastructure_construction_order()
const {
    return infrastructure_construction_manager.getRof_infra_construction_order();
}

const vector<int> &Utility::getDemand_infra_construction_order() const {
    return infrastructure_construction_manager.getDemand_infra_construction_order();
}

const vector<int> Utility::getInfrastructure_built() const {
    return infrastructure_construction_manager.getInfra_built_last_week();
}

double Utility::waterPrice(int week) {
    return weekly_average_volumetric_price[week];
}

void Utility::setRestricted_price(double restricted_price) {
    Utility::restricted_price = restricted_price;
}

void Utility::setNoFinaicalCalculations() {
    used_for_realization = false;
}

double Utility::getLong_term_risk_of_failure() const {
    return long_term_actual_risk_of_failure;
}

double Utility::getLong_term_storage_risk_of_failure() const {
    return long_term_storage_risk_of_failure;
}

double Utility::getLong_term_treatment_risk_of_failure() const {
    return long_term_treatment_risk_of_failure;
}

const vector<WaterSource *> &Utility::getWater_sources() const {
    return water_sources;
}

double Utility::getWaste_water_discharge() const {
    return waste_water_discharge;
}

void Utility::resetTotal_storage_capacity() {
    Utility::total_storage_capacity = 0;
}

double Utility::getUnfulfilled_demand() const {
    return unfulfilled_demand;
}

double Utility::getNet_stream_inflow() const {
    return net_stream_inflow;
}

const InfrastructureManager &Utility::getInfrastructure_construction_manager() const {
    return infrastructure_construction_manager;
}

double Utility::getDemand_offset() const {
    return demand_offset;
}

double Utility::calculateCurrentToNextYearDemandDelta(int current_year) {
    // check one year ahead to update variable WTP allocations
    // Subtract the current year's recorded demand from the next year's projected demand.
    return (annual_demand_projections[current_year+1] - current_year_recorded_demand);
}

void Utility::updateTreatmentCapacity(double capacity_adjustment) {
    // update treatment capacity based on annual changes due to VariableJointWTP action
    total_treatment_capacity += capacity_adjustment;

    if (total_treatment_capacity < 0) {
        total_treatment_capacity = 0;
    }
}

void Utility::setTreatmentCapacity(double total_capacity) {
    // meant to pass treatment capacity from realization model to ROF model
    total_treatment_capacity = total_capacity;
}

double Utility::getSinusoidalFactor(int week, double A, double T, double p) {
    // accepts sinusoidal parameters from LHS RDM sample and returns weekly multiplier
    return (1 + A * sin(2 * 3.14159265 * week / T + p) - A * sin(p));
}