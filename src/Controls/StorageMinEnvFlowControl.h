/**
 * @class StorageMinEnvFlowControl
 * @brief This class is a subclass of the main `MinEnvFlowControl` class and represents storage water sources with minimum environmental flow control.
 * 
 * Created by bernardoct on 6/28/17.
 */

#ifndef TRIANGLEMODEL_STORAGEMINENVFLOWCONTROL_H
#define TRIANGLEMODEL_STORAGEMINENVFLOWCONTROL_H

#include "Base/MinEnvFlowControl.h"

class StorageMinEnvFlowControl : public MinEnvFlowControl {
private:
    /**
     * @brief Calculates the release amount for a specified week based on storage thresholds and corresponding release values.
     * 
     * @param week Integer representing the week for which the release amount is calculated.
     * 
     * @return A double representing the calculated release amount for the specified week.
     */
    double getRelease(int week);

public:
    /**
     * @brief Constructs a `StorageMinEnvFlowControl` object for managing minimum environmental flow controls based on storage levels.
     *        Initializes the control with a specified water source ID, auxiliary water sources, storage thresholds, and corresponding release values.
     * 
     * @param water_source_id Integer representing the ID of the primary water source under environmental flow control.
     * @param aux_water_sources_ids Reference to a vector of integers containing the IDs of auxiliary water sources (e.g., reservoirs or quarries).
     * @param storages Reference to a vector of doubles containing storage thresholds.
     * @param releases Reference to a vector of doubles containing the corresponding release amounts for each storage threshold.
     * 
     * @throw std::invalid_argument If the number of auxiliary water sources is not exactly one.
     */
    StorageMinEnvFlowControl(
            int water_source_id, const vector<int> &aux_water_sources_ids,
            vector<double>& storages, vector<double>& releases);

    /**
     * @brief Copy constructor for the `StorageMinEnvFlowControl` class.
     *        Creates a new `StorageMinEnvFlowControl` object by copying the attributes from an existing one.
     * 
     * @param min_env_control Reference to the `StorageMinEnvFlowControl` object to be copied.
     */
    StorageMinEnvFlowControl(const StorageMinEnvFlowControl &min_env_control);

    /**
     * @brief A reference to the vector of doubles containing storage thresholds.
     */
    const vector<double> &storages;

    /**
     * @brief A reference to the vector of doubles containing the corresponding release amounts for each storage threshold.
     */
    const vector<double> &releases;

};


#endif //TRIANGLEMODEL_STORAGEMINENVFLOWCONTROL_H
