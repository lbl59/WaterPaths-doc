/**
 * @class MasterDataCollector
 * @brief The `MasterDataCollector` class implements the data collection of all WaterSource, Utility, and Bond objects across multiple hydroclimatic realizations.
 * 
 */

// Created by bernardoct on 8/26/17.
#ifndef TRIANGLEMODEL_MASTERDATACOLLECTOR_H
#define TRIANGLEMODEL_MASTERDATACOLLECTOR_H

#include <vector>
#include "Base/DataCollector.h"
#include "UtilitiesDataCollector.h"
#include "../DroughtMitigationInstruments/Base/DroughtMitigationPolicy.h"
#include "RestrictionsDataCollector.h"

class MasterDataCollector {
private:
    /**
     * @brief The output directory where the data will be saved.
     */
    string output_directory;
    /**
     * @brief The number of realizations for which data will be collected
     */
    unsigned long n_realizations;
    /**
     * @brief A 2D vector of DataCollector objects that store the data collected for all
     * water sources in across all realizations.
     */
    vector<vector<DataCollector *>> water_source_collectors;
    /**
     * @brief A 2D vector of DataCollector objects that store the data collected for all
     * drought mitigation policies in across all realizations.
     */
    vector<vector<DataCollector *>> drought_mitigation_policy_collectors;
    /**
     * @brief A 2D vector of UtilitiesDataCollector objects that store the data collected for all
     * utilities in across all realizations.
     */
    vector<vector<UtilitiesDataCollector *>> utility_collectors;
    /**
     * @brief A vector of the IDs of realizations that have crashed.
     */
    vector<unsigned long> crashed_realizations;
    /**
     * @brief A vector of the IDs of realizations that have been successfully run.
     */
    vector<unsigned long> realizations_ran;
    /**
     * @brief The number of realizations that have been created.
     */
    int realizations_created = 0;   
    /**
     * @brief The seed used for the random number generator.
     */
    static int seed;

public:
    /**
     * @brief Constructs a MasterDataCollector to manage and collect data across multiple realizations.
     *        Initializes the number of realizations and the list of realizations to run.
     * 
     * @param realizations_to_run A reference to a vector of unsigned long integers representing the realizations to be run.
     */
    MasterDataCollector(vector<unsigned long> &realizations_to_run);

    /**
     * @brief Destroys the MasterDataCollector, cleaning up all dynamically allocated DataCollector and UtilitiesDataCollector objects.
     */
    virtual ~MasterDataCollector();

    /**
     * @brief Writes utility data collected during the simulation into a NetCDF file.
     *        The data includes variables such as storage, risk of failure, demands, and payments for each utility across all realizations.
     * 
     * FIXME: NetCDF file being saved with hardly any compression. Someone who knows more
     * about NetCDF may be able to improve compression, which would be great given the 
     * amount of output that can be generated in one run.
     * 
     * @param base_file_name The base name for the NetCDF file to be created.
     * 
     * @return An integer representing the status of the operation:
     *         - `0` if the file is successfully written.
     *         - `NC_ERR` if an error occurs during the NetCDF operations.
     *         - `1` if NetCDF is not enabled in this build.
     * 
     * @throws NcException If a NetCDF operation fails, an exception is thrown with error details.
     */
    int printNETCDFUtilities(string base_file_name);

    
    vector<double> calculatePrintObjectives(string file_name, bool print);

    /**
     * @brief Prints the output of drought mitigation policies in a compact CSV format for a specified time range.
     *        This function generates separate CSV files for each realization.
     * 
     * @param week_i Integer representing the starting week of the time range.
     * @param week_f Integer representing the ending week of the time range.
     * @param file_name String specifying the base name for the output files.
     * 
     * @return void
     * 
     * @see DataCollector::printCompactString, DataCollector::printCompactStringHeader
     */
    void printPoliciesOutputCompact(
        int week_i, int week_f, string file_name);

    /**
     * @brief Prints the output of drought mitigation policies in a tabular format for a specified time range.
     *        This function generates separate `.tab` files for each realization.
     * 
     * @param week_i Integer representing the starting week of the time range.
     * @param week_f Integer representing the ending week of the time range.
     * @param file_name String specifying the base name for the output files.
     * 
     * @return void
     * 
     * @see DataCollector::printTabularString, DataCollector::printTabularStringHeaderLine1,
     *      DataCollector::printTabularStringHeaderLine2
     */
    void printPoliciesOutputTabular(int week_i, int week_f, string file_name);

    /**
     * @brief Prints the output of utility data in a compact CSV format for a specified time range.
     *        This function generates separate CSV files for each realization.
     * 
     * @param week_i Integer representing the starting week of the time range.
     * @param week_f Integer representing the ending week of the time range.
     * @param file_name String specifying the base name for the output files.
     * 
     * @return void
     * 
     * @see UtilitiesDataCollector::printCompactString, UtilitiesDataCollector::printCompactStringHeader
     */
    void printUtilitiesOutputCompact(int week_i, int week_f, string file_name);

    /**
     * @brief Prints the output of utility data in a tabular format for a specified time range.
     *        This function generates separate `.tab` files for each realization.
     * 
     * @param week_i Integer representing the starting week of the time range.
     * @param week_f Integer representing the ending week of the time range.
     * @param file_name String specifying the base name for the output files.
     * 
     * @return void
     * 
     * @see UtilitiesDataCollector::printTabularString, 
     *      UtilitiesDataCollector::printTabularStringHeaderLine1, 
     *      UtilitiesDataCollector::printTabularStringHeaderLine2
     */
    void printUtilitesOutputTabular(int week_i, int week_f, string file_name);

    /**
     * @brief Prints the output of water source data in a compact CSV format for a specified time range.
     *        This function generates separate CSV files for each realization.
     * 
     * @param week_i Integer representing the starting week of the time range.
     * @param week_f Integer representing the ending week of the time range.
     * @param file_name String specifying the base name for the output files.
     * 
     * @return void
     * 
     * @throw A warning is printed and the specific realization's data is skipped.
     * 
     * @see DataCollector::printCompactString, DataCollector::printCompactStringHeader
     */
    void
    printWaterSourcesOutputCompact(int week_i, int week_f, string file_name);

    /**
     * @brief Prints the output of water source data in a tabular format for a specified time range.
     *        This function generates separate `.tab` files for each realization.
     * 
     * @param week_i Integer representing the starting week of the time range.
     * @param week_f Integer representing the ending week of the time range.
     * @param file_name String specifying the base name for the output files.
     * 
     * @return void
     * 
     * @see DataCollector::printTabularString, DataCollector::printTabularStringHeaderLine1, 
     *      DataCollector::printTabularStringHeaderLine2
     */
    void
    printWaterSourcesOutputTabular(int week_i, int week_f, string file_name);

    /**
     * @brief Sets the output directory for storing generated output files.
     *        If the specified directory differs from the current one, it creates the directory and updates the output location.
     * 
     * @param io_directory String representing the base directory for output files.
     * @param sub_directory String specifying the sub-directory to append to the base directory.
     * 
     * @return void
     */
    void setOutputDirectory(string io_directory, string sub_directory);

    /**
     * @brief Prints the infrastructure pathways for each utility and realization into an output file.
     *        The output includes a text file with tab-separate columns showing data on
     *        the realization ID, utility ID, week, and infrastructure ID.
     * 
     * @param file_name String specifying the base name for the output file.
     * 
     * @return void
     */
    void printPathways(string file_name);

    /**
     * @brief Adds data collectors for a new realization, initializing data structures and creating collectors for water sources, drought mitigation policies, and utilities.
     * 
     * @param water_sources_realization Vector of pointers to `WaterSource` objects for the realization.
     * @param drought_mitigation_policies_realization Vector of pointers to `DroughtMitigationPolicy` objects for the realization.
     * @param utilities_realization Vector of pointers to `Utility` objects for the realization.
     * @param r Unsigned long integer representing the realization ID.
     * 
     * @return void
     */
    void addRealization(
        vector<WaterSource *> water_sources_realization,
        vector<DroughtMitigationPolicy *> drought_mitigation_policies_realization,
        vector<Utility *> utilities_realization, unsigned long r);

    /**
     * @brief Removes a specified realization, deallocating associated data collectors and updating tracking structures.
     * 
     * @param r Unsigned long integer representing the realization ID to be removed.
     * 
     * @return void
     */
    void removeRealization(unsigned long r);

    /**
     * @brief Cleans up data collector vectors by removing entries corresponding to deleted realizations.
     *        This ensures that no `nullptr` entries remain in the data collector structures.
     * 
     * @return void
     */
    void cleanCollectorsOfDeletedRealizations();

    /**
     * @brief Collects data for a specified realization `r` by invoking the `collect_data` method on all associated data collectors.
     * 
     * @param r Unsigned long integer representing the realization ID for which data will be collected.
     * 
     * @return void
     * 
     * @see DataCollector::collect_data
     */
    void collectData(unsigned long r);

    /**
     * @brief Performs a bootstrap analysis to generate statistical samples for objectives based on multiple sets of realizations.
     * 
     * @param sol_id Integer representing the solution ID for which the bootstrap analysis is conducted.
     * @param n_sets Integer specifying the number of bootstrap sample sets to generate.
     * @param n_samples Integer specifying the number of samples per bootstrap set.
     * @param n_threads Integer representing the number of threads to use for parallelization.
     * @param bootstrap_samples Vector of vectors containing pre-defined bootstrap samples, if available. Otherwise, new samples will be generated.
     * 
     * @return void
     * 
     * @see readOrCreateBSSamples
     */
    void performBootstrapAnalysis(int sol_id, int n_sets, int n_samples, int n_threads,
        vector<vector<int>> bootstrap_samples = vector<vector<int>>());

    /**
     * @brief Creates and returns a data collector for a specified drought mitigation policy based on its type.
     * 
     * @param dmp Pointer to the `DroughtMitigationPolicy` object for which a data collector is to be created.
     * @param r Unsigned long integer representing the realization ID.
     * 
     * @return A pointer to the newly created `DataCollector` object corresponding to the given policy.
     * 
     * @throw std::invalid_argument If the policy type is not recognized or not handled in the implementation.
     */
    DataCollector* createPolicyDataCollector(DroughtMitigationPolicy* dmp, unsigned long r);

    /**
     * @brief Creates and returns a data collector for a specified water source based on its type.
     * 
     * @param ws Pointer to the `WaterSource` object for which a data collector is to be created.
     * @param r Unsigned long integer representing the realization ID.
     * 
     * @return A pointer to the newly created `DataCollector` object corresponding to the given water source.
     * 
     * @throw std::invalid_argument If the water source type is not recognized or not handled in the implementation.
     */
    DataCollector* createWaterSourceDataCollector(WaterSource* ws, unsigned long r);

    /**
     * @brief Calculates and writes utility objectives to a row in an output stream and appends the results to the objectives vector.
     * 
     * @param u Reference to a vector of `UtilitiesDataCollector *` representing the utility data collectors for the realization.
     * @param outStream Reference to the `std::ofstream` where the objectives will be written.
     * @param objectives Reference to a vector of doubles where the calculated objectives will be appended.
     * 
     * @return void
     */
    void printUtilityObjectivesToRowOutStream(vector<UtilitiesDataCollector *> &u, std::ofstream &outStream,
        vector<double> &objectives);

    /**
     * @brief Performs a bootstrap analysis to compute statistical objectives based on multiple sets of realizations and writes results to output files.
     * 
     * @param sol_id Integer representing the solution ID for which the bootstrap analysis is conducted.
     * @param n_sets Integer specifying the number of bootstrap sample sets to generate.
     * @param n_samples Integer specifying the number of samples per bootstrap set.
     * @param n_threads Integer representing the number of threads to use for parallelization.
     * @param bootstrap_samples Vector of vectors containing pre-defined bootstrap samples. If empty, new samples will be generated.
     * 
     * @return void
     * 
     * @see readOrCreateBSSamples, isolateRestrictionDataCollectors, printObjsBSSamples, printBSSamples, printObjectivesOfAllRealizationsForBSAnalysis
     */
    void readOrCreateBSSamples(int sol_id, int n_sets, int n_samples, const vector<vector<int>> &bootstrap_samples,
                            vector<vector<int>> &bootstrap_sample_sets) const;

    
    /**
     * @brief Prints the calculated objectives for bootstrap samples to a CSV file.
     *        Each row represents the objectives for a specific bootstrap set.
     * 
     * @param sol_id Integer representing the solution ID for which the objectives are being printed.
     * @param n_sets Integer specifying the number of bootstrap sample sets.
     * @param n_samples Integer specifying the number of samples per bootstrap set.
     * @param objectives Reference to a vector of vectors containing the calculated objectives for each bootstrap set.
     * 
     * @return void
     */
    void printObjsBSSamples(int sol_id, int n_sets, int n_samples, vector<vector<double>> &objectives);

    void printObjectivesOfAllRealizationsForBSAnalysis(int sol_id, int n_sets, int n_samples);

    static void setSeed(int seed);

    static void unsetSeed();

    void printBSSamples(int sol_id, int n_sets, int n_samples, const vector<vector<int>> &bootstrap_sample_sets) const;

    void isolateRestrictionDataCollectors(vector<UtilitiesDataCollector *> &u,
                                        vector<RestrictionsDataCollector *> &utility_restrictions) const;

    int getRealizations_created() const;
};


#endif //TRIANGLEMODEL_MASTERDATACOLLECTOR_H
