/**
 * @class ControlRules
 * 
 * @brief This class is a header-only abstract class that defines the interface for control rules.
 * 
 * Created by bernardoct on 6/26/17.
 * 
 */

#ifndef TRIANGLEMODEL_CONTROLRULES_H
#define TRIANGLEMODEL_CONTROLRULES_H

class ControlRules {
    /**
     * Returns the double dependent variable for a given independent variable.
     * 
     * @param x independent variable.
     * @return dependent variable.
     */
    virtual double get_dependent_variable(double x) = 0;

    /**
     * Returns the integer dependent variable for a given independent variable.
     * 
     * @param x independent variable.
     * @param week week of the year.
     * @return dependent variable.
     */
    virtual double get_dependent_variable(int x) = 0;

    /**
     * Returns the double dependent variable for a given independent variable at a specific week.
     * 
     * @param x independent variable.
     * @param week week of the year.
     * @return dependent variable.
     */
    virtual double get_dependent_variable(double x, int week) = 0;

    /**
     * Returns the integer dependent variable for a given independent variable at a specific week.
     * 
     * @param x independent variable.
     * @param week week of the year.
     * @return dependent variable.
     */
    virtual double get_dependent_variable(int x, int week) = 0;
};


#endif //TRIANGLEMODEL_CONTROLRULES_H
