//
// Created by bernardoct on 8/25/17.
//

#include <numeric>
#include <algorithm>
#include "ObjectivesCalculator.h"
#include "Utils.h"

double ObjectivesCalculator::calculateReliabilityObjective(
        const vector<UtilitiesDataCollector *>& utility_collector, vector<unsigned long> realizations) {
    // Initialize realizations
    unsigned long n_realizations = utility_collector.size();
    if (realizations.empty()) {
        realizations = vector<unsigned long>(n_realizations);
        iota(realizations.begin(), realizations.end(), 0);
    } else {
        n_realizations = realizations.size();
    }

    // Determine number of weeks and years
    unsigned long n_weeks = utility_collector[realizations[0]]->getCombined_storage().size();
    unsigned long n_years = (unsigned long) round(n_weeks / WEEKS_IN_YEAR);

    vector<vector<int>> realizations_year_reliabilities(
            utility_collector.size(), vector<int>(n_years, NON_INITIALIZED));
    vector<int> year_reliabilities(n_years, 0);

    // Creates a table with years that failed in each realization.
    // Identify years with storage failure for each realization
    for (const unsigned long &r : realizations) {
        for (unsigned long y = 0; y < n_years; ++y) {
            for (int w = (int) round(y * WEEKS_IN_YEAR);
                 w < (int) min((int) n_weeks, (int) round((y + 1) * WEEKS_IN_YEAR));
                 ++w) {
                if (utility_collector[r]->getCombined_storage()[w] /
                    utility_collector[r]->getCapacity()[w] <
                    STORAGE_CAPACITY_RATIO_FAIL) {
                    realizations_year_reliabilities[r][y] = FAILURE;
                }
            }
        }
    }

    // Creates a vector with the number of realizations that failed for each year.
    for (unsigned long y = 0; y < n_years; ++y) {
        for (const unsigned long &r : realizations) {
            if (realizations_year_reliabilities[r][y] == FAILURE)
                year_reliabilities[y]++;
        }
    }

    double check_non_zero = accumulate(year_reliabilities.rbegin(),
                                       year_reliabilities.rend(),
                                       0.0);

    // Returns year with most realization failures, divided by the number of realizations (reliability objective).
    // Compute the reliability objective as 1-max annual failure ratio.
    if (check_non_zero > 0) {
        double obj_value =
                1. - (double) *max_element(year_reliabilities.begin(),
                                           year_reliabilities.end()) /
                     n_realizations;

        if (std::isinf(obj_value)) {
            string error_inf = "Infinite reliability.";
            throw logic_error(error_inf.c_str());
        } else {
            return obj_value;
        }
    } else {
        return 1.;
    }
}

double ObjectivesCalculator::calculateRestrictionFrequencyObjective(
        const vector<RestrictionsDataCollector *>& restriction_data, vector<unsigned long> realizations) {
    
    // Initialize realizations
    unsigned long n_realizations = restriction_data.size();
    if (realizations.empty()) {
        realizations = vector<unsigned long>(n_realizations);
        iota(realizations.begin(), realizations.end(), 0);
    } else {
        n_realizations = realizations.size();
    }

    // Check if there were restriction policies in place.
    if (!restriction_data.empty()) {
        unsigned long n_weeks = restriction_data[realizations[0]]->getRestriction_multipliers().size();
        unsigned long n_years = (unsigned long) round(n_weeks / WEEKS_IN_YEAR);

        double restriction_frequency = 0;

        // Count years with restrictions for each realization
        for (const unsigned long &r : realizations) {
            for (unsigned long y = 0; y < n_years; ++y) {
                for (int w = (int) round(y * WEEKS_IN_YEAR);
                     w < (int) min((int) n_weeks,
                             (int) round((y + 1) * WEEKS_IN_YEAR)); ++w) {
                    if (restriction_data[r]->getRestriction_multipliers()[w] !=
                        1.0) {
                        restriction_frequency++;
                        break;
                    }
                }
            }
        }

        // Calculate restriction frequency as the proportion of years with restrictions to total number of years,
        // averaged across all realizations.
        double obj_value = restriction_frequency / (n_realizations * n_years);

        if (std::isinf(obj_value)) {
            string error_inf = "Infinite restriction frequency.";
            throw logic_error(error_inf.c_str());
        } else {
            return obj_value;
        }
    } else
        return NONE;
}

double ObjectivesCalculator::calculateNetPresentCostInfrastructureObjective(
        const vector<UtilitiesDataCollector *>& utility_data, vector<unsigned long> realizations) {

    unsigned long n_realizations = utility_data.size();
    if (realizations.empty()) {
        realizations = vector<unsigned long>(n_realizations);
        iota(realizations.begin(), realizations.end(), 0);
    } else {
        n_realizations = realizations.size();
    }

    double infrastructure_npc = 0;
    for (const unsigned long &r : realizations) {
	    auto realization = utility_data[r];
        infrastructure_npc += accumulate(
                realization->getNet_present_infrastructure_cost().begin(),
                realization->getNet_present_infrastructure_cost().end(),
                0.);
    }

    // Average infrastructure NPC across all realizations.S
    return infrastructure_npc / n_realizations;
}

double ObjectivesCalculator::calculatePeakFinancialCostsObjective(
        const vector<UtilitiesDataCollector *>& utility_data, vector<unsigned long> realizations) {

    unsigned long n_realizations = utility_data.size();
    if (realizations.empty()) {
        realizations = vector<unsigned long>(n_realizations);
        iota(realizations.begin(), realizations.end(), 0);
    } else {
        n_realizations = realizations.size();
    }

    unsigned long n_weeks = utility_data[realizations[0]]->getGross_revenues().size();
    unsigned long n_years = (unsigned long) round(n_weeks / WEEKS_IN_YEAR);

    double realizations_year_debt_payment = 0;
    double realizations_year_cont_fund_contribution = 0;
    double realizations_year_gross_revenue = 1e-6;
    double realizations_year_insurance_contract_cost = 0;
    vector<double> year_financial_costs;
    vector<double> realization_financial_costs;

    // Creates a table with years that failed in each realization.
    int y;
    for (const unsigned long &r : realizations) {
        year_financial_costs.assign(n_years,  0.0);
        y = 0;
        for (unsigned long w = 0; w < n_weeks; ++w) {
            // Accumulate year's info by summing weekly amounts.
            realizations_year_debt_payment +=
                    utility_data[r]->getDebt_service_payments()[w];
            realizations_year_cont_fund_contribution +=
                    utility_data[r]->getContingency_fund_contribution()[w];
            realizations_year_gross_revenue +=
                    utility_data[r]->getGross_revenues()[w];
            realizations_year_insurance_contract_cost +=
                    utility_data[r]->getInsurance_contract_cost()[w];

            // If last week of the year, close the books and calculate financial cost for the year.
            if (Utils::isFirstWeekOfTheYear(w + 1)) {
                year_financial_costs[y] +=
                        (realizations_year_debt_payment +
                         realizations_year_cont_fund_contribution +
                         realizations_year_insurance_contract_cost) /
                        realizations_year_gross_revenue;
                // Update year count.
                y++;

                // Reset accounts.
                realizations_year_debt_payment = 0;
                realizations_year_cont_fund_contribution = 0;
                realizations_year_gross_revenue = 1e-6;
                realizations_year_insurance_contract_cost = 0;
            }
        }
        // Store highest year cost as the maximum financial cost of the realization.
        realization_financial_costs.push_back(*max_element(year_financial_costs.begin(),
                                                           year_financial_costs.end()));
        if (*max_element(year_financial_costs.begin(),
                         year_financial_costs.end()) > 1e10) {
            printf("Absurdly high financial cost in realization %lu.\n", r);
        }
    }

    double obj_value = accumulate(realization_financial_costs.begin(),
                                  realization_financial_costs.end(),
                                  0.0) / n_realizations;

    if (std::isinf(obj_value)) {
        string error_inf = "Infinite peak financial cost.";
        throw logic_error(error_inf.c_str());
    } else {
        return obj_value;
    }
}

double ObjectivesCalculator::calculateWorseCaseCostsObjective(
        const vector<UtilitiesDataCollector *>& utility_data, vector<unsigned long> realizations) {

    unsigned long n_realizations = utility_data.size();
    if (realizations.empty()) {
        realizations = vector<unsigned long>(n_realizations);
        iota(realizations.begin(), realizations.end(), 0);
    } else {
        n_realizations = realizations.size();
    }

    unsigned long n_weeks = utility_data[realizations[0]]->getGross_revenues().size();
    unsigned long n_years = (unsigned long) round(n_weeks / WEEKS_IN_YEAR);

    vector<double> worse_year_financial_costs;
    vector<double> year_financial_costs;
    double year_drought_mitigation_cost = 0;
    double year_gross_revenue = 1e-6;

    // Creates a table with years that failed in each realization.
    int y;
    for (const unsigned long &r : realizations) {
        y = 0;
        year_financial_costs.assign(n_years, 0);
        for (unsigned long w = 0; w < n_weeks; ++w) {
            // Accumulate current year's info by summing weekly amounts.
            year_drought_mitigation_cost +=
                    utility_data[r]->getDrought_mitigation_cost()[w];
            year_gross_revenue += utility_data[r]->getGross_revenues()[w];

            // If last week of the year, close the books and calculate financial cost for the year.
            if (Utils::isFirstWeekOfTheYear(w + 1)) {
                year_financial_costs[y] =
                        max(year_drought_mitigation_cost
                            - utility_data[r]->getContingency_fund_size()[w],
                            0.0) / year_gross_revenue;

                year_gross_revenue = 1e-6;
                year_drought_mitigation_cost = 0;
                // Update year count.
                y++;
            }
        }
        // Store highest year cost as the drought mitigation cost of the realization.
        worse_year_financial_costs.push_back(*max_element(
                year_financial_costs.begin(),
                year_financial_costs.end()));
    }

    // Sort costs to get the worst 1st percentile.
    // WORSE_CASE_COST_PERCENTILE is defined in Constants.h
    sort(worse_year_financial_costs.begin(),
         worse_year_financial_costs.end());

    double obj_value = worse_year_financial_costs.at(
            (unsigned long) floor(WORSE_CASE_COST_PERCENTILE * n_realizations));

    if (std::isinf(obj_value)) {
        string error_inf = "Infinite worse case cost.";
        throw logic_error(error_inf.c_str());
    } else {
        return obj_value;
    }
}

double ObjectivesCalculator::calculateUnitTotalCostObjective(const vector<UtilitiesDataCollector *> &utility_data,
                                                             vector<unsigned long> realizations) {
    
    // USING CURRENT VALUES OF DEBT SERVICE, NOT TOTAL NPV (WILL NOT CAPTURE DEBT SERVICE PAST 2060)
    unsigned long n_realizations = utility_data.size();
    if (realizations.empty()) {
        realizations = vector<unsigned long>(n_realizations);
        iota(realizations.begin(), realizations.end(), 0);
    } else {
        n_realizations = realizations.size();
    }

    // For each realization, calculate the ratio of NPV infrastructure cost over
    // total water demanded to get an estimate of cost per unit demand.
    
    // June 2020: added first year water demand getter to normalize total water demand
    // by first year, so the unit cost is of infrastructure needed to meet "new" demand
    vector<double> realization_unit_cost;
    double infra_npv_total;
    double utility_demanded_water;
    double first_year_average_demand;
    for (const unsigned long &r : realizations) {
        auto realization = utility_data[r];

        infra_npv_total = 0;
        utility_demanded_water = 0;
        first_year_average_demand = 0;

        infra_npv_total = accumulate(
                realization->getPresent_value_debt_service_payments().begin(),
                realization->getPresent_value_debt_service_payments().end(), 0.);

        utility_demanded_water = accumulate(
                realization->getUnrestricted_demand().begin(),
                realization->getUnrestricted_demand().end(), 0.);

        first_year_average_demand = realization->getRecorded_annual_demand().at(0);

        if (utility_demanded_water <= first_year_average_demand * realization->getRecorded_annual_demand().size()) {
            realization_unit_cost.push_back(infra_npv_total); // make ratio huge, but not negative
        } else {
            // Normalize water demand by subtracting first-year demand.
            realization_unit_cost.push_back(
                    infra_npv_total / (utility_demanded_water -
                    first_year_average_demand * realization->getRecorded_annual_demand().size()));
        }
    }

    // Take the average across all realizations for the objective value
    // in the future, this could be some other statistic.
    // June 2020: because unit cost can be skewed to enormous values if little demand growth occurs,
    // be aware that taking the average (vs the median, for instance) will highlight negative
    // impact of extreme infrastructure spending for little demand growth

    // THIS IS IN UNITS OF MILLIONS OF DOLLARS PER MILLION GALLONS (COMVERTED TO $/kgal)

    // Sept 2020: comment out old code that was for taking the average
    //  ACTUALLY KEEP IT THIS WAY, MEDIAN AND 90TH PERCENTILES AREN'T ANY MORE USEFUL
    return accumulate(realization_unit_cost.begin(), realization_unit_cost.end(), 0.0) / n_realizations * 1000;
}

double ObjectivesCalculator::calculateNetPresentCostInfrastructureObjectiveForVariableDebtService(
        const vector<UtilitiesDataCollector *> &utility_data, vector<unsigned long> realizations) {
    // NOTE: FOR NOW, THIS METHOD TO CALCULATE INFRA NPV DOES NOT CATCH DEBT SERVICE DUE AFTER
    // MODELING PERIOD ENDS. MAY BE BEST TO PRIMARILY USE THIS CALCULATION STYLE WITHIN THE UNIT COST
    // OBJECTIVE CALCULATION. USING THE ORIGINAL INFRA NPV CALCULATION AS AN OBJECTIVE WILL STILL WORK
    // BUT WILL BE BASED ON INITIAL ALLOCATIONS FOR THE VARIABLE INTEREST RATE AND VARIABLE DEBT SERVICE BOND CLASSES.
    unsigned long n_realizations = utility_data.size();
    if (realizations.empty()) {
        realizations = vector<unsigned long>(n_realizations);
        iota(realizations.begin(), realizations.end(), 0);
    } else {
        n_realizations = realizations.size();
    }

    double infrastructure_npc = 0;
    for (const unsigned long &r : realizations) {
        auto realization = utility_data[r];
        infrastructure_npc += accumulate(
                realization->getPresent_value_debt_service_payments().begin(),
                realization->getPresent_value_debt_service_payments().end(),
                0.);
    }

    return infrastructure_npc / n_realizations;
}
