/**
 * @class Simulation
 * @brief The `Simulation` class represents the main simulation object that implements the simulation process.
 * 
 * Created by bernardo on 1/25/17.
 * 
 */

#ifndef TRIANGLEMODEL_SIMULATION_H
#define TRIANGLEMODEL_SIMULATION_H

#include "../Utils/Constants.h"
#include "../SystemComponents/WaterSources/Base/WaterSource.h"
#include "../SystemComponents/Utility/Utility.h"
#include "../DroughtMitigationInstruments/Restrictions.h"
#include "../ContinuityModels/Base/ContinuityModel.h"
#include "../ContinuityModels/ContinuityModelRealization.h"
#include "../ContinuityModels/ContinuityModelROF.h"
#include "../Controls/Base/MinEnvFlowControl.h"
#include "../DataCollector/MasterDataCollector.h"
#include <vector>

using namespace Constants;
using namespace std;

class Simulation {
private:
    /**
     * @brief The total time required to run the simulation.
     */
    const unsigned long total_simulation_time;
    /**
     * @brief The vector containing the IDs of the realizations to run.
     */
    vector<unsigned long> realizations_to_run;
    /**
     * @brief Flag indicating if the simulation should import, export, or generate ROF tables.
     */
    const int import_export_rof_tables;
    /**
     * @brief The number of realizations to run.
     */
    unsigned long n_realizations;
    /**
     * @brief The reference to a vector containing the water sources in the model.
     */
    vector<WaterSource *> &water_sources;
    /**
     * @brief The reference to the graph containing the water sources in the model.
     */
    Graph &water_sources_graph;
    /**
     * @brief The reference to the 2D vector  containing the IDs of the water sources to utilities.
     */
    const vector<vector<int>> &water_sources_to_utilities;
    /**
     * @brief The reference to a vector containing the utilities in the model.
     */
    vector<Utility *> &utilities;
    /**
     * @brief The reference to a vector containing the drought mitigation policies in the model.
     */
    const vector<DroughtMitigationPolicy *> &drought_mitigation_policies;
    /**
     * @brief The reference to a vector containing the minimum environmental flow controls in the model.
     */
    vector<MinEnvFlowControl *> &min_env_flow_controls;
    /**
     * @brief The reference to a 2D vector containing the DU factor demand multipliers for the utilities.
     */
    vector<vector<double>>& utilities_rdm;
    /**
     * @brief The reference to a 2D vector containing the random DU permitting, construction, and financing multipliers for the water sources.
     */
    vector<vector<double>>& water_sources_rdm;
    /**
     * @brief The reference to a 2D vector containing the random DU drought mitigation policies.
     */
    vector<vector<double>>& policies_rdm;
    /**
     * @brief The pointer to a 2D vector containing matrices that store the precomputed ROF tables.
     */
    vector<vector<Matrix2D<double>>>* precomputed_rof_tables;
    /**
     * @brief The pointer to a 2D vector containing the shifts in storage for the ROF tables.
     */
    vector<vector<double>>* table_storage_shift;
    /**
     * @brief The pointer to a 2D vector containing the shifts in base storage for the ROF tables.
     */
    vector<vector<double>>* table_base_storage_shift;
    /**
     * @brief The pointer to a 2D vector containing the shifts in treatment demand for the ROF tables.
     */
    vector<vector<double>>* treatment_demand_buffer_shift;
    /**
     * @brief The pointer to the master data collector.
     */
    MasterDataCollector* master_data_collector = nullptr;
    /**
     * @brief The folder where the ROF tables are stored.
     */
    string rof_tables_folder;

    /**
     * @brief Sets the folder path for the ROF tables.
     * 
     * This function sets the folder path where the ROF tables will be stored or imported from.
     * 
     * @param rof_tables_folder The path to the folder where ROF tables are located or should be stored.
     * 
     * @return void
     */
    void setRof_tables_folder(const string &rof_tables_folder);

public:

    /**
     * @brief Constructor for the Simulation class.
     * 
     * This constructor initializes a simulation with the provided parameters, setting up water sources, utilities, 
     * drought mitigation policies, environmental flow controls, and various randomization matrices for the simulation.
     * It also sets up the realization processes to be run during the simulation.
     * 
     * @param water_sources A vector of water source pointers that are part of the simulation.
     * @param water_sources_graph A graph representing the relationships between water sources.
     * @param water_sources_to_utilities A matrix that maps water sources to utilities.
     * @param utilities A vector of utility pointers involved in the simulation.
     * @param drought_mitigation_policies A vector of drought mitigation policies.
     * @param min_env_flow_controls A vector of minimum environmental flow controls.
     * @param utilities_rdm A matrix of utilities randomization data.
     * @param water_sources_rdm A matrix of water sources randomization data.
     * @param policies_rdm A matrix of policies randomization data.
     * @param total_simulation_time The total duration of the simulation.
     * @param realizations_to_run A vector of realization IDs to be run during the simulation.
     * 
     * @return None
     */
    Simulation(
            vector<WaterSource *> &water_sources, Graph &water_sources_graph,
            const vector<vector<int>> &water_sources_to_utilities,
            vector<Utility *> &utilities,
            const vector<DroughtMitigationPolicy *> &drought_mitigation_policies,
            vector<MinEnvFlowControl *> &min_env_flow_controls,
            vector<vector<double>>& utilities_rdm,
            vector<vector<double>>& water_sources_rdm,
            vector<vector<double>>& policies_rdm,
            const unsigned long total_simulation_time,
            vector<unsigned long> &realizations_to_run);

    /**
     * @brief Constructor for the Simulation class with precomputed ROF tables and shifts.
     * 
     * This constructor initializes a simulation with the provided parameters, setting up water sources, utilities, 
     * drought mitigation policies, environmental flow controls, various randomization matrices, and precomputed ROF 
     * tables for the simulation. It also imports pre-computed ROF tables and 
     * sets up the realization processes to be run during the simulation.
     * 
     * @param water_sources A vector of water source pointers that are part of the simulation.
     * @param water_sources_graph A graph representing the relationships between water sources.
     * @param water_sources_to_utilities A matrix that maps water sources to utilities.
     * @param utilities A vector of utility pointers involved in the simulation.
     * @param drought_mitigation_policies A vector of drought mitigation policies.
     * @param min_env_flow_controls A vector of minimum environmental flow controls.
     * @param utilities_rdm A matrix of utilities randomization data.
     * @param water_sources_rdm A matrix of water sources randomization data.
     * @param policies_rdm A matrix of policies randomization data.
     * @param total_simulation_time The total duration of the simulation.
     * @param realizations_to_run A vector of realization IDs to be run during the simulation.
     * @param precomputed_rof_tables A vector of precomputed ROF tables used for simulation.
     * @param table_storage_shift A matrix of storage shifts for the ROF tables.
     * @param table_base_storage_shift A matrix of base storage shifts for the ROF tables.
     * @param treatment_demand_buffer_shift A matrix of treatment demand buffer shifts for the ROF tables.
     * @param rof_tables_folder A string representing the folder containing the ROF tables.
     * 
     * @return None
     * 
     * @see setupSimulation
     */
    Simulation(vector<WaterSource *> &water_sources, Graph &water_sources_graph,
               const vector<vector<int>> &water_sources_to_utilities,
               vector<Utility *> &utilities,
               const vector<DroughtMitigationPolicy *> &drought_mitigation_policies,
               vector<MinEnvFlowControl *> &min_env_flow_controls,
               vector<vector<double>> &utilities_rdm,
               vector<vector<double>> &water_sources_rdm,
               vector<vector<double>> &policies_rdm,
               const unsigned long total_simulation_time,
               vector<unsigned long> &realizations_to_run,
               vector<vector<Matrix2D<double>>> &precomputed_rof_tables,
               vector<vector<double>> &table_storage_shift,
               vector<vector<double>> &table_base_storage_shift,
               vector<vector<double>> &treatment_demand_buffer_shift,
               string &rof_tables_folder);

    /**
     * @brief Constructor for the Simulation class with the option to export ROF tables.
     * 
     * This constructor initializes a simulation with the provided parameters, setting up water sources, utilities, 
     * drought mitigation policies, environmental flow controls, various randomization matrices, and enables the 
     * export of ROF tables for the simulation. It also allows ROF tables to be exported and 
     * sets up the realization processes to be run during the simulation.
     * 
     * @param water_sources A vector of water source pointers that are part of the simulation.
     * @param water_sources_graph A graph representing the relationships between water sources.
     * @param water_sources_to_utilities A matrix that maps water sources to utilities.
     * @param utilities A vector of utility pointers involved in the simulation.
     * @param drought_mitigation_policies A vector of drought mitigation policies.
     * @param min_env_flow_controls A vector of minimum environmental flow controls.
     * @param utilities_rdm A matrix of utilities randomization data.
     * @param water_sources_rdm A matrix of water sources randomization data.
     * @param policies_rdm A matrix of policies randomization data.
     * @param total_simulation_time The total duration of the simulation.
     * @param realizations_to_run A vector of realization IDs to be run during the simulation.
     * @param rof_tables_folder A string representing the folder containing the ROF tables.
     * 
     * @return None
     */
    Simulation(vector<WaterSource *> &water_sources, Graph &water_sources_graph,
               const vector<vector<int>> &water_sources_to_utilities,
               vector<Utility *> &utilities,
               const vector<DroughtMitigationPolicy *> &drought_mitigation_policies,
               vector<MinEnvFlowControl *> &min_env_flow_controls,
               vector<vector<double>> &utilities_rdm,
               vector<vector<double>> &water_sources_rdm,
               vector<vector<double>> &policies_rdm,
               const unsigned long total_simulation_time,
               vector<unsigned long> &realizations_to_run,
               string &rof_tables_folder);

    /**
     * @brief Assignment operator for the Simulation class.
     * 
     * This operator allows for the assignment of one `Simulation` object to another. It copies the number of realizations
     * from the source `Simulation` object to the current one. The other member variables are not copied, as this operator
     * is minimal and only handles the number of realizations.
     * 
     * @param simulation The Simulation object to assign to the current object.
     * @return A reference to the current Simulation object after assignment.
     */
    Simulation &operator=(const Simulation &simulation);

    /**
     * @brief Destructor for the Simulation class.
     * 
     * This destructor handles the cleanup of any resources allocated during the simulation's execution.
     * It is defined as default, relying on the compiler-generated destructor for cleaning up member variables.
     * 
     * @return None
     */
    virtual ~Simulation();

    /**
     * @brief Runs the full simulation across multiple realizations.
     * 
     * This function executes a full simulation over a specified number of realizations, with each realization 
     * being processed in parallel using OpenMP. The function checks the validity of the imported ROF tables and then initializes
     * the necessary models and data collectors. The simulation calculates risk-of-failure, applies drought mitigation 
     * policies, and performs continuity calculations for each week of the simulation. The data is collected and exported 
     * if required.
     * 
     * @param n_threads The number of threads to be used for parallel execution.
     * @param vars A pointer to an array containing decision variables used for error reporting in case of a failure.
     * 
     * @return A pointer to the `MasterDataCollector` that contains the collected data from all realizations.
     * 
     * @throws Throws an exception if there is a mismatch in the number of utilities or imported ROF tables, 
     * or if an error occurs during the simulation of any realization.
     */
    MasterDataCollector *runFullSimulation(unsigned long n_threads, double *vars);

    /**
     * @brief Set up the simulation by initializing water sources, utilities, and various components.
     * 
     * This function performs initial setup for the simulation, including sorting water sources and utilities by ID, 
     * validating the consistency of IDs, and ensuring that the correct water sources are linked to the respective utilities. 
     * It also creates the data collector for the simulation.
     * 
     * @param water_sources A vector of water source pointers that are part of the simulation.
     * @param water_sources_graph A graph representing the relationships between water sources.
     * @param water_sources_to_utilities A matrix that maps water sources to utilities.
     * @param utilities A vector of utility pointers involved in the simulation.
     * @param drought_mitigation_policies A vector of drought mitigation policies.
     * @param min_env_flow_controls A vector of minimum environmental flow controls.
     * @param utilities_rdm A matrix of utilities randomization data.
     * @param water_sources_rdm A matrix of water sources randomization data.
     * @param policies_rdm A matrix of policies randomization data.
     * @param realizations_to_run A vector of realization IDs to be run during the simulation.
     * 
     * @return None
     * 
     * @throws invalid_argument if any of the following conditions are met:
     *   - Water sources or utilities IDs are not sequential.
     *   - A water source is listed in the construction order of a utility but not in its source list.
     *   - A water source declared for a utility is not present in the list of all water sources.
     */
    void setupSimulation(vector<WaterSource *> &water_sources, Graph &water_sources_graph,
                             const vector<vector<int>> &water_sources_to_utilities, vector<Utility *> &utilities,
                             const vector<DroughtMitigationPolicy *> &drought_mitigation_policies,
                             vector<MinEnvFlowControl *> &min_env_flow_controls,
                             vector<vector<double>> &utilities_rdm, vector<vector<double>> &water_sources_rdm,
                             vector<vector<double>> &policies_rdm, vector<unsigned long> &realizations_to_run);

    /**
     * @brief Creates and initializes realization and ROF models for the simulation.
     * 
     * This function creates two types of models for the specified realization:
     * a `ContinuityModelRealization` and a `ContinuityModelROF`. The function copies the necessary data
     * (e.g., water sources, utilities, drought mitigation policies) and initializes these models.
     * The ROF model is also connected to the relevant realization water sources and utilities, and
     * the appropriate ROF tables are set if needed.
     * 
     * @param realization The index of the realization to create models for.
     * @param realization_model A reference to the `ContinuityModelRealization` object to be created.
     * @param rof_model A reference to the `ContinuityModelROF` object to be created.
     * 
     * @return None
     */
    void createContinuityModels(unsigned long realization, ContinuityModelRealization *&realization_model,
                                    ContinuityModelROF *&rof_model);
    
    /**
     * @brief Prints the progress of a task in percentage and visual bar format.
     * 
     * This function prints a progress bar on the same line in the console, displaying the 
     * percentage of completion along with a visual representation using a progress bar.
     * 
     * @param percentage The percentage of completion (a value between 0 and 1).
     * 
     * @return void
     */
    void printProgress(double percentage);
};


#endif //TRIANGLEMODEL_SIMULATION_H
