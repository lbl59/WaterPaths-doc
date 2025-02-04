//
// Created by bernardo on 2/21/17.
//

#include <numeric>
#include <algorithm>
#include "Transfers.h"
#include "../Utils/Utils.h"

Transfers::Transfers(
        const int id, const int source_utility_id,
        int transfer_water_source_id, const double source_treatment_buffer,
        const vector<int> &buyers_ids,
        const vector<double> &pipe_transfer_capacities,
        const vector<double> &buyers_transfer_triggers,
        const Graph utilities_graph, vector<double> conveyance_costs,
        vector<int> pipe_owner)
        : DroughtMitigationPolicy(id, TRANSFERS),
          source_utility_id(source_utility_id),
          source_treatment_buffer(source_treatment_buffer),
          transfer_water_source_id(transfer_water_source_id),
          buyers_ids(buyers_ids),
          buyers_transfer_triggers(buyers_transfer_triggers) {

    for (int i : buyers_ids)
        if (i == source_utility_id)
            throw std::invalid_argument("Source utility cannot be a buyer "
                                                "utility as well.");

    utilities_ids = buyers_ids;
    utilities_ids.push_back(source_utility_id);
    allocations = vector<double>(utilities_ids.size(), 0);
    for (const double &d : pipe_transfer_capacities)
        average_pipe_capacity += d;

    vector<vector<double>> continuity_matrix =
            utilities_graph.getContinuityMatrix();
    continuity_matrix[source_utility_id][
            continuity_matrix.size() + source_utility_id] = 1;

    int max_buyer_id = *std::max_element(buyers_ids.begin(),
                                                  buyers_ids.end());
    util_id_to_vertex_id = vector<int>(max_buyer_id + 1, NON_INITIALIZED);

    for (int i = 0; i < (int) buyers_ids.size(); ++i) {
        util_id_to_vertex_id[buyers_ids.at((unsigned long) i)] = i;
    }

    unsigned long n_flow_rates_Q_source =
             pipe_transfer_capacities.size() + 1;
    unsigned long n_allocations = buyers_ids.size();
    unsigned long n_vars = n_flow_rates_Q_source + n_allocations;

    H.resize(0, n_vars, n_vars);
    for (unsigned long i = 0; i < n_vars; ++i)
        if (i < n_flow_rates_Q_source) H[i][i] = 1e-6;
        else H[i][i] = 2;

    f.resize(0, n_vars);

    Aeq.resize(0, n_allocations + 1, n_vars);
    for (unsigned long i = 0; i < continuity_matrix.size(); ++i) {
        for (unsigned long j = 0; j < continuity_matrix.at(i).size(); ++j) {
            Aeq[i][j] = continuity_matrix.at(i).at(j);
        }
    }

    beq.resize(0, n_allocations + 1);

    A.resize(0, 0);

    b.resize(0);

    lb.resize(0, n_vars);
    ub.resize(0, n_vars);
    for (unsigned long i = 0; i < pipe_transfer_capacities.size(); ++i) {
        lb[i] = -pipe_transfer_capacities[i];
        ub[i] = pipe_transfer_capacities[i];
    }
}

Transfers::Transfers(
        const int id, const int source_utility_id,
        int transfer_water_source_id, const double source_treatment_buffer,
        const vector<int> &buyers_ids,
        const vector<double> &pipe_transfer_capacities,
        const vector<double> &buyers_transfer_triggers,
        const double seller_transfer_trigger,
        const Graph utilities_graph, vector<double> conveyance_costs,
        vector<int> pipe_owner)
        : DroughtMitigationPolicy(id, TRANSFERS),
          source_utility_id(source_utility_id),
          source_treatment_buffer(source_treatment_buffer),
          transfer_water_source_id(transfer_water_source_id),
          buyers_ids(buyers_ids),
          seller_transfer_trigger(seller_transfer_trigger),
          buyers_transfer_triggers(buyers_transfer_triggers) {

    // Validate that the source utility is not also a buyer
    for (int i : buyers_ids)
        if (i == source_utility_id)
            throw std::invalid_argument("Source utility cannot be a buyer "
                                                "utility as well.");
    // Initialize internal structures for allocations, pipe capacities, and utility mappings.
    utilities_ids = buyers_ids;
    utilities_ids.push_back(source_utility_id);
    allocations = vector<double>(utilities_ids.size(), 0);
    for (const double &d : pipe_transfer_capacities)
        average_pipe_capacity += d;

    // Sets up matrices for Quadratic Programming (QP) problem solving.
    vector<vector<double>> continuity_matrix =
            utilities_graph.getContinuityMatrix();
    continuity_matrix[source_utility_id][
            continuity_matrix.size() + source_utility_id] = 1;

    int max_buyer_id = *std::max_element(buyers_ids.begin(),
                                                  buyers_ids.end());
    util_id_to_vertex_id = vector<int>(max_buyer_id + 1, NON_INITIALIZED);
    
    // Map buyer's IDs to vertexes ID starting from 0 for the sake of determining payments.
    for (int i = 0; i < (int) buyers_ids.size(); ++i) {
        util_id_to_vertex_id[buyers_ids.at((unsigned long) i)] = i;
    }

    unsigned long n_flow_rates_Q_source =
             pipe_transfer_capacities.size() + 1;
    unsigned long n_allocations = buyers_ids.size();
    unsigned long n_vars = n_flow_rates_Q_source + n_allocations;

    H.resize(0, n_vars, n_vars);
    for (unsigned long i = 0; i < n_vars; ++i)
        if (i < n_flow_rates_Q_source) H[i][i] = 1e-6;
        else H[i][i] = 2;

    // Create f vector of 0's to be filled when policy is applied.
    f.resize(0, n_vars);

    // Size is number of variables x number of utilities (source + requesting utilities). 
    // Continuity matrix: +1 for flow entering utility and -1 for leaving.

    // Begin calculation of the continuity matrix for utilities' mass balance.
    Aeq.resize(0, n_allocations + 1, n_vars);
    for (unsigned long i = 0; i < continuity_matrix.size(); ++i) {
        for (unsigned long j = 0; j < continuity_matrix.at(i).size(); ++j) {
            Aeq[i][j] = continuity_matrix.at(i).at(j);
        }
    }

    // Create beq vector of 0 so that mass balance is enforced on the nodes (utilities).
    beq.resize(0, n_allocations + 1);

    // Create empty A and b. They'll be re-dimensioned in the QP solver.
    A.resize(0, 0);
    b.resize(0);

    // Fill in pipe capacities part of lower and upper bound vectors.
    lb.resize(0, n_vars);
    ub.resize(0, n_vars);
    for (unsigned long i = 0; i < pipe_transfer_capacities.size(); ++i) {
        lb[i] = -pipe_transfer_capacities[i];
        ub[i] = pipe_transfer_capacities[i];
    }
}

Transfers::Transfers(const Transfers &transfers) :
        DroughtMitigationPolicy(transfers.id,
                                TRANSFERS),
        source_utility_id(transfers.source_utility_id),
        source_treatment_buffer(transfers.source_treatment_buffer) {

    buyers_ids = transfers.buyers_ids;
    buyers_transfer_triggers = transfers.buyers_transfer_triggers;
    seller_transfer_trigger = transfers.seller_transfer_trigger;
    flow_rates_and_allocations = transfers.flow_rates_and_allocations;
    transfer_water_source_id = transfers.transfer_water_source_id;
    H = transfers.H;
    Aeq = transfers.Aeq;
    A = transfers.A;
    f = transfers.f;
    beq = transfers.beq;
    b = transfers.b;
    lb = transfers.lb;
    ub = transfers.ub;
    allocations_aux = transfers.allocations_aux;
    utilities_ids = transfers.utilities_ids;
    util_id_to_vertex_id = transfers.util_id_to_vertex_id;
    allocations = transfers.allocations;
    conveyed_volumes = transfers.conveyed_volumes;
    average_pipe_capacity = transfers.average_pipe_capacity;
}

Transfers::~Transfers() {
    source_utility = nullptr;
    transfer_water_source = nullptr;
}

void Transfers::addSystemComponents(vector<Utility *> system_utilities,
                                    vector<WaterSource *> water_sources,
                                    vector<MinEnvFlowControl *> min_env_flow_controls) {

    if (!realization_utilities.empty())
        throw std::invalid_argument("Utility were already assigned to "
                                            "transfer policy.");

    //FIXME: RIGHT NOW TRANSFERS CAN ONLY HAVE ONE SOURCE. THIS NEEDS TO BE EXPANDED.
    for (Utility *u : system_utilities) {
        auto it = std::find(buyers_ids.begin(), buyers_ids.end(), u->id);
        if (u->id == source_utility_id)
            source_utility = u; // source of transfers
        else if (it != buyers_ids.end())
            realization_utilities.push_back(u);

    }
    if (transfer_water_source != nullptr)
        throw invalid_argument("Water sources already assigned to transfer "
                                         "policy.");
    else
        transfer_water_source = water_sources[transfer_water_source_id];
}

void Transfers::applyPolicy(int week) {
    vector<double> requesting_utilities_rofs(buyers_ids.size(), 0);
    vector<double> requesting_utilities_deficit(buyers_ids.size(), INITIAL_MASSIVE_VALUE);
    std::fill(allocations.begin(), allocations.end(), 0);

    unsigned long n_vars = f.size();
    unsigned long n_allocations = buyers_ids.size();
    int n_utilities = n_allocations + 1;
    int n_pipes = n_vars - n_utilities;

    unsigned long vertex_id; // position of utility id in the buyers_transfer_triggers vector.
    double sum_rofs = 0;
    int utilities_requesting_transfers = 0;

    // Get summation of rofs of utilities needing transfers.
    // Identify utilities requiring transfers based on ROF thresholds.
    for (auto u : realization_utilities) {
        vertex_id = (unsigned long) util_id_to_vertex_id.at((unsigned long) u->id);
        if (u->getRisk_of_failure() > buyers_transfer_triggers.at(vertex_id)) {
            sum_rofs += u->getRisk_of_failure();
            requesting_utilities_rofs.at(vertex_id) = u->getRisk_of_failure();
            utilities_requesting_transfers++;

            // FEB 2020: if treatment capacity limitations are triggering transfers,
            // set cap on request. Record the expected "deficit" here (using last-week demands):
            if (u->getTreatmentRisk_of_failure() > buyers_transfer_triggers.at(vertex_id) & week > 0)
                requesting_utilities_deficit.at(vertex_id) =
                        u->getUnrestrictedDemand(week-1) -
                        u->getTotal_treatment_capacity() * TREATMENT_CAPACITY_RATIO_FAIL;
        }
    }

    // FEB 2020: ONLY HAVE TRANSFERS IF SELLER ROF IS SUFFICIENTLY LOW
    if (source_utility->getRisk_of_failure() < seller_transfer_trigger) {
        // Check if transfers will be needed and, if so, perform the transfers.
        if (sum_rofs > 0) {
            vector<double> transfer_requests((unsigned long) n_allocations, 0);

            // Total volume available for transfers in source utility.
            double available_transfer_volume =
                    (source_utility->getTotal_treatment_capacity()
                     - source_treatment_buffer) * TRANSFER_PEAKING_FACTOR
                    - source_utility->getUnrestrictedDemand();

            if (available_transfer_volume > 0) {

                // Minimum volume to be allocated to any utility.
                double min_transfer_volume = available_transfer_volume
                                             / (utilities_requesting_transfers + 1);

                // Split up total volume available among the utilities proportionally to their ROFs.
                // FIXME: FIGURE OUT SCALING FACTOR, MAYBE BASED ON TOTAL DEMAND TO PREVENT BIGGEST UTILITY WITH HIGH RISK FROM GETTING ALL WATER.
                for (unsigned long i = 0; i < n_allocations; ++i) {
                    transfer_requests[i] =
                            available_transfer_volume * requesting_utilities_rofs[i]
                            / sum_rofs;

                    // FEB 2020: include consideration of request caps if request is due to treatment capacity shortage
                    if (transfer_requests[i] > requesting_utilities_deficit[i])
                        transfer_requests[i] = requesting_utilities_deficit[i];
                }

                // Solves a QP problem to determine optimal transfers via allocations and flow rates through inter-utility connections.
                flow_rates_and_allocations = solve_QP(transfer_requests,
                                                      available_transfer_volume,
                                                      min_transfer_volume,
                                                      week);
                conveyed_volumes = vector<double>(
                        flow_rates_and_allocations.begin(),
                        flow_rates_and_allocations.begin() + n_pipes);

                allocations = vector<double>(flow_rates_and_allocations.begin() +
                                             n_pipes,
                                             flow_rates_and_allocations.end());
                allocations[source_utility_id] = -allocations[source_utility_id];

                // Mitigate demands.
                double sum_allocations = 0;
                int price_week = Utils::weekOfTheYear(week);

                for (unsigned long id = 0; id < util_id_to_vertex_id.size(); ++id) {
                    int bid = util_id_to_vertex_id[id];
                    if (bid != NON_INITIALIZED) {
                        realization_utilities[bid]->setDemand_offset
                                (allocations[id],
                                 source_utility->waterPrice(price_week));
                        sum_allocations += allocations[id];
                        transfer_water_source->removeWater(id,
                                                           allocations[id]);
                    }
                }

                // Check that selling utility allocation is reduced
                if (sum_allocations > 0) // if transfers occur
                    if (allocations[source_utility_id] >= 0) // if LP did not account for seller reduction
                        allocations[source_utility_id] = -sum_allocations; // total sales subtracted from seller allocation

                source_utility->setDemand_offset(allocations[source_utility_id],
                                                 2. * source_utility->waterPrice
                                                         (price_week));
            }
        }
    }
}

vector<double> Transfers::solve_QP(
        vector<double> allocation_requests,
        double available_transfer_volume, double min_transfer_volume,
        int week) {

    // Set up the QP matrices and bounds based on allocation requests and constraints.
    vector<double> flow_rates_and_allocations;
    auto n_allocations = (unsigned long) allocation_requests.size();
    unsigned long n_vars = f.size();
    unsigned long n_pipes = n_vars - n_allocations - 1;
    Vector<double> x;

    Matrix<double> G = this->H;

    // Set f vector to allocated to 2 * target_allocation.
    for (unsigned long i = 0; i < allocation_requests.size(); ++i) {
        f[n_pipes + buyers_ids[i]] = -2 * allocation_requests[i];
    }

    // Set allocation bounds to 0 if no allocation is requested, and to
    // available transfer volume otherwise.
    for (unsigned long i = 0; i < n_allocations; ++i) {
        if (allocation_requests[i] == NONE) {
            lb[n_pipes + buyers_ids[i]] = NONE;
            ub[n_pipes + buyers_ids[i]] = NONE;
        } else {
            double allocation_available = transfer_water_source
                    ->getAvailableAllocatedVolume(buyers_ids[i]);
            lb[n_pipes + buyers_ids[i]] = min(min_transfer_volume,
                                              allocation_available);
            ub[n_pipes + buyers_ids[i]] = min(
                    realization_utilities[i]->getUnrestrictedDemand(week) *
                    realization_utilities[i]->getDemand_multiplier(),
                    allocation_available);
        }
    }

    // Set upper bound (ub) of the last utility (source) to total amount of water
    // available. This allows for a smaller transferred volume, in case
    // pipes cannot convey full amount available, but sets the cap as total
    // amount available.
    ub[n_pipes + source_utility_id] = available_transfer_volume;

    // Prevent minimum allocation from being more than capacity of pipes
    // connected to utility. If it happens, decrease it to half of the sum of
    // adjacent pipes capacities so that it avoid issues with bottle-necks
    // through the network. The factor of 2 can be changed to any number
    // smaller than 1.
    double max_flow;
    for (unsigned long i = 0; i < n_allocations + 1; ++i) { 
        // skip source
        if (i != (unsigned long) source_utility_id) {
            max_flow = 0;
            for (unsigned long j = 0; j < n_pipes; ++j) {
                max_flow += abs(Aeq[i][j] * ub[j]);
            }
            if (lb[n_pipes + i] > max_flow)
                lb[n_pipes + i] = max_flow / 2;
        }
    }

    // Initialize allocations vector with 0s.
    x.resize(0, n_vars);

    // Run quadratic programming solver to compute optimal allocations
    solve_quadprog_matlab_syntax(G, f, Aeq, beq, A, b, lb, ub, x);

    // Convert xector x into a vector.
    for (unsigned long i = 0; i < x.size(); ++i) {
        flow_rates_and_allocations.push_back(x[i]);
    }

    return flow_rates_and_allocations;
}

const vector<double> &Transfers::getAllocations() const {
    return allocations;
}

void Transfers::setRealization(unsigned long realization_id, vector<double> &utilities_rdm,
                               vector<double> &water_sources_rdm, vector<double> &policy_rdm) {

}
