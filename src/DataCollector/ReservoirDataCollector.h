/**
 * @class ReservoirDataCollector
 * @brief The `ReservoirDataCollector` class is a subclass of the `DataCollector` class that implements the data collection of Reservoir objects.
 * 
 * Created by bernardoct on 8/25/17.
 */

#ifndef TRIANGLEMODEL_RESERVOIRDATACOLLECTOR_H
#define TRIANGLEMODEL_RESERVOIRDATACOLLECTOR_H

#include <vector>
#include "Base/DataCollector.h"
#include "../SystemComponents/WaterSources/Reservoir.h"

class ReservoirDataCollector : public DataCollector {

    /**
     * @brief The reservoir object that this data collector is collecting data for.
     */
    Reservoir *reservoir;   
    /**
     * @brief The total stored volume in the reservoir across all weeks.
     */
    vector<double> stored_volume;
    /**
     * @brief The total upstream inflows to the reservoir across all weeks.
     */
    vector<double> total_upstream_sources_inflows;
    /**
     * @brief The total waterwater inflows to the reservoir across all weeks.
     */
    vector<double> wastewater_inflows;
    /**
     * @brief The total demands from the reservoir across all weeks.
     */
    vector<double> demands;
    /**
     * @brief The total outflow, including spill, from the reservoir across all weeks.
     */
    vector<double> outflows;
    /**
     * @brief The total inflow from catchments to the reservoir across all weeks.
     */
    vector<double> total_catchments_inflow;
    /**
     * @brief The total evaporation from the reservoir across all weeks.
     */
    vector<double> evaporated_volume;
    /**
     * @brief The total surface area of the reservoir across all weeks.
     */
    vector<double> area;
    /**
     * @brief A boolean flag indicating if the reservoir has a fixed area.
     */
    bool fixed_area;
    /**
     * @brief The fixed surface area of the reservoir.
     */
    double fixed_area_value;

public:
     /**
     * @brief Constructs a ReservoirDataCollector for managing and collecting data from a Reservoir.
     *        This function initializes the data collector with specific attributes for a Reservoir.
     * 
     * @param reservoir Pointer to the Reservoir object that this collector will manage.
     * @param realization Unsigned long integer representing the realization (scenario or simulation ID).
     */
    explicit ReservoirDataCollector(Reservoir *reservoir, unsigned long realization);

    /**
     * @brief Constructs a ReservoirDataCollector instance.
     *
     * @details
     * This constructor initializes the ReservoirDataCollector object by associating it with a specified
     * reservoir and configuring its data collection parameters such as type, table width, and realization.
     *
     * @param reservoir Pointer to the Reservoir object whose data is being collected.
     * @param type Integer representing the type of data collection.
     * @param table_width Integer specifying the width of the data table.
     * @param realization Unsigned long integer representing the realization or simulation instance.
     */
    explicit ReservoirDataCollector(Reservoir *reservoir, int type, int table_width,
                                        unsigned long realization);

    /**
     * @brief Generates a tabular string representation of reservoir data for a given week.
     *        This function formats and returns reservoir data values as a tab-separated string.
     *
     * @param week The week index for which the data is to be printed.
     * @return A string containing the formatted reservoir data for the specified week.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates a compact, comma-delimited string representation of reservoir data for a specified week.
     * 
     * This function overrides the `printCompactString` function in the `DataCollector` class.
     * 
     * @param week Integer representing the week for which the compact data is generated.
     * 
     * @return A string containing the compact representation of the demand offsets for the specified week.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates the first header line for the tabular representation of reservoir data.
     * 
     * This function overrides the `printTabularStringHeaderLine1` function in the `DataCollector` class.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates the second header line for the tabular representation of reservoir data.
     * 
     * This function overrides the `printTabularStringHeaderLine2` function in the `DataCollector` class.
     * 
     * @return A string containing the second header line for the tabular representation.
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates a compact, comma-delimited string header for the reservoir data.
     * 
     * This function overrides the `printCompactStringHeader` function in the `DataCollector` class.
     * 
     * @return A string containing the compact header representation for the reservoir data.
     */
    string printCompactStringHeader() override;

    /**
     * @brief Collects the reservoir data for the current week.
     * 
     * This function overrides the `collect_data` function in the `DataCollector` class.
     * 
     * @return void
     */
    void collect_data() override;
};


#endif //TRIANGLEMODEL_RESERVOIRDATACOLLECTOR_H
