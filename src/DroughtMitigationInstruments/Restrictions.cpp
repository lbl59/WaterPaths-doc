//
// Created by bernardo on 2/3/17.
//

#include <iostream>
#include <vector>
#include "Restrictions.h"
#include "../Utils/Utils.h"

/**
 * Restriction policy.
 * @param id
 * @param stage_multipliers
 * @param stage_triggers
 * @todo set lower ROF threshold for utilities to lift restrictions.
 * @todo implement drought surcharges.
 */
Restrictions::Restrictions(const int id, const vector<double> &stage_multipliers,
                           const vector<double> &stage_triggers)
        : DroughtMitigationPolicy(id, RESTRICTIONS),
          stage_multipliers(stage_multipliers),
          stage_triggers(stage_triggers) {
    utilities_ids = vector<int>(1, id);
}

Restrictions::Restrictions(
        const int id, const vector<double> &stage_multipliers,
        const vector<double> &stage_triggers,
        const vector<vector<double>> *typesMonthlyDemandFraction,
        const vector<vector<double>> *typesMonthlyWaterPrice,
        const vector<vector<double>> *priceMultipliers)
        : DroughtMitigationPolicy(id, RESTRICTIONS),
          stage_multipliers(stage_multipliers),
          stage_triggers(stage_triggers) {
    calculateWeeklyAverageWaterPrices(typesMonthlyDemandFraction,
                                      typesMonthlyWaterPrice,
                                      priceMultipliers);
    utilities_ids = vector<int>(1, id);
}

Restrictions::Restrictions(const Restrictions &restrictions) :
        DroughtMitigationPolicy(restrictions),
        stage_multipliers(restrictions.stage_multipliers),
        stage_triggers(restrictions.stage_triggers),
        restricted_weekly_average_volumetric_price(
                restrictions.restricted_weekly_average_volumetric_price) {
    utilities_ids = restrictions.utilities_ids;
}

Restrictions::~Restrictions() {}

void Restrictions::applyPolicy(int week) {

    current_multiplier = 1.0;
    unsigned long stage = 0;
    // Loop through restriction stage ROF triggers
    // Determines the appropriate restriction stage based on risk of failure triggers.
    for (unsigned long i = 0; i < stage_triggers.size(); ++i) {
        if (realization_utilities[0]->getRisk_of_failure() > stage_triggers[i]) {
            // Demand multiplier to be applied, based on the ROFs.
            current_multiplier = stage_multipliers[i];
            stage = i + 1;
        } else
            break;
    }
    realization_utilities[0]->setDemand_multiplier(current_multiplier);

    // If applicable, sets the restricted volumetric price for the utility.
    if (!restricted_weekly_average_volumetric_price.empty() && stage > 0) {
        int week_of_year = Utils::weekOfTheYear(week);
        realization_utilities[0]->setRestricted_price(
                restricted_weekly_average_volumetric_price[stage][week_of_year]);
    }
}

double Restrictions::getCurrent_multiplier() const {
    return current_multiplier;
}

void Restrictions::addSystemComponents(vector<Utility *> systems_utilities,
                                       vector<WaterSource *> water_sources,
                                       vector<MinEnvFlowControl *> min_env_flow_controls) {
    // Get utility whose IDs correspond to restriction policy ID.
    for (Utility *u : systems_utilities) {
        if (u->id == id) {
            if (!realization_utilities.empty())
                throw std::logic_error("This restriction policy already has a systems_utilities assigned to it.");
            // Link utility to policy.
            this->realization_utilities.push_back(u);
        }
    }
    if (systems_utilities.empty())
        throw std::invalid_argument("Restriction policy ID must match systems_utilities's ID.");
}

void Restrictions::calculateWeeklyAverageWaterPrices(
        const vector<vector<double>> *typesMonthlyDemandFraction,
        const vector<vector<double>> *typesMonthlyWaterPrice, const
        vector<vector<double>> *priceMultipliers) {

    if (priceMultipliers) {
        unsigned long n_tiers = typesMonthlyWaterPrice->at(0).size();
        restricted_weekly_average_volumetric_price =
                std::vector<std::vector<double>>();

        // Loop through the different restriction stages
        for (unsigned long s = 0; s < priceMultipliers->size(); ++s) {
            restricted_weekly_average_volumetric_price.emplace_back(
                    (int) WEEKS_IN_YEAR + 1, 0.);
            double monthly_average_price[NUMBER_OF_MONTHS] = {};
            // Loop through each month
            for (int m = 0; m < NUMBER_OF_MONTHS; ++m) 
                // Loop through each consumer type
                for (unsigned long t = 0; t < n_tiers; ++t) 
                    monthly_average_price[m] +=
                            (*typesMonthlyDemandFraction)[m][t] *
                            (*typesMonthlyWaterPrice)[m][t] *
                            (*priceMultipliers)[s][t];

            for (int w = 0; w < (int) (WEEKS_IN_YEAR + 1); ++w)
                restricted_weekly_average_volumetric_price[s][w] =
                        monthly_average_price[(int) (w / WEEKS_IN_MONTH)] / 1e6;
        }
    }
}

void Restrictions::setRealization(unsigned long realization_id, vector<double> &utilities_rdm,
                                  vector<double> &water_sources_rdm, vector<double> &policy_rdm) {
    for (double& sm : stage_multipliers) {
        sm *= policy_rdm.at((unsigned long) id);
    }
}
