/**
 * @class EvaporationSeries
 * @brief This class is a subclass of the main `Catchment` class and represents a series of evaporation data over time.
 * 
 * Created by bernardoct on 6/22/17.
 */

#ifndef TRIANGLEMODEL_EVAPORATIONSERIES_H
#define TRIANGLEMODEL_EVAPORATIONSERIES_H

#include <vector>
#include "../SystemComponents/Catchment.h"

class EvaporationSeries : public Catchment {
public:
    /**
     * @brief Constructs an `EvaporationSeries` object for managing and storing evaporation data over time.
     *        Initializes the series with a pointer to evaporation data and its length.
     * 
     * @param evaporation_series Pointer to a vector of vectors containing the evaporation data series.
     * @param series_length Integer representing the length of the evaporation data series.
     */
    EvaporationSeries(
            vector<vector<double>> *evaporation_series, int series_length);

    /**
     * @brief Copy constructor for the `EvaporationSeries` class.
     *        Creates a new `EvaporationSeries` object by copying an existing one.
     * 
     * @param evaporation_series Reference to the `EvaporationSeries` object to be copied.
     */
    EvaporationSeries(const EvaporationSeries &evaporation_series);

    /**
     * @brief Overloads the assignment operator for the `EvaporationSeries` class, allowing one object to be assigned the attributes of another.
     * 
     * @param evaporation_series Reference to the `EvaporationSeries` object to be assigned.
     * 
     * @return A reference to the updated `EvaporationSeries` object after assignment.
     */
    EvaporationSeries &operator=(const EvaporationSeries &evaporation_series);

    /**
     * @brief Destructor for the `EvaporationSeries` class.
     *        Cleans up resources associated with the object. Since no dynamic allocation is explicitly used, the destructor is empty.
     * 
     * @return void
     */
    ~EvaporationSeries() override;

    /**
     * @brief Retrieves the evaporation value for a specified week.
     *        This is a wrapper for the base class `Catchment::getStreamflow` function.
     * 
     * @param week Integer representing the week for which the evaporation value is requested.
     * 
     * @return A double representing the evaporation value for the specified week.
     */
    double getEvaporation(int week);

    /**
     * @brief Sets the realization for the `EvaporationSeries` object and 
     *      scales the evaporation values based on its corresponding DU factors.
     * 
     * @param r Unsigned long integer representing the realization ID.
     * @param rdm_factors Reference to a vector of doubles containing Random Decision-Making (RDM) factors to modify evaporation values.
     * 
     * @return void
     */
    void setRealization(unsigned long r, vector<double> &rdm_factors) override;
};


#endif //TRIANGLEMODEL_EVAPORATIONSERIES_H
