/**
 * @class MinEnvFlowControl
 * 
 * @brief This class is a defines the control rules for minimum environmental flow.
 * 
 * Created by bernardoct on 6/28/17.
 * 
 */

#ifndef TRIANGLEMODEL_MINENVIRONFLOWCONTROL_H
#define TRIANGLEMODEL_MINENVIRONFLOWCONTROL_H

#include "../../SystemComponents/WaterSources/Base/WaterSource.h"
#include "../../SystemComponents/Utility/Utility.h"

class MinEnvFlowControl {
protected:
    /**
     * @brief A vector of WaterSource objects that require minimum environment flow control.
     */
    vector<WaterSource *> water_sources;
    /**
     * @brief A vector of Catchment objects that enable minimum environment flow control.
     */
    vector<Catchment *> catchments;
    /**
     * @brief A vector of Utility objects that enable minimum environment flow control.
     */
    vector<Utility* > utilities;

public:
    /**
     * @brief A vector of WaterSource IDs that require minimum environment flow control.
     */
    const vector<int> water_sources_ids;
    /**
     * @brief A vector of Utility IDs that enable minimum environment flow control.
     */
    const vector<int> utilities_ids;
    /**
     * @brief The ID of the WaterSource object that requires minimum environment flow control.
     */
    const int water_source_id;
    /**
     * @brief The type of minimum environment flow control.
     */
    const int type;

    /**
     * @brief Constructs a `MinEnvFlowControl` object for managing minimum environmental flow controls on specified water sources.
     *        Initializes the object with associated water source and utility IDs and a control type.
     * 
     * @param water_source_id Integer representing the ID of the primary water source under environmental flow control.
     * @param water_sources_ids Reference to a vector of integers containing the IDs of additional water sources affected by the control.
     * @param aux_utilities_ids Reference to a vector of integers containing the IDs of auxiliary utilities associated with the control.
     * @param type Integer representing the type of environmental flow control.
     */
    MinEnvFlowControl(int water_source_id,
                              const vector<int> &aux_water_sources_id,
                              const vector<int> &aux_utilities_ids, int type);

    /**
     * @brief Copy constructor for a `MinEnvFlowControl` object.
     * This function copies the attributes from an existing `MinEnvFlowControl` object.
     * 
     * @param min_env_control Reference to the `MinEnvFlowControl` object to be copied.
     */
    MinEnvFlowControl(const MinEnvFlowControl &min_env_control);

    /**
     * @brief Destructor for the `MinEnvFlowControl` class.
     *        Cleans up resources associated with the object. Since the destructor is defaulted, no custom cleanup is required.
     * 
     * @return void
     */
    virtual ~MinEnvFlowControl();

    /**
     * @brief Get the Release object. This function is a pure virtual function that must be implemented by derived classes.
     * 
     * @param week  The week for which the release is requested.
     * 
     * @return double 
     */
    virtual double getRelease(int week) = 0;

    /**
     * @brief Associates water sources and utilities with the `MinEnvFlowControl` object based on their IDs.
     *        The associations are established by mapping the provided vectors of water sources and utilities to the corresponding IDs.
     * 
     * @param water_sources A vector of pointers to `WaterSource` objects representing all available water sources.
     * @param utilities A vector of pointers to `Utility` objects representing all available utilities.
     * 
     * @return void
     */
    void addComponents(
            vector<WaterSource *> water_sources, vector<Utility *> utilities);

    /**
     * @brief Sets the realization for the `MinEnvFlowControl` object. 
     *       This function is a pure virtual function that must be implemented by derived classes.
     * 
     * @param r Unsigned long integer representing the realization ID.
     * @param rdm_factors Reference to a vector of doubles containing the RDM factors to be applied.
     * 
     * @return void
     */
    virtual void setRealization(unsigned long r, vector<double> &rdm_factors);
};


#endif //TRIANGLEMODEL_MINENVIRONFLOWCONTROL_H
