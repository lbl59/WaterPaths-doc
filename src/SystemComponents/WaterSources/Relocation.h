/**
 * @class Relocation
 * @brief This subclass of `WaterSource` represents a relocation (reallocation) of water between utilities. 
 * This subclass does not have any water directly routed through it. Continuity equations therefore do not apply to it. 
 * It can only change allocated fractions in the reservoir/water source it is assigned to.
 * 
 * FIXME: Change name of class to reallocation.
 * 
 * Created by bernardoct on 7/25/17.
 */
#ifndef TRIANGLEMODEL_RESERVOIRRELLOCATION_H
#define TRIANGLEMODEL_RESERVOIRRELLOCATION_H


#include "Base/WaterSource.h"

class Relocation : public WaterSource {
public:
    /**
     * @brief The unique water source ID of the parent reservoir assigned to this relocation.
     */
    const unsigned long parent_reservoir_ID;
    /**
     * @brief The new allocated fractions for connected utilities.
     */
    const vector<double> *new_allocated_fractions;
    /**
     * @brief A list of the IDs of the utilities with allocations to the water source
     */
    const vector<int> *utilities_with_allocations;

    /**
     * @brief Constructs a Relocation object, initializing a water source linked 
     * to a parent reservoir with specified allocations and construction details.
     * 
     * @param name The name of the relocation water source as a C-style string.
     * @param id The unique identifier of the relocation water source as an integer.
     * @param parent_reservoir_ID The ID of the parent reservoir associated with this relocation.
     * @param allocated_fractions A pointer to a vector of fractions allocated to utilities.
     * @param utilities_with_allocations A pointer to a vector of integers representing 
     * the utilities that have allocations.
     * @param construction_time_range A vector of doubles representing the start and end 
     * times of the construction period.
     * @param permitting_period The permitting period for the relocation in time units.
     * @param bond A reference to a Bond object representing the financial bond associated 
     * with the relocation.
     */
    Relocation(const char *name, const int id, unsigned long parent_reservoir_ID,
               vector<double> *allocated_fractions, vector<int> *utilities_with_allocations,
               const vector<double> &construction_time_range, double permitting_period,
               Bond &bond);

    /**
     * @brief Constructs a Relocation object as a copy of an existing Relocation object.
     * 
     * @param relocation A reference to the existing Relocation object to copy.
     * 
     * @return A new Relocation object that is a copy of the provided `relocation`.
     */
    Relocation(const Relocation &relocation);

    /**
     * @brief Throws an exception when the applyContinuity function is called on a Relocation object. 
     * 
     * This function indicates that the `applyContinuity` method is not applicable to 
     * Relocation objects, as they only change allocated fractions in the source they are assigned to. 
     * Continuity calculations should be applied only to the source being relocated.
     * 
     * @param week The week number for which the continuity equation would be applied (if applicable).
     * @param upstream_source_inflow The inflow from the upstream source in cubic units.
     * @param wastewater_discharge The discharge from wastewater in cubic units.
     * @param demand_outflow A vector representing the demand outflow for the week.
     * 
     * @throws logic_error This function throws a `logic_error` exception, as continuity 
     * cannot be applied directly to a Relocation object.
     */
    void applyContinuity(int week, double upstream_source_inflow,
                             double wastewater_discharge,
                             vector<double> &demand_outflow) override;

    /**
     * @brief Retrieves the ID of the parent reservoir associated with the Relocation object.
     * 
     * @return The unique ID of the parent reservoir.
     */
    unsigned long getParent_reservoir_ID() const;
};


#endif //TRIANGLEMODEL_RESERVOIRRELLOCATION_H
