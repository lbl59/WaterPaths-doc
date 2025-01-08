//
// Created by bernardoct on 5/3/17.
//

#ifndef TRIANGLEMODEL_WATERREUSE_H
#define TRIANGLEMODEL_WATERREUSE_H


#include "Base/WaterSource.h"

class WaterReuse : public WaterSource {
private:
    /**
     * @brief The volume of water to be treated through the water reuse facility.
     */
    double treated_volume = NON_INITIALIZED;
public:
    /**
     * @brief Constructs a WaterReuse object.
     * 
     * Initializes a `WaterReuse` instance, setting its name, ID, capacity, and source type. The available volume is
     * initialized to the specified capacity.
     * 
     * @param name The name of the water reuse source.
     * @param id The ID of the water reuse source.
     * @param capacity The total capacity of the water reuse source.
     */
    WaterReuse(const char *name, const int id, const double capacity);

    /**
     * @brief Constructs a WaterReuse object with treatment capacity and additional attributes.
     * 
     * Initializes a `WaterReuse` instance specifically for types requiring a treatment capacity.
     * Also includes details on construction time range, permitting period, and bond. 
     * The available volume is initialized to the treatment capacity, and treated volume is also set to the treatment capacity.
     * 
     * @param name The name of the water reuse source.
     * @param id The ID of the water reuse source.
     * @param treatment_capacity The treatment capacity of the water reuse source.
     * @param construction_time_range The construction time range for the water reuse source.
     * @param permitting_period The permitting period for the water reuse source.
     * @param bond The bond associated with the water reuse source.
     */
    WaterReuse(const char *name, const int id, const double treatment_capacity,
                   const vector<double> &construction_time_range, double permitting_period,
                   Bond &bond);

    /**
     * @brief Copy constructor for the WaterReuse class.
     * 
     * Creates a new `WaterReuse` object by copying the attributes from an existing one.
     * This includes copying the base `WaterSource` attributes and the treated volume.
     * 
     * @param reuse The `WaterReuse` object to copy from.
     */
    WaterReuse(const WaterReuse &reuse);

    /**
     * @brief Assignment operator for the WaterReuse class.
     * 
     * This operator assigns the values from another WaterReuse object to the current one, 
     * copying the `available_volume` and the state from the `WaterSource` base class.
     * 
     * @param water_reuse The WaterReuse object to copy values from.
     */
    WaterReuse &operator=(const WaterReuse &water_reuse);

    /**
     * @brief Applies continuity calculations for the WaterReuse system.
     * 
     * This function calculates the total demand and updates the treated volume for the 
     * WaterReuse system based on the minimum of total demand and total treatment capacity.
     * 
     * @param week The current week of the simulation.
     * @param upstream_source_inflow The inflow from upstream sources to the system.
     * @param wastewater_discharge The amount of wastewater being discharged into the system.
     * @param demand_outflow A vector representing the demand outflows for the system.
     */
    void applyContinuity(int week, double upstream_source_inflow,
                             double wastewater_discharge,
                             vector<double> &demand_outflow) override;

    /**
     * @brief Retrieves the volume of water that has been reused.
     * 
     * This function returns the volume of water that has been treated and reused 
     * by the WaterReuse object, based on the current treatment capacity and demand.
     * 
     * @return The volume of water that has been reused.
     */
    double getReused_volume() const;

};

#endif //TRIANGLEMODEL_WATERREUSE_H
