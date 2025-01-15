/**
 * @class WwtpDischargeRule
 * @brief This class is a subclass of the main `ControlRules` class and represents the discharge rules for a wastewater treatment plant (WWTP).
 * 
 * Created by bernardoct on 6/26/17.
 */

#ifndef TRIANGLEMODEL_WWTPDISCHARGERULES_H
#define TRIANGLEMODEL_WWTPDISCHARGERULES_H

#include "Base/ControlRules.h"
#include <vector>
#include <stdexcept>
#include <string>

using namespace std;

class WwtpDischargeRule : public ControlRules {
private:
    /**
     * @brief A vector of IDs mapping the source IDs to the corresponding index in the `discharge_to_source_ids` vector.
     */
    vector<int> source_id_to_vector_index;

public:
    /**
     * @brief A 2D vector of doubles containing the fraction of the week's demand to be discharged as effluent.
     */
    vector<vector<double>> year_series_fraction_discharge;
    /**
     * @brief A vector of integers containing the IDs of the sources receiving the effluent.
     */
    vector<int> discharge_to_source_ids;

    /**
     * @brief Constructs a `WwtpDischargeRule` object to manage discharge rules for a wastewater treatment plant (WWTP).
     *        Initializes the object with time-series discharge fractions and the associated discharge source IDs.
     * 
     * @param year_series_fraction_discharge A vector of vectors, where each inner vector contains discharge fractions for each week of the year.
     *        Each vector must have exactly 53 elements (to account for years with an extra week).
     * @param discharge_to_source_ids A vector of integers representing the IDs of the discharge sources.
     * 
     * @throw std::invalid_argument If any time series does not have exactly 53 elements.
     * @throw std::invalid_argument If the number of discharge time series does not match the number of discharge source IDs.
     */
    WwtpDischargeRule(vector<vector<double>> year_series_fraction_discharge,
                      vector<int> discharge_to_source_ids);

    /**
     * @brief Copy constructor for the `WwtpDischargeRule` class.
     *        Creates a new `WwtpDischargeRule` object by copying the attributes from an existing one.
     * 
     * @param wwtp_discharge_rule Reference to the `WwtpDischargeRule` object to be copied.
     */
    WwtpDischargeRule(WwtpDischargeRule &wwtp_discharge_rule);

    /**
     * @brief Overloads the assignment operator for the `WwtpDischargeRule` class, allowing one object to be assigned the attributes of another.
     * 
     * @param wwtp_discharge_rule Reference to the `WwtpDischargeRule` object to be assigned.
     * 
     * @return A reference to the updated `WwtpDischargeRule` object after assignment.
     */
    WwtpDischargeRule &operator=(const WwtpDischargeRule &wwtp_discharge_rule);

    /**
     * @brief Throws an exception because this overload of `get_dependent_variable` is not valid for `WwtpDischargeRule`.
     * This function serves to catch the potential error where the user misses the `week` parameter.
     * 
     * This function overrides the `get_dependent_variable` method in the `ControlRules` class.
     * 
     * @param water_source_id Double placeholder parameter (not used in this implementation).
     * 
     * @throw std::invalid_argument Always throws an exception with a message indicating that this overload is invalid.
     *        The valid method requires a both water source ID (int) and a week number (int) as arguments.
     */
    double get_dependent_variable(double water_source_id) override;

    /**
     * @brief Retrieves the fraction of the week's demand to be discharged as effluent for a specified water source and week.
     * 
     * This function overrides the `get_dependent_variable` method in the `ControlRules` class.
     * 
     * @param water_source_id Integer representing the ID of the water source for which the discharge fraction is requested.
     * @param week Integer representing the week for which the discharge fraction is requested.
     * 
     * @return A double representing the fraction of the week's demand to be discharged as effluent.
     */
    double get_dependent_variable(int water_source_id) override;

    /**
     * @brief Throws an exception because this overload of `get_dependent_variable` is not valid for `WwtpDischargeRule`.
     * This function serves to catch the potential error where the user attempts to call `get_dependent_variable` with a double parameter.
     * 
     * This function overrides the `get_dependent_variable` method in the `ControlRules` class.
     * 
     * @param x Double placeholder parameter (not used in this implementation).
     * @param week Integer representing the week for which the discharge fraction is requested.
     * 
     * @throw std::invalid_argument Always throws an exception with a message indicating that this overload is invalid.
     *        The valid method requires a water source ID (int) and a week number (int) as arguments.
     */
    double get_dependent_variable(double x, int week) override;

    /**
     * @brief Retrieves the fraction of the week's demand to be discharged as effluent for a specified water source and week.
     * 
     * This function overrides the `get_dependent_variable` method in the `ControlRules` class.
     * 
     * @param water_source_id Integer representing the ID of the water source for which the discharge fraction is requested.
     * @param week Integer representing the week for which the discharge fraction is requested.
     * 
     * @return A double representing the fraction of the week's demand to be discharged as effluent.
     */
    double get_dependent_variable(int x, int week) override;

};


#endif //TRIANGLEMODEL_WWTPDISCHARGERULES_H
