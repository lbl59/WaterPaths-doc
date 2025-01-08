//
// Created by bernardo on 2/10/17.
//

#include <numeric>
#include "../../Utils/Constants.h"
#include "../Catchment.h"
#include "Base/WaterSource.h"
#include "Intake.h"

Intake::Intake(
        const char *name, const int id, const vector<Catchment *> &catchments,
        const double max_treatment_capacity)
        : WaterSource(name, id, catchments, NONE, vector<int>(), max_treatment_capacity, INTAKE) {}

Intake::Intake(const char *name, const int id, const vector<Catchment *> &catchments, const double raw_water_capacity,
               const double max_treatment_capacity)
        : WaterSource(name, id, catchments, raw_water_capacity, vector<int>(), max_treatment_capacity, INTAKE) {}

Intake::Intake(const char *name, const int id, const vector<Catchment *> &catchments, vector<int> connected_sources,
               const double raw_water_main_capacity, const vector<double> construction_time_range,
               double permitting_period, Bond &bond) :
        WaterSource(name, id, catchments, NONE, raw_water_main_capacity, connected_sources, INTAKE, construction_time_range,
                    permitting_period, bond) {}

Intake::Intake(const Intake &intake) : WaterSource(intake) {}

Intake &Intake::operator=(const Intake &intake) {
    WaterSource::operator=(intake);
    return *this;
}

Intake::~Intake() {
    catchments.clear();
}

//FIXME: WHEN IMPLEMENTING RESERVOIR RULES, ADD A "BANK" OF WATER OVER AND UNDERUSE TO BE ADDED/SUBTRACTED FROM THE AVAILABLE VOLUME FOR THE NEXT REALIZATION.
/**
 * Calculates the water volume that will be available in the next time step continuity for the
 * current one. This should work well for small intakes, in which case there
 * should almost always be enough water available to make full use of the
 * intake's capacity.
 *
 * @param week
 * @param upstream_source_inflow upstream sources current outflow.
 * @param demand current demand
 */
void Intake::applyContinuity(int week, double upstream_source_inflow,
                             double wastewater_inflow, vector<double> &demand) {
    
    // Calculate total upstream inflow by adding upstream source inflow and wastewater inflow.
    double total_upstream_inflow = upstream_source_inflow +
                                   wastewater_inflow;

    total_demand = 0;
    for (unsigned long i = 0; i < demand.size(); ++i) {
        total_demand += demand[i];
    }

    // Get all upstream catchment inflow.
    upstream_catchment_inflow = 0;
    for (Catchment &c : catchments)
        upstream_catchment_inflow += c.getStreamflow(week);

    // Water availability for next time step.
    next_upstream_catchment_inflow = 0;
    for (Catchment &c : catchments)
        next_upstream_catchment_inflow += c.getStreamflow(week + 1);

    // Calculate the available volume for the following week: 
    //      next week's catchment inflow + total upstream inflows - minimum environmental outflow.
    // Assumes that next week's env outflow will be approximately the same as this week's env outflow.
    // This calculation will hold as long as the intake capacity is not exceeded (works well for small intakes)
    // Also accounts for non-catcheent inflows in case the intake is downstream in the catchment.
    available_volume = min(total_treatment_capacity,
                           next_upstream_catchment_inflow + total_upstream_inflow -
                           min_environmental_outflow);

    // Records for the sake of output.
    this->total_demand = total_demand + policy_added_demand;
    policy_added_demand = 0;
    total_outflow = total_upstream_inflow + upstream_catchment_inflow -
                    total_demand;
    this->upstream_source_inflow = upstream_source_inflow;
    this->wastewater_inflow = wastewater_inflow;
}

void Intake::setRealization(unsigned long r, vector<double> &rdm_factors) {
    WaterSource::setRealization(r, rdm_factors);

    total_demand = 0;
    available_volume = this->upstream_catchment_inflow -
                       min_environmental_outflow;
}

double Intake::getPrioritySourcePotentialVolume(int utility_id) const {
    /// Oct 2019: returns the remaining "storage" capacity after weekly demands have been accounted for
    return  max(0.0, min(total_treatment_capacity - total_demand - policy_added_demand,
            next_upstream_catchment_inflow - min_environmental_outflow - total_demand - policy_added_demand));
}