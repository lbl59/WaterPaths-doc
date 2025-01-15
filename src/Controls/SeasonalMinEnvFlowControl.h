/**
 * @class SeasonalMinEnvFlowControl
 * @brief This class is a subclass of the main `MinEnvFlowControl` class and represents water sources with seasonal minimum environmental flow control.
 * 
 * Created by bernardoct on 6/28/17.
 */

#ifndef TRIANGLEMODEL_SEASONALMINENVFLOWCONTROL_H
#define TRIANGLEMODEL_SEASONALMINENVFLOWCONTROL_H

#include <vector>
#include "Base/MinEnvFlowControl.h"

using namespace std;

class SeasonalMinEnvFlowControl : public MinEnvFlowControl {
private:
    /**
     * @brief A reference to a vector of integers containing the week thresholds for seasonal minimum environmental flow requirements.
     */
    const vector<int>& week_thresholds;
    /**
     * @brief A reference to a vector of doubles containing the minimum environmental flow requirements for each week.
     */
    const vector<double>& min_env_flows;

public:
    /**
     * @brief Constructs a `SeasonalMinEnvFlowControl` object for managing seasonal minimum environmental flow controls.
     *        Initializes the control with a specified water source ID, seasonal week thresholds, and corresponding minimum flow rates.
     * 
     * @param water_source_id Integer representing the ID of the water source under environmental flow control.
     * @param week_thresholds Reference to a vector of integers specifying the week intervals that define seasonal periods.
     * @param min_env_flows Reference to a vector of doubles specifying the minimum flow rates for each seasonal period.
     * 
     * @throw std::invalid_argument If the size of `week_thresholds` is not one more than the size of `min_env_flows`.
     * @throw std::invalid_argument If the first threshold is not 0 or the last threshold is not 53 (for years with 53 weeks).
     */
    SeasonalMinEnvFlowControl(
            int water_source_id, const vector<int>& week_thresholds,
            const vector<double>& min_env_flows);

    /**
     * @brief Copy constructor for the `SeasonalMinEnvFlowControl` class.
     *        Creates a new `SeasonalMinEnvFlowControl` object by copying the attributes from an existing one.
     * 
     * @param min_env_control Reference to the `SeasonalMinEnvFlowControl` object to be copied.
     */
    SeasonalMinEnvFlowControl(const SeasonalMinEnvFlowControl &min_env_control);

    /**
     * @brief Calculates the release amount for a specified week based on seasonal week thresholds and corresponding flow rates.
     * 
     * This function overrides the `getRelease` function in the `MinEnvFlowControl` class.
     * 
     * @param week Integer representing the week for which the release amount is calculated.
     * 
     * @return A double representing the calculated release amount for the specified week.
     */
    double getRelease(int week) override;
};


#endif //TRIANGLEMODEL_SEASONALMINENVFLOWCONTROL_H
