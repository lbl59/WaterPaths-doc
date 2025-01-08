//
// Created by Bernardo on 4/4/2018.
//

#include <algorithm>
#include "InfrastructureManager.h"
#include "../WaterSources/SequentialJointTreatmentExpansion.h"
#include "../WaterSources/ReservoirExpansion.h"
#include "../WaterSources/IntakeExpansion.h"
#include "../WaterSources/Relocation.h"
#include "../../Utils/Utils.h"
#include "../WaterSources/FixedJointWTP.h"
#include "../WaterSources/VariableJointWTP.h"
#include "../WaterSources/AllocatedIntakeExpansion.h"

/**
 * Steps:
 * 1. Initializes the `id` of the infrastructure manager.
 * 2. Rearranges the infrastructure construction trigger vector to ensure it corresponds with the correct order of 
 *    construction for both ROF (Reliability of Supply) and demand-based projects.
 * 3. Sets up vectors for tracking infrastructure construction orders, and handles projects that need to be removed from
 *    the queue once others are built.
 * 4. Initializes financial parameters such as the discount rate, bond term, and interest rate to be used for project
 *    cost calculations.
 */
InfrastructureManager::InfrastructureManager(int id, const vector<double> &infra_construction_triggers,
                                             const vector<vector<int>> &infra_if_built_remove,
                                             double infra_discount_rate, double bond_term,
                                             double bond_interest_rate, vector<int> rof_infra_construction_order,
                                             vector<int> demand_infra_construction_order) :
        id(id),
        infra_construction_triggers(rearrangeInfraRofVector
                                            (infra_construction_triggers, rof_infra_construction_order,
                                             demand_infra_construction_order)),
        infra_if_built_remove(infra_if_built_remove),
        infra_discount_rate(infra_discount_rate),
        bond_term(bond_term),
        bond_interest_rate(bond_interest_rate),
        rof_infra_construction_order(rof_infra_construction_order),
        demand_infra_construction_order(demand_infra_construction_order) {}


InfrastructureManager::InfrastructureManager() {}


InfrastructureManager::InfrastructureManager(InfrastructureManager &infrastructure_manager) :
        id(infrastructure_manager.id),
        infra_construction_triggers(infrastructure_manager.infra_construction_triggers),
        infra_if_built_remove(infrastructure_manager.infra_if_built_remove),
        infra_discount_rate(infrastructure_manager.infra_discount_rate),
        bond_term(infrastructure_manager.bond_term),
        bond_interest_rate(infrastructure_manager.bond_interest_rate),
        rof_infra_construction_order(infrastructure_manager.rof_infra_construction_order),
        demand_infra_construction_order(infrastructure_manager.demand_infra_construction_order) {}

InfrastructureManager& InfrastructureManager::operator=(const InfrastructureManager& infrastructure_manager) {
    id = infrastructure_manager.id;
    infra_construction_triggers = infrastructure_manager.infra_construction_triggers;
    infra_if_built_remove = infrastructure_manager.infra_if_built_remove;
    infra_discount_rate = infrastructure_manager.infra_discount_rate;
    bond_term = infrastructure_manager.bond_term;
    bond_interest_rate = infrastructure_manager.bond_interest_rate;
    rof_infra_construction_order = infrastructure_manager.rof_infra_construction_order;
    demand_infra_construction_order = infrastructure_manager.demand_infra_construction_order;

    return *this;
}

void InfrastructureManager::addWaterSource(WaterSource *water_source) {
    // Check if the water source's ID exceeds the size of the `under_construction` vector.
    // If the ID exceeds the size, resize the `under_construction` vector to match the size of the `water_sources`.
    // Resize the `construction_end_date` vector to match the size of the `water_sources` list, initializing all entries to `NON_INITIALIZED`.
    if (water_source->id > (int) under_construction.size() - 1) {
        under_construction.resize(water_sources->size(), false);
        construction_end_date.resize(water_sources->size(), NON_INITIALIZED);
    }
}

/**
 * Converts the rof and demand triggers vectors into a vector in which rof
 * and demand values are stored in the idex corresponding to the
 * pertaining source.
 */
vector<double> InfrastructureManager::rearrangeInfraRofVector(
        const vector<double>& infra_construction_triggers,
        const vector<int>& rof_infra_construction_order,
        const vector<int>& demand_infra_construction_order) {
    // Determine the size of the new construction trigger vector based on the largest index in both ROF and demand
    // construction order vectors.
    int size_rof = (rof_infra_construction_order.empty() ? 0 :
                    *max_element(rof_infra_construction_order.begin(),
                                 rof_infra_construction_order.end()));
    int size_demand = (demand_infra_construction_order.empty() ? 0 :
                       *max_element(demand_infra_construction_order.begin(),
                                    demand_infra_construction_order.end()));
    int size = max(size_rof, size_demand) + 1;

    // July 2020: need to adjust how the triggers vector is filled
    // NOTE: ASSUMES ROF TRIGGERS POPULATE VECTOR FIRST, THEN CHECKS FOR DEMAND TRIGGERS?
    
    // Initialize a new vector with a default value of `1e10` to represent untriggered sources.
    vector<double> infra_construction_triggers_new((unsigned long) size, 1e10);
    
    // Populate the new vector with the corresponding construction triggers from both ROF and demand orders.
    for (unsigned long i = 0; i < infra_construction_triggers.size(); ++i) {
        // first, check ROF triggers
        if (i < rof_infra_construction_order.size()) {
            auto ws = (unsigned long) rof_infra_construction_order.at(i);
            infra_construction_triggers_new.at(ws) = infra_construction_triggers.at(i);
        } else { // next, follow with demand triggers
            auto ws = (unsigned long) demand_infra_construction_order.at(i-rof_infra_construction_order.size());
            
            // Ensure that a source can only be triggered by either ROF or demand, not both.
            if (infra_construction_triggers_new.at(ws) != 1e10)
                throw invalid_argument("A source can be triggered only by "
                                       "either rof or by demand.");
            infra_construction_triggers_new.at(ws) = infra_construction_triggers.at(i);
        }
    }

    return infra_construction_triggers_new;
}

void InfrastructureManager::addWaterSourceToOnlineLists(int source_id, double &total_storage_capacity,
                                                        double &total_treatment_capacity,
                                                        double &total_available_volume,
                                                        double &total_stored_volume) {
    // Retrieve the water source based on the provided `source_id`.
    auto ws = water_sources->at((unsigned long) source_id);

    // Add the source's allocated storage capacity and treatment capacity to the utility's totals.
    total_storage_capacity +=
            ws->getAllocatedCapacity(id);
    total_treatment_capacity +=
            ws->getAllocatedTreatmentCapacity(id);
    // Set the total available volume to match the updated storage capacity.
    total_available_volume = total_storage_capacity;

    // Depending on the water source type, either:
    //      - Add the source to the priority list (if it is an intake, intake expansion, allocated intake, allocated intake expansion, or water reuse).
    //      - Add the source to the non-priority list and update the total stored volume.
    if ((ws->source_type == INTAKE ||
         ws->source_type == INTAKE_EXPANSION ||
         ws->source_type == ALLOCATED_INTAKE ||
         ws->source_type == ALLOCATED_INTAKE_EXPANSION ||
         ws->source_type == WATER_REUSE)) {
        priority_draw_water_source->push_back(source_id);
    } else {
        non_priority_draw_water_source->push_back(source_id);
        total_stored_volume += ws->getAvailableAllocatedVolume(id);
    }
}


void
InfrastructureManager::setWaterSourceOnline(unsigned int source_id, int week, double &total_storage_capacity,
        double &total_treatment_capacity, double &total_available_volume, double &total_stored_volume) {
    /// Sets water source online and add its ID to appropriate
    /// priority/non-priority ID vector. If reservoir expansion, add its
    /// capacity to the corresponding existing reservoir.

    // First identify the type of water source
    if (water_sources->at(source_id)->source_type == NEW_WATER_TREATMENT_PLANT) {
        waterTreatmentPlantConstructionHandler(source_id, total_storage_capacity);
    } else if (water_sources->at(source_id)->source_type == NEW_JOINT_WATER_TREATMENT_PLANT) {
        waterTreatmentPlantJointConstructionHandler(source_id, total_storage_capacity);
    } else if (water_sources->at(source_id)->source_type ==
               RESERVOIR_EXPANSION) {
        reservoirExpansionConstructionHandler(source_id);
    } else if (water_sources->at(source_id)->source_type == SOURCE_RELOCATION) {
        sourceRelocationConstructionHandler(source_id);
    } else if (water_sources->at(source_id)->source_type == INTAKE_EXPANSION) {
        intakeExpansionConstructionHandler(source_id);
    } else if (water_sources->at(source_id)->source_type == ALLOCATED_INTAKE_EXPANSION) {
        allocatedIntakeExpansionConstructionHandler(source_id);
    } else {
        water_sources->at(source_id)->setOnline();
        addWaterSourceToOnlineLists(source_id, total_storage_capacity, total_treatment_capacity,
                                    total_available_volume, total_stored_volume);
    }

    /// Updates total storage and treatment variables.
    total_storage_capacity = 0;
    total_treatment_capacity = 0;
    total_available_volume = 0;
    for (WaterSource *ws : *water_sources) {
        if (ws &&
            (find(priority_draw_water_source->begin(),
                  priority_draw_water_source->end(),
                  ws->id) != priority_draw_water_source->end() ||
             find(non_priority_draw_water_source->begin(),
                  non_priority_draw_water_source->end(),
                  ws->id) != non_priority_draw_water_source->end())) {
            total_storage_capacity += ws->getAllocatedCapacity(id);
            total_treatment_capacity += ws->getAllocatedTreatmentCapacity(id);
            total_available_volume += ws->getAvailableAllocatedVolume(id);
        }
    }
}


void
InfrastructureManager::waterTreatmentPlantConstructionHandler(unsigned int source_id, double &total_storage_capacity) {
    
    auto wtp = dynamic_cast<SequentialJointTreatmentExpansion *>
    (water_sources->at(source_id));

    /// Add treatment capacity to source
    double added_capacity = wtp->implementTreatmentCapacity(id);
    try {
        water_sources->at(wtp->parent_reservoir_ID)
                ->addTreatmentCapacity(added_capacity, id);
    } catch (...) {
        throw runtime_error("Could not add treatment capacity to reservoir.");
    }

    /// If source is intake or reuse and is not in the list of active
    /// sources, add it to the priority list.
    /// If source is not intake or reuse and is not in the list of active
    /// sources, add it to the non-priority list.
    bool is_priority_source =
            water_sources->at(wtp->parent_reservoir_ID)->source_type == INTAKE ||
            water_sources->at(wtp->parent_reservoir_ID)->source_type == INTAKE_EXPANSION ||
            water_sources->at(wtp->parent_reservoir_ID)->source_type == ALLOCATED_INTAKE ||
            water_sources->at(wtp->parent_reservoir_ID)->source_type == ALLOCATED_INTAKE_EXPANSION ||
            water_sources->at(wtp->parent_reservoir_ID)->source_type ==
            WATER_REUSE;
    bool is_not_in_priority_list =
            find(priority_draw_water_source->begin(),
                 priority_draw_water_source->end(),
                 wtp->parent_reservoir_ID)
            == priority_draw_water_source->end();
    bool is_not_in_non_priority_list =
            find(non_priority_draw_water_source->begin(),
                 non_priority_draw_water_source->end(),
                 wtp->parent_reservoir_ID)
            == non_priority_draw_water_source->end();

    /// Finally, the checking.
    if (is_priority_source && is_not_in_priority_list) {
        priority_draw_water_source->push_back((int) wtp->parent_reservoir_ID);
        total_storage_capacity +=
                water_sources->at(wtp->parent_reservoir_ID)
                        ->getAllocatedCapacity(id);
    } else if (is_not_in_non_priority_list) {
        non_priority_draw_water_source
                ->push_back((int) wtp->parent_reservoir_ID);
    }
    water_sources->at(source_id)->setOnline();
}

void InfrastructureManager::waterTreatmentPlantJointConstructionHandler(unsigned int source_id, double &total_storage_capacity) {
    // Identify the type of joint water treatment plant being constructed (fixed or variable allocations).
    // For each type, implement the initial treatment capacity using the respective `FixedJointWTP` or `VariableJointWTP` object.
    if (water_sources->at(source_id)->getAgreementType() == NEW_JOINT_WATER_TREATMENT_PLANT_FIXED_ALLOCATIONS) {
        auto wtp = dynamic_cast<FixedJointWTP *> (water_sources->at(source_id));

        /// Add treatment capacity to source
        double added_capacity = wtp->implementInitialTreatmentCapacity(id);
        try {
            water_sources->at(wtp->parent_reservoir_ID)->addTreatmentCapacity(added_capacity, id);
        } catch (...) {
            throw runtime_error("Could not add treatment capacity to reservoir.");
        }

        // If the parent reservoir is not yet online (which happens if previously the utility was not allocated
        // any treatment capacity), add it to the online sources vector
        auto it = find(non_priority_draw_water_source->begin(),
                       non_priority_draw_water_source->end(),
                       (int) wtp->parent_reservoir_ID);
        if (it == non_priority_draw_water_source->end())
            non_priority_draw_water_source->push_back((int) wtp->parent_reservoir_ID);

    } else if (water_sources->at(source_id)->getAgreementType() == NEW_JOINT_WATER_TREATMENT_PLANT_VARIABLE_ALLOCATIONS) {
        auto wtp = dynamic_cast<VariableJointWTP *> (water_sources->at(source_id));

        // Add the allocated treatment capacity to the parent reservoir using the `addTreatmentCapacity` method.
        // FIXME: This should be changed later to be based on current demand levels rather than some initial levels
        //      How allocations at the plant change is based on overall utility demand growth, which is pinned to initial levels
        //      being reasonable at the time the plant comes online...
        //      For early tests, just use presumed set of treatment allocations
        double added_capacity = wtp->implementInitialTreatmentCapacity(id);
        try {
            water_sources->at(wtp->parent_reservoir_ID)->addTreatmentCapacity(added_capacity, id);
        } catch (...) {
            throw runtime_error("Could not add treatment capacity to reservoir.");
        }

        // If the parent reservoir is not online yet, add it to the list of online sources.
        auto it = find(non_priority_draw_water_source->begin(),
                       non_priority_draw_water_source->end(),
                       (int) wtp->parent_reservoir_ID);
        if (it == non_priority_draw_water_source->end())
            non_priority_draw_water_source->push_back((int) wtp->parent_reservoir_ID);

    } else {
        throw logic_error("Error in InfrastructureManager::waterTreatmentPlantJointConstructionHandler, "
                          "WTP to be constructed is not of either fixed or variable treatment allocations.");
    }

    // Set the water treatment plant online using the `setOnline` method.
    water_sources->at(source_id)->setOnline();
}

void InfrastructureManager::reservoirExpansionConstructionHandler(unsigned int source_id) {
    // Casts the water source specified by `source_id` to a `ReservoirExpansion` object.
    ReservoirExpansion re =
            *dynamic_cast<ReservoirExpansion *>(water_sources->at(source_id));
    // Retrieves the parent reservoir ID from the `ReservoirExpansion` object.
    // Adds the allocated capacity of the reservoir expansion to the parent reservoir.
    water_sources->at(re.parent_reservoir_ID)->
            addCapacity(re.getAllocatedCapacity(id), id);

    // Marks the reservoir expansion as online.
    water_sources->at(source_id)->setOnline();
}

void InfrastructureManager::sourceRelocationConstructionHandler(unsigned int source_id) {
    // Casts the water source specified by `source_id` to a `Relocation` object
    Relocation re =
            *dynamic_cast<Relocation *>(water_sources->at(source_id));
    // Extracts the new allocation fractions from the `Relocation` object.
    const vector<double> *new_allocated_fractions = re.new_allocated_fractions;
    // Apply the new allocation fractions on the parent reservoir.
    water_sources->at(re.parent_reservoir_ID)->
            resetAllocations(new_allocated_fractions);
    // Activates the relocation source.
    water_sources->at(source_id)->setOnline();
}

void InfrastructureManager::intakeExpansionConstructionHandler(unsigned int source_id) {
    // Retrieve the `IntakeExpansion` object from the `water_sources` vector using the provided `source_id`.
    IntakeExpansion re =
            *dynamic_cast<IntakeExpansion *>(water_sources->at(source_id));
    // Update the parent intake's allocated capacity using the `addCapacity` method of the `IntakeExpansion` object.
    water_sources->at(re.parent_intake_ID)->
            addCapacity(re.getAllocatedCapacity(id), id);
            
    // Update the parent intake's allocated treatment capacity using the `addTreatmentCapacity` method.
    water_sources->at(re.parent_intake_ID)->
            addTreatmentCapacity(re.getAllocatedTreatmentCapacity(id), NON_INITIALIZED);
    // Set the intake expansion as online using the `setOnline` method.
    water_sources->at(source_id)->setOnline();
}

void InfrastructureManager::allocatedIntakeExpansionConstructionHandler(unsigned int source_id) {
    // Cast the water source to an `AllocatedIntakeExpansion` object.
    AllocatedIntakeExpansion ie = *dynamic_cast<AllocatedIntakeExpansion *>(water_sources->at(source_id));
    // Add the allocated capacity of the intake expansion to the parent intake using the `addCapacity` method.
    water_sources->at(ie.parent_intake_ID)->addCapacity(ie.getAllocatedCapacity(id), id);
    // Add the allocated treatment capacity of the intake expansion to the parent intake using the `addTreatmentCapacity` method.
    water_sources->at(ie.parent_intake_ID)->addTreatmentCapacity(ie.getAllocatedTreatmentCapacity(id), id);
    // Set the intake expansion source online by invoking the `setOnline` method.
    water_sources->at(source_id)->setOnline();
}

/**
 * Force utility to build a certain infrastructure option, if present in its queue.
 */
void InfrastructureManager::forceInfrastructureConstruction(
        int week, vector<int> new_infra_triggered) {
    //FIXME: ALSO TRIGGER RELATED INFRASTRUCTURE IN SEQUENCE
    for (int ws : new_infra_triggered) {
        // Store the position of an infrastructure option in the queue of infrastructure to be built.
        auto it = find(rof_infra_construction_order.begin(),
                       rof_infra_construction_order.end(),
                       ws);
        
        // If one of the infrastructure options built this year by one of the other utilities is in this utility's queue, force it to be triggered.
        if (it != rof_infra_construction_order.end()) {
            // Force infrastructure option new_infra_triggered to be built.
            beginConstruction(week, ws);
        }
    }
}

/**
 * Checks if infrastructure should be built, triggers construction, sets it
 * online, and handles accounting/financing of it.
 */
int InfrastructureManager::infrastructureConstructionHandler(double long_term_rof, int week, double past_year_average_demand,
                                                             double &total_storage_capacity, double &total_treatment_capacity,
                                                             double &total_available_volume, double &total_stored_volume) {
    int new_infra_triggered = NON_INITIALIZED;
    bool under_construction_any = (find(under_construction.begin(),
                                        under_construction.end(), true) !=
                                   under_construction.end());
    
    // Identifies the next project in `demand_infra_construction_order` whose permitting period has passed.
    // Checks whether the target demand has been exceeded for the next infrastructure option in the list.
    // If not already under construction, starts building it.
    // July 2020: DEMAND STATEMENT PUT IN FRONT OF ROF STATEMENT - DEMAND-TRIGGERED PROJECTS GO FIRST
    
    if (!demand_infra_construction_order.empty() && !under_construction_any) { // if there is anything to be built
        // Select next water source whose permitting period is passed.
        int next_construction = NON_INITIALIZED;
        for (int &id : demand_infra_construction_order) {
            auto idd = (unsigned long) id;
            if (week > water_sources->at(idd)->getPermitting_period() && !water_sources->at(idd)->skipConstruction(id)) {
                next_construction = id;
                break;
            }
        }
        // Checks if the `past_year_average_demand` exceeds the trigger threshold for the project.
        // If so, initiates construction of the project using `beginConstruction`.
        // July 2020: `past_year_average_demand` is 0 for year 0. 
        //            If a demand project is meant to be triggered immediately, the trigger should be negative.
        if (next_construction != NON_INITIALIZED &&
            past_year_average_demand > infra_construction_triggers[next_construction]) {
            new_infra_triggered = next_construction;

            /// Begin construction.
            beginConstruction(week, next_construction);
        }
    }

    /// July 2020: Double-check whether a project has been triggered so that two do not
    /// go under construction simultaneously
    under_construction_any = (find(under_construction.begin(),
                                   under_construction.end(), true) !=
                              under_construction.end());

    // Identifies the next project in `rof_infra_construction_order` whose permitting period has passed.
    // Done by checking whether the `long_term_rof` exceeds the trigger threshold for the project.
    // If not already under construction, initiate construction of the project using `beginConstruction`.
    if (!rof_infra_construction_order.empty() && !under_construction_any) {
        // Check if there is anything to be built
        // Selects next water source whose permitting period is passed.
        int next_construction = NON_INITIALIZED;
        for (int id : rof_infra_construction_order) {
            auto idd = (unsigned long) id;
            if (week > water_sources->at(idd)->getPermitting_period() && !water_sources->at(idd)->skipConstruction(id)) {
                next_construction = id;
                break;
            }
        }

        if (next_construction != NON_INITIALIZED &&
            long_term_rof > infra_construction_triggers[next_construction]) {
            new_infra_triggered = next_construction;

            /// Begin construction.
            beginConstruction(week, next_construction);
        }
    }

    // Iterates through the `under_construction` flags to identify completed projects.
    // If there's a water source under construction, check if it's ready and,
    // if so, clear it from the to-build list.
    if (under_construction_any) {
        // Loops through vector of under-construction flags and sets online
        // the ones for which construction period has passed.
        for (unsigned long ws = 0; ws < under_construction.size(); ++ws)
            // If completed, set projects online using `setWaterSourceOnline`.
            if (under_construction[ws] && week > construction_end_date[ws]) {
                vector<int> set_online_now;

                // Check if water source to be set online is part of a series of expansions or constructions
                //      that depend on previous water sources being built, in which case it's as if all of them had
                //      been constructed at the same time. The total cost of building all at the same time is approximated
                //      by the summation of their individual costs, and the construction time is approximated as that of the
                //      source that was called
                // FIXME: both can be pretty rough approximations at times, so any improvement here is welcome.
                if (water_sources->at(ws)->getBuilt_in_sequence().empty()) {
                    set_online_now.push_back((int) ws);
                } else {
                    // MAR 2020: To ensure all parts of a project are set online in a series of expansions,
                    //             check whether the ws being put online is not the first element id in the vector
                    //             of the building sequence. If it is not, then set the elements before it online...
                    vector<int> build_sequence = water_sources->at(ws)->getBuilt_in_sequence();
                    for (int i = 0; i < build_sequence.size(); ++i) {
                        // Check if previous source/expansion is yet to be built.
                        bool yet_to_be_built = find(rof_infra_construction_order.begin(),
                                                    rof_infra_construction_order.end(), build_sequence[i])
                                               != rof_infra_construction_order.end() ||
                                               find(demand_infra_construction_order.begin(),
                                                    demand_infra_construction_order.end(), build_sequence[i])
                                               != demand_infra_construction_order.end();
                        if (yet_to_be_built)
                            set_online_now.push_back(build_sequence[i]);
                        if (build_sequence[i] == ws)
                            break;
                    }
                }

                // Updates the infrastructure queues and marks the project as no longer under construction.
                for (const int &wss : set_online_now) {
                    setWaterSourceOnline((unsigned int) wss, week, total_storage_capacity, total_treatment_capacity,
                                         total_available_volume, total_stored_volume);

                    // Record ID of and when infrastructure option construction was completed. (utility_id, week, new source id)
                    infra_built_last_week = {id, week, wss};

                    // Erase infrastructure option from both vectors of
                    // infrastructure to be built.

                    // Ensures all projects in a sequence are appropriately set online if dependencies exist.
                    bool rof_check = false; bool demand_check = false;
                    if (!rof_infra_construction_order.empty()) {
                        Utils::removeIntFromVector(rof_infra_construction_order, wss);
                        rof_check = true;
                    }
                    if (!demand_infra_construction_order.empty()) {
                        Utils::removeIntFromVector(demand_infra_construction_order, wss);
                        demand_check = true;
                    }

                    // if a project was triggered, but removed from neither list, throw error
                    if (!rof_check && !demand_check)
                        throw logic_error("Infrastructure option whose construction was"
                                            " complete is not in the demand or "
                                            "rof triggered construction lists.");

                    under_construction[wss] = false;
                }
            }
    }

    return new_infra_triggered;
}

/**
 * Checks is piece of infrastructure to be built next prevents another one
 * from being build and, if so, removes the latter from the queue.
 * @param next_construction
 */
void InfrastructureManager::removeRelatedSourcesFromQueue(int next_construction) {
    //  If the `infra_if_built_remove` list is not empty, iterate over its entries.
    if (!infra_if_built_remove.empty()) {
        // For each entry in `infra_if_built_remove`, check if the first element matches `next_construction`.
        for (auto &v : infra_if_built_remove) {
            // For all projects in the matching entry, remove their IDs from both the 
            // `rof_infra_construction_order` and `demand_infra_construction_order` queues
            if (v[0] == next_construction) {
                for (int i : v) {
                    Utils::removeIntFromVector(rof_infra_construction_order, i);
                    Utils::removeIntFromVector(demand_infra_construction_order, i);
                }
            }
        }
    }
}



/**
 * Kicks off construction and records when it was initiated.
 */
void InfrastructureManager::beginConstruction(int week, int infra_id) {
    
    // Check if the infrastructure to be built next prevents another infrastructure from being constructed. 
    // If so, remove the conflicting infrastructure from the construction queue.
    // E.g. if the large expansion of a reservoir is triggered, the small expansion must be removed from the to
    // build list.
    removeRelatedSourcesFromQueue(infra_id);

    try {
        // Mark the specified infrastructure as "under construction."
        under_construction[infra_id] = true;
        // Calculate and set the construction end date based on the current week and the infrastructure's construction time.
        construction_end_date[infra_id] =
                week + (int) water_sources->at((unsigned long) infra_id)->construction_time;
    } catch (...) {
        // Throw an exception to indicate that the infrastructure is not present
        // in case of an invalid infrastructure ID.
        throw out_of_range("Infrastructure not present in infrastructure manager (in utility).");
    }
}

void InfrastructureManager::connectWaterSourcesVectorsToUtilitys(vector<WaterSource *> &water_sources,
                                                                 vector<int> &priority_draw_water_source,
                                                                 vector<int> &non_priority_draw_water_source) {
    // Store the reference to the provided `water_sources` vector, which contains the list of water sources.
    this->water_sources = &water_sources;
    // Store the reference to the provided `priority_draw_water_source` vector, which contains the list of priority water sources.
    this->priority_draw_water_source = &priority_draw_water_source;
    // Store the reference to the provided `non_priority_draw_water_source` vector, which contains the list of non-priority water sources.
    this->non_priority_draw_water_source = &non_priority_draw_water_source;
}

const vector<int> &InfrastructureManager::getRof_infra_construction_order() const {
    return rof_infra_construction_order;
}

const vector<int> &InfrastructureManager::getDemand_infra_construction_order() const {
    return demand_infra_construction_order;
}

const vector<int> &InfrastructureManager::getInfra_built_last_week() const {
    return infra_built_last_week;
}

const vector<bool> &InfrastructureManager::getUnder_construction() const {
    return under_construction;
}

void InfrastructureManager::checkForSequenceProjects(int original_build_id) {
    // Retrieves the sequence of projects associated with the current water source.
    // Iterates through each water source in the sequence to check if any part of the sequence has been built.
    if (!water_sources->at(original_build_id)->getBuilt_in_sequence().empty()) {
        // If a previous project in the sequence has been built, adjusts the capital costs of the current project.
        // ASSUMES BUILT_IN_SEQUENCE VECTOR HAS PROJECTS IN SEQUENTIAL ORDER FROM LOW TO HIGH
        for (int ws : water_sources->at(original_build_id)->getBuilt_in_sequence()) {
            if (ws == original_build_id) // don't double-build current project in sequence
                break;
            bool yet_to_be_built = find(rof_infra_construction_order.begin(),
                                        rof_infra_construction_order.end(), ws)
                                   != rof_infra_construction_order.end() ||
                                   find(demand_infra_construction_order.begin(),
                                        demand_infra_construction_order.end(), ws)
                                   != demand_infra_construction_order.end();
            // Skips the current project in the sequence to avoid double-building it.
            // i.e. if a lower-project-in-sequence has already been built, this reduces current project's capital costs accordingly
            if (!yet_to_be_built) {
                water_sources->at(original_build_id)->getBond(id).adjustCostOfCapital(
                        water_sources->at(ws)->getBond(id).getCostOfCapital());
            }
        }
    }
}

