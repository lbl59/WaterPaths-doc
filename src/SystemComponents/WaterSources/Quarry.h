/**
 * @class Quarry
 * @brief The `Quarry` is a subclass of the `Reservoir` class, which in turn is a subclass of the main `WaterSource` class.
 * It represents a quarry reservoir that diverts water from upstream catchments.
 * 
 * Created by bernardoct on 5/3/17.
 */
#ifndef TRIANGLEMODEL_QUARRY_H
#define TRIANGLEMODEL_QUARRY_H


#include "Reservoir.h"

class Quarry : public Reservoir {
private:
        /**
         * @brief The maximum volume of water that can be diverted through the quarry.
         */
        double max_diversion = NON_INITIALIZED;
        /**
         * @brief The volume of water diverted through the quarry.
         */
        double diverted_flow = NON_INITIALIZED;

public:
        /**
         * @brief Constructs a Quarry object. This function initializes a quarry reservoir
         * with specified parameters such as name, ID, catchments, capacity, treatment 
         * capacity, evaporation data, and more.
         * 
         * @param name The name of the quarry as a C-style string.
         * @param id The unique identifier of the quarry as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the quarry.
         * @param capacity The total storage capacity of the quarry in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the quarry.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the quarry.
         * @param storage_area_curve A pointer to a DataSeries object defining the 
         * relationship between storage and surface area of the quarry. Can be null.
         * @param max_diversion The maximum diversion flow rate allowed from the 
         * quarry.
         */
        Quarry(
            const char *name, const int id,
            const vector<Catchment *> &catchments, const double capacity,
            const double max_treatment_capacity,
            EvaporationSeries &evaporation_series,
            DataSeries *storage_area_curve, double max_diversion);

        /**
         * @brief Constructs a Quarry object. This function initializes a quarry reservoir
         * with specified parameters such as name, ID, catchments, capacity, treatment 
         * capacity, evaporation data. It also includes additional parameters that define the
         * construction timeline, and financial bond details.
         * 
         * @param name The name of the quarry as a C-style string.
         * @param id The unique identifier of the quarry as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the quarry.
         * @param capacity The total storage capacity of the quarry in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the quarry.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the quarry.
         * @param storage_area_curve A pointer to a DataSeries object defining the 
         * relationship between storage and surface area of the quarry. Can be null.
         * @param construction_time_range A vector of doubles representing the start and end 
         * times of the quarry's construction period.
         * @param permitting_period The permitting period for the quarry in time units.
         * @param bond A reference to a Bond object representing the financial bond associated 
         * with the quarry.
         * @param max_diversion The maximum diversion flow rate allowed from the 
         * quarry.
         */
        Quarry(const char *name, const int id, const vector<Catchment *> &catchments, const double capacity,
           const double max_treatment_capacity, EvaporationSeries &evaporation_series,
           DataSeries *storage_area_curve, const vector<double> &construction_time_range,
           double permitting_period, Bond &bond, double max_diversion);

        /**
         * @brief Constructs a Quarry object. This function initializes a quarry reservoir
         * with specified parameters such as name, ID, catchments, capacity, treatment 
         * capacity, evaporation data. It specifies a fixed storage area instead of a storage area curve.
         * 
         * @param name The name of the quarry as a C-style string.
         * @param id The unique identifier of the quarry as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the quarry.
         * @param capacity The total storage capacity of the quarry in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the quarry .
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the quarry.
         * @param storage_area The surface area of the quarry in square units.
         * @param max_diversion The maximum diversion flow rate allowed from the quarry.
         */
        Quarry(
            const char *name, const int id,
            const vector<Catchment *> &catchments, const double capacity,
            const double max_treatment_capacity,
            EvaporationSeries &evaporation_series, double storage_area,
            double max_diversion);

        /**
         * @brief Constructs a Quarry object. This function initializes a quarry reservoir
         * with specified parameters such as name, ID, catchments, capacity, treatment 
         * capacity, evaporation data. It also includes additional parameters that define the
         * construction timeline, and financial bond details. It specifies a fixed storage area 
         * instead of a storage area curve.
         *  
         * @param name The name of the quarry as a C-style string.
         * @param id The unique identifier of the quarry as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the quarry.
         * @param capacity The total storage capacity of the quarry in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the quarry.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the quarry.
         * @param storage_area The surface area of the quarry in square units.
         * @param construction_time_range A vector of doubles representing the start and end 
         * times of the quarry's construction period.
         * @param permitting_period The permitting period for the quarry in time units.
         * @param bond A reference to a Bond object representing the financial bond associated 
         * with the quarry.
         * @param max_diversion The maximum diversion flow rate allowed from the quarry.
         */
        Quarry(const char *name, const int id, const vector<Catchment *> &catchments, const double capacity,
           const double max_treatment_capacity, EvaporationSeries &evaporation_series, double storage_area,
           const vector<double> &construction_time_range, double permitting_period, Bond &bond,
           double max_diversion);

        /**
         * @brief Constructs a Quarry object as a copy of an existing Quarry object, 
         * with an option to set a new maximum diversion rate by overriding the max diversion 
         * of the original Quarry object.
         * 
         * @param quarry A reference to the existing Quarry object to copy.
         * @param max_diversion The maximum diversion flow rate allowed through the 
         * new quarry. This overrides the value from 
         * the copied Quarry object.
         */
        Quarry(const Quarry &quarry, const double max_diversion);

        /**
         * @brief Overloads the assignment operator for the Quarry class, allowing 
         * one Quarry object to be assigned to another.
         * 
         * This function copies the properties of an existing Quarry object, including 
         * the maximum diversion rate, to the current Quarry object.
         * 
         * @param quarry A reference to the existing Quarry object to copy.
         * 
         * @return A reference to the current Quarry object after assignment.
         */
        Quarry &operator=(const Quarry &quarry);

        /**
         * @brief Destructor for the Quarry class. 
         * 
         * This function cleans up resources used by the Quarry object. Since the 
         * class does not dynamically allocate resources directly, the destructor 
         * relies on the base class (`Reservoir`) and standard cleanup mechanisms.
         */
        ~Quarry();

        /**
         * @brief Applies the continuity equation to update the Quarry's inflows, outflows, 
         * and available volume based on weekly data.
         * 
         * This function performs reservoir mass balance by using the total inflow, demand outflow, 
         * and updates the available volume and total outflow based on inflows from upstream sources, 
         * wastewater, combining them with catchment inflows. The function also ensures that the quarry 
         * maintains its capacity limits and adjusts outflows accordingly.
         * 
         * @param week The week number for which the continuity equation is applied.
         * @param upstream_source_inflow The inflow from the upstream source in cubic units.
         * @param wastewater_inflow The inflow from wastewater in cubic units.
         * @param demand_outflow A vector representing the demand outflow for the week.
         */
        void applyContinuity(
            int week, double upstream_source_inflow, double wastewater_inflow,
            vector<double> &demand_outflow) override;

        /**
         * @brief Sets the Quarry object to an online state. 
         * 
         * This function transitions the Quarry to an operational state by marking it 
         * as online. The available volume is initially set to `NONE`, simulating an 
         * empty reservoir that begins to fill as inflows occur.
         */
        void setOnline() override;
};


#endif //TRIANGLEMODEL_QUARRY_H
