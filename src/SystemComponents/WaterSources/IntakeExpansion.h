//
// Created by dgorelic on 7/8/2019.
//

#ifndef TRIANGLEMODEL_INTAKEEXPANSION_H
#define TRIANGLEMODEL_INTAKEEXPANSION_H

#include "Base/WaterSource.h"

class IntakeExpansion : public WaterSource {
public:

        /**
         * @brief The ID of the parent intake.
         */
        const unsigned int parent_intake_ID;

        /**
         * @brief Constructs an IntakeExpansion object.
         *
         * This constructor initializes the IntakeExpansion object, which represents the expansion
         * of an existing intake system by adding extra capacity and treatment capacity.
         *
         * @param name The name of the intake expansion.
         * @param id The unique identifier for the intake expansion.
         * @param parent_intake_ID The ID of the parent intake from which this expansion is derived.
         * @param capacity_added The additional capacity added to the intake.
         * @param treatment_capacity_added The additional treatment capacity added to the intake.
         * @param construction_time_range The time range for the construction of the expansion.
         * @param connected_sources The list of connected sources to the intake expansion.
         * @param permitting_period The permitting period for the expansion.
         * @param bond The bond associated with the intake expansion.
         *
         * @return A new IntakeExpansion object.
         */
        IntakeExpansion(const char *name, const int id, const unsigned int parent_intake_ID,
                        const double capacity_added, const double treatment_capacity_added,
                        const vector<double> &construction_time_range, vector<int> connected_sources,
                        double permitting_period, Bond &bond);

        /**
         * @brief Copy constructor for the IntakeExpansion class.
         *
         * This constructor creates a new `IntakeExpansion` object by copying the properties of an existing one.
         * It ensures that both the `WaterSource` and `parent_intake_ID` are properly copied.
         *
         * @param intake_expansion The `IntakeExpansion` object to copy.
         *
         * @return A new `IntakeExpansion` object with the same properties as `intake_expansion`.
         */
        IntakeExpansion(const IntakeExpansion &intake_expansion);

        /**
         * @brief Assignment operator for the IntakeExpansion class.
         *
         * This operator assigns the properties of one `IntakeExpansion` object to another.
         * It ensures that both the `WaterSource` and `parent_intake_ID` are correctly assigned.
         *
         * @param intake_expansion The `IntakeExpansion` object to assign from.
         *
         * @return A reference to the current `IntakeExpansion` object after the assignment.
         */
        IntakeExpansion &operator=(
                const IntakeExpansion &intake_expansion);

        /**
         * @brief Applies continuity for the IntakeExpansion class.
         *
         * This function applies the continuity equation for the intake expansion process. 
         * This function does not have a specific implementation, taking on the behavior from the parent `WaterSource` class.
         *
         * @param week The current week for which continuity is applied.
         * @param upstream_source_inflow The inflow from upstream sources.
         * @param wastewater_discharge The discharge from wastewater sources.
         * @param demand_outflow A vector containing the outflows of water demands.
         *
         * @return void
         */     
        void applyContinuity(int week, double upstream_source_inflow,
                                double wastewater_discharge,
                                vector<double> &demand_outflow) override;
};


#endif //TRIANGLEMODEL_INTAKEEXPANSION_H
