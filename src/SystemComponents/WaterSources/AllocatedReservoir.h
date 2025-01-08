//
// Created by bernardoct on 7/9/17.
//

#ifndef TRIANGLEMODEL_ALLOCATEDRESERVOIR_H
#define TRIANGLEMODEL_ALLOCATEDRESERVOIR_H


#include "Reservoir.h"

class AllocatedReservoir : public Reservoir {
protected:
    /**
     * @brief A flag to indicate if the reservoir has a water quality pool.
     */
    const bool has_water_quality_pool;
    /**
     * @brief The error in calculating inflow continuity.
     * Calculated as the total_outflow of the reservoir + available_allocated_volumes to a specific utility.
     * Should be zero.
     */
    double continuity_error;

public:
    /**
     * @brief Constructor for the AllocatedReservoir class.
     * 
     * This function initializes an AllocatedReservoir object with the specified parameters.
     * It sets up the reservoir’s name, ID, catchments, capacities, and various allocation fractions.
     * Additionally, it checks if the water quality pool is initialized.
     * 
     * @param name The name of the reservoir.
     * @param id The unique identifier for the reservoir.
     * @param catchments A vector of pointers to catchment objects associated with the reservoir.
     * @param capacity The total capacity of the reservoir.
     * @param max_treatment_capacity The maximum treatment capacity of the reservoir.
     * @param evaporation_series The evaporation series object associated with the reservoir.
     * @param storage_area_curve A pointer to a DataSeries object representing the storage area curve.
     * @param utilities_with_allocations A pointer to a vector of integers representing the utilities with allocations.
     * @param allocated_fractions A pointer to a vector of doubles representing the allocated fractions of the reservoir.
     * @param allocated_treatment_fractions A pointer to a vector of doubles representing the allocated treatment fractions.
     * 
     * @return void
     */
    AllocatedReservoir(
        const char *name, const int id,
        const vector<Catchment *> &catchments, const double capacity,
        const double max_treatment_capacity,
        EvaporationSeries &evaporation_series,
        DataSeries *storage_area_curve,
        vector<int> *utilities_with_allocations,
        vector<double> *allocated_fractions, vector<double>
        *allocated_treatment_fractions);
        
    /**
     * @brief Constructor for the AllocatedReservoir class.
     * 
     * This function initializes an AllocatedReservoir object with the specified parameters.
     * It sets up additional attributes including construction time, permitting period, and bond information.
     * Additionally, it checks if the water quality pool is initialized.
     * 
     * @param name The name of the reservoir.
     * @param id The unique identifier for the reservoir.
     * @param catchments A vector of pointers to catchment objects associated with the reservoir.
     * @param capacity The total capacity of the reservoir.
     * @param max_treatment_capacity The maximum treatment capacity of the reservoir.
     * @param evaporation_series The evaporation series associated with the reservoir.
     * @param storage_area_curve A pointer to a DataSeries object representing the storage area curve.
     * @param construction_time_range A vector of doubles representing the range of construction times for the reservoir.
     * @param permitting_period The permitting period for the reservoir.
     * @param bond The Bond object associated with the reservoir.
     * @param utilities_with_allocations A pointer to a vector of integers representing the utilities with allocations.
     * @param allocated_fractions A pointer to a vector of doubles representing the allocated fractions of the reservoir.
     * @param allocated_treatment_fractions A pointer to a vector of doubles representing the allocated treatment fractions.
     * 
     * @return void
     */
    AllocatedReservoir(const char *name, const int id, const vector<Catchment *> &catchments,
                    const double capacity, const double max_treatment_capacity,
                    EvaporationSeries &evaporation_series, DataSeries *storage_area_curve,
                    const vector<double> &construction_time_range, double permitting_period, Bond &bond,
                    vector<int> *utilities_with_allocations, vector<double> *allocated_fractions,
                    vector<double> *allocated_treatment_fractions);

    /**
     * @brief Constructor for the AllocatedReservoir class.
     * 
     * This function initializes an AllocatedReservoir object with the specified parameters.
     * It sets up a reservoir with a fixed storage area instead of a storage area curve.
     * Additionally, it checks if the water quality pool is initialized.
     * 
     * @param name The name of the reservoir.
     * @param id The unique identifier for the reservoir.
     * @param catchments A vector of pointers to catchment objects associated with the reservoir.
     * @param capacity The total capacity of the reservoir.
     * @param max_treatment_capacity The maximum treatment capacity of the reservoir.
     * @param evaporation_series The evaporation series object associated with the reservoir.
     * @param storage_area The storage area of the reservoir.
     * @param utilities_with_allocations A pointer to a vector of integers representing the utilities with allocations.
     * @param allocated_fractions A pointer to a vector of doubles representing the allocated fractions of the reservoir.
     * @param allocated_treatment_fractions A pointer to a vector of doubles representing the allocated treatment fractions.
     * 
     * @return void
     */
    AllocatedReservoir(
        const char *name, const int id,
        const vector<Catchment *> &catchments, const double capacity,
        const double max_treatment_capacity,
        EvaporationSeries &evaporation_series, double storage_area,
        vector<int> *utilities_with_allocations,
        vector<double> *allocated_fractions, vector<double>
        *allocated_treatment_fractions);

    /**
     * @brief Constructor for the AllocatedReservoir class.
     * 
     * This function initializes an AllocatedReservoir object with the specified parameters.
     * It sets up additional attributes including construction time, permitting period, and bond information.
     * It sets up a reservoir with a fixed storage area instead of a storage area curve.
     * Additionally, it checks if the water quality pool is initialized.
     * 
     * @param name The name of the reservoir.
     * @param id The unique identifier for the reservoir.
     * @param catchments A vector of pointers to catchment objects associated with the reservoir.
     * @param capacity The total capacity of the reservoir.
     * @param max_treatment_capacity The maximum treatment capacity of the reservoir.
     * @param evaporation_series The evaporation series associated with the reservoir.
     * @param storage_area The storage area of the reservoir.
     * @param construction_time_range A vector of doubles representing the range of construction times for the reservoir.
     * @param permitting_period The permitting period for the reservoir.
     * @param bond The Bond object associated with the reservoir.
     * @param utilities_with_allocations A pointer to a vector of integers representing the utilities with allocations.
     * @param allocated_fractions A pointer to a vector of doubles representing the allocated fractions of the reservoir.
     * @param allocated_treatment_fractions A pointer to a vector of doubles representing the allocated treatment fractions.
     * 
     * @return void
     */
    AllocatedReservoir(const char *name, const int id, const vector<Catchment *> &catchments,
                    const double capacity, const double max_treatment_capacity,
                    EvaporationSeries &evaporation_series, double storage_area,
                    const vector<double> &construction_time_range, double permitting_period, Bond &bond,
                    vector<int> *utilities_with_allocations, vector<double> *allocated_fractions,
                    vector<double> *allocated_treatment_fractions);

    /**
     * @brief Copy constructor for the AllocatedReservoir class.
     * 
     * This function creates a new `AllocatedReservoir` object as a copy of an existing one.
     * It initializes the new object by copying the data members from the provided `allocated_reservoir`,
     * including the base class `Reservoir` and the `has_water_quality_pool` member.
     * 
     * @param allocated_reservoir The `AllocatedReservoir` object to be copied.
     * 
     * @return void
     */
    AllocatedReservoir(const AllocatedReservoir &allocated_reservoir);

    /**
     * @brief Assignment operator for the AllocatedReservoir class.
     * 
     * This function assigns the state of one `AllocatedReservoir` object to another using the assignment operator.
     * It copies the data members from the provided `allocated_reservoir` object to the current object (`*this`),
     * including the base class `Reservoir` and the `has_water_quality_pool` member.
     * 
     * @param allocated_reservoir The `AllocatedReservoir` object to be assigned.
     * 
     * @return A reference to the current `AllocatedReservoir` object (`*this`).
     */
    AllocatedReservoir &operator=(
        const AllocatedReservoir
        &allocated_reservoir);

    /**
     * @brief Destructor for the AllocatedReservoir class.
     * 
     * This function cleans up the `AllocatedReservoir` object when it goes out of scope.
     * As it is defaulted, it relies on the compiler-generated destructor to handle cleanup.
     * 
     * @return void
     */
    ~AllocatedReservoir();

    /**
     * @brief Updates reservoir state by applying water balance equations for a given week.
     * 
     * This function calculates the reservoir's continuity, accounting for inflows, outflows, evaporation, 
     * and demand, and updates the state of the reservoir accordingly.
     * This function overrides the base class `Reservoir` function to include continuity calculations.
     * 
     * @param week The current week for which continuity is calculated.
     * @param upstream_source_inflow The inflow from upstream water sources.
     * @param wastewater_inflow The inflow from wastewater sources.
     * @param demand_outflow A vector representing the water demands of different utilities.
     * 
     * @return void
     * 
     * @throws runtime_error If the continuity error exceeds acceptable limits or allocated volumes 
     *         do not match the available volume.
     */
    void applyContinuity(int week, double upstream_source_inflow,
                             double wastewater_inflow,
                             vector<double> &demand_outflow) override;

    /**
     * @brief Sets the reservoir and all allocated volumes to full capacity.
     * 
     * This function marks the reservoir as full by setting its state to maximum capacity. Additionally, 
     * it ensures that all allocations for utilities are set to their respective maximum capacities.
     * This function overrides the base class `Reservoir` function to include continuity calculations.
     * 
     * @return void
     */
    void setFull() override;

    /**
     * @brief Retrieves the available allocated volume for a specific utility.
     * 
     * This function returns the current available volume of water allocated to the specified utility.
     * It retrieves the allocated volume from the `available_allocated_volumes` vector using the `utility_id`.
     * This function overrides the base class `Reservoir` function to include continuity calculations.
     * 
     * @param utility_id The identifier of the utility for which the allocated volume is retrieved.
     * 
     * @return double The available allocated volume for the specified utility.
     * 
     */
    double getAvailableAllocatedVolume(int utility_id) override;

    /**
     * @brief Removes a utility's allocated volume of water from the reservoir updates the reservoir state.
     * 
     * This function reduces the allocated volume for a specific utility by the given amount, adjusts the 
     * total available volume of the reservoir, and updates the total demand and policy-added demand.
     * This function overrides the base class `Reservoir` function to include continuity calculations.
     * 
     * @param allocation_id The identifier of the allocation (utility) from which water is to be removed.
     * @param volume The volume of water to be removed.
     * 
     * @return void
     */
    void removeWater(int allocation_id, double volume) override;

    /**
     * @brief Retrieves the allocated capacity for a specific utility.
     * 
     * This function overrides the base class `Reservoir` function to allow specific utility-related information to be retrieved.
     * 
     * @param utility_id The identifier of the utility for which the allocated capacity is retrieved.
     * 
     * @return double The allocated capacity for the specified utility.
     * 
     */
    double getAllocatedCapacity(int utility_id) override;

    /**
     * @brief Retrieves the allocated fraction of capacity for a specific utility.
     * 
     * This function returns the fraction of the reservoir's total capacity that is allocated to a specific utility.
     * This function overrides the base class `Reservoir` function to allow specific utility-related information to be retrieved.
     * 
     * @param utility_id The identifier of the utility for which the allocated fraction is retrieved.
     * 
     * @return double The allocated fraction for the specified utility (unitless, typically between 0 and 1).
     * 
     */
    double getAllocatedFraction(int utility_id) override;

    /**
     * @brief Adds capacity to the reservoir and adjusts allocations for each utility.
     * 
     * This function increases the reservoir's total capacity and redistributes the added capacity among 
     * utilities based on their allocated fractions.
     * This function overrides the base class `Reservoir` function to include continuity calculations.
     * 
     * @param capacity The additional capacity to be added to the reservoir.
     * @param utility_id The identifier of the utility initiating the capacity increase.
     * 
     * @return void
     * 
     */
    void addCapacity(double capacity, int utility_id) override;

    /**
     * @brief Adds treatment capacity to the reservoir for a specific utility and updates allocation fractions.
     * This function increases the plant's treatment capacity for a specified utility and recalculates the treatment allocation fractions for all utilities.
     * 
     * Both the total treatment capacity of the new plant and the fraction of the treatment
     * capacity allocated to a given utility allow for joint treatment plants. 
     * To have one utility only building an exclusive plant, the fraction will be 1.
     * 
     * This function overrides the base class `Reservoir` function to include continuity calculations.
     * 
     * @param added_plant_treatment_capacity The additional treatment capacity to be added.
     * @param utility_id The identifier of the utility receiving the additional treatment capacity.
     * 
     * @return void
     */
    void addTreatmentCapacity(const double added_plant_treatment_capacity, int utility_id)
    override;

    /**
     * @brief Retrieves the allocated treatment capacity for a specific utility.
     * 
     * This function returns the treatment capacity allocated to the specified utility. 
     * If the utility ID corresponds to the water quality allocation, an exception is thrown as it does not have an allocated treatment capacity.
     * This function overrides the base class `Reservoir` function to reference a specific utility.
     * 
     * @param utility_id The identifier of the utility whose treatment capacity is being queried.
     * 
     * @return double The allocated treatment capacity (m³/day) for the specified utility.
     * 
     * @throws std::invalid_argument If `utility_id` corresponds to the water quality allocation.
     */
    double getAllocatedTreatmentCapacity(int utility_id) const override;

    /**
     * @brief Retrieves the supply allocated fraction for a specific utility.
     * 
     * This function returns the supply allocated fraction for the specified utility. 
     * If the utility ID corresponds to the water quality allocation, an exception is thrown because water quality allocation cannot be used for supply.
     * This function overrides the base class `Reservoir` function to reference a specific utility.
     * 
     * @param utility_id The identifier of the utility whose supply allocation fraction is being queried.
     * 
     * @return double The supply allocated fraction for the specified utility.
     * 
     * @throws std::invalid_argument If `utility_id` corresponds to the water quality allocation.
     */
    double getSupplyAllocatedFraction(int utility_id) override;

    /**
     * @brief Performs the mass balance calculation for the reservoir with the water quality (WQ) pool.
     * 
     * This function calculates the mass balance for the reservoir by considering both the allocations for utilities
     * and the water quality pool. It splits the incoming net inflow among the utilities, subtracts their respective demands, 
     * and handles cases where the allocated volume goes negative by charging the water quality pool. The function also
     * flags when an allocation exceeds its capacity.
     * 
     * @param net_inflow The net inflow into the reservoir (inflow minus outflow from upstream sources).
     * @param demand_outflow A vector containing the demand outflows for each utility.
     * 
     * @return bool Returns `true` if overallocation occurs (i.e., any allocation exceeds its capacity), `false` otherwise.
     */
    bool
    mass_balance_with_wq_pool(double net_inflow, vector<double> &demand_outflow);

    /**
     * @brief Performs the mass balance calculation for the reservoir without considering the water quality (WQ) pool.
     * 
     * This function calculates the mass balance for the reservoir when there is no water quality pool. It splits the incoming 
     * net inflow among the utilities based on their allocation fractions, subtracts their respective demands, and checks if any 
     * allocation exceeds its capacity.
     * 
     * @param net_inflow The net inflow into the reservoir (inflow minus outflow from upstream sources).
     * @param demand_outflow A vector containing the demand outflows for each utility.
     * 
     * @return bool Returns `true` if overallocation occurs (i.e., any allocation exceeds its capacity), `false` otherwise.
     */
    bool
    mass_balance_without_wq_pool(double net_inflow, vector<double> &demand_outflow);

    /**
     * @brief Sets the reservoir online, initializing available volume and allocated volumes.
     * 
     * This function sets the reservoir's state to "online" by initializing its available volume to zero and setting all
     * allocated volumes to zero for each utility with allocations. This represents the transition from an offline state to
     * an active state where the reservoir is ready to receive inflows.
     * This function overrides the base class `Reservoir` function to include continuity calculations.
     * 
     * @return void
     */
    void setOnline() override;
};


#endif //TRIANGLEMODEL_ALLOCATEDRESERVOIR_H
