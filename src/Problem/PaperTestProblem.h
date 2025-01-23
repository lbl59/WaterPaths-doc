/**
 * @class PaperTestProblem
 * @brief The `PaperTestProblem` class is a subclass of the main `Problem` class. This class represents the
 * three-utility Sedento Valley Problem. 
 * 
 */

// Created by dfg42 on 8/28/18.
#ifndef TRIANGLEMODEL_PAPERTESTPROBLEM_H
#define TRIANGLEMODEL_PAPERTESTPROBLEM_H

#ifdef  PARALLEL
#include "../../Borg/borgms.h"
#endif

#include "Base/Problem.h"
#include "../Simulation/Simulation.h"

using namespace std;

class PaperTestProblem : public Problem {
private:
    /**
     * @brief An integer indicating the default number utilities in the Triangle problem.
     * 
     */
    const int n_utilities = 3;
    /**
     * @brief A 2D vector of one of the sources of water to Granite Quarry.
     */
    vector<vector<double>> streamflows_durham;
    /**
     * @brief A 2D vector of one of the downstream streamflows.
     */
    vector<vector<double>> streamflows_clayton;
    /**
     * @brief A 2D vector of one of the sources of water to Autumn Lake.
     */
    vector<vector<double>> streamflows_flat;
    /**
     * @brief A 2D vector of one of the sources of water to Autumn Lake.
     */
    vector<vector<double>> streamflows_llr;
    /**
     * @brief A 2D vector of one of the sources of water to Sugar Creek.
     */
    vector<vector<double>> streamflows_swift;
    /**
     * @brief A 2D vector of one of the sources of water to College Rock Reservoir.
     */
    vector<vector<double>> streamflows_phils;
    /**
     * @brief A 2D vector of one of the sources of water to College Rock Reservoir.
     */
    vector<vector<double>> streamflows_cane;
    /**
     * @brief A 2D vector of one of the sources of water to College Rock Reservoir.
     */
    vector<vector<double>> streamflows_morgan;
    /**
     * @brief A 2D vector of one of the sources of water to College Rock Reservoir.
     */
    vector<vector<double>> streamflows_crabtree;
    /**
     * @brief A 2D vector of one of the sources of water to Lake Michael.
     */
    vector<vector<double>> streamflows_haw;
    /**
     * @brief A 2D vector of one source of downstream flows.
     */
    vector<vector<double>> streamflows_lillington;

    /**
     * @brief A 2D vector of all Watertown's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_durham;
    /**
     * @brief A 2D vector of all Autumn Lake's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_falls_lake;
    /**
     * @brief A 2D vector of all Fallsland's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_owasa;
    /**
     * @brief A 2D vector of all Dryville's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_little_river;
    /**
     * @brief A 2D vector of all Fallsland's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_wheeler_benson;
    /**
     * @brief A 2D vector of all Lake Michael's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_jordan_lake;

    /**
     * @brief A 2D vector of all Watertown's historical and synthetic demands.
     */
    vector<vector<double>> demand_watertown;
    /**
     * @brief A 2D vector of all Dryville's historical and synthetic demands.
     */
    vector<vector<double>> demand_dryville;
    /**
     * @brief A 2D vector of all Fallsland's historical and synthetic demands.
     */
    vector<vector<double>> demand_fallsland;

    /**
     * @brief A 2D vector of all Watertown's annual demand projections.
     */
    vector<double> demand_projection_watertown;
    /**
     * @brief A 2D vector of all Dryville's annual demand projections.
     */
    vector<double> demand_projection_dryville;
    /**
     * @brief A 2D vector of all Fallsland's annual demand projections.
     */
    vector<double> demand_projection_fallsland;

    /**
     * @brief A 2D vector of all Fallsland's demand-to-wastewater fraction.
     */
    vector<vector<double>> demand_to_wastewater_fraction_fallsland;
    /**
     * @brief A 2D vector of all Dryville's demand-to-wastewater fraction.
     */
    vector<vector<double>> demand_to_wastewater_fraction_dryville;

    /**
     * @brief A 2D vector of all Watertown's demand classes fractions.
     */
    vector<vector<double>> watertownDemandClassesFractions;
    /**
     * @brief A 2D vector of all Dryville's demand classes fractions.
     */
    vector<vector<double>> dryvilleDemandClassesFractions;
    /**
     * @brief A 2D vector of all Fallsland's demand classes fractions.
     */
    vector<vector<double>> fallslandDemandClassesFractions;

    /**
     * @brief A 2D vector of all Watertown's user classes water prices.
     */
    vector<vector<double>> watertownUserClassesWaterPrices;
    /**
     * @brief A 2D vector of all Dryville's user classes water prices.
     */
    vector<vector<double>> dryvilleUserClassesWaterPrices;
    /**
     * @brief A 2D vector of all Fallsland's user classes water prices.
     */
    vector<vector<double>> fallslandUserClassesWaterPrices;

public:
    /**
     * @brief Construct a new Paper Test Problem object.
     * 
     * @param n_weeks An integer representing the number of weeks in the simulation.
     * @param import_export_rof_table An integer flag indicating to import/export/ignore mode for ROF tables.
     */
    PaperTestProblem(unsigned long n_weeks, int import_export_rof_table);

    /** @brief Destroy the Paper Test Problem object. 
     *
    */
    ~PaperTestProblem() override;

#ifdef PARALLEL
    /**
     * @brief Set the Problem Definition object if using parallel processing.
     * 
     * @param problem The BORG_Problem object to set the problem definition for.
     */
    void setProblemDefinition(BORG_Problem &problem);
#endif

    /**
     * @brief Runs the three-utility Sedento Valley test case problem.
     * 
     * @param vars The number of variables to run the problem with.
     * @param objs The number of performance objectives to evaluate.
     * @param consts UNUSED PARAMETER
     * 
     * @return int 
     */
    int functionEvaluation(double *vars, double *objs, double *consts) override;

    /**
     * @brief Reads in all input inflow, evaporation, and demand data for the Sedento Valley problem.
     * 
     * @return void
     */
    void readInputData();
};


#endif //TRIANGLEMODEL_PAPERTESTPROBLEM_H