/**
 * @class Triangle
 * @brief The `Triangle` class is a subclass of the main `Problem` class. This class represents the six-utility North Carolina Research Triangle water
 * management and planning problem.
 * 
 * Created by Bernardo on 11/23/2017.
 */
#ifndef TRIANGLEMODEL_TRIANGLE_H
#define TRIANGLEMODEL_TRIANGLE_H

#ifdef PARALLEL
#include "../../Borg/borgmm.h"
#endif
#include "Base/Problem.h"
#include "../Simulation/Simulation.h"

using namespace std;

class Triangle : public Problem {
private:
    /**
     * @brief An integer indicating the default number utilities in the Triangle problem.
     * 
     */
    const int n_utilities = 6;
    /**
     * @brief A 2D vector of all Durham's historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_durham;
    /**
     * @brief A 2D vector of all Lower Flat River's (Raleigh) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_flat;
    /**
     * @brief A 2D vector of all Swift Creek's (Raleigh) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_swift;
    /**
     * @brief A 2D vector of all Little River's (Raleigh) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_llr;
    /**
     * @brief A 2D vector of all Crabtree Creek's (Raleigh) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_crabtree;
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
    /**
     * @brief A 2D vector of all Haw River's (Cary) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_haw;
    /**
     * @brief A 2D vector of all Clayton's (downstream) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_clayton;
    /**
     * @brief A 2D vector of all Lillington's (downstream) historical and synthetic streamflows.
     */
    vector<vector<double>> streamflows_lillington;

    /**
     * @brief A 2D vector of all Cary's historical and synthetic demands.
     */
    vector<vector<double>> demand_cary;
    /**
     * @brief A 2D vector of all Durham's historical and synthetic demands.
     */
    vector<vector<double>> demand_durham;
    /**
     * @brief A 2D vector of all Raleigh's historical and synthetic demands.
     */
    vector<vector<double>> demand_raleigh;
    /**
     * @brief A 2D vector of all OWASA's historical and synthetic demands.
     */
    vector<vector<double>> demand_owasa;
    /**
     * @brief A 2D vector of all Pittsboro's historical and synthetic demands.
     */
    vector<vector<double>> demand_pittsboro;
    /**
     * @brief A 2D vector of all Chatham's historical and synthetic demands.
     */
    vector<vector<double>> demand_chatham;

    /**
     * @brief A 2D vector of all Cary's annual demand projections.
     */
    vector<double> demand_projection_cary;
    /**
     * @brief A 2D vector of all Durham's annual demand projections.
     */
    vector<double> demand_projection_durham;
    /**
     * @brief A 2D vector of all Raleigh's annual demand projections.
     */
    vector<double> demand_projection_raleigh;
    /**
     * @brief A 2D vector of all OWASA's annual demand projections.
     */
    vector<double> demand_projection_owasa;
    /**
     * @brief A 2D vector of all Pittsboro's annual demand projections.
     */
    vector<double> demand_projection_pittsboro;
    /**
     * @brief A 2D vector of all Chatham's annual demand projections.
     */
    vector<double> demand_projection_chatham;

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
     * @brief A 2D vector of all Raleigh's demand-to-wastewater fraction.
     */
    vector<vector<double>> demand_to_wastewater_fraction_owasa_raleigh;
    /**
     * @brief A 2D vector of all Durham's demand-to-wastewater fraction.
     */
    vector<vector<double>> demand_to_wastewater_fraction_durham;
    /**
     * @brief A 2D vector of all Chatham's demand-to-wastewater fraction.
     */
    vector<vector<double>> demand_to_wastewater_fraction_chatham;

    /**
     * @brief A 2D vector of all Cary's demand classes fractions.
     */
    vector<vector<double>> caryDemandClassesFractions;
    /**
     * @brief A 2D vector of all Durham's demand classes fractions.
     */
    vector<vector<double>> durhamDemandClassesFractions;
    /**
     * @brief A 2D vector of all Raleigh's demand classes fractions.
     */
    vector<vector<double>> raleighDemandClassesFractions;
    /**
     * @brief A 2D vector of all OWASA's demand classes fractions.
     */
    vector<vector<double>> owasaDemandClassesFractions;
    /**
     * @brief A 2D vector of all Pittsboro's demand classes fractions.
     */
    vector<vector<double>> pittsboroDemandClassesFractions;
    /**
     * @brief A 2D vector of all Chatham's demand classes fractions.
     */
    vector<vector<double>> chathamDemandClassesFractions;
    
    /**
     * @brief A 2D vector of all Cary's user water prices.
     */
    vector<vector<double>> caryUserClassesWaterPrices;
    /**
     * @brief A 2D vector of all Durham's user water prices.
     */
    vector<vector<double>> durhamUserClassesWaterPrices;
    /**
     * @brief A 2D vector of all Raleigh's user water prices.
     */
    vector<vector<double>> raleighUserClassesWaterPrices;
    /**
     * @brief A 2D vector of all OWASA's user water prices.
     */
    vector<vector<double>> owasaUserClassesWaterPrices;
    /**
     * @brief A 2D vector of all Pittsboro's user water prices.
     */
    vector<vector<double>> pittsboroUserClassesWaterPrices;
    /**
     * @brief A 2D vector of all Chatham's user water prices.
     */
    vector<vector<double>> chathamUserClassesWaterPrices;

    /**
     * @brief A 2D vector of all OWASA's user price surcharges.
     */
    vector<vector<double>> owasaPriceSurcharges;

public:
    /**
     * @brief Constructor for a new Triangle object
     * 
     * @param n_weeks An integer representing the number of weeks in the simulation.
     * @param import_export_rof_table An integer indicating the ROF table import/export/ignore mode.
     */
    Triangle(unsigned long n_weeks, int import_export_rof_table);

    /**
     * @brief Destructor for the Triangle object.
     */
    ~Triangle();

#ifdef PARALLEL
    void setProblemDefinition(BORG_Problem &problem);
#endif

    /**
     * @brief Runs the North Carolina Research Triangle (6-utility) problem.
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
     * @brief Handles exceptions that occur during the simulation by printing the rank of the error-causing thread's decision variables
     *      to a file and printing the exception message to the console. 
     * 
     * @param e An exception object
     * @param s A pointer to the simulation object
     * @param objs A pointer to the objectives array
     * @param vars A pointer to the decision variables array
     * 
     * @return int 
     */
    int simulationExceptionHander(const std::exception &e, Simulation *s,
                                  double *objs, const double *vars);

    /**
     * @brief Reads in all input inflow, evaporation, and demand data for the Triangle problem.
     * 
     * @return void
     */
    void readInputData();

};


#endif //TRIANGLEMODEL_TRIANGLE_H
