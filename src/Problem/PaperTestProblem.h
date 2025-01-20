/**
 * @class PaperTestProblem
 * @brief The `PaperTestProblem` class is a subclass of the main `Problem` class. This class represents the three-utility Sedento Valley test case
 * water management and planning problem.
 * 
 * Created by dfg42 on 8/28/18.
 */
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
     * @brief An integer indicating the default number utilities in the Sedento Valley problem.
     * 
     */
    const int n_utilities = 3;
    /**
     * @brief A 2D vector of all Durham's historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_durham;
    /**
     * @brief A 2D vector of all Clayton's (downstream) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_clayton;
    /**
     * @brief A 2D vector of all Lower Flat River's (Raleigh) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_flat;
    /**
     * @brief A 2D vector of all Little River's (Raleigh) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_llr;
    /**
     * @brief A 2D vector of all Swift Creek's (Raleigh) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_swift;
    /**
     * @brief A 2D vector of all Phils Creek's (OWASA) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_phils;
    /**
     * @brief A 2D vector of all Cane Creek's (OWASA) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_cane;
    /**
     * @brief A 2D vector of all Morgan Creek's (OWASA) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_morgan;
    vector<vector<double>> streamflows_crabtree;
    /**
     * @brief A 2D vector of all Haw River's (Cary) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_haw;
    /**
     * @brief A 2D vector of all Lillington's (downstream) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_lillington;
    /**
     * @brief A 2D vector of all Durham's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_durham;
    /**
     * @brief A 2D vector of all Falls Lake's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_falls_lake;
    /**
     * @brief A 2D vector of all OWASA's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_owasa;
    /**
     * @brief A 2D vector of all Little River Raleigh's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_little_river;
    /**
     * @brief A 2D vector of all Lake Wheeler and Lake Benson's historical and synthetic evaporation data.
     */
    vector<vector<double>> evap_wheeler_benson;
    /**
     * @brief A 2D vector of all Jordan Lake's historical and synthetic evaporation data.
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
     * @brief A 2D vector of all Watertown's user water prices.
     */
    vector<vector<double>> watertownUserClassesWaterPrices;
    /**
     * @brief A 2D vector of all Dryville's user water prices.
     */
    vector<vector<double>> dryvilleUserClassesWaterPrices;
    /**
     * @brief A 2D vector of all Fallsland's user water prices.
     */
    vector<vector<double>> fallslandUserClassesWaterPrices;

public:
    /**
     * @brief Constructor for a new PaperTestProblem object
     * 
     * @param n_weeks An integer representing the number of weeks in the simulation.
     * @param import_export_rof_table An integer indicating the ROF table import/export/ignore mode.
     */
    PaperTestProblem(unsigned long n_weeks, int import_export_rof_table);

    /**
     * @brief Destructor for the Triangle object.
     */
    ~PaperTestProblem() override;

#ifdef PARALLEL
    /**
     * @brief Sets the problem definition linking to Borg MOEA. 
     * 
     * @param problem The Borg MOEA problem 
     * @return void
     */
    void setProblemDefinition(BORG_Problem &problem);
#endif
    /**
     * @brief Runs the Sedento Valley problem.
     * 
     * @param vars A pointer to a double containing the number of variables in the problem.
     * @param objs A pointer to the double containing the number of objectives in the problem.
     * @param consts A pointer to the double containing the number of constraints in the problem.
     * 
     * @todo check for solutions in which a utility does not have an allocation
     * on Jordan Lake (or any generic lake) but still pay for joint treatment
     * infrastructure).
     */
    int functionEvaluation(double *vars, double *objs, double *consts) override;

    /**
     * @brief Reads in all input inflow, evaporation, and demand data for the Sedento Valley problem.
     */
    void readInputData();
};


#endif //TRIANGLEMODEL_PAPERTESTPROBLEM_H
