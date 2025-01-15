//
// Created by Bernardo on 11/24/2017.
//

/**
 * @class Problem
 * @brief The `Problem` class represents the basic structure of a water management and planning problem.
 * 
 * Created by Bernardo on 11/24/2017.
 */

#ifndef TRIANGLEMODEL_PROBLEM_H
#define TRIANGLEMODEL_PROBLEM_H

#include <vector>
#include "../../DataCollector/Base/DataCollector.h"
#include "../../DataCollector/MasterDataCollector.h"
#include "../../Utils/Utils.h"
#include "../../SystemComponents/WaterSources/Reservoir.h"

/**
 * @brief A struct that calls the infraRank function to store the ID and realization of an infrastructure.
 * 
 */
struct infraRank {
    int id;
    double xreal;

    infraRank(int id, double xreal) {
        this->id = id;
        this->xreal = xreal;
    }
};

/**
 * @brief A struct that intializes a boolean operator to compare the infraRank of two infrastructure objects.
 * 
 */
struct by_xreal {
    bool operator()(const infraRank &ir1, const infraRank &ir2) {
        return ir1.xreal < ir2.xreal;
    }
};

class Problem {
protected:
    /**
     * @brief An unsigned long integer indicating the number of realizations to simulate.
     */
    unsigned long n_realizations;
    /**
     * @brief An unsigned long integer indicating the number of weeks to simulate.
     */
    unsigned long  n_weeks;
    /**
     * @brief An unsigned long integer indicating the row of the decision variables to simulate.
     */
    unsigned long solution_no;
    /**
     * @brief An unsigned long integer indicating the number of threads to use in the optimization.
     */
    unsigned long n_threads;

    /**
     * @brief An integer indicating the number of utilities in the problem.
     */
    int n_utilities = NON_INITIALIZED;
    /**
     * @brief A string containing the input/output directory for the problem.
     */
    string io_directory;
    /**
     * @brief A string containing the output subdirectory path for the problem.
     */
    string output_sub_directory;
    /**
     * @brief A string containing the output filename suffix unique to the problem.
     * FIXME: Need to correct variable name to fname_suffix.
     */
    string fname_sufix;
    /**
     * @brief A string containing the filename suffix for the evaporation and inflow files unique to the problem.
     */
    string evap_inflows_suffix;
    /**
     * @brief A string containing the filename suffix for the demand files unique to the problem.
     */
    string demand_path_suffix;
    /**
     * @brief A srting containing the filename for the demand files subfolder.
     */
    string demand_path_subfolder;
    /**
     * @brief A string containing the directory to the ROF tables.
     * 
     */
    string rof_tables_directory;

    /**
     * @brief A vector of unsigned long integers containing the indices realizations to run.
     */
    vector<unsigned long > realizations_to_run;
    /**
     * @brief A pointer to the MasterDataCollector object.
     */
    MasterDataCollector* master_data_collector = nullptr;
    /**
     * @brief A vector of doubles containing the objective values for the problem.
     */
    vector<double> objectives;
    /**
     * @brief A boolean flag indicating whether or not to print the output files.
     */
    bool print_output_files = true;

    /**
     * @brief An integer indicating the DU SOW index.
     */
    int rdm_no = NON_INITIALIZED;
    /**
     * @brief An integer indicating the ROF table import/export/ignore option.
     *  -1: Import tables, 0: Ignore tables, 1: Export tables.
     */
    int import_export_rof_tables;
    /**
     * @brief A double indicating the multiplier for the table generation storage.
     */
    double table_gen_storage_multiplier;
    /**
     * @brief A 2D vector of doubles storing the DU multipliers for all utilities' 
     *  demand growth rates.
     */
    vector<vector<double>> utilities_rdm;
    /**
     * @brief A 2D vector of doubles storing the DU multipliers for all water sources' 
     *  permitting and construction times.
     */
    vector<vector<double>> water_sources_rdm;
    /**
     * @brief A 2D vector of doubles storing the DU multipliers for all utilities' 
     *  water restriction policy effectiveness.
     */
    vector<vector<double>> policies_rdm;
    /**
     * @brief A 2D vector of matrices storing the ROF tables for all utilities across all realizations.
     * 
     */
    vector<vector<Matrix2D<double>>> rof_tables;
    /**
     * @brief An unsigned long flag indicating which problem formulation to run.
     */
    unsigned long formulation;

    /**
     * @brief Prioritizes or merges infrastructure low and high expansions. 
     * If the high-expansion version is prioritized (appears earlier in the list), both the capacities of the low and high expansions are combined. 
     * The lower-priority item is removed from all lists.
     * 
     * @param order A pointer to a vector<int> object containing the construction order of infraRank objects.
     * @param triggers A pointer to a vector<double> object containing the triggers that result in the construction of 
     * each corresponding infraRank vector object.
     * @param id_low The ID of the low-expansion infrastructure.
     * @param id_high The ID of the high-expansion infrastructure.
     * @param capacity_low The capacity of the low-expansion infrastructure.
     * @param capacity_high The capacity of the high-expansion infrastructure.
     * 
     * @return double The combined capacity of the low and high expansions if the high-expansion infrastructure is prioritized.
     * Otherwise, return the original capacity of the high-expansion infrastructure.
     */
    double checkAndFixInfraExpansionHighLowOrder(vector<int> *order, vector<double> *trigger, int id_low, int id_high,
            double capacity_low, double capacity_high);

    /**
     * @brief Converts the vector of infraRank objects to a vector of integers.
     * 
     * @param v A vector of infraRank objects.
     * @return vector<int> A vector of integers denoting each infrastructure's rank (from earliest to last built)
     * after optimization.
     */
    vector<int> vecInfraRankToVecInt(vector<infraRank> v);

public:
    
    /**
     * @brief Constructs a new Problem object. Unset the seed for the random number generator for the reservoir objects. 
     *  NOTE: Unclear what this does; need to see if can delete.
     * 
     * @param n_weeks The number of weeks to simulate.
     */
    Problem(unsigned long n_weeks);

    /**
     * @brief Destructor for the Problem object
     * 
     */
    virtual ~Problem();

    virtual int functionEvaluation(double* vars, double* objs, double* consts)=0;

    /**
     * @brief Sets the number of weeks over which to simulate the problem.
     * 
     * @param n_weeks An integer indicating the number of simulation weeks.
     */
    void setN_weeks(unsigned long n_weeks);

    /**
     * @brief Sets the row of the decision variables of the solution (the solution number) to simulate.
     * 
     * @param sol_number An unsigned long integer indicating the row of the decision variables to simulate.
     */
    void setSol_number(unsigned long sol_number);

    /**
     * @brief Sets the input/output directory for the problem.
     * 
     * @param io_directory A const reference to the string containing the input/output directory for the problem.
     */
    void setIODirectory(const string &io_directory);

    /**
     * @brief Set the output subdirectory path for the problem.
     * 
     * @param output_sub_directory_path A string containing the output subdirectory path.
     */
    void setOutputSubDirectory(const string &output_sub_directory_path);

    /**
     * @brief Gets the values of all the objectives for the current solution.
     * 
     * @return const vector<double>& 
     */
    const vector<double> &getObjectives() const;

    /**
     * @brief Calculates the objectives for the current solution. Prints them to a file if indicated as such.
     * 
     * @param print_files Boolean flag to indicate if the output files should be printed.
     * @return vector<double> The calculated objectives for each utility.
     */
    vector<double> calculateAndPrintObjectives(bool print_files);

    /**
     * @brief Sets the 2D vectors of deeply uncertain factors for the problem optimization.
     * 
     * @param utilities_rdm The reference to a 2D vector of DU factors for the short, middle, long-term 
     * and overall demand growth rate multipliers applied across all utilities 
     * //FIX: RENAME TO BETTER INDICATE RELATION TO DEMAND.
     * @param water_sources_rdm The reference to a 2D vector of DU factors that scale the permitting and 
     * construction time of new water sources and the corresponding evaporation rate multipliers.
     * @param policies_rdm The reference to a 2D vector of DU factors that scale the effectiveness of 
     * water use restriction policies.
     */
    void setRDMOptimization(vector<vector<double>> &utilities_rdm, vector<vector<double>> &water_sources_rdm,
                            vector<vector<double>> &policies_rdm);

    /**
     * @brief Sets the 2D vectors of deeply uncertain factors for the problem reevaluation.
     * 
     * @param rdm_no An integer indicating the DU SOW index.
     * @param utilities_rdm The reference to a 2D vector of DU factors for the short, middle, long-term 
     * and overall demand growth rate multipliers applied across all utilities 
     * @param water_sources_rdm The reference to a 2D vector of DU factors that scale the permitting and 
     * construction time of new water sources and the corresponding evaporation rate multipliers.
     * @param policies_rdm The reference to a 2D vector of DU factors that scale the effectiveness of 
     * water use restriction policies.
     */
    void setRDMReevaluation(int rdm_no, vector<vector<double>> &utilities_rdm,
                                vector<vector<double>> &water_sources_rdm, vector<vector<double>> &policies_rdm);

    /**
     * @brief Sets the number of threads to use in the problem optimization.
     * 
     * @param n_threads An unsigned long integer indicating the number of threads to use.
     */
    void setN_threads(unsigned long n_threads);

    /**
     * @brief Sets the condition whether or not the print the problem optimization output files.
     * 
     * @param print_output_files A boolean flag indicating whether or not to print the output files.
     */
    void setPrint_output_files(bool print_output_files);

    /**
     * @brief Sets the number of total realizations available. 
     * If the vector of realizations is empty, initialize and assign a vector unsigned long integers
     * with length of n_realizations filled with zero values.
     * 
     * @param n_realizations An unsigned long integer indicating the number of realizations to run.
     */
    void setN_realizations(unsigned long n_realizations);

    /**
     * @brief Sets and limits the number of realizations to be
     * 
     * @param realizations_to_run 
     */
    void setRealizationsToRun(vector<unsigned long>& realizations_to_run);

    /**
     * @brief Sets the filename suffix for the evaporation and inflow files unique to the problem.
     * 
     * @param evap_inflows_suffix A const string reference to the evaporation and inflow files suffix.
     */
    void setEvap_inflows_suffix(const string &evap_inflows_suffix);

    /**
     * @brief Sets the output filename suffix unique to the problem.
     * FIXME: Need to correct function name to fname_suffix.
     * 
     * @param fname_sufix A const string reference to the filename suffix.
     */
    void setFname_sufix(const string &fname_sufix);

    MasterDataCollector* getMaster_data_collector();

    /**
     * @brief If the master_data_collector object is not null, delete it. Then assign it to a nullptr.
     * FIXME: better to rename "deleteDataCollector"?
     */
    void destroyDataCollector();

    /**
     * @brief Prints the timeseries of infrastructure pathways to output folder. 
     * Prints utilities, water sources, and policies' time series if indicated as such. 
     * 
     * @param plot_time_series Boolean indicator to print the time series of utilities, water sources, and policies.
     */
    void printTimeSeriesAndPathways(bool plot_time_series = true);

    
    /**
     * @brief Read and load pre-computed ROF tables. Also prints the time taken to load the tables.
     * @param n_realizations An integer indicating the number of realizations.
     * @param rof_tables_directory A string containing the directory path for the ROF tables.
     
     */
    void setRofTables(unsigned long n_realizations, string rof_tables_directory);

    /**
     * @brief Takes the -C flag from main.cpp to indicate the ROF table import/export/ignore option.
     * 
     * @param import_export_rof_tables An integer indicating the ROF table import/export/ignore option.
     *  -1: Import tables, 0: Ignore tables, 1: Export tables.
     *  Throws error "The value entered is invalid" if the input is not -1, 0, or 1.
     *  If import_export_rof_tables is set to IMPORT_ROF_TABLES (-1), assign the ROF tables directory to 
     *  rof_tables_directory. For all other values, create an ROF tables directory with name 
     *  rof_tables_directory.
     * @param rof_tables_directory A string containing the directory path for the ROF tables.
     */
    void setImport_export_rof_tables(int import_export_rof_tables, string rof_tables_directory);
    
    /**
     * @brief Initializes the output directory of the master data collector and performs bootstrap analysis.
     * 
     * @param standard_solution An integer indicating the index of the standard solution.
     * @param n_sets An integer indicating the number of sets to run.
     * @param n_bs_samples An integer indicating the number of bootstrap samples to run.
     * @param threads An integer indicating the number of threads to use.
     * @param realizations_to_run A reference to the 2D vector of integers containing data for the realizations to run.
     */
    void runBootstrapRealizationThinning(int standard_solution, int n_sets, int n_bs_samples,
                                         int threads, vector<vector<int>> &realizations_to_run);

    /**
     * @brief Sets the version number of the problem formulation to use. 
     * Formulation 0: Fixed and variable JLWTP options are all considered and independently ordered.
     * Formulation 1: Only fixed JLWTP options are considered and indepedently ordered.
     * Formulation 2: Only variable JLWTP options are considered and indepedently ordered.
     * 
     * @param triangle_formulation An unsigned long integer indicating the formulation number.
     */
    void setFormulation(unsigned long triangle_formulation);

    /**
     * @brief Sets the filename suffix for the demand files unique to the problem.
     * 
     * @param dem_suf A const string reference to the demand files suffix.
     */
    void setDemand_path_suffix(const string &dem_suf);

    /**
     * @brief Sets the subfolder path to the demand files.
     * 
     * @param dem_subfolder A const string reference to the subfolder containing the demand files.
     */
    void setDemand_path_subfolder(const string &dem_subfolder);
};


#endif //TRIANGLEMODEL_PROBLEM_H
