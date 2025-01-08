/**
 * @brief The `AllocatedIntake` is a subclass of the main `WaterSource` class that represents a raw water intake from upstream catchments with allocations associated to utilities.
 * 
 * Created by dgorelic on 3/2/2020.
 */

#ifndef TRIANGLEMODEL_ALLOCATEDINTAKE_H
#define TRIANGLEMODEL_ALLOCATEDINTAKE_H


#include "Base/WaterSource.h"

class AllocatedIntake : public WaterSource {

public:
    /**
     * @brief The inflow from the adjacent upstream catchment.
     */
    double next_upstream_catchment_inflow = 0;
    /**
     * @brief The demands allocated to each utility to be fulfiled by this intake.
     */
    vector<double> allocated_demands;

    /**
     * @brief Constructor for creating an `AllocatedIntake` object.
     *
     * This constructor initializes an `AllocatedIntake` object, which represents a intake that has allocated raw water and treatment capacities, 
     * as well as associated utilities and catchments. 
     * It initializes various properties by invoking the base class constructor (`WaterSource`) and setting additional member variables such as `allocated_demands`.
     *
     * @param name The name of the intake.
     * @param id The unique identifier for the intake.
     * @param catchments A vector of pointers to the catchments associated with this intake.
     * @param connected_sources A vector of IDs for the connected sources of the intake.
     * @param partner_utilities A vector of IDs for the partner utilities associated with this intake.
     * @param total_raw_water_capacity The total raw water capacity of the intake.
     * @param raw_water_capacity_allocation_fractions A vector of fractions representing the allocation of raw water capacity to different utilities.
     * @param total_treatment_capacity The total treatment capacity of the intake.
     * @param treatment_capacity_allocation_fractions A vector of fractions representing the allocation of treatment capacity to different utilities.
     * 
     * @return A fully constructed `AllocatedIntake` object.
     */
    AllocatedIntake(
            const char *name, const int id, const vector<Catchment *> &catchments,
            vector<int> connected_sources, vector<int> &partner_utilities,
            const double total_raw_water_capacity,
            vector<double> &raw_water_capacity_allocation_fractions,
            const double total_treatment_capacity,
            vector<double> &treatment_capacity_allocation_fractions);

    /**
     * @brief Copy constructor for creating a new `AllocatedIntake` object by copying another `AllocatedIntake` object.
     *
     * This constructor initializes a new `AllocatedIntake` object by copying the values from an existing `AllocatedIntake` object. 
     * It delegates the initialization of the base class (`WaterSource`) to the base class copy constructor and copies the `allocated_demands` vector from the intake object.
     *
     * @param intake The `AllocatedIntake` object to copy from.
     * 
     * @return A new `AllocatedIntake` object that is a copy of the provided `intake` object.
     */
    AllocatedIntake(const AllocatedIntake &intake);

    /**
     * @brief Assignment operator for copying the contents of one `AllocatedIntake` object to another.
     *
     * This operator allows for the assignment of one `AllocatedIntake` object to another, copying the values of the base class (`WaterSource`) 
     * and the `allocated_demands` vector from the source object to the target object.
     *
     * @param intake The `AllocatedIntake` object to assign from.
     * 
     * @return A reference to the target `AllocatedIntake` object (`*this`).
     */
    AllocatedIntake &operator=(const AllocatedIntake &intake);

    /**
     * @brief Destructor for the `AllocatedIntake` class.
     *
     * This destructor is responsible for cleaning up the resources used by the `AllocatedIntake` object. 
     * It clears the `catchments` vector to free any associated memory when the object is destroyed.
     * 
     * This destructor overrides the base class destructor (`WaterSource`) to ensure proper cleanup.
     * 
     * @return void
     */
    ~AllocatedIntake() override;

    /**
     * @brief Applies the continuity equation to update water availability, demand, and allocations for the current time step.
     *
     * This function updates various properties related to water inflow, outflow, and demand based on the continuity equation. 
     * It computes the total upstream inflows, allocates available volumes to utilities based on their allocations, and updates the outflows and demands for the current week.
     * 
     * This function overrides the base class method `applyContinuity` to provide specific implementation for `AllocatedIntake`.
     *
     * @param week The current week for which the continuity equation is being applied.
     * @param upstream_source_min_env_flow The minimum environmental flow from upstream sources.
     * @param wastewater_inflow The inflow of wastewater.
     * @param demand A fraction of supply allocated to demands from each utility.
     * 
     * @return void
     */
    void applyContinuity(int week, double upstream_source_min_env_flow,
                         double wastewater_inflow, vector<double> &demand) override;

    /**
     * @brief Sets the realization for the `AllocatedIntake` object and updates water demand and availability.
     *
     * This function sets the realization for the `AllocatedIntake` object by calling the base class's `setRealization` method. 
     * It then updates the total demand and available volume based on the current upstream catchment inflow and the minimum environmental outflow.
     * 
     * This function overrides the base class method `setRealization` to provide specific implementation for `AllocatedIntake`.
     *
     * @param r The realization index (e.g., representing a specific scenario or time step).
     * @param rdm_factors A vector of random distribution factors used in the realization process.
     * 
     * @return void
     */
    void setRealization(unsigned long r, vector<double> &rdm_factors) override;

    /**
     * @brief Calculates the priority source potential volume for a given utility.
     *
     * This function calculates the potential volume of water available to a utility based on its allocated treatment capacity, current demand, 
     * environmental requirements, and upstream catchment inflow. The result is the maximum available volume that can be allocated to the utility, 
     * considering the constraints set by treatment capacities and environmental flows.
     * 
     * This function overrides the base class method `getPrioritySourcePotentialVolume` to provide specific implementation for `AllocatedIntake`.
     *
     * @param utility_id The ID of the utility for which the priority source potential volume is being calculated.
     * 
     * @return The calculated priority source potential volume for the specified utility.
     */
    double getPrioritySourcePotentialVolume(int utility_id) const override;

    /**
     * @brief Retrieves the allocated demands for all utilities.
     *
     * This function returns a vector containing the allocated demands for all utilities associated with the `AllocatedIntake` object. 
     * The demands are represented by the `allocated_demands` vector, which is updated based on the current inflows and allocations.
     * 
     * @return A vector of `double` representing the allocated demands for each utility.
     */
    vector<double> getAllocatedDemands() const;

    /**
     * @brief Adds capacity to the `AllocatedIntake` object and updates utility allocations.
     *
     * This function adds the specified capacity to both the total capacity of the `AllocatedIntake` object and the allocated capacity for a specific utility. 
     * It also ensures that the allocation fractions for all utilities are updated to reflect the new capacity.
     *
     * @param capacity The amount of capacity to add to the `AllocatedIntake`.
     * @param utility_id The ID of the utility for which the capacity is being added.
     * 
     * @return void
     */
    void addCapacity(double capacity, int utility_id);

    /**
     * @brief Adds treatment capacity to the `AllocatedIntake` object and updates utility allocations.
     *
     * This function adds the specified treatment capacity to both the total treatment capacity of the `AllocatedIntake` object 
     * and the allocated treatment capacity for a specific utility. 
     * It also ensures that the allocation fractions for all utilities are updated to reflect the new treatment capacity.
     *
     * @param added_treatment_capacity The amount of treatment capacity to add to the `AllocatedIntake`.
     * @param utility_id The ID of the utility for which the treatment capacity is being added.
     * 
     * @return void
     */
    void addTreatmentCapacity(const double added_treatment_capacity, int utility_id);

    /**
     * @brief Retrieves the available allocated volume for a specific utility.
     *
     * This function returns the available allocated volume for a given utility. 
     * The available volume is determined based on the current allocation and environmental conditions.
     * This function overrides the base class method `getAvailableAllocatedVolume` to provide specific implementation for `AllocatedIntake`.
     *
     * @param utility_id The ID of the utility for which the available allocated volume is being retrieved.
     * 
     * @return The available allocated volume for the specified utility.
     */
    double getAvailableAllocatedVolume(int utility_id) override;
};


#endif //TRIANGLEMODEL_ALLOCATEDINTAKE_H
