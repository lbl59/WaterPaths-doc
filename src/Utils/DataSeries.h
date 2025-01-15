/**
 * @class DataSeries
 * @brief The DataSeries subclass that extends the ControlRules class by providing detailed representation for a series of data points.
 * 
 * Created by bernardoct on 6/21/17.
 */
#ifndef TRIANGLEMODEL_DATASERIES_H
#define TRIANGLEMODEL_DATASERIES_H

#include <vector>
#include <stdexcept>
#include "../Controls/Base/ControlRules.h"

using namespace std;

class DataSeries : public ControlRules {
private:
    /**
     * @brief A vector of independent x-variable values.
     */
    vector<double> *series_x;
    /**
     * @brief A vector of dependent y-variable values.
     */
    vector<double> *series_y;

public:
    /**
     * @brief The length of the DataSeries.
     */
    const unsigned long length;

    /**
     * @brief Constructs a `DataSeries` object by initializing it with x and y data series.
     *        Ensures that the lengths of the independent (`x`) and dependent (`y`) series match.
     *
     * @param series_x Pointer to a `vector<double>` representing the x-values (independent variable).
     * @param series_y Pointer to a `vector<double>` representing the y-values (dependent variable).
     * @throw std::invalid_argument if the lengths of `series_x` and `series_y` are not equal.
     */
    DataSeries(vector<double> *series_x, vector<double> *series_y);

    /**
     * @brief Retrieves a constant reference to the x-values (independent variable) of the data series.
     *
     * @return A constant reference to the `vector<double>` containing the x-values.
     */
    const vector<double> &getSeries_x() const;

    /**
     * @brief Retrieves the dependent variable (`y`) corresponding to a given independent variable (`x`) using interpolation.
     *        If the data series contains only one point, it returns that single point.
     * 
     * This function overrides the `get_dependent_variable` method from the `ControlRules` class.
     *
     * @param x The value of the independent variable (`x`) for which the dependent variable (`y`) is to be calculated.
     * 
     * @return double
     * @return The interpolated or directly matched value of the dependent variable (`y`).
     */
    double get_dependent_variable(double x) override;

    /**
     * @brief Throws an exception if called with an integer argument.
     *        This function prevents misuse by disallowing calls with an integer `x` for `get_dependent_variable`.
     * 
     * This function overrides the `get_dependent_variable` method from the `ControlRules` class.
     *
     * @param x An integer value that is not supported as input for this function.
     * 
     * @return double
     * @throw std::invalid_argument Always thrown, with a message indicating that integers are not allowed.
     */
    double get_dependent_variable(int x) override;

private:
    /**
     * @brief Throws an exception if called with an double `x` argument and integer `week` argument.
     *        This function prevents misuse by disallowing calls with an double `x` or integer `week`.
     * 
     * This function overrides the `get_dependent_variable` method from the `ControlRules` class.
     *
     * @param x A double value that is not supported as input for this function.
     * @param week An integer value that is not supported as input for this function.
     * 
     * @return double
     * @throw std::invalid_argument Always thrown, with a message indicating that two parameters are not allowed.
     */
    double get_dependent_variable(double x, int week) override;

    /**
     * @brief Throws an exception if called with an integer `x` argument and integer `week` argument.
     *        This function prevents misuse by disallowing calls with an integer `x` or integer `week`.
     * 
     * This function overrides the `get_dependent_variable` method from the `ControlRules` class.
     *
     * @param x An integer value that is not supported as input for this function.
     * @param week An integer value that is not supported as input for this function.
     * 
     * @return double
     * @throw std::invalid_argument Always thrown, with a message indicating that two parameters are not allowed.
     */
    double get_dependent_variable(int x, int week) override;

};


#endif //TRIANGLEMODEL_DATASERIES_H
