/**
 * @class Constants
 * @brief This class is defined in a header-only file that contains the default value of the constants used throughout the model.
 * Update this file to change the default values of the constants.
 * 
 * Created by bernardo on 1/12/17.
 */
#ifndef TRIANGLEMODEL_CONSTANTS_H
#define TRIANGLEMODEL_CONSTANTS_H


#include <map>
#include <cmath>
#include <cstring>
#include <vector>
#include <string>

namespace Constants {
#ifdef _WIN32
    const std::string BAR = "\\";
    const std::string DEFAULT_SYS_IO_DIR = "c:\\%HOMEPATH%\\";
#else
    const std::string BAR = "/";
    const std::string DEFAULT_SYS_IO_DIR = "./";
#endif
    /**
     * @brief The number of objectives in the optimization problem.
     */
    const int NUM_OBJECTIVES = 5;
    
    //#define NUM_DEC_VAR 57;
    //    const int NUM_DEC_VAR = 57; // infrastructure turned on
    //    const int NUM_DEC_VAR = 30; // infrastructure turned off
    
    /**
     * @brief The number of decision variables in the optimization problem.
     * This number includes the decision variables for the infrastructure and the decision variables for the utilities.
     */
    const int NUM_DEC_VAR = 70; 

    /**
     * @brief The default directory where input data files are stored.
     */
    const std::string DEFAULT_DATA_DIR = "TestFiles" + BAR;
    /**
     * @brief The default directory where output files are stored.
     * This folder will always be created if it doesn't exist.
     */
    const std::string DEFAULT_OUTPUT_DIR = "output" + BAR;
    /**
     * @brief The default directory where the ROF tables are stored.
     * This folder will always be created if it doesn't exist.
     */
    const std::string DEFAULT_ROF_TABLES_DIR = "rof_tables" + BAR;
    /**
     * @brief The default directory where the input DU factor timeseries are.
     * By default, WaterPaths assumes that all DU factor files are located in the same directory as the WaterPaths executable.
     */
    const std::string DEFAULT_RDM_TSERIES_DIR = "";

    /**
     * @brief Peaking factor used to move from daily capacity to average weekly capacity.
     */
    const double TRANSFER_PEAKING_FACTOR = 0.85; 
    /**
     * @brief Demand peaking factor used to scale the Joint WTP treatment re-allocation calculation.
     */
    const double DEMAND_PEAKING_FACTOR = 1.2; 
    /**
     * @brief The number of weeks in a year.
     */
    const double WEEKS_IN_YEAR = 52.179;
    /**
     * @brief The number of weeks in a year rounded to the nearest integer.
     */
    const int WEEKS_IN_YEAR_ROUND = 52;
    /**
     * @brief The number of months in a year.
     */
    const int NUMBER_OF_MONTHS = 12;
    /**
     * @brief The number of weeks in a month.
     */
    const double WEEKS_IN_MONTH = WEEKS_IN_YEAR / (double) NUMBER_OF_MONTHS;
    /**
     * @brief The number of weeks used to calculate the short-term ROF.
     */
    const int WEEKS_ROF_SHORT_TERM = 52;
    /**
     * @brief The number of weeks used to calculate the long-term ROF.
     */
    const int WEEKS_ROF_LONG_TERM = 78;
    /**
     * @brief The default starting value of the short-term ROF.
     */
    const int SHORT_TERM_ROF = 0;
    /**
     * @brief The default starting value of the long-term ROF.
     */
    const int LONG_TERM_ROF = 1;
    /**
     * @brief The number of realizations used to calculate the ROF for insurance pricing.
     * FIXME: Need to clarify this.
     */
    const int NUMBER_REALIZATIONS_INSURANCE_PRICING = 50;
    /**
     * @brief The number of realizations used to calculate ROFs in general.
     */
    const int NUMBER_REALIZATIONS_ROF = 50;
    /**
     * @brief The treatment capacity of a small WTP in MGD.
     * FIXME: Need to clarify this.
     */
    const int TREATMENT_CAPACITY_VS_VOLUME_SMALL_WTP = 200;
    /**
     * @brief The treatment capacity of a large WTP in MGD.
     * FIXME: Need to clarify this.
     */
    const int ILLIMITED_TREATMENT_CAPACITY = 99999;
    /**
     * @brief The storage-to-capacity ratio below which the system is considered to have failed.
     */
    const double STORAGE_CAPACITY_RATIO_FAIL = 0.2;
    /**
     * @brief The treatment-to-capacity ratio below which the system is considered to have failed.
     * Based on this PDF here by the `North Carolina Office of Administrative Hearings <http://reports.oah.state.nc.us/ncac/title%2015a%20-%20environmental%20quality/chapter%2002%20-%20environmental%20management/subchapter%20t/15a%20ncac%2002t%20.0118.pdf>`
     */
    const double TREATMENT_CAPACITY_RATIO_FAIL = 0.9; 
    /**
     * @brief A flag to indicate that a water source has not been allocated to a utility.
     */
    const int NOT_ALLOCATED = 1;
    /**
     * @brief The maximum number of utilities that can be considered in the model.
     * FIXME: Could be expanded.
     */
    const int MAX_NUMBER_OF_UTILITIES = 50;
    /**
     * @brief The maximum fraction of contingency fund to revenue after which insurance is purchased.
     */
    const double MAX_FRACTION_CF_TO_BUY_INSURANCE = 0.7;

    /**
     * @brief A boolean flag to indicate whether the demand buffer is applied.
     */
    const bool APPLY_DEMAND_BUFFER = true;
    /**
     * @brief A boolean flag to indicate whether the demand projection is applied.
     */
    const bool APPLY_DEMAND_PROJECTION = true;
    /**
     * @brief A boolean flag to indicate whether the demand is reprojected.
     */
    const bool REPROJECT_DEMAND = true;
    /**
     * @brief The number of years to look ahead for demand projection.
     */
    const int LOOK_AHEAD_YEARS_FOR_DEMAND_PROJECTION = 5;
    /**
     * @brief The number of years to look back for demand reprojection.
     */
    const int LOOK_BACK_YEARS_FOR_DEMAND_REPROJECTION = 5;
    /**
     * @brief The frequency of demand reprojection in years.
     */
    const int FREQUENCY_OF_DEMAND_REPROJECTION_YEARS = 5;
    /**
     * @brief The proportion of the utility's capacity that is considered as a floor after which it can no longer supply water.
     * FIXME: Need to clarify this.
     */

    const double PROPORTION_OF_UTILITY_CAPACITY_FLOOR = 0.1;

    /**
     * @brief The initial value of the massive value used in the model.
     * FIXME: Need to clarify this.
     */
    const int INITIAL_MASSIVE_VALUE = 99999;

    /**
     * @brief A boolean flag indicating whether the model should use pre-calculated ROF tables.
     */
    const bool USE_PRE_CALCULATED_ROF_TABLES = true;
    /**
     * @brief A boolean flag indicating if the model should NOT use pre-calculated ROF tables.
     */
    const bool DO_NOT_USE_PRE_CALCULATED_ROF_TABLES = false;

    /**
     * @brief A flag indicating that the model should export ROF tables.
     */
    const int EXPORT_ROF_TABLES = 1;
    /**
     * @brief A flag indicating that the model should not import nor export ROF tables.
     */
    const int DO_NOT_EXPORT_OR_IMPORT_ROF_TABLES = 0;
    /**
     * @brief A flag indicating that the model should import ROF tables.
     */
    const int IMPORT_ROF_TABLES = -1;
    /**
     * @brief The baseline storage capacity multiplier
     * FIXME: Need to clarify this.
     */
    const double BASE_STORAGE_CAPACITY_MULTIPLIER = 4.;

    /**
     * @brief A boolean flag indicating that a water source is online.
     */
    const bool ONLINE = true;
    /**
     * @brief A boolean flag indicating that a water source is offline.
     */
    const bool OFFLINE = false;

    /**
     * @brief A flag indicating that a utility is in a state of failure.
     */
    const int FAILURE = 1;
    /**
     * @brief A flag indicating that a utility is not in a state of failure.
     */
    const int NON_FAILURE = 0;

    /**
     * @brief The default `NONE` value.
     */
    const int NONE = 0;
    /**
     * @brief A flag indicating that an object has not yet been initialized.
     */
    const int NON_INITIALIZED = -1;

    /**
     * @brief The default ID of a utility.
     */
    const int UTILITY = 100;

    // 0 - 99 for non-storage sources, 100 - 199 for
    // storage sources, 200 - 299 for treatment and
    // capacity expansions.
    const int INTAKE = 0;
    const int ALLOCATED_INTAKE = 10;
    const int INTAKE_EXPANSION = 2;
    const int ALLOCATED_INTAKE_EXPANSION = 202;
    const int WATER_REUSE = 1;
    const int RESERVOIR = 100;
    const int QUARRY = 101;
    const int ALLOCATED_RESERVOIR = 102;
    const int NEW_WATER_TREATMENT_PLANT = 200;
    const int NEW_JOINT_WATER_TREATMENT_PLANT = 300;
    const int NEW_JOINT_WATER_TREATMENT_PLANT_FIXED_ALLOCATIONS = 301;
    const int NEW_JOINT_WATER_TREATMENT_PLANT_VARIABLE_ALLOCATIONS = 302;
    const int SOURCE_RELOCATION = 201;
    const int RESERVOIR_EXPANSION = 203;

    // Use 0 - 99 for supply instruments and 100 - 199
    // for financial instruments.
    const int TRANSFERS = 0;
    const int RESTRICTIONS = 1;
    const int INSURANCE_STORAGE_ROF = 100;

    const int SUPPLY_INSTRUMENT_LIM = 99;
    const int FINANCIAL_INSTRUMENT_LIM = 199;

    /**
     * @brief The ID of the fixed flow controls `MinEnvFlow` object.
     */
    const int FIXED_FLOW_CONTROLS = 0;
    /**
     * @brief The ID of the inflow controls `MinEnvFlow` object.
     */
    const int INFLOW_CONTROLS = 1;
    /**
     * @brief The ID of the seasonal controls `MinEnvFlow` object.
     */
    const int SEASONAL_CONTROLS = 2;
    /**
     * @brief The ID of the storage controls `MinEnvFlow` object.
     */
    const int STORAGE_CONTROLS = 3;
    /**
     * @brief The ID of the Jordan Lake controls `MinEnvFlow` object.
     */
    const int JORDAN_CONTROLS = 4;
    /**
     * @brief The ID of the Falls Lake controls `MinEnvFlow` object.
     */
    const int FALLS_CONTROLS = 5;

    /**
     * @brief The ID of the level debt service Bond object.
     */
    const int LEVEL_DEBT_SERVICE = 0;
    /**
     * @brief The ID of the balloon payment Bond object.
     */
    const int BALLOON_PAYMENT = 1;
    /**
     * @brief The ID of the floating interest Bond object.
     */
    const int FLOATING_INTEREST = 2;
    /**
     * @brief The ID of the variable interest Bond object.
     */
    const int VARIABLE_INTEREST = 3;

    /**
     * @brief A boolean flag indicating if a bond began its repayment at issuance.
     */
    const bool BEGIN_REPAYMENT_AT_ISSUANCE = true;
    
    /**
     * @brief The default column with for printing table (`.tab`) files.
     */
    const int COLUMN_WIDTH = 10;
    /**
     * @brief The default precision for printing table (`.tab`) files.
     */
    const int COLUMN_PRECISION = 4;

    /**
     * @brief The default worst-case cost percentile.
     * FIXME: Should be renamed to `WORST_CASE_COST_PERCENTILE`.
     */
    const double WORSE_CASE_COST_PERCENTILE = 0.99;
    /**
     * @brief The default unit cost percentile.
     */
    const double UNIT_COST_PERCENTILE = 0.90;

    /**
     * @brief The default storage curve threshold above which insurance is purchased.
     * FIXME: Need to clarify this.
     */
    const double INSURANCE_SHIFT_STORAGE_CURVES_THRESHOLD = 1. / 75;
    /**
     * @brief The number of insurance storage tiers as a function of the storage curve threshold.
     * FIXME: Need to clarify this.
     */
    const int NO_OF_INSURANCE_STORAGE_TIERS = (int) std::round(1. / INSURANCE_SHIFT_STORAGE_CURVES_THRESHOLD);

    /**
     * @brief A flag to identify if a water source has a water quality allocation.
     */
    const int WATER_QUALITY_ALLOCATION = -1;

    /**
     * @brief The number of a specific week of a year across all years in the simulation model up to week 4017.
     */
    static constexpr int WEEK_OF_YEAR[4017] = {
            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};

};

#endif //TRIANGLEMODEL_CONSTANTS_H
