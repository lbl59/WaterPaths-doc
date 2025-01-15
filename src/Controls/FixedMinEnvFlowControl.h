/**
 * @class FixedMinEnvFlowControl
 * @brief This class is a subclass of the main `MinEnvFlowControl` class and represents a water source with fixed minimum environmental flow control.
 * 
 * Created by bernardoct on 6/29/17.
 */

#ifndef TRIANGLEMODEL_FIXEDMINENVFLOWCONTROL_H
#define TRIANGLEMODEL_FIXEDMINENVFLOWCONTROL_H

#include "Base/MinEnvFlowControl.h"

class FixedMinEnvFlowControl : public MinEnvFlowControl {
private:
    /**
     * @brief The release to be maintained to ensure minimum environmental flow.
     */
    double release;
public:
    /**
     * @brief Constructs a `FixedMinEnvFlowControl` object for managing fixed minimum environmental flow controls on a water source.
     *        Initializes the control with a specified water source ID and a fixed release amount.
     * 
     * @param water_source_id Integer representing the ID of the water source under environmental flow control.
     * @param release Double representing the fixed release amount for the environmental flow.
     */
    FixedMinEnvFlowControl(int water_source_id, double release);

    /**
     * @brief Copy constructor for the `FixedMinEnvFlowControl` class.
     *        Creates a new `FixedMinEnvFlowControl` object by copying the attributes from an existing one.
     * 
     * @param min_env_control Reference to the `FixedMinEnvFlowControl` object to be copied.
     */
    FixedMinEnvFlowControl(const FixedMinEnvFlowControl &min_env_control);

    /**
     * @brief Destructor for the `FixedMinEnvFlowControl` class.
     *        Cleans up resources associated with the object. Since the destructor is defaulted, no custom cleanup is required.
     * 
     * @return void
     */
    ~FixedMinEnvFlowControl() override;

    /**
     * @brief Retrieves the fixed release amount for the environmental flow control.
     * 
     * This function overrides the `getRelease` function in the `MinEnvFlowControl` class.
     * 
     * @param week Integer representing the week for which the release amount is requested.
     *        (Note: For this fixed flow control, the release amount does not depend on the week.)
     * 
     * @return A double representing the fixed release amount.
     */
    double getRelease(int week) override;
};


#endif //TRIANGLEMODEL_FIXEDMINENVFLOWCONTROL_H
