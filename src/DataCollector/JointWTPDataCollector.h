/**
 * @class JointWTPDataCollector
 * @brief The `JointWTPDataCollector` class is a subclass of the `DataCollector` class that implements the data collection of joint WTP objects.
 * 
 * Created by dgorelic on 11/4/2019.
 */

#ifndef TRIANGLEMODEL_JOINTWTPDATACOLLECTOR_H
#define TRIANGLEMODEL_JOINTWTPDATACOLLECTOR_H


#include "Base/DataCollector.h"
#include "../SystemComponents/WaterSources/JointWTP.h"

class JointWTPDataCollector : public DataCollector {
    /**
     * @brief The JointWTP object that this data collector is collecting data for.
     */
    JointWTP *joint_wtp;
    /**
     * @brief The IDs of the utilities that have allocations at the JointWTP.
     */
    vector<int> utilities_with_allocations;
    /**
     * @brief The allocated treatment capacities for each utility at the JointWTP.
     */
    vector<vector<double>> partner_allocated_treatment_capacities;

public:
    /**
     * @brief Constructs a JointWTPDataCollector for managing and collecting data from a Joint Water Treatment Plant (WTP).
     *        This function initializes the data collector with specific attributes for a Joint WTP.
     * 
     * @param joint_wtp Pointer to the JointWTP object that this collector will manage.
     * @param realization Unsigned long integer representing the realization (scenario or simulation ID).
     */
    explicit JointWTPDataCollector(JointWTP *joint_wtp, unsigned long realization);

    /**
     * @brief Generates a tabular string representation of the Joint WTP data for a given week.
     *        This includes allocated treatment capacities for each utility partner with a treatment allocation
     *        to the Joint WTP, formatted in columns.
     * 
     * This function overrides the DataCollector::printTabularString() function.
     * 
     * @param week Integer representing the week for which the tabular data is being generated.
     * 
     * @return A string containing the tabular representation of the allocated treatment capacities for the specified week.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates a comma, delimited compact string representation of the Joint WTP data for a given week.
     *        This includes allocated treatment capacities for each utility partner with a treatment allocation
     *        to the Joint WTP.
     * 
     * This function overrides the DataCollector::printCompactString() function.
     * 
     * @param week Integer representing the week for which the compact data is being generated.
     * 
     * @return A string containing the compact representation of the allocated treatment capacities for the specified week.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates the first header line for a tabular representation of Joint WTP data.
     *        This line includes column headers for treatment capacities of utility partners.
     * 
     * This function overrides the DataCollector::printTabularStringHeaderLine1() function.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates the second header line for a tabular representation of Joint WTP data.
     *        This line includes column headers for treatment allocations of utility partners.
     * 
     * This function overrides the DataCollector::printTabularStringHeaderLine2() function.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates a compact string header, in a comma-delimited format, for the Joint WTP data.
     *        This header includes column identifiers for allocated treatment capacities of utility partners.
     * 
     * This function overrides the DataCollector::printCompactStringHeader() function.
     * 
     * @return A string containing the compact header representation for the Joint WTP data.
     */
    string printCompactStringHeader() override;

    /**
     * @brief Collects data for the Joint WTP, specifically the allocated treatment capacities for utility partners.
     *        This function stores the treatment capacity data for the current week.
     * 
     * This function overrides the DataCollector::collect_data() function.
     * 
     * @return void
     */
    void collect_data() override;
};


#endif //TRIANGLEMODEL_JOINTWTPDATACOLLECTOR_H
