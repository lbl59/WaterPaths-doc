//
// Created by bernardo on 4/17/18.
//

#ifndef TRIANGLEMODEL_BALLOONPAYMENTBOND_H
#define TRIANGLEMODEL_BALLOONPAYMENTBOND_H

#include <vector>


#include "Base/Bond.h"
/**
 * Class that represents a balloon bond that pays interest for a number of years and then pays the face value of the bond plus
 * the last interest payment.
 */

class BalloonPaymentBond : public Bond {
private:
    /**
     * @brief Interest payments for each period.
     */
    double interest_payments;
    /**
     * @brief Number of years after which payments to the bond begins
     */
    const int begin_repayment_after_n_years;
    /**
     * @brief Number of payments made so far.
     */
    int n_payments_made = 0;

public:
    /**
     * @brief Constructs a BalloonPaymentBond object with specified attributes.
     *        This function initializes a balloon payment bond with the provided parameters.
     * 
     * @param id The unique identifier for the bond.
     * @param cost_of_capital The cost of capital associated with the bond. Must be non-negative.
     * @param n_payments The total number of payments for the bond.
     * @param coupon_rate The fixed coupon rate for the bond.
     * @param pay_on_weeks A vector specifying the weeks when payments are due.
     * 
     * @return None
     */
    BalloonPaymentBond(const int id, const double cost_of_capital, const int n_payments,
                         const double coupon_rate, vector<int> pay_on_weeks);

    /**
     * @brief Constructs a BalloonPaymentBond object with specified attributes, including the repayment start time.
     *        This function initializes a balloon payment bond with the provided parameters, 
     *        including the number of payments and repayment start time.
     * 
     * @param id The unique identifier for the bond.
     * @param cost_of_capital The cost of capital associated with the bond. Must be non-negative.
     * @param n_payments The total number of payments for the bond.
     * @param coupon_rate The fixed coupon rate for the bond.
     * @param pay_on_weeks A vector specifying the weeks when payments are due.
     * @param begin_repayment_after_n_years The number of years after which repayment should begin. 
     *                                      A value of `NONE` indicates no specific start time.
     * 
     * @return None
     */
    BalloonPaymentBond(const int id, const double cost_of_capital, const int n_payments, const double coupon_rate,
                       vector<int> pay_on_weeks, const int begin_repayment_after_n_years);

    /**
     * @brief Calculates the debt service for a balloon payment bond at a specific week.
     *        This function determines whether a payment is due based on the current week and the bond's payment schedule.
     *        It returns the debt service, which includes interest payments for the period and the principal payment for the final period.
     * 
     * @param week The current week for which to calculate the debt service.
     * 
     * @return double The debt service for the given week, including interest payments and principal payment if due.
     */ 
    double getDebtService(int week) override;

    /**
     * @brief Calculates the present value of the balloon payment bond's debt service for a specific week.
     *        This function computes the present value of the bond's debt service based on the interest payments, the principal payment (if due), and the discount rate.
     * 
     * @param week The current week to calculate the present value of the debt service for.
     * @param discount_rate The discount rate used to calculate the present value of the future payments.
     * 
     * @return double The present value of the debt service for the given week.
     */
    double getPresentValueDebtService(int week, double discount_rate) override;

    /**
     * @brief Calculates the net present value (NPV) of the balloon payment bond at issuance.
     *        This function computes the NPV of the bond based on the interest payments and the discount rate.
     *        The NPV is calculated using the bond's payment schedule, adjusting for the time until repayment begins.
     * 
     * @param yearly_discount_rate The annual discount rate used to calculate the present value of future payments.
     * @param week The week of the year to calculate the NPV for, typically the week the bond is issued.
     * 
     * @return double The net present value at issuance of the balloon payment bond.
     */
    double getNetPresentValueAtIssuance(double yearly_discount_rate, int week) const override;

    /**
     * @brief Issues the balloon payment bond, initializing the bond parameters and setting the interest payments.
     *        This function sets up the bond's issuance details, including adjusting the interest payments based on the bond's parameters.
     * 
     * @param week The week in which the bond is being issued.
     * @param construction_time The time it takes to complete the construction associated with the bond, in weeks.
     * @param bond_term_multiplier A multiplier for adjusting the term length of the bond.
     * @param bond_interest_rate_multiplier A multiplier for adjusting the bond's interest rate.
     * 
     * @return None
     */
    void issueBond(int week, int construction_time, double bond_term_multiplier, double bond_interest_rate_multiplier) override;
};


#endif //TRIANGLEMODEL_BALLOONPAYMENTBOND_H
