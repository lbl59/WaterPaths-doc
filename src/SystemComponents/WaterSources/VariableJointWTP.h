/**
 * @brief This is a subclass of JointWTP that allows for variable allocations of treatment capacity to utilities. 
 * The JointWTP subclass is in turn a subclass of WaterSource.
 * 
 * Created by dgorelic on 10/29/2019.
 * 
 * FIXME: Unclear what the purpose of this class is and how it differs from JointWTP.
 */

#ifndef TRIANGLEMODEL_VARIABLEJOINTWTP_H
#define TRIANGLEMODEL_VARIABLEJOINTWTP_H


#include "JointWTP.h"

class VariableJointWTP : public JointWTP {
private:
    /**
     * @brief The initial allocations for the treatment capacity of this JointWTP.
     */
    vector<double> initial_treatment_allocations;

public:
    /**
     * @brief Constructs a `VariableJointWTP` object with the specified parameters.
     * 
     * This constructor initializes a `VariableJointWTP` object, which represents a joint water treatment plant
     * with variable treatment capacity allocations, and associates it with a specific parent reservoir.
     * 
     * @param name The name of the joint water treatment plant.
     * @param id The ID of the joint water treatment plant.
     * @param parent_reservoir_ID The ID of the parent reservoir for the joint water treatment plant.
     * @param expansion_sequence_id The expansion sequence ID for the joint water treatment plant.
     * @param total_treatment_capacity The total treatment capacity of the plant.
     * @param connected_sources A list of source IDs connected to the plant.
     * @param agreement_utility_ids A list of utility IDs that have agreements with the plant.
     * @param initial_treatment_capacity_allocations A list of initial treatment capacity allocations for each utility.
     * @param bonds A list of bonds associated with the plant.
     * @param construction_time_range A vector specifying the time range for construction.
     * @param permitting_period The permitting period for the plant.
     */
    VariableJointWTP(const char *name, const int id,
                     const int parent_reservoir_ID,
                     const int expansion_sequence_id,
                     const double total_treatment_capacity,
                     vector<int> connected_sources,
                     vector<int> &agreement_utility_ids,
                     vector<double> &initial_treatment_capacity_allocations,
                     vector<Bond *> &bonds,
                     const vector<double> &construction_time_range,
                     double permitting_period);

    /**
     * @brief Copy constructor for the `VariableJointWTP` class.
     * 
     * This constructor creates a new `VariableJointWTP` object as a copy of an existing one,
     * inheriting the properties and treatment capacity allocations from the original plant.
     * 
     * @param variable_joint_water_treatment_plant The `VariableJointWTP` object to copy from.
     */
    VariableJointWTP(const VariableJointWTP &variable_joint_water_treatment_plant);

    /**
     * @brief Destructor for the `VariableJointWTP` class. 
     * 
     * This function cleans up resources used by the Quarry object. Since the 
     * class does not dynamically allocate resources directly, the destructor 
     * relies on the base class (`WaterSource`) and standard cleanup mechanisms.
     */
    ~VariableJointWTP() override;

    /**
     * @brief Assignment operator for copying data for another `VariableJointWTP` object.
     * 
     * This operator assigns the values from another `VariableJointWTP` object to the current object, 
     * copying the storage area curve and performing a deep copy of the evaporation series. 
     * The base class `WaterSource`'s assignment operator is also called to copy its attributes.
     * 
     * @param variable_joint_water_treatment_plant The `VariableJointWTP` object to copy data from.
     * 
     * @return A reference to the current `VariableJointWTP` object after assignment.
     */
    VariableJointWTP &operator=(const VariableJointWTP &variable_joint_water_treatment_plant);

    /**
     * @brief Applies continuity for the `VariableJointWTP` class.
     * 
     * This function calls the base class `JointWTP`'s `applyContinuity` method to perform the continuity calculations
     * for the water treatment plant, considering the upstream inflow, wastewater discharge, and demand outflow.
     * 
     * @param week The week for which the continuity is applied.
     * @param upstream_source_inflow The inflow from the upstream source.
     * @param wastewater_discharge The discharge of wastewater into the plant.
     * @param demand_outflow A vector containing the demand outflow for each utility.
     */
    void applyContinuity(int week, double upstream_source_inflow,
                         double wastewater_discharge,
                         vector<double> &demand_outflow) override;

    /**
     * @brief Implements the initial treatment capacity for a specific utility.
     * 
     * This function returns the treatment capacity allocated to a utility based on the initial treatment allocations.
     * 
     * @param utility_id The ID of the utility for which the treatment capacity is retrieved.
     * 
     * @return The initial treatment capacity allocated to the specified utility.
     */
    double implementInitialTreatmentCapacity(int utility_id) override;

    /**
     * @brief Resets the treatment allocations based on demand deltas for the utilities.
     * 
     * This function updates the treatment capacity allocations for each utility based on changes in demand. It adjusts
     * the treatment allocations for the current year based on the estimated future demand deltas. If the allocations
     * exceed the total treatment capacity, the fractions and capacities are adjusted accordingly to ensure proper allocation.
     * 
     * @param demand_deltas A vector containing the estimated demand changes for each utility.
     */
    void resetAllocations(const vector<double> *demand_ratios) override; // for storage capacity in WaterSource, for treatment capacity here
};


#endif //TRIANGLEMODEL_VARIABLEJOINTWTP_H
