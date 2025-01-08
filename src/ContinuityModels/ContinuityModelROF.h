//
// Created by bernardo on 1/26/17.
//

#ifndef TRIANGLEMODEL_CONTINUITYMODELROF_H
#define TRIANGLEMODEL_CONTINUITYMODELROF_H


#include "Base/ContinuityModel.h"
#include "../Utils/Matrices.h"


class ContinuityModelROF : public ContinuityModel {
private:
//    Matrix3D<double> storage_to_rof_table;
//    Matrix3D<double> storage_to_rof_rof_realization;
    /**
     * @brief A vector of `Matrix2D` objects representing the storage-to-ROF tables for each utility.
     */
    vector<Matrix2D<double>> ut_storage_to_rof_rof_realization;
    /**
     * @brief A vector of integers representing the IDs of all online downstream sources
     */
    vector<int> online_downstream_sources;
    /**
     * @brief A boolean array indicating whether a source has downstream sources.
     */
    bool *storage_wout_downstream;
    /**
     * @brief The number of topological sources in the system.
     */
    const int n_topo_sources;
    /**
     * @brief A flag indicating whether pre-computer ROF tables are being used.
     */
    const int use_precomputed_rof_tables;

protected:
    /**
     * @brief An integer representing the beginning tier for ROF table calculation.
     */
    int beginning_tier = 0;
    /**
     * @brief A vector of `WaterSource` pointers representing the water sources in the realization.
     */
    vector<WaterSource *> realization_water_sources;
    /**
     * @brief A vector of `Utility` pointers representing the utilities in the realization.
     */
    vector<Utility *> realization_utilities;
    /**
     * @brief A vector of `Matrix2D` objects representing the storage-to-ROF tables for each utility.
     */
    vector<Matrix2D<double>> ut_storage_to_rof_table;
    /**
     * @brief A 2D vector of doubles representing the shifts in storage-to-ROF tables for each utility.
     */
    vector<vector<double>> table_storage_shift;
    /**
     * @brief A 2D vector of doubles representing the shifts in base storage-to-ROF tables for each utility.
     */
    vector<vector<double>> table_base_storage_shift;
    /**
     * @brief A 2D vector of doubles representing the shifts in treatment demand buffer for each utility.
     */
    vector<vector<double>> treatment_demand_buffer_shift;
    /**
     * @brief A vector of doubles representing the utilities' base storage capacities.
     */
    vector<double> utility_base_storage_capacity;
    /**
     * @brief A vector of doubles representing the utilities' base delta capacities.
     */
    vector<double> utility_base_delta_capacity_table;
    /**
     * @brief A vector of doubles representing the utilities' current and base storage capacity ratios.
     */
    vector<double> current_and_base_storage_capacity_ratio;
    /**
     * @brief A vector of doubles representing the utilities' current storage table shifts.
     */
    vector<double> current_storage_table_shift;
    /**
     * @brief A vector of doubles representing the utilities' current base storage table shifts.
     */
    vector<double> current_base_storage_table_shift;
    /**
     * @brief A vector of doubles representing the utilities' current demand ratio buffers.
     */
    vector<double> current_demand_ratio_buffer;

public:
    /**
     * @brief Initializes the ContinuityModelROF object for simulating risks of failure (ROF).
     * 
     * This constructor sets up the ContinuityModelROF by initializing storage structures, updating utilities' 
     * total available volume, and preparing risk-of-failure (ROF) tables or calculations based on pre-computed data.
     * 
     * @param water_sources Vector of water source objects used in the model.
     * @param water_sources_graph Graph representing the topological relationships between water sources.
     * @param water_sources_to_utilities Mapping of water sources to utilities.
     * @param utilities Vector of utility objects to manage water demand and allocation.
     * @param min_env_flow_controls Vector of minimum environmental flow controls.
     * @param utilities_rdm Vector of random deviations for utilities.
     * @param water_sources_rdm Vector of random deviations for water sources.
     * @param total_weeks_simulation Total number of weeks for the simulation.
     * @param use_precomputed_rof_tables Indicator for using pre-computed ROF tables (1: use, 0: generate dynamically).
     * @param realization_id Unique identifier for the simulation realization.
     */
    ContinuityModelROF(vector<WaterSource *> water_sources, const Graph &water_sources_graph,
                       const vector<vector<int>> &water_sources_to_utilities, vector<Utility *> utilities,
                       vector<MinEnvFlowControl *> min_env_flow_controls, vector<double>& utilities_rdm,
                       vector<double>& water_sources_rdm, unsigned long total_weeks_simulation,
                       const int use_precomputed_rof_tables, const unsigned long realization_id);
    
    /**
     * @brief Destructor for the ContinuityModelROF class.
     * 
     * This function cleans up resources allocated during the lifetime of the ContinuityModelROF object.
     * Specifically, it releases memory associated with dynamically allocated arrays.
     */
    virtual ~ContinuityModelROF();

    /**
     * @brief This function calculates the short-term risk of failure (ROF) for all utilities.
     * 
     * Depending on whether precomputed ROF tables are imported, the function either performs
     * calculations using the tables or calculates the ROF values directly.
     * 
     * @param week The current week in the simulation for which the short-term ROF is being calculated.
     * @param import_export_rof_tables Indicator for using precomputed ROF tables:
     *        - IMPORT_ROF_TABLES: Use precomputed ROF tables.
     *        - Any other value: Perform full calculations for ROF.
     * 
     * @return A 2D vector of doubles where each row corresponds to a utility, and columns represent
     *         ROF values based on different storage or demand conditions.
    */
    vector<vector<double>> calculateShortTermROF(int week, int import_export_rof_tables);

    /**
     * @brief This function performs a full calculation of the short-term risk of failure (ROF) for each utility
     * for the given week.
     * 
     * The calculation considers both storage and treatment failure risks over a series of realizations. It simulates
     * the system behavior for multiple years, adjusting for failures based on available storage and treatment capacity.
     * 
     * @param week The current week of the simulation to calculate the short-term ROF for.
     * 
     * @return A 2D vector of doubles representing the risk of failure for each utility:
     *         - The first row corresponds to storage-related ROF.
     *         - The second row corresponds to treatment-related ROF.
     * 
     * @throws logic_error If NaN values are encountered in the ROF calculations for storage or treatment.
     * 
     * @see updateStorageToROFTable, resetUtilitiesAndReservoirs, updateOnlineInfrastructure, setInitialTableTier, 
     *      recordROFStorageTable, calculateEmptyVolumes
     */
    vector<vector<double>> calculateShortTermROFFullCalcs(int week);

    /**
     * @brief This function calculates the short-term risk of failure (ROF) for each utility based on the current
     *        storage levels and treatment capacity ratios, using a predefined ROF table.
     * 
     * The function estimates the risk of failure in both storage and treatment for each utility by comparing
     * the current storage levels against the storage-to-ROF table. It also checks if the demand-to-treatment
     * capacity ratio exceeds a threshold to flag potential treatment failures.
     * 
     * @param week The current week of the simulation for which the short-term ROF should be calculated.
     * @param utilities The list of utilities for which the ROF will be calculated.
     * @param utilities_base_storage_capacity The base storage capacities of each utility.
     * @param ut_storage_to_rof_table The precomputed storage-to-ROF table for each utility.
     * @param current_storage_table_shift The current shift applied to the storage table for each utility.
     * @param current_base_storage_table_shift The current shift applied to the base storage table for each utility.
     * @param current_demand_ratio_buffer The buffer added to the demand-to-treatment capacity ratio to avoid early failure triggers.
     * 
     * @return A 2D vector of doubles representing the risk of failure for each utility:
     *         - The first row corresponds to the storage-related ROF.
     *         - The second row corresponds to the treatment-related ROF.
     * 
     * @see updateOnlineInfrastructure
     */
    vector<vector<double>> calculateShortTermROFTable(int week, vector<Utility *> utilities,
                                              vector<double> utilities_base_storage_capacity,
                                              const vector<Matrix2D<double>> &ut_storage_to_rof_table,
                                              vector<double> current_storage_table_shift,
                                              vector<double> current_base_storage_table_shift,
                                              vector<double> current_demand_ratio_buffer);

    /**
     * @brief This function calculates the long-term risk of failure (ROF) for each utility, considering both storage
     *        and treatment capacity, based on projected demand and predefined failure thresholds.
     * 
     * The function performs a long-term simulation of the utilities' storage and treatment capacities over a series
     * of realizations and time steps, calculating the risk of failure for each utility based on storage and treatment
     * conditions.
     * 
     * @param week The current week of the simulation for which the long-term ROF should be calculated.
     * 
     * @return A 2D vector of doubles representing the risk of failure for each utility:
     *         - The first row corresponds to the storage-related ROF.
     *         - The second row corresponds to the treatment-related ROF.
     * 
     * @see resetUtilitiesAndReservoirs, updateOnlineInfrastructure
     */
    vector<vector<double>> calculateLongTermROF(int week);

    /**
     * @brief Resets the utilities' and reservoirs' storage, last release, and combined storage respectively
     * for the specified risk of failure (ROF) type.
     * 
     * This function updates the water sources' available volumes and outflows based on the current ROF type (short-term or long-term).
     * It also updates the total available volume for each utility.
     * 
     * @param rof_type The type of ROF being simulated. Can be either:
     *        - `SHORT_TERM_ROF`: For resetting to current storage conditions.
     *        - `LONG_TERM_ROF`: For resetting to full capacity conditions.
     * 
     * @return None
     */
    void resetUtilitiesAndReservoirs(int rof_type);

    /**
     * @brief Connects this specific realization's water sources to the model by passing the water sources
     * to the ROF continuity model for the specific realization it calculated ROFs for.
     * 
     * This function assigns the provided vector of water sources to the realization's water sources.
     * It updates the internal `realization_water_sources` vector to reflect the current simulation state.
     * 
     * @param realization_water_sources A vector containing the one realization's water sources to be connected to the model.
     * 
     * @return None
     */
    void connectRealizationWaterSources(const vector<WaterSource *> &realization_water_sources);

    /**
     * @brief Connects the realization utilities to the model by passing in the locations of the utilities
     * of the realization it calculated ROFs for.
     * 
     * This function assigns the provided vector of utilities to the realization's utilities.
     * It updates the internal `realization_utilities` vector to reflect the current simulation state.
     * 
     * @param realization_utilities A vector containing the utilities to be connected to the model.
     * 
     * @return None
     */
    void connectRealizationUtilities(const vector<Utility *> &realization_utilities);

    /**
     * @brief Updates the online infrastructure for water sources and utilities.
     * 
     * This function checks for any new infrastructure that has become online in the
     * realization model and updates the corresponding infrastructure status in the 
     * continuity model (ROF model). It updates water sources' capacities, utilities' 
     * storage capacities, and their ratios to status-quo capacities. It also updates 
     * the list of downstream sources for each source.
     * 
     * @param week The current week number for the simulation.
     * 
     * @return None
     */
    virtual void updateOnlineInfrastructure(int week);

    /**
     * @brief Updates the approximate storage-to-ROF table based on the available water volumes and failure conditions.
     * 
     * This function updates the storage-to-ROF table for each week by shifting storage volumes and 
     * checking for utility failures at various storage levels. The function reduces storage levels 
     * incrementally and records failures for utilities that do not meet the required storage capacity ratio.
     * The process stops once all utilities have failed at a particular storage level.
     * 
     * @param storage_percent_decrement The percentage decrement to apply to the storage levels.
     * @param week_of_the_year The week of the year to update the table for.
     * @param to_full Array of available water volumes to full storage capacity for each water source.
     * 
     * @return None
     * 
     * @see shiftStorages
     */
    void updateStorageToROFTable(double storage_percent_decrement,
                                 int week_of_the_year,
                                 const double *to_full_toposort);

    
    /**
     * @brief Shifts storage volumes based on delta values.
     * 
     * This function adjusts the storage volumes of water sources according to the 
     * given delta values. It handles the spillover of water to downstream sources 
     * if a source's storage exceeds its capacity. The function is designed to 
     * respect the topological order of sources, ensuring that upstream sources 
     * are processed before downstream sources.
     * 
     * FIXME: MAKE THIS MORE EFFICIENT. THIS METHOD IS THE MOST EXPENSIVE ONE IN THE CODE.
     * 
     * @param available_volumes_shifted A pointer to an array representing the 
     *                                  shifted available volumes for each water source.
     * @param delta_storage A pointer to an array of delta values representing 
     *                      the change in storage for each water source.
     * 
     * @return None
     */
    void shiftStorages(double *available_volumes_shifted, const double *delta_storage);

    /**
     * @brief Prints the ROF table to CSV files for each utility.
     * 
     * FIXME: This is where changes from CSV to more efficient file formats should be made.
     * 
     * This function generates CSV files containing the Risk of Failure (ROF) data 
     * for each utility. It iterates over the number of utilities and writes the 
     * ROF table data to a separate file for each utility. The files are saved 
     * in the specified folder with filenames indicating the realization ID and 
     * the utility index.
     * 
     * @param folder The folder path where the CSV files will be saved.
     * 
     * @return None
     */
    void printROFTable(const string &folder);

    /**
     * @brief Sets the ROF tables and shifts for the model.
     * 
     * This function assigns the provided ROF tables and related shift data 
     * to the corresponding member variables in the model. The shift data is 
     * used for adjusting the storage and demand conditions during the ROF 
     * calculation processes.
     * 
     * @param storage_to_rof_table The 2D matrix of storage-to-ROF data.
     * @param table_storage_shift The shift values for the storage table.
     * @param table_base_storage_shift The shift values for the base storage table.
     * @param treatment_demand_buffer_shift The shift values for the treatment demand buffer.
     * 
     * @return None
     */
    void setROFTablesAndShifts(const vector<Matrix2D<double>> &storage_to_rof_table,
                               const vector<vector<double>> &table_storage_shift,
                               const vector<vector<double>> &table_base_storage_shift,
                               const vector<vector<double>> &treatment_demand_buffer_shift);

    /**
     * @brief Handles exceptions related to ROF table extrapolation or errors during ROF calculations.
     * 
     * This function throws an exception if the ROF tables need to be extrapolated 
     * due to a utility's current capacity exceeding the table's base capacity. 
     * If the error is related to table extrapolation, it provides guidance 
     * to regenerate tables with adjusted parameters. If the error occurs in a 
     * different context, a runtime exception is thrown with the appropriate details.
     * 
     * @param m The ratio of current storage capacity to the base storage capacity.
     * @param u The index of the utility where the exception occurred.
     * @param week The current week in the simulation where the error occurred.
     * 
     * @return None
     * 
     * @throws logic_error If `m` is greater than 1, indicating that the current capacity is greater than the table base capacity.
     * @throws runtime_error For other exceptions that occur within a specific week for a utility.
     */
    void tableROFExceptionHandler(double m, int u, int week);

    /**
     * @brief Sets the initial table tier based on the failure conditions for utilities at a specific week.
     * 
     * This function determines the initial table tier by checking for failures across utilities and 
     * adjusting the tier accordingly. It searches through the storage to ROF table and identifies the 
     * first tier where no failures occurred, setting that as the beginning tier for the simulation.
     * 
     * @param week The current week in the simulation.
     * @param n_utilities The number of utilities to be considered in the table.
     * @param ut_storage_to_rof_table The table containing storage to ROF data for utilities.
     * @param beginning_tier The tier to be set as the starting point for the calculations.
     * 
     * @return None
     */
    void setInitialTableTier(int week, const int &utilities, vector<Matrix2D<double>> &vector, int &tier);

    /**
     * @brief Records the risk of failure (ROF) storage table data for a given utility and week.
     * 
     * This function updates the `ut_storage_to_rof_table` by adding ROF data for each utility 
     * at the specified week. The ROF data is retrieved from the corresponding realization table, 
     * averaged over all realizations, and then added to the storage table for that week.
     * 
     * @param ut_storage_to_rof_rof_realization The ROF data for all utilities across multiple realizations.
     * @param ut_storage_to_rof_table The storage to ROF table to which the data is to be added.
     * @param n_utilities The number of utilities being considered.
     * @param week The current week in the simulation.
     * @param week_of_the_year The specific week of the year to record data for.
     * 
     * @return None
     */
    void recordROFStorageTable(vector<Matrix2D<double>> &ut_storage_to_rof_rof_realization,
                               vector<Matrix2D<double>> &ut_storage_to_rof_table, const int &n_utilities, int &week,
                               int &week_of_the_year);

    /**
     * @brief Calculates the empty volume for each water source. This information is later used for updating the ROF tables.
     * 
     * This function calculates the empty volume (the volume required to fill a water source to its capacity) 
     * for each water source in the `realization_water_sources` list. The calculation is based on the supply 
     * capacity and the available volume for each source. If a water source is offline, the empty volume is set to zero.
     * 
     * @param realization_water_sources A list of water sources to calculate empty volumes for.
     * @param to_full An array where the empty volume for each water source will be stored.
     * 
     * @return None
     */
    void calculateEmptyVolumes(vector<WaterSource *> &realization_water_sources, double *to_full);

    /**
     * @brief Updates treatment allocations for joint water treatment plants.
     * 
     * This function updates the treatment allocations for water sources of type `NEW_JOINT_WATER_TREATMENT_PLANT` 
     * in the `non_rof_water_sources` list. It ensures that the treatment allocations are synchronized between 
     * the continuity and realization water sources if the source is online.
     * 
     * FIXME: WHAT IS THE FUNCTIONAL DIFFERENCE BETWEEN WATER_SOURCES VECTORS HERE?
     * 
     * @param non_rof_water_sources A list of non-ROF water sources whose treatment allocations will be updated.
     * 
     * @return None
     */
    void updateJointWTPTreatmentAllocations(const vector<WaterSource *> &non_rof_water_sources);

    /**
     * @brief Updates treatment capacities for utilities.
     * 
     * This function updates the treatment capacities for utilities in the `non_rof_utilities` list by synchronizing 
     * the treatment capacities between the continuity and realization utilities.
     * 
     * FIXME: WHAT IS THE FUNCTIONAL DIFFERENCE BETWEEN WATER_SOURCES VECTORS HERE?
     * 
     * @param non_rof_utilities A list of non-ROF utilities whose treatment capacities will be updated.
     * 
     * @return None
     */
    void updateUtilityTreatmentAllocations(const vector<Utility *> &non_rof_utilities);

    /**
     * @brief Retrieves the storage-to-ROF table.
     * 
     * This function returns a reference to the storage-to-ROF table, which contains 
     * the calculated risk of failure (ROF) for each utility at various storage levels.
     * 
     * @param None
     * 
     * @return A reference to the storage-to-ROF table (`ut_storage_to_rof_table`).
     */
    vector<Matrix2D<double>> &getUt_storage_to_rof_table();
};


#endif //TRIANGLEMODEL_CONTINUITYMODELROF_H
