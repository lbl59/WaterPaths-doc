
/**
 * @class InfrastructureManager
 * @brief The `InfrastructureManager` class manages the construction, sequencing, and financial aspects of infrastructure projects.
 * 
 * Created by Bernardo on 4/4/2018.
 */
#ifndef TRIANGLEMODEL_INFRASTRUCTUREMANAGER_H
#define TRIANGLEMODEL_INFRASTRUCTUREMANAGER_H

#include <vector>
#include "../WaterSources/Base/WaterSource.h"

using namespace std;

class InfrastructureManager {

private:
    /**
     * @brief An integer representing the ID of the infrastructure option.
     */
    int id;
    /**
     * @brief A vector of doubles storing the ROF trigger values for infrastructure construction.
     */
    vector<double> infra_construction_triggers;
    /**
     * @brief A pointer to the vector of integers storing the ID sequence of water sources to be 
     *  utilized for fulfilling water demand
     */
    vector<int> *priority_draw_water_source;
    /**
     * @brief A pointer to the vector of integers storing the IDs for water sources that are not
     *  prioritized for water demand fulfillment. Allocation of demand is executed proportionally
     *  to the available capacity of these sources.
     */
    vector<int> *non_priority_draw_water_source;
    /**
     * @brief A 2D vector of integers storing the IDs of infrastructure options to be removed
     *  if a given infrastructure option is built.
     */
    vector<vector<int>> infra_if_built_remove;
    /**
     * @brief A pointer to the vector of WaterSource pointers storing all water sources that supply
     *  water to a specific supply infrastructure.
     */
    vector<WaterSource *> *water_sources;
    /**
     * @brief A double representing the discount rate for infrastructure construction.
     */
    double infra_discount_rate;
    /**
     * @brief A double representing the term of the bond for infrastructure construction.
     */
    double bond_term;
    /**
     * @brief A double representing the interest rate for the bond for infrastructure construction.
     */
    double bond_interest_rate;
    /**
     * @brief A vector representing the order of infrastructure projects based on long-term ROFs.
     */
    vector<int> rof_infra_construction_order;
    /**
     * @brief A vector representing the order of infrastructure projects based on demand.
     */
    vector<int> demand_infra_construction_order;
    /**
     * @brief A vector of integers recording the ID, week of completion, and new water supply source (WSS) 
     *  for an infrastructure option (utility_id, week, new source id)
    */
    vector<int> infra_built_last_week;
    /**
     * @brief A vector of integers corresponding to the vector of infrastructure options indicating the week 
     *  in which each infrastructure option's construction was completed.
     *  Indexed using the infrastructure ID number.
     */
    vector<int> construction_end_date;
    /**
     * @brief A vector of booleans storing whether each infrastructure option is currently under
     *  construction.
     *  Indexed using the infrastructure ID number.
     */
    vector<bool> under_construction;
    /**
     * @brief A double representing the net present cost of the infrastructure option.
     */
    double infra_net_present_cost = NONE;

public:
    /**
 * @brief Constructs an InfrastructureManager object.
 *
 * This constructor initializes an instance of the `InfrastructureManager` class, setting up various vectors and financial
 * parameters required for managing infrastructure construction, sequencing, and financial adjustments.
 *
 * 
 * @param id The unique identifier for the infrastructure manager.
 * @param infra_construction_triggers A vector of thresholds that trigger infrastructure construction, ordered by
 *                                    both ROF and demand-based projects.
 * @param infra_if_built_remove A 2D vector that determines which infrastructure should be removed from the queue once
 *                              other infrastructure projects are built.
 * @param infra_discount_rate The discount rate applied to the infrastructure's financial calculations.
 * @param bond_term The term of the bond used for financing infrastructure projects.
 * @param bond_interest_rate The interest rate applied to the bond financing the infrastructure projects.
 * @param rof_infra_construction_order A vector representing the order of infrastructure projects based on ROF.
 * @param demand_infra_construction_order A vector representing the order of infrastructure projects based on demand.
 * 
 * @see InfrastructureManager::rearrangeInfraRofVector() for rearranging the construction triggers to match construction order.
 */
    InfrastructureManager(int id, const vector<double> &infra_construction_triggers,
                              const vector<vector<int>> &infra_if_built_remove,
                              double infra_discount_rate, double bond_term,
                              double bond_interest_rate, vector<int> rof_infra_construction_order,
                              vector<int> demand_infra_construction_order);

    /**
     * @brief Basic constructor a new Infrastructure Manager object
     */
    InfrastructureManager();

    /**
     * @brief An assignment operator that copies the values of the infrastructure manager object to another infrastructure manager object
     * 
     * @param infrastructure_manager The InfrastructureManager object to be copied
     * @return InfrastructureManager& A reference to the new InfrastructureManager object
     */
    InfrastructureManager& operator=(const InfrastructureManager& infrastructure_manager);

    /**
     * @brief Constructs a new Infrastructure Manager object from an existing Infrastructure Manager object
     * 
     * @param infrastructure_manager A reference to the InfrastructureManager object to be copied
     */
    InfrastructureManager(InfrastructureManager &infrastructure_manager);

    /**
     * @brief Rearranges the infrastructure construction trigger vector to match the specified order of construction 
     *        based on ROF and demand.
     *
     * This function takes the infrastructure construction triggers and reorders them according to the specified order 
     * of infrastructure construction for both ROF (Reliability of Supply) and demand-based projects. It ensures that 
     * the construction triggers are applied correctly to each infrastructure, respecting the prioritization of ROF-based 
     * projects followed by demand-based projects.
     * 
     * FIXME: THIS DOESN'T WORK IF A UTILITY HAS BOTH ROF AND DEMAND TRIGGERED INFRASTRUCTURE
     * FIXME: AND THERE ARE NOT AN EQUAL NUMBER OF EACH
     *
     * @param infra_construction_triggers A vector of construction triggers (thresholds) for infrastructure.
     * @param rof_infra_construction_order A vector representing the order of infrastructure projects based on ROF.
     * @param demand_infra_construction_order A vector representing the order of infrastructure projects based on demand.
     * 
     * @return A reordered vector of construction triggers that matches the infrastructure construction order for both ROF 
     *         and demand projects.
     * 
     * @throws invalid_argument if a source is triggered by both ROF and demand.
     * 
     * @see InfrastructureManager::rearrangeInfraRofVector() for details on how triggers are adjusted and applied in the construction process.
     */
    vector<double> rearrangeInfraRofVector(const vector<double> &infra_construction_triggers,
                                           const vector<int> &rof_infra_construction_order,
                                           const vector<int> &demand_infra_construction_order);

    /**
     * @brief Identifies the type of water source and sets a water source online and updates related capacities and volumes.
     *
     * This function activates a specified water source and categorizes it as either priority or non-priority. It also handles
     * special cases such as new water treatment plants, reservoir expansions, and source relocations. The function updates
     * total storage capacity, treatment capacity, available volume, and stored volume for all active water sources.
     *
     * @param source_id The unique identifier of the water source to be set online.
     * @param week The current week, used to track the activation time of the water source.
     * @param total_storage_capacity A reference to the total storage capacity of all active sources, updated after the function execution.
     * @param total_treatment_capacity A reference to the total treatment capacity of all active sources, updated after the function execution.
     * @param total_available_volume A reference to the total available volume of all active sources, updated after the function execution.
     * @param total_stored_volume A reference to the total stored volume of all active sources, updated after the function execution.
     
     * @see waterTreatmentPlantConstructionHandler
     * @see waterTreatmentPlantJointConstructionHandler
     * @see reservoirExpansionConstructionHandler
     * @see sourceRelocationConstructionHandler
     * @see intakeExpansionConstructionHandler
     * @see allocatedIntakeExpansionConstructionHandler
    */
    void setWaterSourceOnline(unsigned int source_id, int week, double &total_storage_capacity, double &total_treatment_capacity,
                                  double &total_available_volume, double &total_stored_volume);

    /**
     * @brief Handles the construction of a water treatment plant and updates the associated water source's capacity and priority lists.
     *
     * This function manages the expansion of a water treatment plant by adding treatment capacity to the associated water source.
     * It also ensures the source is appropriately categorized as either a priority or non-priority source and updates the priority
     * and non-priority lists accordingly.
     *
     * @param source_id The unique identifier of the water source for which the treatment plant is being expanded.
     * @param total_storage_capacity A reference to the total storage capacity, which is updated if the source is added to the priority list.
     * 
     * @throws std::runtime_error If the treatment capacity cannot be added to the parent reservoir.
     * @see SequentialJointTreatmentExpansion
     */
    void waterTreatmentPlantConstructionHandler(unsigned int source_id, double &total_storage_capacity);

    /**
     * @brief Handles the construction of a reservoir expansion by adding capacity to its parent reservoir. 
     * 
     *
     * This function manages the logic for integrating a reservoir expansion into the system. It dynamically casts the 
     * specified water source to a `ReservoirExpansion` type, adds its allocated capacity to the associated parent reservoir, 
     * and marks the reservoir expansion as online.
     *
     * @param source_id The unique identifier of the reservoir expansion source.
     * 
     * @throws std::bad_cast If the specified `source_id` does not correspond to a `ReservoirExpansion` type.
     * 
     * @see ReservoirExpansion
     * @see WaterSource::addCapacity
     * @see WaterSource::setOnline
     */
    void reservoirExpansionConstructionHandler(unsigned int source_id);

    /**
     * @brief Handles the relocation of a water source by updating its allocation fractions and activating it.
     *
     * This function manages the process of relocating a water source by adjusting the allocation fractions for its parent reservoir
     * and marking the relocation source as online.
     *
     * @param source_id The unique identifier of the relocation source to be processed.
     * 
     * @throws std::bad_cast If the specified `source_id` does not correspond to a `Relocation` type.
     * 
     * @see Relocation
     * @see WaterSource::resetAllocations
     * @see WaterSource::setOnline
     */
    void sourceRelocationConstructionHandler(unsigned int source_id);

    /**
     * @brief Checks if piece of infrastructure to be built next prevents another one from being build.
     * If yes, remove the latter from the queue.
     *
     * This function ensures that dependent or redundant infrastructure projects are removed from the construction queues
     * (`rof_infra_construction_order` and `demand_infra_construction_order`) when a specific project (`next_construction`) is scheduled.
     *
     * @param next_construction The identifier of the infrastructure project that is scheduled for construction.
     * 
     * @see Utils::removeIntFromVector
     */
    void removeRelatedSourcesFromQueue(int next_construction);

    /**
     * @brief Handles the initiation, progression, and completion of infrastructure construction based on demand and reliability triggers.
     * Also handles the accounting and financing of the infrastructure.
     *
     * This function manages the construction of infrastructure projects based on demand and long-term reliability of failure (ROF) metrics.
     * It ensures infrastructure is built when thresholds are exceeded, and sets completed projects online while updating the relevant system capacities.
     *
     * @param long_term_rof The long-term reliability of failure metric used to trigger construction of infrastructure.
     * @param week The current simulation week.
     * @param past_year_average_demand The average demand observed in the past year, used to trigger demand-based projects.
     * @param[out] total_storage_capacity Updates the total storage capacity of all active water sources.
     * @param[out] total_treatment_capacity Updates the total treatment capacity of all active water sources.
     * @param[out] total_available_volume Updates the total available volume of all active water sources.
     * @param[out] total_stored_volume Updates the total stored volume of all active water sources.
     * 
     * @return The ID of the infrastructure project triggered for construction, or `NON_INITIALIZED` if none were triggered.
     * 
     * @throws `logic_error` if a completed project is not found in either construction queue.
     */
    int infrastructureConstructionHandler(double long_term_rof, int week, double past_year_average_demand,
                                              double &total_storage_capacity, double &total_treatment_capacity,
                                              double &total_available_volume, double &total_stored_volume);

    /**
     * @brief Forces the construction of specified infrastructure options for all
     * other utilities if they are in the queue.
     *
     * This function checks if the specified infrastructure options are present in the 
     * queue of infrastructure to be built (`rof_infra_construction_order`). If so, it 
     * forces their construction by calling `beginConstruction`.
     * 
     * This function also handles infrastructure triggered by other utilities if the 
     * options exist in this utility's construction queue.
     * 
     * FIXME: The function currently includes a placeholder for triggering related 
     * infrastructure in sequence (marked as FIXME).
     *
     * @param week The current simulation week.
     * @param new_infra_triggered A vector containing the IDs of the infrastructure 
     *                                options that need to be forced into construction.
     *
     * @see beginConstruction
     */
    void forceInfrastructureConstruction(int week, vector<int> new_infra_triggered);

    /**
     * @brief Initiates the construction of the specified infrastructure and records the start and end dates.
     *
     * This function starts the construction process for a given infrastructure by marking it as "under construction"
     * and calculating its construction end date. Additionally, it ensures that any conflicting infrastructure options
     * are removed from the construction queue if necessary.
     * 
     * @param week The current simulation week.
     * @param infra_id The ID of the infrastructure that is to be constructed.
     *
     * @throws out_of_range If the infrastructure ID is not valid or not found in the infrastructure manager.
     *
     * @see removeRelatedSourcesFromQueue
     */
    void beginConstruction(int week, int infra_id);

    /**
     * @brief Adds a water source to the utility's online lists and updates the relevant capacity and volume totals.
     *
     * This function adds the specified water source to either the priority or non-priority list of online sources,
     * depending on the source type. It also updates the total storage capacity, treatment capacity, available volume,
     * and stored volume for the utility.
     * 
     * @param source_id The ID of the water source to be added to the online lists.
     * @param total_storage_capacity The total storage capacity of the utility, updated by this function.
     * @param total_treatment_capacity The total treatment capacity of the utility, updated by this function.
     * @param total_available_volume The total available volume of water in the utility, updated by this function.
     * @param total_stored_volume The total stored volume of water in the utility, updated by this function.
     */
    void addWaterSourceToOnlineLists(int source_id, double &total_storage_capacity, double &total_treatment_capacity,
                                     double &total_available_volume, double &total_stored_volume);

    /**
     * @brief Adds a water source to the construction management system.
     *
     * This function checks if the ID of the water source exceeds the current size of the `under_construction` list.
     * If so, it resizes the `under_construction` and `construction_end_date` vectors to accommodate the new water source.
     * 
     * @param water_source A pointer to the `WaterSource` object to be added to the system.
     */
    void addWaterSource(WaterSource *water_source);

    /**
     * @brief Connects the provided water source vectors to the utility.
     *
     * This function establishes references to the provided vectors of water sources, priority draw water sources, 
     * and non-priority draw water sources. These references are stored in the utility class for later use.
     * @param water_sources A reference to the vector of `WaterSource` objects.
     * @param priority_draw_water_source A reference to the vector containing the IDs of priority water sources.
     * @param non_priority_draw_water_source A reference to the vector containing the IDs of non-priority water sources.
     */
    void connectWaterSourcesVectorsToUtilitys(vector<WaterSource *> &water_sources,
                                              vector<int> &priority_draw_water_source,
                                              vector<int> &non_priority_draw_water_source);

    /**
     * @brief Retrieves the list of infrastructure construction statuses.
     *
     * This function returns a constant reference to the `under_construction` vector, which contains the 
     * status (true or false) for each infrastructure item, indicating whether it is currently under construction.
     *
     * @return A constant reference to the vector containing the construction status of each infrastructure item.
     */
    const vector<bool> &getUnder_construction() const;

    /**
     * @brief Handles the construction of an intake expansion.
     *
     * This function processes the construction of an intake expansion. It updates the parent intake's capacity 
     * and treatment capacity based on the values from the intake expansion and sets the intake expansion as online.
     *
     * FIXME: [July 2019]:There may be an issue here if this were to be used for an allocated source
     *  where it mattered what utility ID was associated with the increase in treatment capacity
     *  but this addTreatmentCapacity function is not overrode within the Intake or IntakeExpansion class
     *  so it should be fine
     * 
     * @param source_id The ID of the intake expansion water source.
     */
    void intakeExpansionConstructionHandler(unsigned int source_id);

    /**
     * @brief Handles the construction of a joint water treatment plant (WTP) with either fixed or variable treatment allocations.
     *
     * This function handles the construction process of a joint water treatment plant (WTP), including updating the
     * treatment capacity of the associated parent reservoir. It sets the water treatment plant and the associated parent
     * reservoir online. The function accounts for both fixed and variable allocation types for joint WTPs.
     *
     * @param source_id The ID of the joint water treatment plant being constructed.
     * @param total_storage_capacity The total storage capacity to be updated during the process.
     * 
     * @throws runtime_error If there is an issue adding treatment capacity to the reservoir.
     * @throws logic_error If the water treatment plant type is not recognized.
     * 
     * @see InfrastructureManager::addWaterSourceToOnlineLists() for related water source handling and online status setting.
     */
    void waterTreatmentPlantJointConstructionHandler(unsigned int source_id, double &total_storage_capacity);
    
    /**
     * @brief Handles the construction of an allocated intake expansion and updates related capacities.
     *
     * This function manages the construction process of an allocated intake expansion. It updates the parent intake
     * with the new allocated capacity and treatment capacity, then sets the intake expansion source online.
     *
     * @param source_id The ID of the allocated intake expansion being constructed.
     * 
     * @throws runtime_error If there is an issue adding capacity or treatment capacity to the parent intake.
     * 
     * @see InfrastructureManager::addWaterSourceToOnlineLists() for related handling of water sources and setting online status.
     */
    void allocatedIntakeExpansionConstructionHandler(unsigned int source_id);

    /**
     * @brief Checks if a water source is part of a sequence of projects and adjusts capital costs based on the sequence.
     *
     * This function checks if the given water source is part of a sequence of infrastructure projects. If so, it ensures that the
     * previous parts of the sequence have been built or are under construction. If the prior projects are already completed, the
     * current project’s capital costs are adjusted accordingly.
     * 
     * @param original_build_id The ID of the water source to check and adjust costs for.
     * 
     * @throws logic_error If a project in the sequence is found but its construction status is undetermined.
     * 
     * @see InfrastructureManager::waterTreatmentPlantJointConstructionHandler() for related project construction handling.
     */
    void checkForSequenceProjects(int original_build_id);

    /**
     * @brief Retrieves the list of water sources in the ROF infrastructure construction order.
     *
     * This function returns a constant reference to the `rof_infra_construction_order` vector, which contains 
     * the list of water sources in the order they are to be constructed based on the ROF (Reliability of Flow) criteria.
     * 
     * @return A constant reference to the vector containing the order of infrastructure construction based on ROF.
     */
    const vector<int> &getRof_infra_construction_order() const;

    /**
     * @brief Retrieves the list of water sources in the demand infrastructure construction order.
     *
     * This function returns a constant reference to the `demand_infra_construction_order` vector, which contains 
     * the list of water sources in the order they are to be constructed based on demand criteria.
     * 
     * @return A constant reference to the vector containing the order of infrastructure construction based on demand.
     */
    const vector<int> &getDemand_infra_construction_order() const;

    /**
     * @brief Retrieves the list of infrastructure built in the last week.
     *
     * This function returns a constant reference to the `infra_built_last_week` vector, which contains the 
     * information about the infrastructure that was completed in the previous week, including the utility ID, 
     * the week of completion, and the infrastructure ID.
     *
     * @return A constant reference to the vector containing information about the infrastructure built in the last week.
     */
    const vector<int> &getInfra_built_last_week() const;
};


#endif //TRIANGLEMODEL_INFRASTRUCTUREMANAGER_H
