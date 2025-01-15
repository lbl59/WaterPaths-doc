/**
 * @class AllocatedIntakeDataCollector
 * @brief The `AllocatedIntakeDataCollector` class is a subclass of the `DataCollector` class that implements the data collection of allocated intake objects.
 * 
 * Created by dgorelic on 3/2/2020.
 */

#ifndef TRIANGLEMODEL_ALLOCATEDINTAKEDATACOLLECTOR_H
#define TRIANGLEMODEL_ALLOCATEDINTAKEDATACOLLECTOR_H

#include "Base/DataCollector.h"
#include "../SystemComponents/WaterSources/AllocatedIntake.h"

class AllocatedIntakeDataCollector : public DataCollector {
    /**
     * @brief The AllocatedIntake object from which data will be collected.
     */
    AllocatedIntake *intake;
    /**
     * @brief The timeseries of total inflow from upstream sources.
     */
    vector<double> total_upstream_sources_inflows;
    /**
     * @brief The timeseries of demands.
     */
    vector<double> demands;
    /**
     * @brief The timeseries of inflow from wastewater sources.
     */
    vector<double> wastewater_inflows;
    /**
     * @brief The timeseries of total outflows from each intake.
     */
    vector<double> outflows;
    /**
     * @brief The timeseries of total inflow from catchments.
     */
    vector<double> total_catchments_inflow;
    /**
     * @brief A 2D vector of across demands from all sources allocated to each utility.
     */
    vector<vector<double>> allocated_demands;
    /**
     * @brief A 2D vector of supply capacities across all sources allocated to each utility.
     */
    vector<vector<double>> allocated_supply_capacity;
    /**
     * @brief A IDs of utilities with allocations to the intake.
     */
    vector<int> utilities_with_allocations;

public:
    /**
     * @brief Constructor for the AllocatedIntakeDataCollector class. This function initializes an AllocatedIntakeDataCollector object 
     * with the specified intake and realization.
     * 
     * This constructor initializes the AllocatedIntakeDataCollector object by calling the base DataCollector constructor with specific 
     * parameters derived from the intake. It also initializes the `utilities_with_allocations` member using data from the `AllocatedIntake` object.
     * 
     * @param intake A pointer to the AllocatedIntake object associated with this data collector.
     * @param realization The realization number associated with the data collection.
     * 
     * @see DataCollector
     * 
     */
    AllocatedIntakeDataCollector(AllocatedIntake *intake, unsigned long realization);

    /**
     * @brief Generates a tabular string representation of data for a specific week.
     * 
     * This function creates a formatted string that represents the weekly data for demands, inflows, outflows, and allocations. 
     * The data is formatted into a tabular structure with fixed column widths and precision. It includes:
     * - Demand for the week.
     * - Total upstream source inflows.
     * - Wastewater inflows.
     * - Total inflows from catchments.
     * - Outflows.
     * - Allocated supply capacities and demands for utilities with allocations.
     * 
     * This function overrides the base class `printTabularString` function.
     * 
     * @param week The week number for which the data should be printed.
     * @return A string containing the tabular data for the specified week.
     * 
     * @throws std::out_of_range If the `week` parameter is out of bounds for any of the accessed data arrays.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates a compact, comma-separated string representation of data for a specific week.
     * 
     * This function creates a compact string that represents the weekly data for demands, inflows, outflows, and allocations. 
     * The data is structured as a comma-separated list and includes:
     * - Demand for the week.
     * - Total upstream source inflows.
     * - Wastewater inflows.
     * - Total inflows from catchments.
     * - Outflows.
     * - Allocated supply capacities and demands for utilities with allocations.
     * 
     * This function overrides the base class `printCompactString` function.
     * 
     * @param week The week number for which the data should be printed.
     * @return A string containing the compact data for the specified week, with values separated by commas.
     * 
     * @throws std::out_of_range If the `week` parameter is out of bounds for any of the accessed data arrays.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates the first header line for a tabular string representation.
     * 
     * This function creates a formatted header line for tabular data representation. The header includes:
     * - Blank space for alignment.
     * - Column headers for upstream inflows, wastewater inflows, and catchment inflows.
     * - Identifiers for utilities with allocations, listed twice for supply capacities and demands.
     * 
     * This function overrides the base class `printTabularStringHeaderLine1` function.
     * 
     * @return A string containing the first header line of the tabular data.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates the second header line for a tabular string representation.
     * 
     * This function creates a formatted second header line for tabular data representation. The header includes:
     * - Column labels for demands, spillage, inflows (upstream, wastewater, and catchment), and outflows.
     * - Labels for allocated supply capacities and demands for utilities with allocations.
     * 
     * This function overrides the base class `printTabularStringHeaderLine2` function.
     * 
     * @return A string containing the second header line of the tabular data.
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates a compact, comma-separated header string for the compact data representation.
     * 
     * This function creates a compact header string for the data, where each column is prefixed by the `id` of the AllocatedIntakeDataCollector object. 
     * The header includes:
     * - Demand.
     * - Upstream spillage.
     * - Wastewater inflow.
     * - Catchment inflow.
     * - Downstream spillage.
     * - Allocated supply capacities and demands for utilities with allocations, each uniquely identified with the utility ID.
     * 
     * This function overrides the base class `printCompactStringHeader` function.
     * 
     * @return A string containing the header for the compact data, with column identifiers separated by commas.
     */
    string printCompactStringHeader() override;

    /**
     * @brief Collects and stores data from the associated AllocatedIntake object.
     * 
     * This function retrieves data from the AllocatedIntake object and appends it to the corresponding member variables. 
     * The collected data includes:
     * - Current demand.
     * - Total inflow from upstream sources.
     * - Inflow from wastewater sources.
     * - Total inflow from upstream catchments.
     * - Total outflow.
     * - Allocated supply capacities for utilities with allocations.
     * - Allocated demands for utilities with allocations.
     * 
     * This function overrides the base class `collect_data` function.
     * 
     * @return void
     */
    void collect_data() override;
};


#endif //TRIANGLEMODEL_ALLOCATEDINTAKEDATACOLLECTOR_H
