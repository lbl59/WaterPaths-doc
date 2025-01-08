//
// Created by bernardoct on 7/18/17.
//

#ifndef TRIANGLEMODEL_JOINTWATERTREATMENTPLANT_H
#define TRIANGLEMODEL_JOINTWATERTREATMENTPLANT_H


#include "Base/WaterSource.h"

class SequentialJointTreatmentExpansion : public WaterSource {
private:

    /**
     * @brief The total treatment capacity of the joint treatment plant.
     */
    double total_treatment_capacity;
    /**
     * @brief The treatment capacity added at each expansion.
     */
    vector<double> &added_treatment_capacities;

public:

    /**
     * @brief The ID of the expansion sequence to which this expansion belongs.
     */
    const int expansion_sequence_id;

    /**
     * @brief The ID of the parent reservoir to which this expansion is associated.
     */
    const unsigned int parent_reservoir_ID;

    /**
     * @brief Constructor for creating a SequentialJointTreatmentExpansion object.
     * 
     * This constructor initializes a `SequentialJointTreatmentExpansion` object with the specified parameters 
     * and sets various attributes including the treatment capacities, expansion sequence ID, and parent reservoir ID. 
     * It also calls the constructor of the base class `WaterSource` to initialize shared attributes.
     * 
     * @param name The name of the sequential joint treatment expansion.
     * @param id The unique identifier for the expansion.
     * @param parent_reservoir_ID The ID of the parent reservoir to which the expansion is associated.
     * @param expansion_sequence_id The ID that represents the sequence in which this expansion is applied.
     * @param connected_sources A list of sources connected to this expansion.
     * @param sequential_treatment_capacity The capacities of the sequential treatment process.
     * @param bonds A vector of bonds associated with the expansion.
     * @param construction_time_range The time range for constructing the expansion.
     * @param permitting_period The permitting period for the expansion.
     * 
     * @throws invalid_argument If the `sequential_treatment_capacity` vector is not empty when initialized.
     */
    SequentialJointTreatmentExpansion(const char *name, const int id,
                                      const int parent_reservoir_ID,
                                      const int expansion_sequence_id,
                                      vector<int> connected_sources,
                                      vector<double> &sequential_treatment_capacity,
                                      vector<Bond *> &bonds,
                                      const vector<double> &construction_time_range,
                                      double permitting_period);

    /**
     * @brief Copy constructor for creating a copy of a SequentialJointTreatmentExpansion object.
     * 
     * This constructor initializes a new `SequentialJointTreatmentExpansion` object by copying the values 
     * from an existing `SequentialJointTreatmentExpansion` object. It also calls the base class copy constructor.
     * 
     * @param joint_water_treatment_plant The `SequentialJointTreatmentExpansion` object to copy from.
     */
    SequentialJointTreatmentExpansion(
            const SequentialJointTreatmentExpansion &joint_water_treatment_plant);

    /**
     * @brief Destructor for the SequentialJointTreatmentExpansion class.
     * 
     * This destructor is the default destructor, which ensures proper cleanup of resources when a 
     * `SequentialJointTreatmentExpansion` object is destroyed.
     */
    ~SequentialJointTreatmentExpansion() override;

    /**
     * @brief Assignment operator for assigning one SequentialJointTreatmentExpansion object to another.
     * 
     * This operator assigns the values from one `SequentialJointTreatmentExpansion` object to another, 
     * using the base class assignment operator for common attributes.
     * 
     * @param joint_water_treatment_plant The `SequentialJointTreatmentExpansion` object to assign from.
     * 
     * @return A reference to the current `SequentialJointTreatmentExpansion` object.
     */
    SequentialJointTreatmentExpansion &operator=(
            const SequentialJointTreatmentExpansion &joint_water_treatment_plant);

    /**
     * @brief Applies continuity for the SequentialJointTreatmentExpansion object.
     * 
     * This method throws a `logic_error` because `SequentialJointTreatmentExpansion` only adds 
     * storage volume to the reservoir it's assigned to, and continuity cannot be applied to it directly.
     * Continuity must be applied to the reservoir it expands.
     * 
     * @param week The week for which the continuity is applied.
     * @param upstream_source_inflow The amount of water inflow from upstream sources.
     * @param wastewater_discharge The amount of wastewater discharged.
     * @param demand_outflow A vector representing the demand outflow.
     * 
     * @throws logic_error This function throws a `logic_error` indicating that continuity cannot be applied
     *         directly to a `SequentialJointTreatmentExpansion`.
     */
    void applyContinuity(int week, double upstream_source_inflow,
                             double wastewater_discharge,
                             vector<double> &demand_outflow) override;

    
    /**
     * @brief Returns the treatment capacity added for a specific utility.
     * 
     * This fuynction returns the capacity to be installed for a given utility and 
     * deducts it from maximum planned expansion.
     * 
     * @param utility_id The ID of the utility for which the treatment capacity is requested.
     * 
     * @return The added treatment capacity for the specified utility.
     */
    double implementTreatmentCapacity(int utility_id);

};


#endif //TRIANGLEMODEL_JOINTWATERTREATMENTPLANT_H
