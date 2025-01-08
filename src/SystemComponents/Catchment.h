//
// Created by bernardo on 1/13/17.
//

#ifndef TRIANGLEMODEL_CATCHMENT_H
#define TRIANGLEMODEL_CATCHMENT_H

#include <vector>
#include <stdexcept>
#include "../Utils/Constants.h"

using namespace std;
using namespace Constants;

class Catchment {
protected:
    /**
     * @brief A pointer to a 2D vector containing all combined historical and synthetic streamflow realizations.
     */
    vector<vector<double>> *streamflows_all;

    /**
     * @brief A 1D vector containing the streamflows for a specific realization for the catchment.
     */
    vector<double> streamflows_realization;

    /**
     * @brief An integer representing the length of the streamflow time series. It should be longer than
     *                   WEEKS_IN_YEAR * NUMBER_REALIZATIONS_ROF.
     */
    int series_length;

    /**
     * @brief A boolean indicating whether this catchment is a parent catchment or not. Parent catchments are
     *  catchments (INSERT DESCRIPTION HERE).
     * 
     */
    bool parent = true;

    /**
     * @brief An integer indicating the number of weeks to skip in the beginning of the time series to identify 
     *  the correct week for the start of a given realization. 
     *  This is the number of weeks of historical data used to calculate the ROFs.
     *  Should take on the value of WEEKS_IN_YEAR * NUMBER_REALIZATIONS_ROF.
     * 
     * 
     */
    // Number of historical years of data - used to set week delta_week to week 0.
    int delta_week = (int) std::round(
            Constants::WEEKS_IN_YEAR * Constants::NUMBER_REALIZATIONS_ROF);
 

public:

    /**
     * @brief Constructor for a new Catchment object
     * 
     * @param streamflows_all A 2D vector containing all combined historical and synthetic streamflow realizations.
     * @param series_length An integer representing the length of the streamflow time series.
     */
    Catchment(vector<vector<double>> *streamflows_all, int series_length);

    /**
     * @brief Copy constructor for a Catchment object. This constructor does not copy the parent pointer.
     *  Set parent to false to distinguish between a parent Catchment and those created as copies.
     *  Initializes a new instance of Catchment with data copied from the original Catchment object.
     * 
     * @param catchment A Catchment object.
     */
    Catchment(const Catchment &catchment);

    /**
     * @brief Destructor for a Catchment object.
     */
    virtual ~Catchment();

    /**
     * @brief Copy assignment operator.
     * 
     * @param catchment
     */
    Catchment &operator=(const Catchment &catchment);

    /**
     * @brief Gets streamflow for a given week AFTER the first delta_week weeks used to calculate the ROFs. 
     *  This function assures that the number of past inflows used for ROF calculations are reserved in the beginning of 
     *  the time series for ROF calculations.
     * 
     * @param week An integer representing the week of the entire streamflow time series.
     * @return A double representing the streamflow for the given week.
     */
    double getStreamflow(int week);

    /**
     * @brief Sets the 1D vector time series corresponding to realization index and eliminate reference to
     *  the full 2D comprehensive streamflow data set.
     * 
     * @param r An unsigned long integer representing the realization index.
     * @param rdm_factors A reference to a vector of doubles representing the random factors used to 
     *  generate the streamflow realization (UNUSED).
     */
    virtual void setRealization(unsigned long r, vector<double> &rdm_factors);
};


#endif //TRIANGLEMODEL_CATCHMENT_H
