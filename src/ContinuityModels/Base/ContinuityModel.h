//
// Created by bernardo on 1/12/17.
//

#ifndef TRIANGLEMODEL_CONTINUITYMODEL_H
#define TRIANGLEMODEL_CONTINUITYMODEL_H

#include "../../SystemComponents/WaterSources/Base/WaterSource.h"
#include "../../Utils/Constants.h"
#include "../../SystemComponents/Utility/Utility.h"
#include "../../Utils/Graph/Graph.h"
#include "../../Controls/Base/MinEnvFlowControl.h"
#include <vector>

using namespace Constants;
using namespace std;

class ContinuityModel {

protected:

    /**
     * @brief A vector of WaterSource objects representing the water sources in the system.
     */
    vector<WaterSource *> continuity_water_sources;
    /**
     * @brief A vector of Utility objects representing the utilities in the system.
     */
    vector<Utility *> continuity_utilities;
    /**
     * @brief A vector of MinEnvFlowControl objects representing the minimum environmental flow controls in the system.
     */
    vector<MinEnvFlowControl *> min_env_flow_controls;
    /**
     * @brief A bidirectional Graph object representing the connections between the water sources in the system.
     */
    Graph water_sources_graph;
    /**
     * @brief A 2D vector of integers representing the water sources supplying each utility, irrespective
     * of their online/offline status.
     */
    vector<vector<int> > water_sources_to_utilities;
    /**
     * @brief A 2D vector of integers representing the water sources supplying each utility that are
     * is already built and functional.
     */
    vector<vector<int> > water_sources_online_to_utilities;
    /**
     * @brief A 2D vector of integers representing the utilities that draw water from each water source.
     */
    vector<vector<int> > utilities_to_water_sources;
    /**
     * @brief A vector of integers representing the downstream source of each water source.
     */
    vector<int> downstream_sources;
    /**
     * @brief A vector of integers representing the topological order of each water source
     */
    const vector<int> sources_topological_order;
    /**
     * @brief A vector of doubles representing the capacities of each water source.
     */
    vector<double> water_sources_capacities;
    /**
     * @brief A vector of doubles representing the capacities of each utility.
     */
    vector<double> utilities_capacities;
    /**
     * @brief A 2D vector of doubles representing the demand timeseries of each utility.
     */
    vector<vector<double>> demands;
    /**
     * @brief A vector of doubles representing the DU factor multipliers for each utility's water demands.
     */
    vector<double> utilities_rdm;
    /**
     * @brief A vector of doubles representing the DU factor multipliers scaling the construction,
     * permitting, and financing aspects of a water source.
     */
    vector<double> water_sources_rdm;
    /**
     * @brief An integer representing the number of utilities in the system.
     */
    const int n_utilities;
    /**
     * @brief An integer representing the number of water sources in the system.
     */
    const int n_sources;
    /**
     * @brief An array of integers representing the number of weeks to be added to the current week
     * to get the actual current week of the realization.
     */
    int delta_realization_weeks[NUMBER_REALIZATIONS_ROF + 1];

public:
    /**
     * @brief An unsigned long integer representing the realization id.
     */
    const unsigned long realization_id;

    /**
     * @brief Constructs a ContinuityModel object to simulate water flow and demand.
     * 
     * This constructor initializes a continuity model, linking water sources, utilities,
     * and minimum environmental flow controls to simulate water distribution and demand
     * over time. It sets up various internal tables and vectors for faster calculations 
     * and ensures proper linking of utilities to water sources. Additionally, it handles 
     * initialization for various properties like capacities and realization data.
     * 
     * @param water_sources A vector of pointers to the water sources in the model.
     * @param utilities A vector of pointers to the utilities in the model.
     * @param min_env_flow_controls A vector of pointers to the minimum environmental flow controls.
     * @param water_sources_graph A graph representing the water sources and their relationships.
     * @param water_sources_to_utilities A matrix mapping each utility to the water sources it uses.
     * @param utilities_rdm A vector of randomization factors for the utilities.
     * @param water_sources_rdm A vector of randomization factors for the water sources.
     * @param realization_id An identifier for the current realization.
     * 
     * @throws invalid_argument If any of the water sources are not correctly linked to utilities,
     *                          or if utilities lack storage capacity, an exception is thrown.
     */
    ContinuityModel(vector<WaterSource *> &water_sources, vector<Utility *> &utilities,
                    vector<MinEnvFlowControl *> &min_env_flow_controls,
                    const Graph &water_sources_graph,
                    const vector<vector<int>> &water_sources_to_utilities,
                    vector<double> &utilities_rdm,
                    vector<double> &water_sources_rdm,
                    unsigned long realization_id);

    /**
     * @brief Copy constructor for a new Continuity Model object
     * 
     * @param continuity_model A reference to the ContinuityModel object to be copied.
     * 
     * @return A new ContinuityModel object with the same properties as the original.
     */
    ContinuityModel(ContinuityModel &continuity_model);

    /**
     * @brief Destructor for ContinuityModel to clean up dynamically allocated resources.
     * 
     * This destructor is responsible for deleting all dynamically allocated objects 
     * associated with the ContinuityModel instance, including water sources, utilities,
     * and minimum environmental flow controls. It ensures proper cleanup to avoid memory 
     * leaks when the model is no longer needed.
     * 
     * This is a virtual function meant to be overridden by subclasses to ensure proper 
     * cleanup of any additional resources they may have allocated.
     *
     */
    virtual ~ContinuityModel();

    /**
     * @brief Performs a single step of the continuity calculation for water sources and utilities.
     * 
     * This function performs a mass balance calculation for each water source in the
     * continuity model, updating the available volume and accounting for upstream spillage,
     * wastewater discharges, and demand projections. It also updates the minimum environmental
     * flows and utility storage volumes.
     * 
     * @param week The current week of the simulation.
     * @param rof_realization The realization index for the ROF calculation (use `NON_INITIALIZED` for a regular simulation).
     * @param apply_demand_buffer Flag to indicate whether to apply the demand buffer to the simulation.
     * @param apply_demand_projection Flag to indicate whether to apply the demand projection to the simulation.
     * 
     * @return void
     */
    void continuityStep(
            int week, int rof_realization = -1, 
            bool apply_demand_buffer = false, 
            bool apply_demand_projection = false);

    /**
     * @brief Sets the realization ID for utilities, water sources, and minimum environmental flow controls.
     * 
     * This function assigns a specific realization ID to each utility, water source, and minimum environmental flow control
     * in the continuity model. This ensures that the correct realization data is used for the simulation.
     * 
     * @param realization_id The ID of the realization to be set.
     * @param utilities_rdm A vector containing random demand values for each utility, indexed by utility ID.
     * @param water_sources_rdm A vector containing random demand values for each water source, indexed by source ID.
     * 
     * @return void
     */
    void setRealization(unsigned long realization_id, vector<double> &utilities_rdm,
                        vector<double> &water_sources_rdm);

    /**
     * @brief Retrieves a list of online downstream water sources for each water source.
     * 
     * This function iterates through the water sources in topological order and identifies the first online downstream
     * source for each water source. The result is a vector where each element represents the first online downstream 
     * source for the corresponding water source, or `NON_INITIALIZED` if no online downstream source is found.
     * 
     * @return A vector of integers representing the IDs of the online downstream sources for each water source.
     */
    vector<int> getOnlineDownstreamSources();

    /**
     * @brief Retrieves the list of water sources in the continuity model.
     * 
     * This function returns a constant reference to the vector of water sources that are included in the continuity model.
     * The water sources represent all sources involved in the model's calculations for continuity.
     * 
     * @return A constant reference to the vector of `WaterSource` pointers.
     */
    const vector<WaterSource *> &getContinuity_water_sources() const;

    /**
     * @brief Retrieves the list of utilities in the continuity model.
     * 
     * This function returns a constant reference to the vector of utilities that are included in the continuity model.
     * The utilities represent all entities that rely on the water sources for their operations in the model's calculations.
     * 
     * @return A constant reference to the vector of `Utility` pointers.
     */
    const vector<Utility *> &getContinuity_utilities() const;
};


#endif //TRIANGLEMODEL_REGION_H
