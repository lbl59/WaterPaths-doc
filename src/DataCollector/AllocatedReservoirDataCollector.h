/**
 * @class AllocatedReservoirDataCollector
 * @brief The `AllocatedReservoirDataCollector` class is a subclass of the `ReservoirDataCollector` class, which in turn is a subclass of the main `DataCollector` class.
 * It implements the data collection of allocated reservoir objects.
 * 
 * Created by Created by bernardoct on 8/26/17.
 */

#ifndef TRIANGLEMODEL_ALLOCATEDRESERVOIRDATACOLLECTOR_H
#define TRIANGLEMODEL_ALLOCATEDRESERVOIRDATACOLLECTOR_H


#include "ReservoirDataCollector.h"
#include "../SystemComponents/WaterSources/AllocatedReservoir.h"

class AllocatedReservoirDataCollector : public ReservoirDataCollector {
    /**
     * @brief The AllocatedReservoir object from which data will be collected. 
     */
    AllocatedReservoir *allocated_reservoir;
    /**
     * @brief A 2D vector containing each utility's allocation of the stored volume for each utility in each week.
     */
    vector<vector<double>> allocated_stored_volumes;
    /**
     * @brief A vector containing the IDs of the utilities with allocations.
     */
    vector<int> utilities_with_allocations;

public:
    /**
     * @brief Constructs an AllocatedReservoirDataCollector for managing and collecting data from an AllocatedReservoir.
     *        This function initializes the data collector with specific attributes for allocated reservoirs.
     * 
     * @param allocated_reservoir Pointer to the AllocatedReservoir object that this collector will manage.
     * @param realization Unsigned long integer representing the realization (scenario or simulation ID).
     */
    AllocatedReservoirDataCollector(AllocatedReservoir *allocated_reservoir,
                                        unsigned long realization);

    /**
     * @brief Generates a tabular string representation of the allocated reservoir's data for a given week.
     *        This function extends the tabular output by including data for utilities with allocations.
     * 
     * This function overrides the virtual function `printTabularString` from the `ReservoirDataCollector` class.
     * 
     * @param week Integer representing the week for which the tabular data is being generated.
     * 
     * @return A string containing the tabular representation of the reservoir data for the specified week.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates a compact, comma delimited string representation of the allocated reservoir's data for a given week.
     *        This function extends the compact output by appending data for utilities with allocations.
     * 
     * This function overrides the virtual function `printCompactString` from the `ReservoirDataCollector` class.
     * 
     * @param week Integer representing the week for which the compact data is being generated.
     * 
     * @return A string containing the compact representation of the reservoir data for the specified week.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates the first header line for a tabular representation of allocated reservoir data.
     *        This includes the base header line and additional columns for the stored volume of 
     *        utilities with allocations.
     * 
     * This function overrides the virtual function `printTabularStringHeaderLine1` from the `ReservoirDataCollector` class.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates the second header line for a tabular representation of allocated reservoir data.
     *        This includes the base header line and additional columns for allocation percentage for each 
     *        utility.
     * 
     * This function overrides the virtual function `printTabularStringHeaderLine2` from the `ReservoirDataCollector` class.
     * 
     * @return A string containing the second header line for the tabular representation.
     * 
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates a compact string header for the allocated reservoir's data.
     *        This includes the base header and additional columns for utility allocations.
     * 
     * This function overrides the virtual function `printCompactStringHeader` from the `ReservoirDataCollector` class.
     * 
     * @return A string containing the compact header representation for the reservoir data.
     * 
     * @see ReservoirDataCollector::printCompactStringHeader
     */
    string printCompactStringHeader() override;

    /**
     * @brief Collects data for the allocated reservoir, including its allocated stored volumes.
     *        This function extends the base class data collection by storing allocated volume data.
     * 
     * This function overrides the virtual function `collect_data` from the `DataCollector` class.
     * 
     * @return void
     */
    void collect_data() override;
};


#endif //TRIANGLEMODEL_ALLOCATEDRESERVOIRDATACOLLECTOR_H
