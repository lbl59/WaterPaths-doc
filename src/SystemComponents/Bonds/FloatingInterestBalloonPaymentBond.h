//
// Created by bernardo on 4/17/18.
//

#ifndef TRIANGLEMODEL_FLOATINGINTERESTBOND_H
#define TRIANGLEMODEL_FLOATINGINTERESTBOND_H


#include "Base/Bond.h"

class FloatingInterestBalloonPaymentBond : public Bond {
private:
    /**
     * @brief The interest rate time series beginning from week 0 of simulation.
     */
    vector<double> interest_rate_series;
    /**
     * @brief The time after which bond repayment begins.
     */
    const int begin_repayment_after_n_years;
    /**
     * @brief Number of payments made so far.
     */
    int n_payments_made = 0;

public:
    /**
     * @brief Constructs a FloatingInterestBalloonPaymentBond object with specified attributes.
     *        This function initializes a floating interest balloon payment bond with the provided parameters.
     * 
     * @param id The unique identifier for the bond.
     * @param cost_of_capital The cost of capital associated with the bond. Must be non-negative.
     * @param n_payments The total number of payments for the bond.
     * @param interest_rate_series A vector representing the series of interest rates for the bond.
     * @param pay_on_weeks A vector specifying the weeks when payments are due.
     * 
     * @return None
     */
    FloatingInterestBalloonPaymentBond(const int id, const double cost_of_capital, const double n_payments,
                         const vector<double> interest_rate_series, vector<int> pay_on_weeks);

    /**
     * @brief Constructs a FloatingInterestBalloonPaymentBond object with specified attributes.
     *        This function initializes a floating interest balloon payment bond with the provided parameters and 
     *        sets the start of repayment after a specified number of years.
     * 
     * @param id The unique identifier for the bond.
     * @param cost_of_capital The cost of capital associated with the bond. Must be non-negative.
     * @param n_payments The total number of payments for the bond.
     * @param interest_rate_series A vector representing the series of interest rates for the bond.
     * @param pay_on_weeks A vector specifying the weeks when payments are due.
     * @param starts_paying_after_n_years The number of years after which repayment begins.
     * 
     * @return None
     */
    FloatingInterestBalloonPaymentBond(const int id, const double cost_of_capital, double n_payments,
                         const vector<double> interest_rate_series, vector<int> pay_on_weeks,
                         const int starts_paying_after_n_years);

    /**
     * @brief Calculates the debt service for the bond at a specified week.
     *        This function computes the bond's debt service payment based on the floating interest rate at a specific week.
     * 
     * @param week The current week for which to calculate the debt service.
     * 
     * @return double The debt service amount for the given week, or `0.0` if no payment is due.
     * 
     * @throws std::out_of_range If the interest rate series is shorter than the number of payments made.
     */
    double getDebtService(int week) override;

    /**
     * @brief Calculates the present value of the debt service for the bond at a specific week.
     *        This function computes the present value of the debt service based on the bond’s floating interest rate and a discount rate for the given week.
     * 
     * FIXME: Check calculation for this class. Clarify what a floating interest bond is.
     * @param week The current week for which to calculate the present value of the debt service.
     * @param discount_rate The discount rate used to calculate the present value of future payments.
     * 
     * @return double The present value of the debt service for the given week, or `0.0` if no payment is due.
     * 
     * @throws std::out_of_range If the interest rate series is shorter than the number of payments made.
     */
    double getPresentValueDebtService(int week, double discount_rate) override;

    /**
     * @brief Calculates the net present value (NPV) of the bond at the time of issuance.
     *        This function computes the NPV of the bond based on its cost of capital, the interest rate for the given week, and a yearly discount rate.
     * 
     * @param yearly_discount_rate The yearly discount rate used to calculate the present value.
     * @param week The week for which to calculate the NPV.
     * 
     * @return double The net present value of the bond at the time of issuance.
     */
    double getNetPresentValueAtIssuance(double yearly_discount_rate, int week) const override;

    /**
     * @brief Issues a floating interest balloon payment bond and adjusts its interest rate series.
     *        This function issues the bond and multiplies the entire interest rate series by the provided bond interest rate multiplier.
     * 
     * @param week The week when the bond is issued.
     * @param construction_time The duration of construction in weeks
     *      FIXME: not used in this function.
     * @param bond_term_multiplier A multiplier to adjust the total number of payments (`n_payments`).
     * @param bond_interest_rate_multiplier A multiplier to adjust the bond's interest rate series.
     * 
     * @return None
     */
    void issueBond(int week, int construction_time, double bond_term_multiplier, double bond_interest_rate_multiplier) override;
};


#endif //TRIANGLEMODEL_FLOATINGINTERESTBOND_H
