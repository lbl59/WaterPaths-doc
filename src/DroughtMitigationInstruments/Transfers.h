/**
 * @class Transfers
 * @brief The `Transfers` class is a subclass of the `DroughtMitigationPolicy` class that implements treated water transfers based on risk of failure (ROF) triggers.
 * 
 * Created by bernardo on 2/21/17.
 */
#ifndef TRIANGLEMODEL_TRANSFERS_H
#define TRIANGLEMODEL_TRANSFERS_H

#include "Base/DroughtMitigationPolicy.h"
#include "../Utils/QPSolver/QuadProg++.h"
#include "../Utils/Graph/Graph.h"

class Transfers : public DroughtMitigationPolicy {
private:

    /**
     * @brief The ID of the utility selling water to other utilities to fulfil transfer requests.
     */
    const int source_utility_id;
    /**
     * @brief Treatment capacity to be left unused in source utility.
     * 
     */
    const double source_treatment_buffer;
    /**
     * @brief The average pipe capacity that limits the volume of transfers that can be conveyed through the network.
     * 
     */
    double average_pipe_capacity = 0;
    /**
     * @brief The ID of the water source from which the source utility will be transferring water.
     */
    int transfer_water_source_id;
    /**
     * @brief The vector mapping the ID of each utility to their topological vertex in the water transfer network.
     */
    vector<int> util_id_to_vertex_id;
    /**
     * @brief The IDs of the utilities purchasing water transfers from `source_utility_id`.
     */
    vector<int> buyers_ids;
    /**
     * @brief A vector of each utility's water allocations.
     */
    vector<double> allocations;
    /**
     * @brief The volume of water conveyed through a pipe during a water transfer.
     */
    vector<double> conveyed_volumes;
    /**
     * @brief A vector of each buyer's ROF threshold after which they will request a water transfer.
     */
    vector<double> buyers_transfer_triggers;
    /**
     * @brief The short-term ROF threshold above which the seller will not fulfil transfer requests. 
     */
    double seller_transfer_trigger;
    /**
     * @brief A vector of flow rates and allocations for each pipe and utility.
     * 
     */
    vector<double> flow_rates_and_allocations;
    /**
     * @brief A pointer to the source Utility object.
     */
    Utility *source_utility = nullptr;
    /**
     * @brief A pointer to the source WaterSource object.
     */
    WaterSource *transfer_water_source = nullptr;
    /**
     * @brief The quadratic programming matrices and vectors.
     */
    Matrix<double> H, Aeq, A;
    /**
     * @brief The quadratic programming vectors.
     * 
     */
    Vector<double> f, beq, b, allocations_aux, lb, ub;

public:
    /**
     * @brief Constructs a Transfers object to manage water transfer allocations among utilities without seller information.
     *        This function initializes all data structures and ensures input validity.
     * 
     * This constructor allocates transfers by trying to minimize the mean square error between the
     * optimal allocations and allocation requests, and between the available amount of water from source 
     * and actual conveyed capacity, which is limited by pipe conveyance capacities.
     *
     * @param id Transfer policy ID.
     * @param source_utility_id The ID of the utility selling water to other utilities to fulfil transfer requests.
     * @param transfer_water_source_id The ID of the water source from which the source utility will be transferring water.
     * @param source_treatment_buffer Treatment capacity to be left unused in source utility.
     * @param buyers_ids IDs of the buyer utilities.
     * @param pipe_transfer_capacities Capacities of transfer pipes.
     * @param buyers_transfer_triggers A vector of each buyer's ROF threshold after which they will request a water transfer.
     * @param utilities_graph A Graph object representing utilities and their connections.
     * @param conveyance_costs Amount charged by pipe owning utility to convey water to another utility.
     * @param pipe_owner IDs of utilities owning each pipe.
     * 
     * @throws std::invalid_argument if the source utility is also listed as a buyer.
     */
    Transfers(
            const int id, const int source_utility_id,
            int transfer_water_source_id, const double source_treatment_buffer,
            const vector<int> &buyers_ids,
            const vector<double> &pipe_transfer_capacities,
            const vector<double> &buyers_transfer_triggers,
            const Graph utilities_graph, vector<double> conveyance_costs,
            vector<int> pipe_owner);

    /**
     * @brief Constructs a Transfers object to manage water transfer allocations among utilities including seller information.
     *        This function initializes all data structures and ensures input validity.
     * 
     * This constructor allocates transfers by trying to minimize the mean square error between the
     * optimal allocations and allocation requests, and between the available amount of water from source 
     * and actual conveyed capacity, which is limited by pipe conveyance capacities.
     *
     * @param id Transfer policy ID.
     * @param source_utility_id The ID of the utility selling water to other utilities to fulfil transfer requests.
     * @param transfer_water_source_id The ID of the water source from which the source utility will be transferring water.
     * @param source_treatment_buffer Treatment capacity to be left unused in source utility.
     * @param buyers_ids IDs of the buyer utilities.
     * @param pipe_transfer_capacities Capacities of transfer pipes.
     * @param buyers_transfer_triggers A vector of each buyer's ROF threshold after which they will request a water transfer.
     * @param seller_transfer_trigger The short-term ROF threshold above which the seller will not fulfil transfer requests. 
     * @param utilities_graph A Graph object representing utilities and their connections.
     * @param conveyance_costs Amount charged by pipe owning utility to convey water to another utility.
     * @param pipe_owner IDs of utilities owning each pipe.
     * 
     * @throws std::invalid_argument if the source utility is also listed as a buyer.
     */
    Transfers(
            const int id, const int source_utility_id,
            int transfer_water_source_id, const double source_treatment_buffer,
            const vector<int> &buyers_ids,
            const vector<double> &pipe_transfer_capacities,
            const vector<double> &buyers_transfer_triggers,
            const double seller_transfer_trigger,
            const Graph utilities_graph, vector<double> conveyance_costs,
            vector<int> pipe_owner);

    /**
     * @brief Creates a copy of an existing Transfers object.
     *        This function duplicates all properties and settings of the given Transfers object.
     *
     * @param transfers The Transfers object to be copied.
     */
    Transfers(const Transfers &transfers);

    /**
     * @brief Destroys the Transfers object, releasing allocated resources.
     */
    ~Transfers();

    /**
     * @brief Retrieves the current water allocations.
     *
     * @return A constant reference to the vector of allocations.
     */
    const vector<double> &getAllocations() const;

    /**
     * @brief Applies the water transfer policy for a given week.
     *
     * @param week The current week to solve.
     * 
     * @return void
     */
    void applyPolicy(int week) override;

    /**
     * @brief Adds water sources, as well as buying and purchasing utilities to the Transfers policy 
     * for initialization.
     * FIXME: RIGHT NOW TRANSFERS CAN ONLY HAVE ONE SOURCE. THIS NEEDS TO BE EXPANDED.
     * 
     * @param system_utilities Vector of pointers to the system utilities.
     * @param water_sources Vector of pointers to water sources.
     * @param min_env_flow_controls Vector of minimum environmental flow controls.
     * 
     * @return void
     * 
     * @throws std::invalid_argument if utilities or water sources are already assigned.
     * 
     * @see solve_QP
     */
    void addSystemComponents(vector<Utility *> system_utilities,
                                 vector<WaterSource *> water_sources,
                                 vector<MinEnvFlowControl *> min_env_flow_controls) override;

    /**
     * @brief Solves the Quadratic Programming (QP) problem to find optimal allocations given
     * transfer requests and constraints in conveyance capacities.
     *
     * @param allocation_requests Allocation requests by each utility (0 for
     * utilities not requesting transfers).
     * @param available_transfer_volume Total volume available for transfers from source utilities.
     * @param min_transfer_volume Minimum transfer volume to each utility.
     * @param week The current week to solve.
     *
     * @return A vector of flow rates and allocations computed by the QP solver.
     */
    vector<double>
    solve_QP(vector<double> allocation_requests, double available_transfer_volume, double min_transfer_volume,
                 int week);

    /**
     * @brief Sets the realization parameters for the current Transfers policy.
     * 
     * This function overrides the base class function in DroughtMitigationPolicy.
     *
     * @param realization_id ID of the realization.
     * @param utilities_rdm Random decision matrix for utilities.
     * @param water_sources_rdm Random decision matrix for water sources.
     * @param policy_rdm Random decision matrix for the policy.
     */
    void setRealization(unsigned long realization_id, vector<double> &utilities_rdm,
                        vector<double> &water_sources_rdm, vector<double> &policy_rdm) override;

};


#endif //TRIANGLEMODEL_TRANSFERS_H
