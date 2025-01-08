//
// Created by Bernardo on 11/24/2017.
//

#include <algorithm>
#include <numeric>
#include <random>
#include <set>
//#include <zconf.h>
#include "Problem.h"
#include "../../Utils/Utils.h"
//#include <omp.h>
#include <chrono>

vector<double> Problem::calculateAndPrintObjectives(bool print_files) {
    if (this->master_data_collector != nullptr) {
        if (print_files) {
            this->master_data_collector->setOutputDirectory(io_directory, output_sub_directory);
        }
        string fo = "Objectives";
        objectives = this->master_data_collector->calculatePrintObjectives(
                fo + "_s" + std::to_string(solution_no) + fname_sufix, print_files);
        return objectives;
    } else {
        objectives = vector<double>(25, 1e5);
        return objectives;
    }
}

void Problem::printTimeSeriesAndPathways(bool plot_time_series) {
    /// Calculate objective values.
    if (this->master_data_collector != nullptr) {
        // this->master_data_collector->setOutputDirectory(io_directory, output_sub_directory);

        /// Print output files.
        string fu = "Utilities";
        string fws = "WaterSources";
        string fp = "Policies";
        string fpw = "Pathways";

        //FIXME:PRINT_POLICIES_OUTPUT_TABULAR BLOWING UP MEMORY.
        this->master_data_collector->setOutputDirectory(io_directory, output_sub_directory);
        this->master_data_collector->printPathways(
                fpw + "_s" + std::to_string(solution_no) + fname_sufix);

        // Only print time series if requested.
        if (plot_time_series) {
            this->master_data_collector->printUtilitiesOutputCompact(
                    0, (int) n_weeks, fu + "_s" + std::to_string(solution_no) +
                                      fname_sufix);
            this->master_data_collector->printWaterSourcesOutputCompact(
                    0, (int) n_weeks, fws + "_s" + std::to_string(solution_no) +
                                      fname_sufix);
            this->master_data_collector->printPoliciesOutputCompact(
                    0, (int) n_weeks, fp + "_s" + std::to_string(solution_no) +
                                      fname_sufix);
        }
        //    data_collector->printUtilitesOutputTabular(0,
        //                                               n_weeks,
        //                                               fu + "_s"
        //                                               + std::to_string(solution_no));
        //    data_collector->printWaterSourcesOutputTabular(0,
        //                                                   n_weeks,
        //                                                   fws + "_s"
        //                                                   + std::to_string(solution_no));
        //    data_collector->printPoliciesOutputTabular(0,
        //                                               n_weeks,
        //                                               fp + "_s"
        //                                               + std::to_string(solution_no));
    } else {
        printf("Trying to print pathways but data collector is empty. Either your simulation crashed or you deleted the data collector too early.\n");
    }

}

vector<int> Problem::vecInfraRankToVecInt(vector<infraRank> v) {
    vector<int> sorted;
    for (infraRank ir : v) {
        sorted.push_back(ir.id);
    }

    return sorted;
}

double Problem::checkAndFixInfraExpansionHighLowOrder(
        vector<int> *order, vector<double> *triggers, int id_low,
        int id_high, double capacity_low, double capacity_high) {

    auto pos_low = distance(order->begin(),
                            find(order->begin(),
                                 order->end(),
                                 id_low));

    auto pos_high = distance(order->begin(),
                             find(order->begin(),
                                  order->end(),
                                  id_high));

    if (pos_high < pos_low) {
        capacity_high += capacity_low;
        order->erase(order->begin() + pos_low);
        triggers->erase(triggers->begin() + pos_low);
    }

    return capacity_high;
}

void Problem::setN_weeks(unsigned long n_weeks) {
    Problem::n_weeks = n_weeks;
}

void Problem::setSol_number(unsigned long sol_number) {
    Problem::solution_no = sol_number;
}

void Problem::setIODirectory(const string &io_directory) {
    this->io_directory = io_directory;
}

void Problem::setRDMOptimization(vector<vector<double>> &utilities_rdm,
                                 vector<vector<double>> &water_sources_rdm,
                                 vector<vector<double>> &policies_rdm) {
    this->utilities_rdm = utilities_rdm;
    this->water_sources_rdm = water_sources_rdm;
    this->policies_rdm = policies_rdm;
}

void Problem::setRDMReevaluation(int rdm_no, vector<vector<double>> &utilities_rdm,
                                 vector<vector<double>> &water_sources_rdm,
                                 vector<vector<double>> &policies_rdm) {
    this->rdm_no = rdm_no;
    this->utilities_rdm = utilities_rdm;
    this->water_sources_rdm = water_sources_rdm;
    this->policies_rdm = policies_rdm;
}

void Problem::setFname_sufix(const string &fname_sufix) {
    Problem::fname_sufix = fname_sufix;
}


void Problem::setEvap_inflows_suffix(const string &evap_inflows_suffix) {
    Problem::evap_inflows_suffix = evap_inflows_suffix;
}

void Problem::setDemand_path_suffix(const string &dem_suf) {
    Problem::demand_path_suffix = dem_suf;
}

void Problem::setDemand_path_subfolder(const string &dem_subfolder) {
    Problem::demand_path_subfolder = dem_subfolder;
}


void Problem::setN_threads(unsigned long n_threads) {
    Problem::n_threads = n_threads;
}

void Problem::setFormulation(unsigned long triangle_formulation) {
    Problem::formulation = triangle_formulation;

    //cout << "The current model formulation flag is " << triangle_formulation << endl;
}

const vector<double> &Problem::getObjectives() const {
    return objectives;
}

void Problem::setPrint_output_files(bool print_output_files) {
    Problem::print_output_files = print_output_files;
}

void Problem::setN_realizations(unsigned long n_realizations) {
    Problem::n_realizations = n_realizations;

    if (realizations_to_run.empty()) {
        realizations_to_run = vector<unsigned long>(n_realizations);
        iota(begin(realizations_to_run), end(realizations_to_run), 0);
    }
}

void Problem::setRealizationsToRun(vector<unsigned long> &realizations_to_run) {
    this->realizations_to_run = realizations_to_run;
}

MasterDataCollector *Problem::getMaster_data_collector() {
    return master_data_collector;
}

Problem::~Problem() {}

void Problem::destroyDataCollector() {
    if (master_data_collector != nullptr) {
        delete master_data_collector;
        master_data_collector = nullptr;
    } else {
        cerr << "Tried to delete nullptr master data collector.\n";
    }
}

Problem::Problem(unsigned long n_weeks) : n_weeks(n_weeks) {
    Reservoir::unsetSeed();
}

void Problem::setImport_export_rof_tables(int import_export_rof_tables, string rof_tables_directory) {
    if (std::abs(import_export_rof_tables) > 1)
        throw invalid_argument("Import/export ROF tables can be assigned as:\n"
                               "-1 - import tables\n"
                               "0 - ignore tables\n"
                               "1 - export tables.\n"
                               "The value entered is invalid.");
    this->import_export_rof_tables = import_export_rof_tables;
    this->rof_tables_directory = rof_tables_directory;

    if (import_export_rof_tables == IMPORT_ROF_TABLES) {
        setRofTables(n_realizations, rof_tables_directory);
    } else {
        Utils::createDir(rof_tables_directory);
    }
}

void
Problem::setRofTables(unsigned long n_realizations, string rof_tables_directory) {

    // Check that the correct number of insurance storage tiers is used (correct is 75 -- why?).
    // Number of columns in ROF table should be equal to number of insurance storage tiers.
    //double start_time = omp_get_wtime();
    auto start = std::chrono::high_resolution_clock::now();
    printf("Reading ROF tables.\n");
    string file_name = rof_tables_directory + "tables_r0_u0.csv";
    auto data_r0_u0 = Utils::parse2DCsvFile(file_name);
    auto n_weeks_in_table = (int) data_r0_u0.size();
    auto n_tiers = (int) data_r0_u0.at(0).size();

    if (n_tiers != NO_OF_INSURANCE_STORAGE_TIERS) {
        char error[75];
        sprintf(error, "Number of tiers in tables does not match number of tiers for this problem.");
    }

    // Count the total number of utilities with associated ROF tables.
    // This is done by incrementing the number of utilities until a file is not found.
    n_utilities = 0;
    string fname = rof_tables_directory + "tables_r0_u0.csv";
    fstream f;
    std::ifstream ifile(fname.c_str()); 
    while ((bool) ifile) {
        n_utilities += 1;
        fname = rof_tables_directory + "tables_r0_u" + to_string(n_utilities) + ".csv"; 
        ifile = std::ifstream(fname.c_str());
    }

    // Create a 2-layer matrix to sotre the ROF tables for each realization and utility
    rof_tables = vector<vector<Matrix2D<double>>>(
            n_realizations,
            vector<Matrix2D<double>>((unsigned long) n_utilities,
                                     Matrix2D<double>(n_weeks_in_table, n_tiers)));

    // For each realization and utility, load the ROF tables.
    for (unsigned long r = 0; r < n_realizations; ++r) {
        for (int u = 0; u < n_utilities; ++u) {
            string file_name = rof_tables_directory + "tables_r" + to_string(r) + "_u" + to_string(u) + ".csv";
            auto tables_utility_week = Utils::parse2DCsvFile(file_name);

            for (int w = 0; w < n_weeks; ++w) {
                rof_tables[r][u].setPartialData(w, tables_utility_week[w].data(), tables_utility_week[w].size());
            }
//        u = 1;
//        file_name = rof_tables_directory + "tables_r" + to_string(r) + "_u" + to_string(u) + ".csv";
//        tables_utility_week = Utils::parse2DCsvFile(file_name);
//
//        for (int w = 0; w < n_weeks; ++w) {
//            rof_tables[r][u].setPartialData(w, tables_utility_week[w].data(), tables_utility_week[w].size());
//        }
//        u = 2;
//        file_name = rof_tables_directory + "tables_r" + to_string(r) + "_u" + to_string(u) + ".csv";
//        tables_utility_week = Utils::parse2DCsvFile(file_name);
//
//        for (int w = 0; w < n_weeks; ++w) {
//            rof_tables[r][u].setPartialData(w, tables_utility_week[w].data(), tables_utility_week[w].size());
//        }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    //printf("Loading tables took %f seconds.\n", omp_get_wtime() - start_time);
    printf("Loading tables took %f seconds.\n", duration.count());
}

void Problem::runBootstrapRealizationThinning(int standard_solution, int n_sets, int n_bs_samples,
                                              int threads, vector<vector<int>> &realizations_to_run) {
    master_data_collector->setOutputDirectory(io_directory, output_sub_directory);
    master_data_collector->performBootstrapAnalysis(standard_solution, n_sets, n_bs_samples, threads, realizations_to_run);
}

void Problem::setOutputSubDirectory(const string &output_sub_directory_path) {
    this->output_sub_directory = output_sub_directory_path;
}
