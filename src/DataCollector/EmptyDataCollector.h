/**
 * @class EmptyDataCollector
 * @brief The `EmptyDataCollector` class is a subclass of the `EmptyDataCollector` class that implements default data collection behavior.
 * 
 * Created by bernardoct on 8/26/17.
 */

#ifndef TRIANGLEMODEL_EMPTYDATACOLLECTOR_H
#define TRIANGLEMODEL_EMPTYDATACOLLECTOR_H


#include "Base/DataCollector.h"

class EmptyDataCollector : public DataCollector {
public:
    /**
     * @brief Constructs an EmptyDataCollector with default values for uninitialized or placeholder data collection.
     *        This is a default constructor initializing the base class with non-initialized values.
     */
    EmptyDataCollector();

    /**
     * @brief Generates an empty tabular string representation for a given week.
     *        This function is a placeholder implementation for data collectors that do not manage data.
     * 
     * This function is overrides the pure virtual function in the base class.
     * 
     * @param week Integer representing the week for which the tabular data is being generated.
     * 
     * @return An empty string as no data is managed by this collector.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates an empty comma-delimited string representation for a given week.
     *        This function is a placeholder implementation for data collectors that do not manage data.
     * 
     * This function is overrides the pure virtual function in the base class.
     * 
     * @param week Integer representing the week for which the tabular data is being generated.
     * 
     * @return An empty string as no data is managed by this collector.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates an empty string representation for the first header line.
     *        This function is a placeholder implementation for data collectors that do not manage data.
     * 
     * This function is overrides the pure virtual function in the base class.
     * 
     * @return An empty string as no data is managed by this collector.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates an empty string representation for the first header line.
     *        This function is a placeholder implementation for data collectors that do not manage data.
     * 
     * This function is overrides the pure virtual function in the base class.
     * 
     * @return An empty string as no data is managed by this collector.
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates an empty string representation for the first header line.
     *        This function is a placeholder implementation for data collectors that do not manage data.
     * 
     * This function is overrides the pure virtual function in the base class.
     * 
     * @return An empty string as no data is managed by this collector.
     */
    string printCompactStringHeader() override;

    /**
     * @brief A placeholder implementation for data collection.
     *        This function does nothing, as the EmptyDataCollector does not manage or collect any data.
     * 
     * @return void
     */
    void collect_data() override;
};


#endif //TRIANGLEMODEL_EMPTYDATACOLLECTOR_H
