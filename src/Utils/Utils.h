/**
 * @class Utils
 * @brief The Utils class defines a set of utility functions that are used throughout the model.
 * 
 * Created by bernardo on 1/13/17.
 */
#ifndef TRIANGLEMODEL_AUX_H
#define TRIANGLEMODEL_AUX_H


#include "../SystemComponents/WaterSources/Base/WaterSource.h"
#include "../SystemComponents/WaterSources/Intake.h"
#include "../SystemComponents/Utility/Utility.h"
#include "../DroughtMitigationInstruments/Base/DroughtMitigationPolicy.h"
#include "../DroughtMitigationInstruments/Restrictions.h"
#include "../Controls/Base/MinEnvFlowControl.h"

class Utils {
public:
        /**
         * @brief Reads a CSV file into a 2D vector of doubles.
         *        This function parses the contents of a CSV file and returns it as a 2D vector, optionally reading a limited number of lines or specific rows.
         *
         * @param file_name The full path of the input CSV file.
         * @param max_lines The maximum number of lines to read from the file.
         * @param rows_to_read Optional; a vector of row indices to read from the file.
         * 
         * @return A vector of vectors containing the parsed data as doubles.
         * 
         * @throw std::invalid_argument if the file is not found or appears to be space-separated.
         */
        static vector<vector<double>> parse2DCsvFile(basic_string<char, char_traits<char>, allocator<char>> file_name,
                                                 unsigned long max_lines = 10000000,
                                                 vector<unsigned long> rows_to_read = vector<unsigned long>());

        /**
         * @brief Reads a CSV file into a 1D vector of doubles.
         *        Parses a CSV file and extracts its data into a 1D vector with optional row filtering.
         *
         * @param file_name The full path of the input CSV file.
         * @param max_lines The maximum number of lines to read from the file.
         * @param rows_to_read Optional; a vector of row indices to read from the file.
         * 
         * @return A vector containing the parsed data as doubles.
         * 
         * @throw std::invalid_argument if the file cannot be read.
         */
        static vector<double> parse1DCsvFile(basic_string<char, char_traits<char>, allocator<char>> file_name,
                                         unsigned long max_lines = 10000000,
                                         vector<unsigned long> rows_to_read = vector<unsigned long>());

        /**
         * @brief Creates a copy of a vector of `WaterSource` objects.
         *        This function deep copies water source objects based on their derived types.
         *
         * @param water_sources_original The original vector of `WaterSource` pointers.
         * 
         * @return A new vector containing deep copies of the input water sources.
         * 
         * @throw std::invalid_argument if a water source type is not implemented.
         */
        static vector<WaterSource *> copyWaterSourceVector(vector<WaterSource *> water_sources_original);

        /**
         * @brief Creates a copy of a vector of `Utility` objects.
         *        Optionally clears water sources in the copied utilities.
         *
         * @param utility_original The original vector of `Utility` pointers.
         * @param clear_water_sources Whether to clear water sources in the copied utilities.
         * 
         * @return A new vector containing deep copies of the input utilities.
         */
        static vector<Utility *> copyUtilityVector(vector<Utility *> utility_original, bool clear_water_sources = false);

        /**
         * @brief Creates a copy of a vector of `DroughtMitigationPolicy` objects.
         *        This function duplicates drought mitigation policies based on their derived types.
         *
         * @param drought_mitigation_policy_original The original vector of `DroughtMitigationPolicy` pointers.
         * 
         * @return A new vector containing deep copies of the input policies.
         */
        static vector<DroughtMitigationPolicy *> copyDroughtMitigationPolicyVector(
            vector<DroughtMitigationPolicy *> drought_mitigation_policy_original);

        /**
         * @brief Creates a copy of a vector of `MinEnvFlowControl` objects.
         *        This function duplicates a vector of environmental flow controls based on their specific derived types.
         *
         * @param min_env_flow_controls_original The original vector of `MinEnvFlowControl` pointers.
         * 
         * @return A new vector containing deep copies of the input objects.
         * 
         * @throw std::invalid_argument if a control type is not implemented.
         */
        static vector<MinEnvFlowControl *> copyMinEnvFlowControlVector(
            vector<MinEnvFlowControl *> min_env_flow_controls_original);

        /**
         * @brief Determines if a given week corresponds to the first week of the year.
         *
         * @param week The week number to check.
         * @return True if the given week is the first week of the year; otherwise, false.
         */ 
        static bool isFirstWeekOfTheYear(int week);

        /**
         * @brief Returns the week of the year for a given week index.
         *
         * @param week The week number to process.
         * @return The corresponding week of the year.
         */
        static int weekOfTheYear(int week);

        /**
         * @brief Deletes a specified element from an integer vector.
         *
         * @param vec The vector from which to remove the element.
         * @param el The integer value to be removed.
         * 
         * @return void
         */
        static void removeIntFromVector(vector<int> &vec, int el);

        /**
         * @brief Prints details of an exception with nesting levels.
         *        Recursively prints exception messages and nested exceptions.
         *
         * @param e The exception to be printed.
         * @param level The current nesting level for formatting purposes.
         * 
         * @return void
         */
        static void print_exception(const exception &e, int level = 0);

        /**
         * @brief Creates a copy of a vector of `Bond` objects.
         *        This function deep copies bond objects based on their derived types.
         *
         * @param bonds_original The original vector of `Bond` pointers.
         * 
         * @return A new vector containing deep copies of the input bonds.
         * 
         * @throw std::invalid_argument if a bond type is not implemented.
         */
        static vector<Bond *> copyBonds(vector<Bond *> bonds_original);

        /**
         * @brief Creates a directory at the specified path.
         *
         * @param directory The path of the directory to create.
         * 
         * @return void
         */
        static void createDir(string directory);
};


#endif //TRIANGLEMODEL_AUX_H
