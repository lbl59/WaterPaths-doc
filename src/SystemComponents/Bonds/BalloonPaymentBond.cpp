//
// Created by bernardo on 4/17/18.
//

#include <cmath>
#include <algorithm>
#include "BalloonPaymentBond.h"
#include "../../Utils/Utils.h"

BalloonPaymentBond::BalloonPaymentBond(const int id, const double cost_of_capital, const int n_payments,
                                       const double coupon_rate, vector<int> pay_on_weeks) :
    Bond(id, cost_of_capital, n_payments, pay_on_weeks, coupon_rate, BALLOON_PAYMENT),
    begin_repayment_after_n_years(NONE) {}

/**
 * Balloon payment at maturity corresponding to face value + last interest payment.
 * @param id
 * @param cost_of_capital
 * @param n_payments
 * @param coupon_rate
 * @param pay_on_weeks
 * @param begin_repayment_after_n_years
 */
BalloonPaymentBond::BalloonPaymentBond(const int id, const double cost_of_capital, const int n_payments,
                                       const double coupon_rate, vector<int> pay_on_weeks,
                                       const int begin_repayment_after_n_years) :
        Bond(id, cost_of_capital, n_payments, pay_on_weeks, coupon_rate, BALLOON_PAYMENT),
        begin_repayment_after_n_years(begin_repayment_after_n_years) {}

double BalloonPaymentBond::getDebtService(int week) {
    /// If there are still payments to be made AND repayment has begun AND this is a payment week -> issue payment.
    if (n_payments_made < n_payments - 1 &&
          week > week_issued + begin_repayment_after_n_years * WEEKS_IN_YEAR &&
          std::find(pay_on_weeks.begin(), pay_on_weeks.end(), Utils::weekOfTheYear(week)) != pay_on_weeks.end()) {
        
        // Increment the number of payments made and return the interest payment.
        n_payments_made++;
        return interest_payments;

    } else if (n_payments_made == n_payments - 1) {
        // If this is the last payment  pay principal and last interest.
        n_payments_made++;
        return interest_payments + cost_of_capital;
    } else {
        // Otherwise, return 0.
        return 0.;
    }
}

double BalloonPaymentBond::getNetPresentValueAtIssuance(double yearly_discount_rate, int week) const {
    double npv_at_first_payment_date = interest_payments *
                                       (1. - pow(1. + (yearly_discount_rate / pay_on_weeks.size()), -n_payments)) /
                                       (yearly_discount_rate / pay_on_weeks.size());

    return npv_at_first_payment_date / pow(1. + yearly_discount_rate, begin_repayment_after_n_years);
}

void BalloonPaymentBond::issueBond(int week, int construction_time, double bond_term_multiplier,
                                   double bond_interest_rate_multiplier) {
    Bond::issueBond(week, 0, bond_term_multiplier, bond_interest_rate_multiplier);

    // Interest to be paid every pay period.
    interest_payments = coupon_rate * cost_of_capital;
}

double BalloonPaymentBond::getPresentValueDebtService(int week, double discount_rate) {
    // NOTE: pay_on_weeks is a vector with the weeks of each CALENDAR YEAR when payments are due
    // to designate how many times (and when) per year debt service is due. default is a
    // length-one vector with value 0 (single annual payment in first (0th) week of year)
    
    // FIXME: NOT SURE THIS IS A CORRECT CALCULATION FOR THIS CLASS
    
    // If there are still payments to be made AND repayment has begun AND this is a payment week -> issue payment.
    if (n_payments_made < n_payments - 1 &&
        week > week_issued + begin_repayment_after_n_years * WEEKS_IN_YEAR &&
        std::find(pay_on_weeks.begin(), pay_on_weeks.end(), Utils::weekOfTheYear(week)) != pay_on_weeks.end()) {
        
        return (interest_payments / pay_on_weeks.size()) /
               pow(1. + discount_rate, round((week - week_issued)/WEEKS_IN_YEAR_ROUND - 1));
    } else if (n_payments_made == n_payments - 1) {
        /// Pay principal and last interest.
        return ((interest_payments + cost_of_capital) / pay_on_weeks.size()) /
               pow(1. + discount_rate, round((week - week_issued)/WEEKS_IN_YEAR_ROUND - 1));
    } else {
        return 0.;
    }
}
