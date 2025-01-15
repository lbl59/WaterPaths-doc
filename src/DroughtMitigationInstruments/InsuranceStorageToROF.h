/**
 * @class InsuranceStorageToROF
 * @brief The `InsuranceStorageToROF` class is a subclass of the `DroughtMitigationPolicy` and `ContinuityModelROF` classes that implements drought insurance policies based on storage-to-ROF triggers.
 * 
 * Created by bernardoct on 5/1/17.
 */

#ifndef TRIANGLEMODEL_INSURANCESTORAGETOROF_H
#define TRIANGLEMODEL_INSURANCESTORAGETOROF_H

#include "Base/DroughtMitigationPolicy.h"
#include "../ContinuityModels/ContinuityModelROF.h"
#include "../ContinuityModels/ContinuityModelRealization.h"

class InsuranceStorageToROF : public DroughtMitigationPolicy,
                              public ContinuityModelROF {
private:
    /**
     * @brief A vector of triggers for insurance payouts based on storage-to-ROF triggers.
     */
    vector<double> rof_triggers;// = vector<double>(40, 0.);
    /**
     * @brief Total simulation duration in weeks.
     */
    const unsigned long total_simulation_time;
    /**
     * @brief The percentage of revenue that is used to pay for insurance.
     */
    const double insurance_premium;
    /**
     * @brief A vector of payout multiplier to adjust for partially paid insurance price due to insufficient contingency fund.
     */
    vector<double> payout_multiplier;
    /**
     * @brief A vector of insurance prices for each utility as a function of fixed_payouts,revenue, and insurance premiums.
     */
    vector<double> insurance_price;
    /**
     * @brief A vector of fixed payouts for each utility in case of an insurance trigger.
     */
    const vector<double> &fixed_payouts;
    /**
     * @brief A vector for the updated (current year) revenues for each utility.
     */
    vector<double> utilities_revenue_update;
    /**
     * @brief A vector for the prior year's revenues for each utility.
     */
    vector<double> utilities_revenue_last_year;
    /**
     * @brief A vector of drought mitigation policies.
     */
    vector<DroughtMitigationPolicy *> drought_mitigation_policies;

public:
    /**
     * @brief Constructs an InsuranceStorageToROF object for drought insurance policies based on storage-to-ROF triggers.
     * 
     * @param id Insurance policy ID.
     * @param water_sources Vector of water sources used in the model.
     * @param water_sources_graph Graph of connections between water sources.
     * @param water_sources_to_utilities Mapping between water sources and utilities.
     * @param utilities Vector of utilities involved in the policy.
     * @param drought_mitigation_policies Vector of drought mitigation policies.
     * @param min_env_flow_controls Vector of minimum environmental flow controls.
     * @param utilities_rdm Random decision matrix for utilities.
     * @param water_sources_rdm Random decision matrix for water sources.
     * @param policy_rdm Random decision matrix for policies.
     * @param rof_triggers Risk-of-failure triggers for insurance payouts.
     * @param insurance_premium Premium multiplier for insurance pricing.
     * @param fixed_payouts Fixed payouts for each utility in case of an insurance trigger.
     * @param total_simulation_time Total simulation duration in weeks.
     */
    InsuranceStorageToROF(const int id, vector<WaterSource *> &water_sources,
                              const Graph &water_sources_graph,
                              const vector<vector<int>> &water_sources_to_utilities,
                              vector<Utility *> &utilities,
                              vector<DroughtMitigationPolicy *> &drought_mitigation_policies,
                              vector<MinEnvFlowControl *> min_env_flow_controls,
                              vector<vector<double>>& utilities_rdm,
                              vector<vector<double>>& water_sources_rdm,
                              vector<vector<double>>& policy_rdm, vector<double> &rof_triggers,
                              const double insurance_premium, const vector<double> &fixed_payouts,
                              unsigned long total_simulation_time);

    /**
     * @brief Creates a copy of an existing InsuranceStorageToROF object.
     *
     * @param insurance The InsuranceStorageToROF object to copy.
     */
    InsuranceStorageToROF(InsuranceStorageToROF &insurance);

    /**
     * @brief Destroys the InsuranceStorageToROF object and releases resources.
     */
    ~InsuranceStorageToROF() override;

    /**
     * @brief Calculates the insurance price based on payout probabilities.
     *
     * @param week The current week number.
     * 
     * @return void
     */
    void priceInsurance(int week);

    /**
     * @brief Applies the insurance policy for a given week.
     * 
     * This function runs an ROF calculation of 50 year long simulations to estimate the likelihood that payouts will occur. 
     * The price of the insurance is set as (average sum of payouts across all 50 years * insurance premium).
     * 
     * This function overrides the pure virtual function applyPolicy in DroughtMitigationPolicy.
     *
     * @param week The current week.
     * 
     * @return void
     */
    void applyPolicy(int week) override;

    /**
     * @brief Adds Utility and WaterSource objects to the InsuranceStorageToROF policy.
     * 
     * This function overrides the pure virtual function addSystemComponents in DroughtMitigationPolicy.
     *
     * @param utilities Vector of system utilities.
     * @param water_sources Vector of water sources.
     * @param min_env_flow_controls Vector of minimum environmental flow controls.
     * 
     * @return void
     */
    void addSystemComponents(vector<Utility *> utilities,
                                 vector<WaterSource *> water_sources,
                                 vector<MinEnvFlowControl *> min_env_flow_controls) override;

    /**
     * @brief Sets the realization parameters for the InsuranceStorageToROF policy.
     *
     * @param realization_id ID of the realization.
     * @param utilities_rdm DU factor multipliers for utilities' demand growth rates.
     * @param water_sources_rdm DU factor multipliers for water sources financing, construction, and permitting times.
     * @param policy_rdm DU factor multipliers for utilities' drought mitigation policies effectiveness.
     * 
     * @return void
     */
    void setRealization(unsigned long realization_id, vector<double> &utilities_rdm,
                        vector<double> &water_sources_rdm, vector<double> &policy_rdm) override;

    /**
     * @brief Runs a short-term ROF calculation for a given week. 
     * 
     * FIXME: Needs to be extended from the original function from ContinuityModelROF.cpp.
     * 
     * @param week The current week for which ROFs should be calculated.
     * @param utilities Vector of utilities for the ROF calculation.
     * @param n_utilities Number of utilities involved.
     *
     * @return A vector of calculated ROF values for each utility.
     */
    vector<double> calculateShortTermROFTable(int week, const vector<Utility *> &utilities, const int &n_utilities);

    /**
     * @brief Updates the online infrastructure for the current week.
     * 
     * This function overrides the pure virtual function updateOnlineInfrastructure in DroughtMitigationPolicy.
     *
     * @param week The current week number.
     * 
     * @return void
     */
    void updateOnlineInfrastructure(int week) override;
};


#endif //TRIANGLEMODEL_INSURANCESTORAGETOROF_H
