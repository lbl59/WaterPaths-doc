/**
 * @class IntakeDataCollector
 * @brief The `IntakeDataCollector` class is a subclass of the `DataCollector` class that implements the data collection of intake objects.
 * 
 * Created by bernardoct on 8/26/17.
 */
#ifndef TRIANGLEMODEL_INTAKEDATACOLLECTOR_H
#define TRIANGLEMODEL_INTAKEDATACOLLECTOR_H


#include "../SystemComponents/WaterSources/Intake.h"
#include "Base/DataCollector.h"

class IntakeDataCollector : public DataCollector {
    /**
     * @brief The intake object associated with this DataCollector
     */
    Intake *intake;
    /**
     * @brief A vector with the total inflows from all upstream sources.
     */
    vector<double> total_upstream_sources_inflows;
    /**
     * @brief A vector with the total demands from utilities.
     */
    vector<double> demands;
    /**
     * @brief A vector with the total inflows from wastewater treatment plants.
     */
    vector<double> wastewater_inflows;
    /**
     * @brief A vector with the total outflows from the intake.
     */
    vector<double> outflows;
    /**
     * @brief A vector with the total inflows from all upstream catchments.
     */
    vector<double> total_catchments_inflow;

public:
    /**
     * @brief Constructs an IntakeDataCollector for managing and collecting data from an Intake.
     *        This function initializes the data collector with specific attributes for intakes.
     * 
     * @param intake Pointer to the Intake object that this collector will manage.
     * @param realization Unsigned long integer representing the realization (scenario or simulation ID).
     */
    IntakeDataCollector(Intake *intake, unsigned long realization);

    /**
     * @brief Generates a tabular string representation of intake data for a given week.
     *        This includes demand, upstream inflows, wastewater inflows, catchment inflows, and outflows.
     *        Each of these values is separated by a column with a fixed width.
     * 
     * This function overrides the virtual function in the DataCollector class.
     * 
     * @param week Integer representing the week for which the tabular data is being generated.
     * 
     * @return A string containing the tabular representation of the intake data for the specified week.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates a compact string representation of intake data for a given week.
     *        This includes demand, upstream inflows, wastewater inflows, catchment inflows, and outflows in a compact format.
     *        Each of these values is separated by a column.
     * 
     * This function overrides the virtual function in the DataCollector class.
     * 
     * @param week Integer representing the week for which the compact data is being generated.
     * 
     * @return A string containing the compact representation of the intake data for the specified week.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates the first header line for a tabular representation of intake data.
     *        This line includes column headers for upstream, wastewater, and catchment inflows, separated into
     *        columns with a fixed width.
     * 
     * This function overrides the virtual function in the DataCollector class.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates the second header line for a tabular representation of intake data.
     *        This line includes detailed column headers for demands, spillage, inflows, and outflows,
     *        separated into columns with a fixed width.
     * 
     * This function overrides the virtual function in the DataCollector class.
     * 
     * @return A string containing the second header line for the tabular representation.
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates a compact string header for the intake data.
     *        This header includes column identifiers for demand, upstream spillage, wastewater inflow, 
     *        catchment inflow, and downstream spillage, in a comma-delimited format.
     * 
     * This function overrides the virtual function in the DataCollector class.
     * 
     * @return A string containing the compact header representation for the intake data.
     */
    string printCompactStringHeader() override;

    /**
     * @brief Collects data for the intake, including demand, wasterwater and catchment inflows, as well as
     *        total outflows. This function stores the intake's weekly data into corresponding data structures.
     * 
     * This function overrides the virtual function in the DataCollector class.
     * 
     * @return void
     */
    void collect_data() override;
};


#endif //TRIANGLEMODEL_INTAKEDATACOLLECTOR_H
