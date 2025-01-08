//
// Created by bernardo on 1/22/17.
//

#include <iostream>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include "WaterSource.h"
#include "../../../Utils/Utils.h"

using namespace std;

int WaterSource::seed = NON_INITIALIZED;

/**
 * Constructor for when water source is built and operational.
 * @param name
 * @param id
 * @param min_environmental_outflow
 * @param catchments
 * @param capacity
 * @param treatment_capacity
 * @param source_type
 */
WaterSource::WaterSource(const char *name, const int id, const vector<Catchment *> &catchments,
                         const double capacity, vector<int> connected_sources, double treatment_capacity,
                         const int source_type)
        : available_volume(capacity),
          permitting_time(NON_INITIALIZED),
          capacity(capacity),
          built_in_sequence(connected_sources),
          online(ONLINE),
          total_treatment_capacity(treatment_capacity),
          highest_alloc_id(NOT_ALLOCATED),
          id(id),
          name(name),
          source_type(source_type),
          construction_time(NON_INITIALIZED) {
    for (Catchment *c : catchments) {
        this->catchments.push_back(Catchment(*c));
    }
}

WaterSource::WaterSource(const char *name, const int id, const vector<Catchment *> &catchments,
                         const double capacity, double treatment_capacity, vector<int> connected_sources,
                         const int source_type, const vector<double> construction_time_range,
                         double permitting_period, Bond &bond)
        : available_volume(capacity),
          permitting_time(permitting_period),
          bonds(Utils::copyBonds(vector<Bond *>(1, &bond))),
          capacity(capacity),
          built_in_sequence(connected_sources),
          online(OFFLINE),
          total_treatment_capacity(treatment_capacity),
          highest_alloc_id(NOT_ALLOCATED),
          id(id),
          name(name),
          source_type(source_type),
          construction_time(randomConstructionTime(construction_time_range[0], construction_time_range[1])) {
    for (Catchment *c : catchments) {
        this->catchments.emplace_back(*c);
    }
    checkForInputErrorsConstruction();
}

WaterSource::WaterSource(const char *name, const int id, const vector<Catchment *> &catchments,
                         const double capacity, double treatment_capacity, vector<int> connected_sources,
                         const int source_type, const vector<double> construction_time_range,
                         double permitting_period, vector<Bond *> bonds)
        : available_volume(capacity),
          permitting_time(permitting_period),
          bonds(bonds),
          capacity(capacity),
          built_in_sequence(connected_sources),
          online(OFFLINE),
          total_treatment_capacity(treatment_capacity),
          highest_alloc_id(NOT_ALLOCATED),
          id(id),
          name(name),
          source_type(source_type),
          construction_time(randomConstructionTime(construction_time_range[0], construction_time_range[1])) {
    for (Catchment *c : catchments) {
        this->catchments.emplace_back(*c);
    }
    checkForInputErrorsConstruction();
}

WaterSource::WaterSource(const char *name, const int id, const vector<Catchment *> &catchments,
                         const double capacity, double treatment_capacity, vector<int> connected_sources,
                         vector<int> *utilities_with_allocations, vector<double> *utility_treatment_allocations,
                         const int source_type, const vector<double> construction_time_range,
                         double permitting_period, vector<Bond *> bonds)
        : available_volume(capacity),
          permitting_time(permitting_period),
          bonds(bonds),
          capacity(capacity),
          built_in_sequence(connected_sources),
          online(OFFLINE),
          total_treatment_capacity(treatment_capacity),
          utilities_with_allocations(utilities_with_allocations),
          highest_alloc_id(NOT_ALLOCATED),
          id(id),
          name(name),
          source_type(source_type),
          construction_time(randomConstructionTime(construction_time_range[0], construction_time_range[1])) {
    for (Catchment *c : catchments) {
        this->catchments.emplace_back(*c);
    }
    checkForInputErrorsConstruction();

    // Set water quality pool as a reservoir with ID next to highest ID allocation.
    wq_pool_id = static_cast<unsigned int>(
            *std::max_element(utilities_with_allocations->begin(),
                              utilities_with_allocations->end()) + 1);
    highest_alloc_id = wq_pool_id;
    unsigned long length = (unsigned long) wq_pool_id + 1;

    this->allocated_treatment_capacities.reserve(length);
    this->allocated_treatment_capacities.assign(length, 0.0);

    this->allocated_treatment_fractions.reserve(length);
    this->allocated_treatment_fractions.assign(length, 0.0);

    // Populate treatment vectors.
    for (unsigned long i = 0; i < utilities_with_allocations->size(); ++i) {
        auto u = (unsigned int) utilities_with_allocations->at(i);

        // Replace the -1 in the utilities_with_allocations vector with the
        // ID assigned to the water quality pool.
        u = ((int) u == WATER_QUALITY_ALLOCATION ? wq_pool_id : u);

        if ((int) u != wq_pool_id) {
            this->allocated_treatment_capacities[u] = utility_treatment_allocations->at(u);
            this->allocated_treatment_fractions[u] =
                    allocated_treatment_capacities[u] / total_treatment_capacity;
        }

        (*this->utilities_with_allocations)[i] = u;
    }
}


/**
 * Constructor for when shared water source does not exist in the beginning of the simulation.
 * NEW CONSTRUCTOR IS FOR ALLOCATEDINTAKE/EXPANSION CLASSES
 * INDEXING TO ASSIGN ALLOCATIONS IS ADJUSTED FROM OTHER CONSTRUCTORS, BE CAREFUL
 */
WaterSource::WaterSource(const char *name, const int id, const vector<Catchment *> &catchments,
                         const double capacity, double treatment_capacity, vector<int> connected_sources,
                         vector<int> *utilities_with_allocations,
                         vector<double> *utility_supply_allocations,
                         vector<double> *utility_treatment_allocations,
                         const int source_type, const vector<double> construction_time_range,
                         double permitting_period, vector<Bond *> bonds)
        : available_volume(capacity),
          permitting_time(permitting_period),
          bonds(bonds),
          capacity(capacity),
          built_in_sequence(connected_sources),
          online(OFFLINE),
          total_treatment_capacity(treatment_capacity),
          utilities_with_allocations(utilities_with_allocations),
          highest_alloc_id(NOT_ALLOCATED),
          id(id),
          name(name),
          source_type(source_type),
          construction_time(randomConstructionTime(construction_time_range[0], construction_time_range[1])) {
    for (Catchment *c : catchments) {
        this->catchments.emplace_back(*c);
    }
    checkForInputErrorsConstruction();

    // Have water quality pool as a reservoir with ID next to highest ID
    // allocation.
    wq_pool_id = static_cast<unsigned int>(
            *std::max_element(utilities_with_allocations->begin(),
                              utilities_with_allocations->end()) + 1);
    highest_alloc_id = wq_pool_id;
    unsigned long length = (unsigned long) wq_pool_id + 1;

    this->allocated_treatment_capacities.reserve(length);
    this->allocated_treatment_capacities.assign(length, 0.0);

    this->allocated_treatment_fractions.reserve(length);
    this->allocated_treatment_fractions.assign(length, 0.0);

    this->allocated_capacities.reserve(length);
    this->allocated_capacities.assign(length, 0.0);

    this->allocated_fractions.reserve(length);
    this->allocated_fractions.assign(length, 0.0);

    // Populate treatment vectors.
    for (unsigned long i = 0; i < utilities_with_allocations->size(); ++i) {
        auto u = (unsigned int) utilities_with_allocations->at(i);

        // Replace the -1 in the utilities_with_allocations vector with the
        // ID assigned to the water quality pool.
        u = ((int) u == WATER_QUALITY_ALLOCATION ? wq_pool_id : u);

        if ((int) u != wq_pool_id) {
            this->allocated_treatment_capacities[u] = utility_treatment_allocations->at(i);
            this->allocated_treatment_fractions[u] =
                    allocated_treatment_capacities[u] / total_treatment_capacity;

            this->allocated_capacities[u] = utility_supply_allocations->at(i);
            this->allocated_fractions[u] =
                    allocated_capacities[u] / capacity;
        }

        (*this->utilities_with_allocations)[i] = u;
    }
}

WaterSource::WaterSource(const char *name, const int id, const vector<Catchment *> &catchments,
                         const double capacity, double treatment_capacity, vector<int> connected_sources,
                         const int source_type, vector<double> *allocated_treatment_fractions,
                         vector<double> *allocated_fractions, vector<int> *utilities_with_allocations)
        : available_volume(capacity),
          permitting_time(NON_INITIALIZED),
          capacity(capacity),
          built_in_sequence(connected_sources),
          utilities_with_allocations(utilities_with_allocations),
          wq_pool_id(NON_INITIALIZED),
          online(ONLINE),
          total_treatment_capacity(treatment_capacity),
          highest_alloc_id(NOT_ALLOCATED),
          id(id),
          name(name),
          source_type(source_type),
          construction_time(NON_INITIALIZED) {
    setAllocations(utilities_with_allocations,
                   allocated_fractions,
                   allocated_treatment_fractions);

    for (Catchment *c : catchments) {
        this->catchments.push_back(Catchment(*c));
    }
}

WaterSource::WaterSource(const char *name, const int id, const vector<Catchment *> &catchments,
                         const double capacity, double treatment_capacity, vector<int> built_in_sequence,
                         const int source_type, vector<double> *allocated_treatment_fractions,
                         vector<double> *allocated_fractions, vector<int> *utilities_with_allocations,
                         const vector<double> construction_time_range, double permitting_period, Bond &bond)
        : available_volume(capacity),
          permitting_time(permitting_period),
          bonds(Utils::copyBonds(vector<Bond *>(1, &bond))),
          capacity(capacity),
          built_in_sequence(built_in_sequence),
          utilities_with_allocations(utilities_with_allocations),
          wq_pool_id(NON_INITIALIZED),
          online(OFFLINE),
          total_treatment_capacity(treatment_capacity),
          highest_alloc_id(NOT_ALLOCATED),
          id(id),
          name(name),
          source_type(source_type),
          construction_time(randomConstructionTime(construction_time_range[0], construction_time_range[1])) {
    setAllocations(utilities_with_allocations,
                   allocated_fractions,
                   allocated_treatment_fractions);

    for (Catchment *c : catchments) {
        this->catchments.emplace_back(*c);
    }

    checkForInputErrorsConstruction();
}

void WaterSource::checkForInputErrorsConstruction() {
    // Check if `permitting_time` is NaN or negative
    if (std::isnan(permitting_time) || permitting_time < 0) {
        string error = "Invalid permitting period for water source " + to_string(id);
        throw invalid_argument(error.c_str());
    }
    // Check if `construction_time` is NaN or negative
    if (std::isnan(construction_time) || construction_time < 0) {
        string error = "Invalid construction time for water source " + to_string(id);
        throw invalid_argument(error.c_str());
    }
}

WaterSource::WaterSource(const WaterSource &water_source) :
        available_volume(water_source.available_volume),
        total_outflow(water_source.total_outflow),
        upstream_source_inflow(water_source.upstream_source_inflow),
        upstream_catchment_inflow(water_source.upstream_catchment_inflow),
        permitting_time(water_source.permitting_time),
        bonds(Utils::copyBonds(water_source.bonds)),
        upstream_min_env_inflow(water_source.upstream_min_env_inflow),
        capacity(water_source.capacity), 
        built_in_sequence(water_source.built_in_sequence),
        available_allocated_volumes(water_source.available_allocated_volumes),
        allocated_capacities(water_source.allocated_capacities),
        allocated_treatment_capacities(water_source.allocated_treatment_capacities),
        allocated_treatment_fractions(water_source.allocated_treatment_fractions),
        allocated_fractions(water_source.allocated_fractions),
        supply_allocated_fractions(water_source.supply_allocated_fractions),
        utilities_with_allocations(water_source.utilities_with_allocations),
        online(water_source.online),
        min_environmental_outflow(water_source.min_environmental_outflow),
        total_treatment_capacity(water_source.total_treatment_capacity),
        highest_alloc_id(water_source.highest_alloc_id),
        id(water_source.id),
        name(water_source.name),
        source_type(water_source.source_type),
        construction_time(water_source.construction_time) {
    if (water_source.wq_pool_id != NON_INITIALIZED) {
        wq_pool_id = water_source.wq_pool_id;

        unsigned long length = (unsigned long) wq_pool_id + 1;

        available_allocated_volumes.reserve(length);
        allocated_capacities.reserve(length);
        allocated_treatment_capacities.reserve(length);
        allocated_treatment_fractions.reserve(length);
        allocated_fractions.reserve(length);

    }

    catchments.clear();
    for (const auto &catchment : water_source.catchments) {
        catchments.emplace_back(catchment);
    }
}

WaterSource::~WaterSource() {
    for (Bond *b : bonds) {
        delete b;
    }
}

WaterSource &WaterSource::operator=(const WaterSource &water_source) {
    if (wq_pool_id != NON_INITIALIZED) {
        wq_pool_id = water_source.wq_pool_id;

        unsigned long length = (unsigned long) wq_pool_id + 1;
        highest_alloc_id = water_source.highest_alloc_id;

        allocated_fractions.reserve(length);
        allocated_treatment_fractions.reserve(length);
        available_allocated_volumes.reserve(length);
        allocated_capacities.reserve(length);
        allocated_treatment_capacities.reserve(length);

        allocated_fractions = water_source.allocated_fractions;
        allocated_treatment_fractions = water_source.allocated_treatment_fractions;
        available_allocated_volumes = water_source.available_allocated_volumes;
        allocated_capacities = water_source.allocated_capacities;
        allocated_treatment_capacities = water_source.allocated_treatment_capacities;
    } 

    catchments.clear();
    for (Catchment &c : catchments) {
        this->catchments.emplace_back(c);
    }

    return *this;
}

bool WaterSource::operator<(const WaterSource *other) {
    return id < other->id;
}

bool WaterSource::operator>(const WaterSource *other) {
    return id > other->id;
}

bool WaterSource::operator==(const WaterSource *other) {
    return id == other->id;
}

bool WaterSource::compare(WaterSource *lhs, WaterSource *rhs) {
    return lhs->id < rhs->id;
}

void WaterSource::setAllocations(
        vector<int> *utilities_with_allocations,
        vector<double> *allocated_fractions,
        vector<double> *allocated_treatment_fractions) {
    /**
     * Initial set up of allocations with full reservoir in the beginning of the
     * simulations. To be used in constructors only.
     */
    if (utilities_with_allocations->size() != allocated_fractions->size())
        throw invalid_argument("There must be one allocation fraction in "
                                         "utilities_with_allocations for "
                                         "each utility id in "
                                         "allocated_fractions.");

    total_allocated_fraction = accumulate(allocated_fractions->begin(),
                                          allocated_fractions->end(),
                                          0.0);
    
    // Calculates the total allocated fraction and adjusts for water quality pool allocation if necessary.
    if (total_allocated_fraction < 1.0) {
        for (int i = 0; i < (int) utilities_with_allocations->size(); ++i)
            if ((*utilities_with_allocations)[i] == WATER_QUALITY_ALLOCATION) {
                (*allocated_fractions)[i] += 1. - total_allocated_fraction;
            }
    } else if (total_allocated_fraction > 1.) {
        string error = "Water Source " + to_string(id) + 
		" has allocation fractions whose sum are more than 1:\n ";
	for (int i = 0; i < allocated_fractions->size(); ++i) {
		error += "Utility " + to_string((*utilities_with_allocations)[i]) 
			+ ": " + to_string((*allocated_fractions)[i]) + "\n";
	}

	char error_carray[error.size() + 1];
	strcpy(error_carray, error.c_str());

        throw invalid_argument(error_carray);
    }

    // Ensure that no treatment capacity is allocated to the water quality pool.
    auto it = std::find(utilities_with_allocations->begin(),
            utilities_with_allocations->end(), WATER_QUALITY_ALLOCATION);
    if (it != utilities_with_allocations->end() &&
            utilities_with_allocations->size() ==
            allocated_treatment_fractions->size()) {
        string error = "Water Source ";
        error += to_string(id) + " has treatment capacity allocated to water "
                                 "quality pool.";
        throw invalid_argument(error.c_str());
    }

    // Check if the sizes of the vectors are consistent.
    if (it == utilities_with_allocations->end()) {
        if (utilities_with_allocations->size() != allocated_fractions->size()) {
            string error = "Water Source ";
            error += to_string(id) + " either has capacity fractions allocated to "
                                     "no utilities or utilities with no allocated "
                                     "capacities.";
            throw invalid_argument(error.c_str());
        }
        if (utilities_with_allocations->size() != allocated_treatment_fractions->size()) {
            string error = "Water Source ";
            error += to_string(id) + " either has treatment fractions allocated to "
                                     "no utilities or utilities with no allocated "
                                     "treatment fractions.";
            throw invalid_argument(error.c_str());
        }
    } else {
        if (utilities_with_allocations->size() != allocated_fractions->size()) {
            string error = "Water Source ";
            error += to_string(id) + " either has capacity fractions allocated to "
                                     "no utilities or utilities with no allocated "
                                     "capacities.";
            throw invalid_argument(error.c_str());
        }
        if (utilities_with_allocations->size() != allocated_treatment_fractions->size() + 1) {
            string error = "Water Source ";
            error += to_string(id) + " either has treatment fractions allocated to "
                                     "no utilities or utilities with no allocated "
                                     "treatment fractions.";
            throw invalid_argument(error.c_str());
        }
    }

    // Updates water quality pool as a reservoir with ID next to highest ID allocation.
    wq_pool_id = static_cast<unsigned int>(
            *std::max_element(utilities_with_allocations->begin(),
                              utilities_with_allocations->end()) + 1);
    highest_alloc_id = wq_pool_id;
    unsigned long length = (unsigned long) wq_pool_id + 1;

    this->available_allocated_volumes.reserve(length);
    this->available_allocated_volumes.assign(length, 0.0);

    this->allocated_capacities.reserve(length);
    this->allocated_capacities.assign(length, 0.0);

    this->allocated_treatment_capacities.reserve(length);
    this->allocated_treatment_capacities.assign(length, 0.0);

    this->allocated_treatment_fractions.reserve(length);
    this->allocated_treatment_fractions.assign(length, 0.0);

    this->allocated_fractions.reserve(length);
    this->allocated_fractions.assign(length, 0.0);

    this->supply_allocated_fractions.reserve(length - 1);
    this->supply_allocated_fractions.assign(length - 1, 0.0);

    // Reserves and assigns values to the allocation vectors.
    // Populate vectors.
    for (unsigned long i = 0; i < utilities_with_allocations->size(); ++i) {
        auto u = (unsigned int) utilities_with_allocations->at(i);

        // Replace the -1 in the utilities_with_allocations vector with the
        // ID assigned to the water quality pool.
        u = ((int) u == WATER_QUALITY_ALLOCATION ? wq_pool_id : u);

        if ((int) u != wq_pool_id) {
            this->allocated_treatment_fractions[u] =
                    allocated_treatment_fractions->at(i);
            this->allocated_treatment_capacities[u] = total_treatment_capacity *
                                                      this->allocated_treatment_fractions[u];
        } else
            (*this->utilities_with_allocations)[i] = u;

        this->allocated_fractions[u] = (*allocated_fractions)[i];
        (*this->utilities_with_allocations)[i] = u;
        allocated_capacities[u] = this->capacity * (*allocated_fractions)[i];
        available_allocated_volumes[u] = allocated_capacities[u];
    }

    for (int i = 0; i < wq_pool_id; ++i) {
        supply_allocated_fractions[i] = this->allocated_fractions[i] /
                (1. - this->allocated_fractions[wq_pool_id]);
    }
}

void WaterSource::continuityWaterSource(int week, double upstream_source_inflow,
                                        double wastewater_inflow,
                                        vector<double> &demand_outflow) {
    // Check if the water source is online
    // If online, process inflows and update demand outflows
    if (online)
        applyContinuity(week, upstream_source_inflow, wastewater_inflow,
                        demand_outflow);
    // Otherwise, bypass continuity calculations
    else
        bypass(week, upstream_source_inflow + wastewater_inflow);
}

void WaterSource::bypass(int week, double total_upstream_inflow) {
    upstream_catchment_inflow = 0;
    for (Catchment &c : catchments) {
        upstream_catchment_inflow += c.getStreamflow((week));
    }

    total_demand = NONE;
    available_volume = NONE;
    total_outflow = upstream_catchment_inflow + total_upstream_inflow;
    this->upstream_source_inflow = total_upstream_inflow;
}

void WaterSource::removeWater(int allocation_id, double volume) {
    /**
     * If creating a new water source that can be allocated to different utilities,
     * this function must be overwritten to:
     * available_allocated_volumes[allocation_id] -= volume;
     * available_volume -= volume;
     * demand += volume;
     */
    available_volume -= volume;
    total_demand += volume;
    policy_added_demand += volume;
}

void WaterSource::addCapacity(double capacity, int utility_id) {
    WaterSource::capacity += capacity;
}

void WaterSource::addTreatmentCapacity(const double added_treatment_capacity, int utility_id) {
    total_treatment_capacity += added_treatment_capacity;
}

bool WaterSource::skipConstruction(int utility_id) const {
    return false;
}

/**
 * Set inflows, evaporation and rdm values for water source for a given utility r
 * @param r
 * @param rdm_factors table with one realization per row in which the first column
 * is the evaporation multiplier, followed by pairs of values for each source representing
 * permitting time and construction cost.
 */
void WaterSource::setRealization(unsigned long r, vector<double> &rdm_factors) {
    for (Catchment &c : catchments)
        c.setRealization(r, rdm_factors);
 
    //  Update the total upstream catchment inflow, demand, and available volume 
    // by summing the streamflow of all catchments for week 0.
    this->upstream_catchment_inflow = 0;
    for (Catchment &c : catchments) {
        this->upstream_catchment_inflow = c.getStreamflow(0);
    }

    // Scale permitting times according to corresponding DU factors.
    permitting_time *= rdm_factors.at((unsigned int) 1 + 2 * id);

    /// Scale construction cost overruns according to corresponding DU factors.
    double construction_cost_multiplier = rdm_factors.at((unsigned int) 1 + 2 * id + 1);
    vector<double> construction_cost_multiplier_vec = vector<double>(1, construction_cost_multiplier);
    for (Bond *bond : bonds) {
        bond->setRealizationWaterSource(r, construction_cost_multiplier_vec);
    }
}

double WaterSource::getAvailableVolume() const {
    return available_volume;
}

double WaterSource::getAllocatedInflow(int utility_id) const {
    if (wq_pool_id == NON_INITIALIZED)
        return upstream_catchment_inflow;
    else
        // Return the fraction of inflow from the upstream catchment for the given utility
        return upstream_catchment_inflow * allocated_fractions[utility_id];
}

double WaterSource::getAvailableSupplyVolume() const {
    if (wq_pool_id == NON_INITIALIZED)
        return available_volume;
    else
        // Subtract the allocated volume of the water quality pool from the total available volume.
        return available_volume - available_allocated_volumes[wq_pool_id];
}

double WaterSource::getAvailableAllocatedVolume(int utility_id) {
    /**
     * If creating a new water source that can be allocated to different utilities,
     * this function must be overwritten to:
     * return available_allocated_volumes[utility_id];
     */
    return getAvailableVolume();
}

double WaterSource::getPrioritySourcePotentialVolume(int utility_id) const {
    return 0.0;
}

bool WaterSource::isOnline() const {
    return online;
}

void WaterSource::setFull() {
    WaterSource::available_volume = capacity;
}

void WaterSource::setOutflow_previous_week(double outflow_previous_week) {
    WaterSource::total_outflow = outflow_previous_week;
}

void WaterSource::setOnline() {
    online = ONLINE;
}

double WaterSource::getTotal_outflow() const {
    return total_outflow;
}

double WaterSource::getSupplyCapacity() {
    if (wq_pool_id == NON_INITIALIZED)
        return capacity;
    else
        return capacity - allocated_capacities[wq_pool_id];
}

double WaterSource::getAllocatedCapacity(int utility_id) {
    /**
     * Most sources will not have different allocations, so by default this
     * function will return the total capacity. If a source is to have multiple
     * allocations, this function must be overriden to make sure it returns the
     * right value.
     */
    return capacity;
}

double WaterSource::getDemand() const {
    return total_demand;
}

double WaterSource::getUpstream_source_inflow() const {
    return upstream_source_inflow;
}

double WaterSource::getUpstreamCatchmentInflow() const {
    return upstream_catchment_inflow;
}

void WaterSource::setMin_environmental_outflow(
        double min_environmental_outflow) {
    WaterSource::min_environmental_outflow = min_environmental_outflow;
}

double WaterSource::getMin_environmental_outflow() const {
    return min_environmental_outflow;
}

double WaterSource::getAllocatedFraction(int utility_id) {
    return 1.0;
}

double WaterSource::getSupplyAllocatedFraction(int utility_id) {
    return 1.0;
}

double WaterSource::getEvaporated_volume() const {
    return evaporated_volume;
}

double WaterSource::getAllocatedTreatmentCapacity(int utility_id) const {
    return total_treatment_capacity;
}

double WaterSource::getAllocatedTreatmentFraction(int utility_id) const {
    return 1.0;
}

vector<double> WaterSource::getAllocatedTreatmentCapacities() const {
    return allocated_treatment_capacities;
}

vector<double> WaterSource::getAllocatedSupplyCapacities() const {
    return allocated_capacities;
}

void WaterSource::setTreatmentAllocations(const vector<double> treatment_capacity_allocations) {
    allocated_treatment_capacities = treatment_capacity_allocations;
    for (unsigned long i = 0; i < utilities_with_allocations->size(); ++i) {
        int u = utilities_with_allocations->at(i);
        // Note: will not check for over-allocation here, it is done elsewhere
        allocated_treatment_fractions[u] = allocated_treatment_capacities[u] / total_treatment_capacity;
    }
}

double WaterSource::getTotal_treatment_capacity(int utility_id) const {
    return total_treatment_capacity;
}

void WaterSource::resetAllocations(
        const vector<double>
        *new_allocated_fractions) {
    // For each utility with an allocation, populate the vector with their corresponding allocation 
    // fraction from `new_allocated_fractions`
    for (unsigned long i = 0; i < utilities_with_allocations->size(); ++i) {
        int u = utilities_with_allocations->at(i);
        u = (u == WATER_QUALITY_ALLOCATION ? wq_pool_id : u);

        // Update allocated fractions
        allocated_fractions[u] = (*new_allocated_fractions)[i];
        (*this->utilities_with_allocations)[i] = u;

        // Update allocated capacities
        allocated_capacities[u] = capacity * (*new_allocated_fractions)[i];

        // Update available allocated volumes
        available_allocated_volumes[u] =
                available_volume * allocated_fractions[u];
    }
}

void WaterSource::resetTreatmentAllocations(
        const vector<double> current_treatment_allocations,
        const vector<double> new_treatment_allocations) {
    // For each utility, calculate the change in treatment capacity by subtracting the current allocation from the new allocation.
    for (unsigned long i = 0; i < utilities_with_allocations->size(); ++i) {
        int u = utilities_with_allocations->at(i);
        // check for negativity issues
        if (allocated_treatment_capacities[u] < 0) {
            allocated_treatment_capacities[u] = 0;

        }
        double capacity_change = new_treatment_allocations[u] - current_treatment_allocations[u];

        allocated_treatment_capacities[u] += capacity_change;
        (*this->utilities_with_allocations)[i] = u;
        // Calculate the new treatment fractions based on the updated allocated treatment capacities.
        allocated_treatment_fractions[u] = allocated_treatment_capacities[u]/total_treatment_capacity;
    }

    // If the total allocated treatment capacity exceeds the total treatment capacity, 
    // adjust the treatment fractions to ensure they sum up to 1, and recalculate the treatment capacities accordingly.
    double temporary_total_allocated_capacity =
            accumulate(allocated_treatment_capacities.begin(),allocated_treatment_capacities.end(),0.0);
    if (temporary_total_allocated_capacity > total_treatment_capacity)
        for (unsigned long i = 0; i < utilities_with_allocations->size(); ++i) {
            int u = utilities_with_allocations->at(i);
            allocated_treatment_fractions[u] = allocated_treatment_capacities[u]/temporary_total_allocated_capacity;
            allocated_treatment_capacities[u] = allocated_treatment_fractions[u] * total_treatment_capacity;
        }
}

void WaterSource::setAvailableAllocatedVolumes(
        vector<double> available_allocated_volumes, double available_volume) {
    if (utilities_with_allocations)
      this->available_allocated_volumes = available_allocated_volumes;
    this->available_volume = available_volume;
}

vector<double> WaterSource::getAvailable_allocated_volumes() const {
    return available_allocated_volumes;
}

vector<int> *WaterSource::getUtilities_with_allocations() const {
    return utilities_with_allocations;
}

double WaterSource::getWastewater_inflow() const {
    return wastewater_inflow;
}

int WaterSource::getWaterQualityPoolID() const {
    return wq_pool_id;
}

int WaterSource::getAgreementType() const {
    throw logic_error("Error in WaterSource, getAgreementType: "
                      "only JointWTP class or child classes "
                      "should try to access agreement type.");
    return 9999;
}

double WaterSource::getPermitting_period() const {
    return permitting_time;
}

const vector<int> &WaterSource::getBuilt_in_sequence() const {
    return built_in_sequence;
}

Bond &WaterSource::getBond(int utility_id) {
    if (bonds.size() == 1) {
        return *bonds[0];
    } else {
        return *bonds[utility_id];
    }
}

const double WaterSource::getConstruction_time() const {
    return construction_time;
}

int WaterSource::randomConstructionTime(double t0, double tf) {
    std::mt19937 rng(std::random_device{}());
    if (seed > NON_INITIALIZED) {
        rng.seed((unsigned long) id + seed);
    } else {
        rng.seed(std::random_device()());
    }

    // Generate a random floating-point value between `t0` and `tf` using a uniform real distribution.
    std::uniform_real_distribution<> dist(t0, tf);
    int construction_time = (int) (dist(rng) * WEEKS_IN_YEAR);

    return construction_time;
}

void WaterSource::setSeed(int seed) {
    WaterSource::seed = seed;
}

void WaterSource::unsetSeed() {
    WaterSource::seed = NON_INITIALIZED;
}

int WaterSource::getParentWaterSourceID() const {
    throw logic_error("Error in WaterSource::getParentWaterSourceID, "
                      "this should only be called through override in"
                      "JointWTP class.");

    return -999;
}
