/**
 * @class TransfersDataCollector
 * @brief The `TransfersDataCollector` class is a subclass of the `DataCollector` class that implements the data collection of treated water transfers.
 * 
 * Created by bernardoct on 8/25/17.
 */

#ifndef TRIANGLEMODEL_TRANSFERSDATACOLLECTOR_H
#define TRIANGLEMODEL_TRANSFERSDATACOLLECTOR_H

#include <vector>
#include "Base/DataCollector.h"
#include "../DroughtMitigationInstruments/Transfers.h"

class TransfersDataCollector : public DataCollector {
private:
    /**
     * @brief The IDs of the utilities that are part of the transfer policy.
     */
    vector<int> utilities_ids;
    /**
     * @brief The demand offsets for each utility in each week from purchasing transfers.
     */
    vector<vector<double>> demand_offsets;
    /**
     * @brief A pointer to the transfer policy associated with the data collector.
     */
    Transfers *transfer_policy;

public:
    /**
     * @brief Constructs a `TransfersDataCollector` for managing and collecting data related to a transfer policy.
     *        Initializes the data collector with the policy's attributes and associated utilities.
     * 
     * @param transfer_policy Pointer to the `Transfers` object representing the transfer policy.
     * @param realization Unsigned long integer representing the realization ID.
     */
    TransfersDataCollector(Transfers *transfer_policy, unsigned long realization);

    /**
     * @brief Generates a tabular string representation of demand offsets for a specified week.
     *        This representation includes the demand offset values for all utilities involved in the transfer policy.
     * 
     * This function overrides the `printTabularString` function in the `DataCollector` class.
     * 
     * @param week Integer representing the week for which the tabular data is generated.
     * 
     * @return A string containing the tabular representation of the demand offsets for the specified week.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates a compact, comma-delimited string representation of demand offsets for a specified week.
     *        This representation includes the demand offset values for all utilities involved in the transfer policy, separated by commas.
     * 
     * This function overrides the `printCompactString` function in the `DataCollector` class.
     * 
     * @param week Integer representing the week for which the compact data is generated.
     * 
     * @return A string containing the compact representation of the demand offsets for the specified week.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates the first header line for the tabular representation of transfer allocation data.
     *        This line includes repeated column headers for the transfer demand offsets for each utility involved in the transfer policy.
     * 
     * This function overrides the `printTabularStringHeaderLine1` function in the `DataCollector` class.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates the second header line for the tabular representation of transfer allocation data.
     *        This line includes repeated column headers for the transfer demand offsets for each utility involved in the transfer policy.
     * 
     * This function overrides the `printTabularStringHeaderLine2` function in the `DataCollector` class.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates a compact, comma-delimited string header for the transfer data.
     *        This header includes identifiers for the transfer demand offsets for each utility involved in the transfer policy.
     * 
     * This function overrides the `printCompactStringHeader` function in the `DataCollector` class.
     * 
     * @return A string containing the compact header representation for the transfer data.
     */
    string printCompactStringHeader() override;

    /**
     * @brief Collects the demand offset data for the current week from the transfer policy and appends it to the `demand_offsets` vector.
     * 
     * This function overrides the `collect_data` function in the `DataCollector` class.
     * 
     * @return void
     */
    void collect_data() override;

};


#endif //TRIANGLEMODEL_TRANSFERSDATACOLLECTOR_H
