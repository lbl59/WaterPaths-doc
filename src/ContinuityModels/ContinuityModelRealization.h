/**
 * @class ContinuityModelRealization
 * 
 * @brief The `ContinuityModelRealization` subclass extends the `ContinuityModel` class to include the implementation of drought
 * mitigation policies and the setting of short-term and long-term risks of failure.
 * 
 * Created by bernardo on 1/26/17.
 */

#ifndef TRIANGLEMODEL_CONTINUITYMODELREALIZATION_H
#define TRIANGLEMODEL_CONTINUITYMODELREALIZATION_H

#include "Base/ContinuityModel.h"
#include "../DroughtMitigationInstruments/Base/DroughtMitigationPolicy.h"

class ContinuityModelRealization : public ContinuityModel {
private:
    /**
     * @brief A vector of pointers to the drought mitigation policies in the system.
     */
    vector<DroughtMitigationPolicy *> drought_mitigation_policies;

public:
    /**
     * @brief Constructs a ContinuityModelRealization object.
     * 
     * This constructor initializes a `ContinuityModelRealization` object, which builds upon the `ContinuityModel`
     * by incorporating drought mitigation policies. It initializes a ContinuityModelRealization object by 
     * calling the ContinuityModel constructor, adding susytem components to the drought mitigation policies.
     * It also sets the realization for each drought mitigation policy with the provided realization data.
     * 
     * @param water_sources A vector of `WaterSource` pointers representing the water sources involved in the model.
     * @param water_sources_graph A `Graph` representing the relationships and connections between the water sources.
     * @param water_sources_to_utilities A 2D vector indicating which water sources are connected to which utilities.
     * @param utilities A vector of `Utility` pointers representing the utilities that rely on the water sources.
     * @param drought_mitigation_policies A vector of `DroughtMitigationPolicy` pointers, each representing a policy for managing drought conditions.
     * @param min_env_flow_control A vector of `MinEnvFlowControl` pointers used to set minimum environmental flows for water sources.
     * @param utilities_rdm A vector containing random demand values for the utilities in the model.
     * @param water_sources_rdm A vector containing random demand values for the water sources in the model.
     * @param policy_rdm A vector containing random demand values for the drought mitigation policies.
     * @param realization_id The realization identifier used to associate a specific realization of the model.
     * 
     * @return A `ContinuityModelRealization` object initialized with the provided components and realization.
     */
    ContinuityModelRealization(
            vector<WaterSource *> &water_sources,
            const Graph &water_sources_graph,
            const vector<vector<int>> &water_sources_to_utilities,
            vector<Utility *> &utilities,
            const vector<DroughtMitigationPolicy *> &drought_mitigation_policies,
            vector<MinEnvFlowControl *> &min_env_flow_control,
            vector<double>& utilities_rdm,
            vector<double>& water_sources_rdm,
            vector<double>& policy_rdm,
            const unsigned int realization_index);

    /**
     * @brief Destructor for the ContinuityModelRealization class.
     * 
     * This function releases the memory allocated for drought mitigation policies.
     * It ensures proper cleanup of resources associated with the `ContinuityModelRealization` object.
     */
    ~ContinuityModelRealization() override;

    /**
     * @brief Sets the short-term risks of failure (ROFs) for utilities.
     * 
     * This function updates the short-term ROFs for each utility based on the given
     * risks of failure for storage and treatment capacity. The higher of the two
     * risks is used as the overall ROF for the utility, and both individual risks
     * are also recorded.
     * 
     * @param risks_of_failure A 2D vector where each row corresponds to a utility, 
     *                         and each column contains risks of failure for storage 
     *                         and treatment capacity, respectively.
     * 
     * @return void
     */
    void setShortTermROFs(const vector<vector<double>> &risks_of_failure);

    /**
     * @brief Applies drought mitigation policies for the given week.
     * 
     * This function iterates through all the drought mitigation policies and applies 
     * each policy to the system for the specified week.
     * 
     * @param week The current week for which drought mitigation policies are to be applied.
     * 
     * @return void
     */
    void applyDroughtMitigationPolicies(int week);

    /**
     * @brief Retrieves the drought mitigation policies associated with the continuity model realization.
     * 
     * This function returns a vector containing pointers to all drought mitigation policies 
     * in the system.
     * 
     * @return A `vector` of pointers to `DroughtMitigationPolicy` objects associated 
     *         with the continuity model realization.
     */
    const vector<DroughtMitigationPolicy *> getDrought_mitigation_policies() const;

    /**
     * @brief Sets the long-term risks of failure (ROFs) for utilities and triggers new infrastructure construction if necessary.
     * 
     * This function calculates and updates the long-term risks of failure for each utility based on 
     * storage and treatment capacities. If the risk exceeds a threshold, it triggers the construction 
     * of new infrastructure and ensures consistency across utilities.
     * 
     * @param risks_of_failure A 2D vector where each row represents a utility and contains two ROF values:
     *                         - `risks_of_failure[u][0]`: Storage risk of failure.
     *                         - `risks_of_failure[u][1]`: Treatment risk of failure.
     * @param week The current simulation week.
     * 
     * @return void
     */
    void setLongTermROFs(const vector<vector<double>> &risks_of_failure, const int week);

    /**
     * @brief Updates the demand projection estimates for utilities based on risk of failure (ROF) utilities.
     * 
     * This function synchronizes the current-year demand records and future demand estimates of 
     * the `continuity_utilities` with the corresponding values from the provided `rof_utilities`.
     * 
     * @param rof_utilities A vector of utility pointers containing the risk of failure (ROF) utilities 
     *                      with up-to-date demand projection estimates.
     * 
     * @return void
     */
    void setLongTermROFDemandProjectionEstimate(const vector<Utility *> &rof_utilities);

    /**
     * @brief Updates treatment capacity allocations for Joint Water Treatment Plants (WTPs) under variable allocation agreements.
     * 
     * This function recalculates treatment capacity allocations for utilities that share a Joint Water Treatment Plant (WTP) 
     * with variable allocation agreements, adjusting for demand growth or reduction. Allocations are based on the expected 
     * growth of each partner until the plant's capacity is reached.
     * 
     * @param current_week The current simulation week, used to determine the current year.
     * 
     * @return void
     */
    void updateJointWTPTreatmentAllocations(int current_week);
};


#endif //TRIANGLEMODEL_CONTINUITYMODELREALIZATION_H
