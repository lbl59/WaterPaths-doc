/**
 * @class Intake
 * @brief The `Intake` is a subclass of the main `WaterSource` class that represents a pipeline that draws water from a surface water source.
 * 
 * Created by bernardo on 2/10/17.
 */
#ifndef TRIANGLEMODEL_INTAKE_H
#define TRIANGLEMODEL_INTAKE_H


#include "Reservoir.h"

using namespace Constants;

class Intake : public WaterSource {

public:

        /**
         * @brief The inflow to the intake from the adjacent upstream catchment.
         */
        double next_upstream_catchment_inflow = 0;

        /**
         * @brief Constructs an Intake object with the specified properties.
         * 
         * This constructor initializes an `Intake` object, which represents a water source
         * typically used for the intake of water for treatment. It inherits from the `WaterSource` class,
         * setting up the properties of the intake such as name, ID, associated catchments, and maximum
         * treatment capacity.
         * 
         * @param name The name of the intake.
         * @param id The unique identifier for the intake.
         * @param catchments A vector of pointers to `Catchment` objects associated with this intake.
         * @param max_treatment_capacity The maximum treatment capacity of the intake.
         * 
         * @return A constructed `Intake` object with the provided properties.
         */      
        Intake(
                const char *name, const int id,
                const vector<Catchment *> &catchments,
                const double max_treatment_capacity);

        /**
         * @brief Constructs an Intake object with the specified properties, including raw water capacity.
         * 
         * This constructor initializes an `Intake` object, which represents a water source typically used
         * for the intake of raw water for treatment. It inherits from the `WaterSource` class, setting up
         * the properties of the intake such as name, ID, associated catchments, raw water capacity, and
         * maximum treatment capacity.
         * 
         * @param name The name of the intake.
         * @param id The unique identifier for the intake.
         * @param catchments A vector of pointers to `Catchment` objects associated with this intake.
         * @param raw_water_capacity The raw water intake capacity of the intake.
         * @param max_treatment_capacity The maximum treatment capacity of the intake.
         * 
         * @return A constructed `Intake` object with the provided properties.
         */
        Intake(
                const char *name, const int id, const vector<Catchment *> &catchments,
                const double raw_water_capacity, const double max_treatment_capacity);

        /**
         * @brief Constructor for the Intake class.
         *
         * This function initializes the Intake class with the details on construction, permitting, and 
         * financing.
         *
         * @param name The name of the intake.
         * @param id The ID of the intake.
         * @param catchments A vector of Catchment objects associated with this intake.
         * @param connected_sources A vector of integers representing the connected water sources.
         * @param raw_water_main_capacity The capacity for raw water intake.
         * @param construction_time_range A vector containing the time range for construction.
         * @param permitting_period The permitting period for the intake.
         * @param bond The Bond object associated with the intake.
         * 
         * @return A new instance of the Intake class initialized with the provided values.
         */
        Intake(const char *name, const int id, const vector<Catchment *> &catchments, vector<int> connected_sources,
                const double raw_water_main_capacity, const vector<double> construction_time_range,
                double permitting_period, Bond &bond);

        /**
         * @brief Copy constructor for the Intake class.
         *
         * This constructor initializes a new Intake object as a copy of an existing Intake object.
         *
         * @param intake The Intake object to copy.
         * 
         * @return A new instance of the Intake class that is a copy of the provided Intake.
         */
        Intake(const Intake &intake);

        /**
         * @brief Assignment operator for the Intake class.
         *
         * This operator assigns the values of an existing Intake object to the current object.
         *
         * @param intake The Intake object to assign from.
         * 
         * @return A reference to the current Intake object after the assignment.
         */
        Intake &operator=(const Intake &intake);

        /**
         * @brief Destructor for the Intake class.
         *
         * This destructor clears the list of catchments associated with the Intake object.
         * This destructor overrides the default destructor for the WaterSource class.
         *
         * @return None
         */
        ~Intake() override;

        /**
         * @brief Applies continuity to update the water availability and demand for the Intake system.
         *
         * This function calculates the total upstream inflow, downstream demand, and the water availability for
         * the next time step. It also computes the total outflow based on the given upstream source inflow, wastewater
         * inflow, and demand outflow.
         * This function overrides the applyContinuity function in the WaterSource class.
         * 
         * FIXME: WHEN IMPLEMENTING RESERVOIR RULES, ADD A "BANK" OF WATER OVER AND UNDERUSE TO BE ADDED/SUBTRACTED FROM THE AVAILABLE VOLUME FOR THE NEXT REALIZATION.
         *
         * @param week The current week for which continuity is being applied.
         * @param upstream_source_inflow The upstream source inflow during the current week.
         * @param wastewater_inflow The wastewater inflow during the current week.
         * @param demand The demand for water for each connected utility or system during the current week.
         * 
         * @return None
         */
        void applyContinuity(int week, double upstream_source_min_env_flow,
                                double wastewater_inflow, vector<double> &demand) override;

        /**
         * @brief Sets the realization for the Intake system based on random factors.
         *
         * This function updates the total demand and available volume for the Intake system
         * based on a new realization and corresponding random factors.
         * Thid function overrides the setRealization function in the WaterSource class.
         *
         * @param r The realization index, used to set the state for the given realization.
         * @param rdm_factors The random factors that influence the realization.
         * 
         * @return None
         */
        void setRealization(unsigned long r, vector<double> &rdm_factors) override;

        /**
         * @brief Gets the potential volume available from the priority source.
         *
         * This function calculates the remaining "storage" capacity after accounting for weekly demands
         * and environmental flows. The volume is based on the treatment capacity and inflows.
         * This function overrides the getPrioritySourcePotentialVolume function in the WaterSource class.
         *
         * @param utility_id The utility identifier. Currently unused in this method but kept for consistency.
         * 
         * @return The remaining potential volume available from the priority source.
         */
        double getPrioritySourcePotentialVolume(int utility_id) const override;

};

#endif //TRIANGLEMODEL_INTAKE_H
