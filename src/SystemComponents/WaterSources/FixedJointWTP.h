/**
 * @class FixedJointWTP
 * @brief The `FixedJointWTP` is a subclass of the `JointWTP` class, which in turn is a subclass of the main `WaterSource` class.
 * This subclass represents the Joint WTP with fixed treatment capacity allocations to utilities.
 * 
 * Created by dgorelic on 10/28/2019.
 */

#ifndef TRIANGLEMODEL_FIXEDJOINTWTP_H
#define TRIANGLEMODEL_FIXEDJOINTWTP_H


#include "JointWTP.h"

class FixedJointWTP : public JointWTP {
private:
    /**
     * @brief Permanent treatment allocations for utilities.
     */
    vector<double> *permanent_treatment_allocations;

public:
    /**
     * @brief Constructs a FixedJointWTP object.
     * 
     * This constructor initializes an instance of the `FixedJointWTP` class, which represents a fixed allocation water treatment plant 
     * with a specified treatment capacity, parent reservoir, and various other parameters. It calls the base class `JointWTP` constructor 
     * and sets the `permanent_treatment_allocations` pointer.
     * 
     * @param name The name of the water treatment plant.
     * @param id The unique identifier for the water treatment plant.
     * @param parent_reservoir_ID The ID of the parent reservoir.
     * @param expansion_sequence_id The ID representing the expansion sequence of the plant.
     * @param total_treatment_capacity The total treatment capacity of the water treatment plant.
     * @param connected_sources A vector of integers representing the connected water sources to the treatment plant.
     * @param agreement_utility_ids A vector of integers representing the utility IDs associated with agreements for treatment capacity.
     * @param fixed_treatment_capacity_allocations A vector of doubles representing the fixed treatment capacity allocations for each utility.
     * @param bonds A vector of pointers to `Bond` objects representing the associated bonds with the treatment plant.
     * @param construction_time_range A vector of doubles representing the construction time range for the treatment plant.
     * @param permitting_period The permitting period for the plant.
     * 
     * @return A `FixedJointWTP` object initialized with the specified parameters.
     */
    FixedJointWTP(const char *name, const int id,
                  const int parent_reservoir_ID,
                  const int expansion_sequence_id,
                  const double total_treatment_capacity,
                  vector<int> connected_sources,
                  vector<int> &agreement_utility_ids,
                  vector<double> &fixed_treatment_capacity_allocations,
                  vector<Bond *> &bonds,
                  const vector<double> &construction_time_range,
                  double permitting_period);

    /**
     * @brief Copy constructor for the FixedJointWTP class.
     * 
     * This constructor creates a new `FixedJointWTP` object as a copy of the provided `FixedJointWTP` object. It copies all member 
     * variables from the given instance, including the `permanent_treatment_allocations` pointer.
     * 
     * @param fixed_joint_water_treatment_plant The `FixedJointWTP` object to copy.
     * 
     * @return A new `FixedJointWTP` object that is a copy of the provided object.
     */
    FixedJointWTP(const FixedJointWTP &fixed_joint_water_treatment_plant);

    /**
     * @brief Destructor for the FixedJointWTP class.
     * 
     * This destructor is the default destructor for the `FixedJointWTP` class and ensures proper cleanup of resources when 
     * an object of this class is destroyed. Since the class does not manage any dynamic memory or resources directly, 
     * the destructor simply calls the default destructor of its base class (`JointWTP`).
     * 
     * @return None
     */
    ~FixedJointWTP() override;

    /**
     * @brief Assignment operator for the FixedJointWTP class.
     * 
     * This operator allows for assignment of one `FixedJointWTP` object to another. It ensures that the base class (`JointWTP`)
     * assignment operator is properly invoked and then returns the current object for chained assignments.
     * 
     * @param fixed_joint_water_treatment_plant The `FixedJointWTP` object to be assigned to the current object.
     * 
     * @return A reference to the current `FixedJointWTP` object after the assignment.
     */
    FixedJointWTP &operator=(const FixedJointWTP &fixed_joint_water_treatment_plant);

    /**
     * @brief Applies continuity for the FixedJointWTP class.
     * 
     * This function calls the base class (`JointWTP`) implementation of `applyContinuity` to perform the necessary
     * continuity checks and calculations for water treatment. It handles the inflows and outflows, ensuring the
     * consistency of water volumes in the treatment process.
     * This function overrides the virtual method in the base `Reservoir` class.
     * 
     * @param week The week for which continuity is being applied.
     * @param upstream_source_inflow The volume of water inflow from upstream sources.
     * @param wastewater_discharge The volume of water from wastewater discharge.
     * @param demand_outflow A reference to a vector of water demand outflows for each utility.
     * 
     * @return None.
     */
    void applyContinuity(int week, double upstream_source_inflow,
                         double wastewater_discharge,
                         vector<double> &demand_outflow) override;


    /**
     * @brief Implements the initial treatment capacity for a specific utility.
     * 
     * This function retrieves the initial treatment capacity allocated to a utility from the permanent treatment allocations. 
     * It assumes that the indices in the  `permanent_treatment_allocations` vector correspond to the order of utility
     * IDs in the `agreement_utility_ids` vector. If the indices do not match, the returned value may be incorrect.
     * This function overrides the virtual method in the base `Reservoir` class.
     * 
     * @param utility_id The ID of the utility for which the initial treatment capacity is being retrieved.
     * 
     * @return The initial treatment capacity allocated to the specified utility.
     * 
     */
    double implementInitialTreatmentCapacity(int utility_id) override;
};


#endif //TRIANGLEMODEL_FIXEDJOINTWTP_H
