/**
 * @brief The `AllocatedIntakeExpansion` is a subclass of the main `WaterSource` class that represents a raw water intake from upstream catchments with allocations associated to utilities.
 * 
 * Created by dgorelic on 3/2/2020.
 */

#ifndef TRIANGLEMODEL_ALLOCATEDINTAKEEXPANSION_H
#define TRIANGLEMODEL_ALLOCATEDINTAKEEXPANSION_H


#include "Base/WaterSource.h"

class AllocatedIntakeExpansion : public WaterSource {

public:
    /**
     * @brief The water source ID of the parent intake object
     */
    const unsigned int parent_intake_ID;
    /**
     * @brief A vector of the supply capacity added to the intake for each utility with allocation to this intake.
     */
    const vector<double> supply_capacity_added;
    /**
     * @brief A vector of the treatment capacity added to the intake for each utility with allocation to this intake.
     */
    const vector<double> treatment_capacity_added;

    /**
     * @brief Constructor for creating an `AllocatedIntakeExpansion` object.
     *
     * This constructor initializes an `AllocatedIntakeExpansion` object by calling the base class's `WaterSource` constructor and setting additional 
     * specific attributes for the expansion, such as supply and treatment capacities, construction time range, and partner utilities. 
     * It also stores the parent intake ID and updates the relevant capacities for the expansion.
     *
     * @param name The name of the intake expansion.
     * @param id The unique identifier for the intake expansion.
     * @param parent_intake_ID The ID of the parent intake to which this expansion is associated.
     * @param total_supply_capacity_added The total additional supply capacity for the expansion.
     * @param total_treatment_capacity_added The total additional treatment capacity for the expansion.
     * @param partner_utility_ids A vector of IDs representing the utilities involved with this expansion.
     * @param supply_capacity_allocations_added A vector of allocation fractions for the added supply capacity.
     * @param treatment_capacity_allocations_added A vector of allocation fractions for the added treatment capacity.
     * @param construction_time_range A vector containing the range of possible construction times for the expansion.
     * @param connected_sources A vector of water source IDs connected to the intake expansion.
     * @param permitting_period The permitting period for the expansion.
     * @param bonds A vector of `Bond` objects associated with the intake expansion.
     * 
     * @return An `AllocatedIntakeExpansion` object initialized with the provided parameters.
     */
    AllocatedIntakeExpansion(const char *name, const int id, const unsigned int parent_intake_ID,
                    const double total_supply_capacity_added, double total_treatment_capacity_added,
                    vector<int> &partner_utility_ids,
                    vector<double> &supply_capacity_allocations_added,
                    vector<double> &treatment_capacity_allocations_added,
                    const vector<double> &construction_time_range, vector<int> connected_sources,
                    double permitting_period, vector<Bond *> &bonds);

    /**
     * @brief Copy constructor for creating a copy of an `AllocatedIntakeExpansion` object.
     *
     * This constructor creates a new `AllocatedIntakeExpansion` object by copying the values from another `AllocatedIntakeExpansion` object. 
     * It calls the base class's copy constructor to copy common attributes and also copies the specific attributes related to the intake expansion, 
     * such as the `parent_intake_ID`, `supply_capacity_added`, and `treatment_capacity_added`.
     *
     * @param intake_expansion The `AllocatedIntakeExpansion` object to copy.
     * 
     * @return A new `AllocatedIntakeExpansion` object that is a copy of the provided `intake_expansion`.
     */
    AllocatedIntakeExpansion(const AllocatedIntakeExpansion &intake_expansion);

    /**
     * @brief Assignment operator for copying an `AllocatedIntakeExpansion` object.
     *
     * This operator assigns the values from one `AllocatedIntakeExpansion` object to another, ensuring that all attributes are copied. 
     * It calls the base class's assignment operator to copy common attributes and handles the assignment of specific attributes related to the intake expansion.
     *
     * @param intake_expansion The `AllocatedIntakeExpansion` object to assign to the current object.
     * 
     * @return A reference to the current `AllocatedIntakeExpansion` object after the assignment.
     */
    AllocatedIntakeExpansion &operator=(
            const AllocatedIntakeExpansion &intake_expansion);

    /**
     * @brief Applies continuity for the intake expansion, updating the available flow and demand outflows.
     *
     * This function computes the continuity for the intake expansion based on the current week’s inflows, wastewater discharge, and the demand outflows. 
     * It updates the relevant internal variables to ensure the system’s water balance is maintained.
     * This function does not have a specific implementation in this subclass, inheriting its behavior from the base class.
     *
     * @param week The week for which the continuity is being applied.
     * @param upstream_source_inflow The inflow into the intake from the upstream source.
     * @param wastewater_discharge The volume of wasterwater discharged by the intake.
     * @param demand_outflow A vector containing the demand outflows for each associated utility.
     * 
     * @return None
     */
    void applyContinuity(int week, double upstream_source_inflow,
                         double wastewater_discharge,
                         vector<double> &demand_outflow) override;

    /**
     * @brief Retrieves the allocated supply capacity for a specific utility.
     *
     * This function locates the utility within the allocation list and returns the allocated supply capacity for the given utility. 
     * This function overrides the base class implementation to handle the specific allocation structure of the intake expansion.
     *
     * @param utility_id The ID of the utility whose allocated supply capacity is being requested.
     * 
     * @return The allocated supply capacity for the specified utility.
     * 
     * @throws logic_error If the utility ID cannot be found in the allocation list.
     */
    double getAllocatedCapacity(int utility_id) override;

    /**
     * @brief Retrieves the allocated treatment capacity for a specific utility.
     *
     * This function locates the utility within the allocation list and returns the allocated treatment capacity for the given utility. 
     * If the utility ID cannot be found, a logic error is thrown.
     * This function overrides the base class implementation to handle the specific allocation structure of the intake expansion.
     *
     * @param utility_id The ID of the utility whose allocated treatment capacity is being requested.
     * 
     * @return The allocated treatment capacity for the specified utility.
     * 
     * @throws logic_error If the utility ID cannot be found in the allocation list.
     */
    double getAllocatedTreatmentCapacity(int utility_id) const override;

    /**
     * @brief Retrieves the bond associated with a specific utility.
     *
     * This function locates the utility within the allocation list and retrieves the corresponding bond object. 
     * If the utility ID cannot be found, a logic error is thrown.
     * This function overrides the base class implementation to handle the specific allocation structure of the intake expansion.
     *
     * @param utility_id The ID of the utility whose bond is being requested.
     * 
     * @return A reference to the `Bond` object associated with the specified utility.
     * 
     * @throws logic_error If the utility ID cannot be found in the allocation list or if no bond is associated with the utility.
     */
    Bond &getBond(int utility_id) override;
};


#endif //TRIANGLEMODEL_ALLOCATEDINTAKEEXPANSION_H
