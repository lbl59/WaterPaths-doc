/**
 * @class JointWTP
 * @brief The `JointWTP` is a subclass of the main `WaterSource` class. This subclass represents the joint Jordan Lake Water Treatment Plant.
 * 
 * Created by dgorelic on 10/28/2019.
 */
#ifndef TRIANGLEMODEL_JOINTWTP_H
#define TRIANGLEMODEL_JOINTWTP_H


#include "Base/WaterSource.h"

class JointWTP : public WaterSource {
public:
    /**
     * @brief The low or high expansion ID of the JointWTP object.
     */
    const int expansion_sequence_id;
    /**
     * @brief The ID of the parent reservoir.
     */
    const unsigned int parent_reservoir_ID;
    /**
     * @brief The type of agreement for the JointWTP.
     */
    const int joint_agreement_type;

    /**
     * @brief Constructor for the JointWTP class.
     *
     * This constructor initializes a Joint Water Treatment Plant (WTP) with the given parameters, including the name, ID, agreement type, 
     * parent reservoir ID, expansion sequence, treatment capacity, connected sources, and various other settings. It also initializes the 
     * associated bonds and construction time range.
     *
     * @param name The name of the JointWTP.
     * @param id The unique identifier of the JointWTP.
     * @param agreement_type The agreement type for the JointWTP (e.g., shared, private).
     * @param parent_reservoir_ID The ID of the parent reservoir associated with the JointWTP.
     * @param expansion_sequence_id The ID that defines the sequence of expansion for the JointWTP (low/high).
     * @param total_treatment_capacity The total treatment capacity of the JointWTP.
     * @param connected_sources A vector containing the IDs of sources connected to the JointWTP.
     * @param agreement_utility_ids A vector containing the IDs of utilities involved in the agreement.
     * @param agreement_utility_treatment_capacities A vector containing the treatment capacities allocated to the utilities in the agreement.
     * @param bonds A vector containing bonds related to the JointWTP.
     * @param construction_time_range A vector specifying the construction time range for the JointWTP.
     * @param permitting_period The permitting period for the JointWTP.
     *
     * @return A new instance of the JointWTP class.
     */
    JointWTP(const char *name, const int id, const int agreement_type,
             const int parent_reservoir_ID,
             const int expansion_sequence_id,
             const double total_treatment_capacity,
             vector<int> connected_sources,
             vector<int> &agreement_utility_ids,
             vector<double> &agreement_utility_treatment_capacities,
             vector<Bond *> &bonds,
             const vector<double> &construction_time_range,
             double permitting_period);

    
    /**
     * @brief Copy constructor for the JointWTP class.
     *
     * This constructor creates a new instance of the JointWTP class by copying the data from an existing `joint_water_treatment_plant` object.
     * It initializes the new instance by copying the base class (`WaterSource`) data and the specific attributes of the `JointWTP` class.
     *
     * @param joint_water_treatment_plant The JointWTP instance to copy from.
     *
     * @return A new instance of the JointWTP class, with data copied from the given `joint_water_treatment_plant`.
     */
    JointWTP(const JointWTP &joint_water_treatment_plant);

    /**
     * @brief Destructor for the JointWTP class.
     *
     * This destructor is automatically invoked when an instance of the JointWTP class goes out of scope or is deleted.
     * It performs any necessary cleanup, but in this case, the destructor is defaulted and doesn't perform any custom actions.
     *
     * @return None
     */
    ~JointWTP() override;

    /**
     * @brief Assignment operator for the JointWTP class.
     * 
     * This operator allows for assignment of one `JointWTP` object to another. 
     * 
     * @param[in] joint_water_treatment_plant The `JointWTP` object to be assigned to the current object.
     * 
     * @return A reference to the current `JointWTP` object after the assignment.
     * 
     * @throws None.
     */
    JointWTP &operator=(const JointWTP &joint_water_treatment_plant);

    /**
     * @brief Applies continuity for the JointWTP class.
     *
     * This function is called to apply the continuity of the water treatment process. However, it is not designed to be called on a JointWTP instance.
     * It throws a `logic_error` to indicate that continuity should be applied only to the associated reservoir being expanded, not the JointWTP itself.
     * This function overrides the `applyContinuity` function in the base class `WaterSource`.
     *
     * @param week The current week in the simulation (used for inflow and outflow calculations).
     * @param upstream_source_inflow The volume of water inflow from upstream sources for the given week.
     * @param wastewater_discharge The volume of wastewater being discharged into the system.
     * @param demand_outflow A vector representing the water demand outflow for each utility or demand point.
     * 
     * @return None
     * @throws logic_error If called on a JointWTP instance, throws an error indicating that continuity should be applied to the reservoir being expanded, not the JointWTP.
     */
    void applyContinuity(int week, double upstream_source_inflow,
                         double wastewater_discharge,
                         vector<double> &demand_outflow) override;

    /**
     * @brief Implements the initial treatment capacity for the JointWTP class.
     *
     * This function is intended to be overridden in child classes of `JointWTP` to define how the initial treatment capacity is implemented.
     * When called on a `JointWTP` instance, it throws a `logic_error` because treatment capacity should be specified in the derived class.
     *
     * @param utility_id The ID of the utility for which the treatment capacity is being implemented.
     * 
     * @return Returns 0 as a placeholder, but an exception will be thrown before this value is returned.
     * @throws logic_error If called on a `JointWTP` instance, throws an error indicating that the function should be overridden in a derived class to handle the implementation of treatment capacity.
     */
    virtual double implementInitialTreatmentCapacity(int utility_id);

    /**
     * @brief Retrieves the agreement type of the JointWTP.
     *
     * This function returns the type of the agreement associated with the `JointWTP` instance.
     * This function overrides the `getAgreementType` function in the base class `WaterSource`.
     *
     * @return The agreement type as an integer.
     */
    int getAgreementType() const override;

    /**
     * @brief Retrieves the parent reservoir ID associated with the JointWTP.
     *
     * This function returns the ID of the reservoir that the `JointWTP` is associated with.
     * This function overrides the `getParentWaterSourceID` function in the base class `WaterSource`.
     *
     * @return The parent reservoir ID as an integer.
     */
    int getParentWaterSourceID() const override;

    /**
     * @brief Calculates the allocated treatment fraction for a given utility.
     *
     * This function computes the fraction of the total allocated treatment capacity for a specific utility
     * relative to the total fraction allocated across all utilities.
     * This function overrides the `getAllocatedTreatmentFraction` function in the base class `WaterSource`.
     *
     * @param utility_id The ID of the utility for which the allocated treatment fraction is requested.
     * @return The allocated treatment fraction for the specified utility.
     */
    double getAllocatedTreatmentFraction(int utility_id) const override;
};


#endif //TRIANGLEMODEL_JOINTWTP_H
