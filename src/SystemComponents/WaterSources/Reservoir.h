/**
 * @class Reservoir
 * @brief The `Reservoir` class is a subclass of the main `WaterSource` class. It represents a general reservoir object with a fixed or variable storage area curve.
 * 
 * Created by bernardoct on 1/12/17.
 */
#ifndef TRIANGLEMODEL_RESERVOIR_H
#define TRIANGLEMODEL_RESERVOIR_H

#include <string>
#include "../Catchment.h"
#include "Base/WaterSource.h"
#include "../../Utils/DataSeries.h"
#include "../../Controls/EvaporationSeries.h"

using namespace std;


class Reservoir : public WaterSource {
protected:
    /**
     * @brief The DataSeries object representing the storage area curve of the reservoir.
     */
    DataSeries *storage_area_curve;
    /**
     * @brief The area of the reservoir.
     */
    double area = NON_INITIALIZED;

public:
        /**
         * @brief The fixed area of the reservoir
         */
        const bool fixed_area;
        /**
         * @brief The EvaporationSeries object representing the evaporation series of the reservoir.
         */
        EvaporationSeries evaporation_series;

        /**
         * @brief Constructs a basic Reservoir object with a storage area curve. It also ensures that the 
         * last storage value in the storage area curve matches the reservoir's capacity.
         * 
         * Called for a reservoir that is already built and operational.
         * 
         * @param name The name of the reservoir as a C-style string.
         * @param id The unique identifier of the reservoir as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the reservoir.
         * @param capacity The total storage capacity of the reservoir in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the reservoir 
         * in cubic units per time unit.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the reservoir.
         * @param storage_area_curve A pointer to a DataSeries object representing the 
         * storage area curve for the reservoir.
         * @param source_type The type of the source (e.g., a constant or specific reservoir type).
         * 
         * @throws invalid_argument If the last storage value in the storage area curve 
         * does not match the reservoir's capacity.
         */
        Reservoir(
            const char *name, const int id,
            const vector<Catchment *> &catchments, const double capacity,
            const double max_treatment_capacity,
            EvaporationSeries &evaporation_series,
            DataSeries *storage_area_curve, int source_type = RESERVOIR);

        /**
         * @brief Constructs a Reservoir object with a storage area curve, treatment capacity. Also 
         * includes financing, construction, and permitting information.
         * 
         * Called when the reservoir does not yet exist at the beginning of the simulation.
         * 
         * This function initializes a Reservoir object with parameters such as the name, 
         * ID, catchments, capacity, maximum treatment capacity, evaporation series, 
         * storage area curve, and other construction and permitting details. It also ensures 
         * that the last storage value in the storage area curve matches the reservoir's capacity.
         * 
         * @param name The name of the reservoir as a C-style string.
         * @param id The unique identifier of the reservoir as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the reservoir.
         * @param capacity The total storage capacity of the reservoir in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the reservoir 
         * in cubic units per time unit.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the reservoir.
         * @param storage_area_curve A pointer to a DataSeries object representing the 
         * storage area curve for the reservoir.
         * @param construction_time_range A vector of doubles representing the start and end 
         * times of the construction period.
         * @param permitting_period The permitting period for the reservoir in time units.
         * @param bond A reference to a Bond object representing the financial bond associated 
         * with the reservoir.
         * @param source_type The type of the source (e.g., a constant or specific reservoir type).
         * 
         * @throws invalid_argument If the last storage value in the storage area curve 
         * does not match the reservoir's capacity.
         */
        Reservoir(const char *name, const int id, const vector<Catchment *> &catchments, const double capacity,
              const double max_treatment_capacity, EvaporationSeries &evaporation_series,
              DataSeries *storage_area_curve, const vector<double> &construction_time_range,
              double permitting_period, Bond &bond, int source_type = RESERVOIR);

        /**
         * @brief Constructs a Reservoir object with a fixed storage area, 
         * treatment capacity, and evaporation data. 
         * 
         * Called for a reservoir that is already built and operational.
         * 
         * @param name The name of the reservoir as a C-style string.
         * @param id The unique identifier of the reservoir as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the reservoir.
         * @param capacity The total storage capacity of the reservoir in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the reservoir 
         * in cubic units per time unit.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the reservoir.
         * @param storage_area The area of the reservoir in square units.
         * @param source_type The type of the source (e.g., a constant or specific reservoir type).
         */
        Reservoir(
            const char *name, const int id,
            const vector<Catchment *> &catchments, const double capacity,
            const double max_treatment_capacity,
            EvaporationSeries &evaporation_series, double storage_area,
            int source_type = RESERVOIR);

        /**
         * @brief Constructs a Reservoir object with a fixed storage area, treatment capacity. Also 
         * includes financing, construction, and permitting information.
         * 
         * Called when the reservoir does not yet exist at the beginning of the simulation.
         * 
         * This function initializes a Reservoir object with parameters such as the name, 
         * ID, catchments, capacity, maximum treatment capacity, evaporation series, 
         * storage area curve, and other construction and permitting details. It also ensures 
         * that the last storage value in the storage area curve matches the reservoir's capacity.
         * 
         * @param name The name of the reservoir as a C-style string.
         * @param id The unique identifier of the reservoir as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the reservoir.
         * @param capacity The total storage capacity of the reservoir in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the reservoir 
         * in cubic units per time unit.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the reservoir.
         * @param storage_area_curve A pointer to a DataSeries object representing the 
         * storage area curve for the reservoir.
         * @param construction_time_range A vector of doubles representing the start and end 
         * times of the construction period.
         * @param permitting_period The permitting period for the reservoir in time units.
         * @param bond A reference to a Bond object representing the financial bond associated 
         * with the reservoir.
         * @param source_type The type of the source (e.g., a constant or specific reservoir type).
         * 
         * @throws invalid_argument If the last storage value in the storage area curve 
         * does not match the reservoir's capacity.
         */
        Reservoir(const char *name, const int id, const vector<Catchment *> &catchments, const double capacity,
              const double max_treatment_capacity, EvaporationSeries &evaporation_series,
              double storage_area, const vector<double> &construction_time_range, double permitting_period,
              Bond &bond, int source_type = RESERVOIR);

        /**
         * @brief Constructs a Reservoir object with specified storage area curve. 
         * Includes additional details like treatment fractions and allocation details to connected utilities.
         * 
         * Called for a reservoir that is already built and operational.
         * 
         * @param name The name of the reservoir as a C-style string.
         * @param id The unique identifier of the reservoir as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the reservoir.
         * @param capacity The total storage capacity of the reservoir in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the reservoir 
         * in cubic units per time unit.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the reservoir.
         * @param storage_area_curve A pointer to a DataSeries object representing the 
         * storage area curve for the reservoir.
         * @param allocated_treatment_fractions A pointer to a vector of treatment 
         * fractions allocated to utilities.
         * @param allocated_fractions A pointer to a vector of fractions allocated to utilities.
         * @param utilities_with_allocations A pointer to a vector of integers representing 
         * the utilities that have allocations.
         * @param source_type The type of the source (e.g., a constant or specific reservoir type).
         * 
         * @throws invalid_argument If the last storage value in the storage area curve 
         * does not match the reservoir's capacity.
         */
        Reservoir(
            const char *name, const int id,
            const vector<Catchment *> &catchments, const double capacity,
            const double max_treatment_capacity,
            EvaporationSeries &evaporation_series,
            DataSeries *storage_area_curve,
            vector<double> *allocated_treatment_fractions,
            vector<double> *allocated_fractions,
            vector<int> *utilities_with_allocations,
            int source_type = RESERVOIR);

        /**
         * @brief Constructs a Reservoir object with specified storage area curve. 
         * Includes additional details like treatment fractions and allocation details to connected utilities.
         * 
         * Called for a reservoir that is already built and operational.
         * 
         * @param name The name of the reservoir as a C-style string.
         * @param id The unique identifier of the reservoir as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the reservoir.
         * @param capacity The total storage capacity of the reservoir in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the reservoir 
         * in cubic units per time unit.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the reservoir.
         * @param storage_area The area of the reservoir in square units.
         * @param allocated_treatment_fractions A pointer to a vector of treatment 
         * fractions allocated to utilities.
         * @param allocated_fractions A pointer to a vector of fractions allocated to utilities.
         * @param utilities_with_allocations A pointer to a vector of integers representing 
         * the utilities that have allocations.
         * @param source_type The type of the source (e.g., a constant or specific reservoir type).
         */
        Reservoir(
            const char *name, const int id,
            const vector<Catchment *> &catchments, const double capacity,
            const double max_treatment_capacity,
            EvaporationSeries &evaporation_series,
            double storage_area,
            vector<double> *allocated_treatment_fractions,
            vector<double> *allocated_fractions,
            vector<int> *utilities_with_allocations,
            int source_type = RESERVOIR);

        /**
         * @brief Constructs a Reservoir object with specified storage area curve. 
         * Includes additional details like treatment fractions and allocation details to connected utilities.
         * Also includes financing, construction, and permitting information.
         * 
         * Called when the reservoir does not yet exist at the beginning of the simulation.
         * 
         * @param name The name of the reservoir as a C-style string.
         * @param id The unique identifier of the reservoir as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the reservoir.
         * @param capacity The total storage capacity of the reservoir in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the reservoir 
         * in cubic units per time unit.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the reservoir.
         * @param storage_area_curve A pointer to a DataSeries object representing the 
         * storage area curve for the reservoir.
         * @param allocated_treatment_fractions A pointer to a vector of treatment 
         * fractions allocated to utilities.
         * @param allocated_fractions A pointer to a vector of fractions allocated to utilities.
         * @param utilities_with_allocations A pointer to a vector of integers representing 
         * the utilities that have allocations.
         * @param construction_time_range A vector of doubles representing the start and end 
         * times of the construction period.
         * @param permitting_period The permitting period for the reservoir in time units.
         * @param bond A reference to a Bond object representing the financial bond associated 
         * with the reservoir.
         * @param source_type The type of the source (e.g., a constant or specific reservoir type).
         * 
         * @throws invalid_argument If the last storage value in the storage area curve 
         * does not match the reservoir's capacity.
         */
        Reservoir(const char *name, const int id, const vector<Catchment *> &catchments, const double capacity,
              const double max_treatment_capacity, EvaporationSeries &evaporation_series,
              DataSeries *storage_area_curve, vector<double> *allocated_treatment_fractions,
              vector<double> *allocated_fractions, vector<int> *utilities_with_allocations,
              const vector<double> &construction_time_range, double permitting_period,
              Bond &bond, int source_type = RESERVOIR);

        /**
         * @brief Constructs a Reservoir object with fixed storage area. 
         * Includes additional details like treatment fractions and allocation details to connected utilities.
         * Also includes financing, construction, and permitting information.
         * 
         * Called when the reservoir does not yet exist at the beginning of the simulation.
         * 
         * @param name The name of the reservoir as a C-style string.
         * @param id The unique identifier of the reservoir as an integer.
         * @param catchments A vector of pointers to Catchment objects representing 
         * the catchments linked to the reservoir.
         * @param capacity The total storage capacity of the reservoir in cubic units.
         * @param max_treatment_capacity The maximum treatment capacity of the reservoir 
         * in cubic units per time unit.
         * @param evaporation_series A reference to an EvaporationSeries object 
         * containing evaporation data for the reservoir.
         * @param storage_area The area of the reservoir in square units.
         * @param allocated_treatment_fractions A pointer to a vector of treatment 
         * fractions allocated to utilities.
         * @param allocated_fractions A pointer to a vector of fractions allocated to utilities.
         * @param utilities_with_allocations A pointer to a vector of integers representing 
         * the utilities that have allocations.
         * @param construction_time_range A vector of doubles representing the start and end 
         * times of the construction period.
         * @param permitting_period The permitting period for the reservoir in time units.
         * @param bond A reference to a Bond object representing the financial bond associated 
         * with the reservoir.
         * @param source_type The type of the source (e.g., a constant or specific reservoir type).
         * 
         * @throws invalid_argument If the last storage value in the storage area curve 
         * does not match the reservoir's capacity.
         */
        Reservoir(const char *name, const int id, const vector<Catchment *> &catchments, const double capacity,
              const double max_treatment_capacity, EvaporationSeries &evaporation_series,
              double storage_area, vector<double> *allocated_treatment_fractions,
              vector<double> *allocated_fractions, vector<int> *utilities_with_allocations,
              const vector<double> &construction_time_range, double permitting_period,
              Bond &bond, int source_type = RESERVOIR);

        /**
         * @brief Copy constructor for creating a copy of an existing Reservoir object.
         * A new `EvaporationSeries` is created to ensure deep copying 
         * of the evaporation data.
         * 
         * @param reservoir The `Reservoir` object to copy.
         */
        Reservoir(const Reservoir &reservoir);

        /**
         * @brief Assignment operator for copying data from another Reservoir object.
         * 
         * This operator assigns the values from another `Reservoir` object to the current object, 
         * copying the storage area curve and performing a deep copy of the evaporation series. 
         * The base class `WaterSource`'s assignment operator is also called to copy its attributes.
         * 
         * @param reservoir The `Reservoir` object to copy data from.
         * 
         * @return A reference to the current `Reservoir` object after assignment.
         */
        Reservoir &operator=(const Reservoir &reservoir);

        /**
         * @brief Destructor for the Reservoir class.
         * 
         * This destructor is the default destructor for the `Reservoir` class and overrides 
         * the corresponding function in the base class. It ensures proper cleanup of any dynamically 
         * allocated resources when a `Reservoir` object is destroyed.
         */
        ~Reservoir() override;

        /**
         * @brief Applies continuity calculations for the reservoir over a given week.
         * 
         * This function calculates the new stored volume and outflow for the reservoir based 
         * on the continuity equation, taking into account inflows, demand outflows, evaporation, 
         * and spillage. It also updates various internal state variables like total demand, 
         * available volume, and total outflow.
         * 
         * This function overrides the `applyContinuity` function in the base class `WaterSource`.
         * 
         * @param week The week for which continuity calculations are performed.
         * @param upstream_source_inflow The inflow from upstream sources.
         * @param wastewater_inflow The inflow from wastewater sources.
         * @param demand_outflow A vector of demand outflows for the week.
         */
        void applyContinuity(int week, double upstream_source_inflow,
                             double wastewater_discharge,
                             vector<double> &demand_outflow) override;

        /**
         * @brief Marks the reservoir as online and initializes available volume.
         * 
         * This function sets the reservoir to an online state, where it begins accepting inflows. 
         * It also sets the `available_volume` to `NONE`, indicating that the reservoir starts 
         * empty and will gradually fill as inflows start coming in.
         * 
         * This function overrides the `setOnline` function in the base class `WaterSource`.
         */
        void setOnline() override;

        /**
         * @brief Sets the realization for the reservoir and updates the evaporation series.
         * 
         * This function sets the realization for the `Reservoir` object, which may include 
         * applying random decision-making (RDM) factors to the reservoir's behavior. It also 
         * updates the `evaporation_series` by setting its realization based on the provided factors.
         * 
         * This function overrides the `setRealization` function in the base class `WaterSource`.
         * 
         * @param r The realization index or identifier.
         * @param rdm_factors A reference to a vector of RDM factors to apply during the realization.
         */
        void setRealization(unsigned long r, vector<double> &rdm_factors) override;

        /**
         * @brief Returns the area of the reservoir.
         * 
         * This function retrieves the area of the reservoir, which is used to calculate 
         * various hydrological properties related to the reservoir's size.
         * 
         * @return The area of the reservoir as a `double`.
         */
        double getArea() const;
};


#endif //TRIANGLEMODEL_RESERVOIR_H
