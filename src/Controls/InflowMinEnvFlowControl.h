/**
 * @class InflowMinEnvFlowControl
 * @brief This class is a subclass of the main `MinEnvFlowControl` class and represents inflow water sources with minimum environmental flow control.
 * 
 * Created by bernardoct on 6/29/17.
 */

#ifndef TRIANGLEMODEL_INFLOWMINENVFLOWCONTROL_H
#define TRIANGLEMODEL_INFLOWMINENVFLOWCONTROL_H

#include "Base/MinEnvFlowControl.h"

class InflowMinEnvFlowControl : public MinEnvFlowControl {
private:
    /**
     * @brief Calculates the release amount for a specified week based on the inflow thresholds and corresponding release values.
     * 
     * @param week Integer representing the week for which the release amount is calculated.
     * 
     * @return A double representing the calculated release amount for the specified week.
     */
    double getRelease(int week) override;

public:
    /**
     * @brief A reference to a vector of doubles containing inflow.
     */
    const vector<double>& inflows;
    /**
     * @brief A reference to a vector of doubles containing the corresponding release amounts.
     */
    const vector<double>& releases;

    /**
     * @brief Constructs an `InflowMinEnvFlowControl` object for managing minimum environmental flow controls based on inflow values.
     *        Initializes the control with a specified water source ID, inflow thresholds, and corresponding release amounts.
     * 
     * @param water_source_id Integer representing the ID of the water source under environmental flow control.
     * @param inflows Reference to a vector of doubles containing inflow thresholds.
     * @param releases Reference to a vector of doubles containing the corresponding release amounts for each inflow threshold.
     */
    InflowMinEnvFlowControl(int water_source_id, const vector<double> &inflows,
                                const vector<double> &releases);

    /**
     * @brief Copy constructor for the `InflowMinEnvFlowControl` class.
     *        Creates a new `InflowMinEnvFlowControl` object by copying the attributes from an existing one.
     * 
     * @param min_env_control Reference to the `InflowMinEnvFlowControl` object to be copied.
     */
    InflowMinEnvFlowControl(const InflowMinEnvFlowControl &min_env_control);

    /**
     * @brief Destructor for the `InflowMinEnvFlowControl` class.
     *        Cleans up resources associated with the object. Since the destructor is defaulted, no custom cleanup is required.
     * 
     * @return void
     */
    ~InflowMinEnvFlowControl() override;

};


#endif //TRIANGLEMODEL_INFLOWMINENVFLOWCONTROL_H
