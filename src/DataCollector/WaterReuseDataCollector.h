/**
 * @class WaterReuseDataCollector
 * 
 * @brief The `WaterReuseDataCollector` class is a subclass of the `DataCollector` class that implements the data collection of WaterSource objects.
 * It is the main subclass that results in the WaterSource's output CSV files.
 * 
 * Created by bernardoct on 8/26/17.
 */

#ifndef TRIANGLEMODEL_WATERREUSEDATACOLLECTOR_H
#define TRIANGLEMODEL_WATERREUSEDATACOLLECTOR_H

#include "Base/DataCollector.h"
#include "../SystemComponents/WaterSources/WaterReuse.h"

class WaterReuseDataCollector : public DataCollector {
private:
    /**
     * @brief Pointer to the WaterReuse object from which data will be collected.
     */
    WaterReuse *water_reuse;
    /**
     * @brief Volume of water reused.
     */
    double reused_volume;

public:
    /**
     * @brief Constructs a `WaterReuseDataCollector` for managing and collecting data related to water reuse.
     *        Initializes the data collector with water reuse-specific attributes.
     * 
     * @param water_reuse Pointer to the `WaterReuse` object representing the water reuse entity.
     * @param realization Unsigned long integer representing the realization ID.
     */
    WaterReuseDataCollector(WaterReuse *water_reuse, unsigned long realization);

    /**
     * @brief Generates a tabular string representation of the reused water volume for a specified week.
     * 
     * This function overrides the `printTabularString` function in the `DataCollector` class.
     * 
     * @param week Integer representing the week for which the reused volume data is generated.
     * 
     * @return A string containing the tabular representation of the reused water volume for the specified week.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates a compact, comma-delimited string representation of the reused water volume for a specified week.
     * 
     * This function overrides the `printCompactString` function in the `DataCollector` class.
     * 
     * @param week Integer representing the week for which the reused volume data is generated.
     * 
     * @return A string containing the compact, comma-delimited representation of the reused water volume for the specified week.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates the first header line for the tabular representation of water reuse data.
     *        This header provides a title for the reused water volume column.
     * 
     * This function overrides the `printTabularStringHeaderLine1` function in the `DataCollector` class.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates the second header line for the tabular representation of water reuse data.
     *        This header provides a title for the reused water volume column.
     * 
     * This function overrides the `printTabularStringHeaderLine2` function in the `DataCollector` class.
     * 
     * @return A string containing the second header line for the tabular representation.
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates the compact, comma-delimited header line for the tabular representation of water reuse data.
     *        This header provides a title for the reused water volume column.
     * 
     * This function overrides the `printCompactStringHeader` function in the `DataCollector` class.
     * 
     * @return A string containing the header line for the compact, comma-delimited representation.
     */
    string printCompactStringHeader() override;

    void collect_data() override;
};


#endif //TRIANGLEMODEL_WATERREUSEDATACOLLECTOR_H
