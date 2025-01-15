/**
 * @class ObjectivesCalculator
 * @brief The ObjectivesCalculator class calculates the objectives of the model based on the data collected during the simulation.
 * 
 * Created by bernardoct on 8/25/17.
 */
#ifndef TRIANGLEMODEL_OBJECTIVESCALCULATOR_H
#define TRIANGLEMODEL_OBJECTIVESCALCULATOR_H


#include "../DataCollector/UtilitiesDataCollector.h"
#include "../DataCollector/RestrictionsDataCollector.h"

class ObjectivesCalculator {

public:
        /**
         * @brief Calculates the reliability objective based on storage capacity ratios.
         *        The reliability objective is defined as 1 minus the maximum annual failure ratio across realizations.
         *
         * @param utility_collector Vector of `UtilitiesDataCollector` pointers containing utility data for all realizations.
         * @param realizations Vector of realization indices to be evaluated. If empty, evaluates all realizations.
         * 
         * @return The calculated reliability objective as a double.
         * 
         * @throw std::logic_error if the objective value is infinite.
         */
        static double calculateReliabilityObjective(
            const vector<UtilitiesDataCollector *>& utility_collector,
            vector<unsigned long> realizations = vector<unsigned long>(0));

        /**
         * @brief Calculates the restriction frequency objective.
         *        The restriction frequency objective measures the average ratio of years with restrictions to total years across all realizations.
         *
         * @param restriction_data Vector of `RestrictionsDataCollector` pointers containing restriction data for all realizations.
         * @param realizations Vector of realization indices to be evaluated. If empty, evaluates all realizations.
         * 
         * @return The calculated restriction frequency objective as a double.
         * 
         * @throw std::logic_error if the objective value is infinite.
         */
        static double calculateRestrictionFrequencyObjective(
            const vector<RestrictionsDataCollector *>& restriction_data,
            vector<unsigned long> realizations = vector<unsigned long>(0));

        /**
         * @brief Calculates the net present cost (NPC) of infrastructure.
         *        This objective averages the total NPC across all realizations.
         *
         * @param utility_data Vector of `UtilitiesDataCollector` pointers containing utility data for all realizations.
         * @param realizations Vector of realization indices to be evaluated. If empty, evaluates all realizations.
         * @return The average NPC as a double.
         */
        static double calculateNetPresentCostInfrastructureObjective(
            const vector<UtilitiesDataCollector *>& utility_data,
            vector<unsigned long> realizations = vector<unsigned long>(0));

        /**
         * @brief Calculates the peak financial costs objective.
         *        This objective is based on the maximum annual financial cost ratio across all realizations.
         *        It will also print a statement if the total financial cost is absurdly high (>10^10).
         *
         * @param utility_data Vector of `UtilitiesDataCollector` pointers containing utility data for all realizations.
         * @param realizations Vector of realization indices to be evaluated. If empty, evaluates all realizations.
         * 
         * @return The average peak financial cost as a double.
         * 
         * @throw std::logic_error if the objective value is infinite.
         */
        static double calculatePeakFinancialCostsObjective(
            const vector<UtilitiesDataCollector *>& utility_data,
            vector<unsigned long> realizations = vector<unsigned long>(0));

        /**
         * @brief Calculates the worst-case financial costs objective.
         *        The worst-case cost is defined as the financial cost at the specified worst-case percentile across all realizations.
         *
         * @param utility_data Vector of `UtilitiesDataCollector` pointers containing utility data for all realizations.
         * @param realizations Vector of realization indices to be evaluated. If empty, evaluates all realizations.
         * 
         * @return The worst-case financial cost as a double.
         * 
         * @throw std::logic_error if the objective value is infinite.
         */
        static double calculateWorseCaseCostsObjective(
            const vector<UtilitiesDataCollector *>& utility_data,
            vector<unsigned long> realizations = vector<unsigned long>(0));

        /**
         * @brief Calculates the unit total cost objective.
         *        This objective is based on the ratio of infrastructure cost to total water demand, normalized by the first-year demand.
         *        The unit cost output is in $mil/MGD that should be converted to $/kGal for comparison with other objectives.
         *
         * @param utility_data Vector of `UtilitiesDataCollector` pointers containing utility data for all realizations.
         * @param realizations Vector of realization indices to be evaluated. If empty, evaluates all realizations.
         * 
         * @return The average unit total cost as a double.
         */
        static double calculateUnitTotalCostObjective(
            const vector<UtilitiesDataCollector *>& utility_data,
            vector<unsigned long> realizations = vector<unsigned long>(0));

        /**
         * @brief Calculates the net present cost (NPC) for variable debt service.
         *        This objective averages the total NPC for variable debt service across all realizations.
         *
         * @param utility_data Vector of `UtilitiesDataCollector` pointers containing utility data for all realizations.
         * @param realizations Vector of realization indices to be evaluated. If empty, evaluates all realizations.
         * @return The average NPC for variable debt service as a double.
         */
        static double calculateNetPresentCostInfrastructureObjectiveForVariableDebtService(
            const vector<UtilitiesDataCollector *>& utility_data,
            vector<unsigned long> realizations = vector<unsigned long>(0));
};


#endif //TRIANGLEMODEL_OBJECTIVESCALCULATOR_H
