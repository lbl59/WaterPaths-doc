/**
 * @class InsuranceStorageToROF
 * @brief The `InsuranceStorageToROF` class is a subclass of the `DroughtMitigationPolicy` class that implements water use restrictions based on risk of failure (ROF) triggers.
 * 
 * Created by bernardo on 2/3/17.
 */

#ifndef TRIANGLEMODEL_RESTRICTIONS_H
#define TRIANGLEMODEL_RESTRICTIONS_H


#include "../SystemComponents/Utility/Utility.h"
#include "Base/DroughtMitigationPolicy.h"

class Restrictions : public DroughtMitigationPolicy {

private:
        /**
         * @brief DU factor multipliers that scale the volume of demand successfully restricted using
         * water use restrictions at a variety of stages (tiers).
         */
        vector<double> stage_multipliers;
        /**
         * @brief A vector of ROF thresholds after which different water use restriction stages are 
         * triggered.
         */
        const vector<double> stage_triggers;
        /** 
         * @brief Current multiplier to be applied to demand. 
         * */
        double current_multiplier = 0;
        /**
         * @brief A 2D vector of weekly average volumetric price of water for each restriction stage.
         * 
         */
        vector<vector<double>> restricted_weekly_average_volumetric_price;

public:
        /**
         * @brief DU factor multipliers that scale the volume of demand successfully restricted using
         * water use restrictions at a variety of stages (tiers).
         * 
         * This initializes the restriction stages and associated triggers.
         *
         * @param id Restriction policy ID.
         * @param stage_multipliers Multipliers for water usage demand based on restriction stages.
         * @param stage_triggers A vector of ROF thresholds after which different water use restriction stages are 
         * triggered.
         */
        Restrictions(const int id, const vector<double> &stage_multipliers,
                 const vector<double> &stage_triggers);

        /**
         * @brief DU factor multipliers that scale the volume of demand successfully restricted using
         * water use restrictions at a variety of stages (tiers). This constructor has additional parameters for
         * pricing water during water restrictions.
         *
         * @param id Restriction policy ID.
         * @param stage_multipliers Multipliers for water usage demand based on restriction stages.
         * @param stage_triggers A vector of ROF thresholds after which different water use restriction stages are 
         * triggered.
         * @param typesMonthlyDemandFraction Monthly water demand fractions by consumer type.
         * @param typesMonthlyWaterPrice Monthly water prices by consumer type.
         * @param priceMultipliers Price multipliers for each restriction stage.
         */
        Restrictions(
            const int id, const vector<double> &stage_multipliers,
            const vector<double> &stage_triggers,
            const vector<vector<double>> *typesMonthlyDemandFraction,
            const vector<vector<double>> *typesMonthlyWaterPrice,
            const vector<vector<double>> *priceMultipliers);

        /**
         * @brief Creates a copy of an existing Restrictions object.
         *
         * @param restrictions The Restrictions object to be copied.
         */
        Restrictions(const Restrictions &reservoir);

        /**
         * @brief Destroy the Restrictions object.
         * 
         * @return void
         * 
         */
        ~Restrictions();

        /**
         * @brief Applies the restriction policy for a given week based on the utility's risk of failure.
         *
         * @param week The current week number.
         * 
         * @return void
         */
        void applyPolicy(int week) override;

        /**
         * @brief Links utilities to the Restrictions policy based on the policy ID.
         *
         * @param systems_utilities Vector of system Utility objects.
         * @param water_sources Vector of water sources (not used in this implementation).
         * @param min_env_flow_controls Vector of minimum environmental flow controls (not used in this implementation).
         *
         * @throws std::logic_error if the policy already has an assigned utility.
         * @throws std::invalid_argument if no matching utility is found for the policy ID.
         */
        void addSystemComponents(vector<Utility *> systems_utilities,
                                 vector<WaterSource *> water_sources,
                                 vector<MinEnvFlowControl *> min_env_flow_controls) override;

        /**
         * @brief Retrieves the current demand multiplier based on the active restriction stage.
         *
         * @return The current demand multiplier.
         */
        double getCurrent_multiplier() const;

        /**
         * @brief Calculates the average weekly water prices for each restriction stage and consumer type
         * across each  month.
         *
         * @param typesMonthlyDemandFraction Monthly water demand fractions by consumer type.
         * @param typesMonthlyWaterPrice Monthly water prices by consumer type.
         * @param priceMultipliers Price multipliers for each restriction stage.
         * 
         * @return void
         */
        void calculateWeeklyAverageWaterPrices(
            const vector<vector<double>> *typesMonthlyDemandFraction,
            const vector<vector<double>> *typesMonthlyWaterPrice,
            const vector<vector<double>> *priceMultipliers);

        /**
         * @brief Updates the restriction multipliers based on the policy random decision matrix.
         *
         * @param realization_id Realization ID for the current simulation.
         * @param utilities_rdm Random decision matrix for utilities (not used in this implementation).
         * @param water_sources_rdm Random decision matrix for water sources (not used in this implementation).
         * @param policy_rdm Random decision matrix for policies.
         */
        void setRealization(unsigned long realization_id, vector<double> &utilities_rdm,
                        vector<double> &water_sources_rdm, vector<double> &policy_rdm) override;
};


#endif //TRIANGLEMODEL_RESTRICTIONS_H
