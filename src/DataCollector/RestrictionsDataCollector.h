/**
 * @class RestrictionsDataCollector
 * @brief The `RestrictionsDataCollector` class is a subclass of the `DataCollector` class that implements the data collection of water use restrictions.
 * 
 * Created by bernardoct on 8/25/17.
 */

#ifndef TRIANGLEMODEL_DROUGHTMITIGATIONPOLICIESDATACOLLECTOR_H
#define TRIANGLEMODEL_DROUGHTMITIGATIONPOLICIESDATACOLLECTOR_H

#include <vector>
#include "Base/DataCollector.h"
#include "../DroughtMitigationInstruments/Restrictions.h"

class RestrictionsDataCollector : public DataCollector {
private:
    /**
     * @brief A pointer to the restrictions policy associated with the data collector.
     */
    Restrictions *restriction_policy;
    /**
     * @brief The DU factor multipliers applied to scale the effectiveness of water use restrictions for each week.
     */
    vector<double> restriction_multipliers;

public:
    
    /**
     * @brief Constructs a `RestrictionsDataCollector` object to collect data related to a specific restriction policy.
     *        Initializes the base `DataCollector` with relevant identifiers and type information.
     *
     * @param restriction_policy A pointer to the `Restrictions` object representing the restriction policy.
     * @param realization The realization index associated with the restriction policy.
     */
    explicit RestrictionsDataCollector(Restrictions *restriction_policy, unsigned long realization);

    /**
     * @brief Generates a tabular string representation of water restriction multipliers.
     * 
     * This function overrides the `printTabularString` function in the `DataCollector` class.
     * 
     * @param week Integer representing the week for which the tabular data is generated.
     * 
     * @return A string containing the tabular representation of the demand offsets for the specified week.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates a compact, comma-delimited string representation of restriction multipliers for a specified week.
     * 
     * This function overrides the `printCompactString` function in the `DataCollector` class.
     * 
     * @param week Integer representing the week for which the compact data is generated.
     * 
     * @return A string containing the compact representation of the demand offsets for the specified week.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates the first header line for the tabular representation of restriction multipliers.
     * 
     * This function overrides the `printTabularStringHeaderLine1` function in the `DataCollector` class.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates the second header line for the tabular representation of restriction multipliers.
     * 
     * This function overrides the `printTabularStringHeaderLine2` function in the `DataCollector` class.
     * 
     * @return A string containing the second header line for the tabular representation.
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates a compact, comma-delimited string header for the water restriction multipliers.
     * 
     * This function overrides the `printCompactStringHeader` function in the `DataCollector` class.
     * 
     * @return A string containing the compact header representation for the water restriction multipliers.
     */
    string printCompactStringHeader() override;

    /**
     * @brief Collects the current restriction multiplier from the associated restriction policy.
     * 
     * This function overrides the `collect_data` function in the `DataCollector` class.
     * 
     * @return void
     */
    void collect_data() override;

    /**
     * @brief Retrieves the vector of collected restriction multipliers.
     *
     * @return A constant reference to the `vector<double>` containing the collected restriction multipliers.
     */
    const vector<double> &getRestriction_multipliers() const;
};


#endif //TRIANGLEMODEL_DROUGHTMITIGATIONPOLICIESDATACOLLECTOR_H
