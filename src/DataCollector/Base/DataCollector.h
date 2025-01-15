/**
 * @class DataCollector
 * @brief The `DataCollector` class is an abstract class that defines the interface for collecting data in the Triangle Model.
 * Created by bernardoct on 8/25/17.
 */

#ifndef TRIANGLEMODEL_DATACOLLECTOR_H
#define TRIANGLEMODEL_DATACOLLECTOR_H

#include <string>

using namespace std;

class DataCollector {
public:
    /**
     * @brief The unique ID of the data collector.
     */
    const int id;
    /**
     * @brief The type of data collector.
     */
    const int type;
    /**
     * @brief The width of the table to print the data.
     */
    const int table_width;
    /**
     * @brief The name of the data collector.
     */
    const char *name;
    /**
     * @brief The realization number associated with the data collector.
     */
    unsigned long realization;

    /**
     * @brief Constructor for the DataCollector class. This function initializes a DataCollector object with the specified attributes.
     * 
     * This constructor is marked as explicit to avoid implicit conversions from integer to DataCollector.
     * 
     * @param id The unique identifier for the DataCollector object.
     * @param name The name of the DataCollector object as a constant character pointer.
     * @param realization The realization number associated with the DataCollector.
     * @param type The type of the DataCollector object.
     * @param table_width The width of the data table.
     */
    explicit DataCollector(int id, const char *name, unsigned long realization, int type, int table_width);

    /**
     * @brief Destructor for the DataCollector class. 
     * 
     * This function cleans up resources used by the DataCollector object.
     */
    virtual ~DataCollector();

    /**
     * @brief Prints the data collected by the DataCollector object in a tabular format.
     * 
     * This function does not have an implementation in the DataCollector class and must be implemented by derived classes.
     * 
     * @param week The week number to print the data for.
     * @return A string containing the data collected by the DataCollector object in a tabular format.
     */
    virtual string printTabularString(int week) = 0;

    /**
     * @brief Prints the data collected by the DataCollector object in a compact format.
     * 
     * This function does not have an implementation in the DataCollector class and must be implemented by derived classes.
     * 
     * @param week The week number to print the data for.
     * @return A string containing the data collected by the DataCollector object in a compact format.
     */
    virtual string printCompactString(int week) = 0;

    /**
     * @brief Prints the header for the tabular data collected by the DataCollector object.
     * 
     * This function does not have an implementation in the DataCollector class and must be implemented by derived classes.
     * 
     * @return A string containing the header for the tabular data collected by the DataCollector object.
     */
    virtual string printTabularStringHeaderLine1() = 0;

    /**
     * @brief Prints the second line of the header for the tabular data collected by the DataCollector object.
     * 
     * This function does not have an implementation in the DataCollector class and must be implemented by derived classes.
     * 
     * @return A string containing the second line of the header for the tabular data collected by the DataCollector object.
     */
    virtual string printTabularStringHeaderLine2() = 0;

    /**
     * @brief Prints the header for the compact data collected by the DataCollector object.
     * 
     * This function does not have an implementation in the DataCollector class and must be implemented by derived classes.
     * 
     * @return A string containing the header for the compact data collected by the DataCollector object.
     */
    virtual string printCompactStringHeader() = 0;

    /**
     * @brief Collects the data for the DataCollector object.
     * 
     * This function does not have an implementation in the DataCollector class and must be implemented by derived classes.
     * 
     * @return void
     */
    virtual void collect_data() = 0;
};


#endif //TRIANGLEMODEL_DATACOLLECTOR_H
