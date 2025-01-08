//
// Created by dgorelic on 3/2/2020.
//

#include <iostream>
#include "AllocatedIntake.h"

AllocatedIntake::AllocatedIntake(const char *name, const int id, const vector<Catchment *> &catchments, vector<int> connected_sources,
                                 vector<int> &partner_utilities, const double total_raw_water_capacity,
                                 vector<double> &raw_water_capacity_allocation_fractions,
                                 const double total_treatment_capacity,
                                 vector<double> &treatment_capacity_allocation_fractions)
        : WaterSource(name, id, catchments,
                total_raw_water_capacity, total_treatment_capacity, connected_sources,
                ALLOCATED_INTAKE, &treatment_capacity_allocation_fractions,
                &raw_water_capacity_allocation_fractions, &partner_utilities) {

    allocated_demands.reserve(allocated_capacities.size());
    allocated_demands.assign(allocated_capacities.size(), 0.0);
}

AllocatedIntake::AllocatedIntake(const AllocatedIntake &intake) : WaterSource(intake),
        allocated_demands(intake.allocated_demands) {}

AllocatedIntake &AllocatedIntake::operator=(const AllocatedIntake &intake) {
    WaterSource::operator=(intake);
    allocated_demands = intake.allocated_demands;
    return *this;
}

AllocatedIntake::~AllocatedIntake() {
    catchments.clear();
}

void AllocatedIntake::applyContinuity(int week, double upstream_source_min_env_flow, double wastewater_inflow,
                                      vector<double> &demand) {
    // Calculate the total upstream inflow
    double total_upstream_inflow = upstream_source_inflow +
            upstream_source_min_env_flow +
            wastewater_inflow;

    total_demand = 0;
    for (int u : *utilities_with_allocations) {
        total_demand += demand[u];
        allocated_demands[u] = demand[u];
    }

    /// Get all upstream catchment inflow.
    upstream_catchment_inflow = 0;
    for (Catchment &c : catchments)
        upstream_catchment_inflow += c.getStreamflow(week);

    /// Calculate water availability for next time step.
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

    // Distribute the available volume among utilities based on their allocations, ensuring that the intake capacity is not exceeded.
    // Include dummy indices for other utilities if they have a lower ID than the ones meant to be included.
    for (int u : *utilities_with_allocations)
        available_allocated_volumes[u] = min(this->capacity, available_volume) * allocated_fractions[u];

    // Update the `total_demand` and calculate the `total_outflow` based on the updated demand and upstream inflows.
    // Update the `upstream_source_inflow` and `wastewater_inflow` for tracking.
    this->total_demand = total_demand + policy_added_demand;
    policy_added_demand = 0;
    total_outflow = total_upstream_inflow + upstream_catchment_inflow -
                    total_demand;
    this->upstream_source_inflow = upstream_source_inflow;
    this->wastewater_inflow = wastewater_inflow;
}

void AllocatedIntake::setRealization(unsigned long r, vector<double> &rdm_factors) {
    WaterSource::setRealization(r, rdm_factors);

    total_demand = 0;
    available_volume = this->upstream_catchment_inflow -
                       min_environmental_outflow;
}

double AllocatedIntake::getPrioritySourcePotentialVolume(int utility_id) const {
    return  max(0.0, min(allocated_treatment_capacities[utility_id] - allocated_demands[utility_id] - policy_added_demand,
                         next_upstream_catchment_inflow - min_environmental_outflow - allocated_demands[utility_id] - policy_added_demand));
}

vector<double> AllocatedIntake::getAllocatedDemands() const {
    return allocated_demands;
}

void AllocatedIntake::addCapacity(double capacity, int utility_id) {
    // update the total capacity of the intake
    WaterSource::addCapacity(capacity, utility_id); 
    // Add the specified capacity to the allocated capacity for the specified utility
    allocated_capacities[utility_id] += capacity; 

    // Recalculate the allocation fractions for all utilities based on the updated capacity
    // Ensure that the sum of the fractions reflects the current total capacity.
    for (int u : *utilities_with_allocations)
        allocated_fractions[u] = allocated_capacities[u]/this->capacity;
}

void AllocatedIntake::addTreatmentCapacity(const double added_treatment_capacity, int utility_id) {
    // Update the total treatment capacity of the intake.
    WaterSource::addTreatmentCapacity(added_treatment_capacity, utility_id); 
    // Add the specified treatment capacity to the allocated treatment capacity for the specified utility
    allocated_treatment_capacities[utility_id] += added_treatment_capacity; // adds to utility allocation

    // Recalculate the treatment allocation fractions for all utilities based on the updated treatment capacity
    // Ensure that the sum of the fractions reflects the current total treatment capacity.
    for (int u : *utilities_with_allocations)
        allocated_treatment_fractions[u] = allocated_treatment_capacities[u]/this->total_treatment_capacity;
}

double AllocatedIntake::getAvailableAllocatedVolume(int utility_id) {
    return available_allocated_volumes[utility_id];
}
