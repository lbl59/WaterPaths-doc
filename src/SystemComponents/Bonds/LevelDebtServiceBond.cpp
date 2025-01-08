//
// Created by bernardo on 4/12/18.
//

#include <cmath>
#include <algorithm>
#include "LevelDebtServiceBond.h"
#include "../../Utils/Utils.h"

LevelDebtServiceBond::LevelDebtServiceBond(const int id, const double cost_of_capital, const int n_payments,
                                           const double coupon_rate, vector<int> pay_on_weeks,
                                           bool begin_repayment_at_issuance) :
        Bond(id, cost_of_capital, n_payments, pay_on_weeks, coupon_rate, LEVEL_DEBT_SERVICE, begin_repayment_at_issuance) {
}

LevelDebtServiceBond::~LevelDebtServiceBond() = default;

double LevelDebtServiceBond::getDebtService(int week) {
    /// If there are still payments to be made AND repayment has begun,
    /// AND this is a payment week, issue payment.
    if (n_payments_made < n_payments &&
            week > week_issued + begin_repayment_after_n_years
                                 * WEEKS_IN_YEAR - 1 &&
            std::find(pay_on_weeks.begin(), pay_on_weeks.end(),
                      Utils::weekOfTheYear(week))
            != pay_on_weeks.end()) {

        // Increment the number of payments made and return level payment amount.
        n_payments_made++;
        return level_debt_service_payment;
    } else {
        // Otherwise, return 0.0
        return 0.;
    }
}

double LevelDebtServiceBond::getNetPresentValueAtIssuance(
        double yearly_discount_rate, int week) const {
    // Calculates the NPV of the bond starting at the first payment date 
    // using the fixed level debt service payments.
    double npv_at_first_payment_date =
            level_debt_service_payment
            * (1. - pow(1. + (yearly_discount_rate / pay_on_weeks.size()),
                        -n_payments)) / (yearly_discount_rate /
                                         pay_on_weeks.size());

    // Discount NPV back to issuance date
    double npv = npv_at_first_payment_date
            / pow(1. + yearly_discount_rate, begin_repayment_after_n_years);

    return npv;
}

void LevelDebtServiceBond::issueBond(int week, int construction_time,
                                     double bond_term_multiplier,
                                     double bond_interest_rate_multiplier) {
    Bond::issueBond(week, construction_time, bond_term_multiplier,
                    bond_interest_rate_multiplier);

    /// Level debt service payment value
    level_debt_service_payment = cost_of_capital * (coupon_rate
            * pow(1. + coupon_rate, n_payments)) /
                                 (pow(1. + coupon_rate, n_payments) - 1.);
}

double LevelDebtServiceBond::getPresentValueDebtService(int week, double discount_rate) {
    // NOTE: pay_on_weeks is a vector with the weeks of each CALENDAR YEAR when payments are due
    // to designate how many times (and when) per year debt service is due. Default is a
    // length-one vector with value 0 (single annual payment in first (0th) week of year)
    
    // If there are still payments to be made, repayment has begun,
    // and this is a payment week, issue payment.
    if (n_payments_made < n_payments &&
        week > week_issued + begin_repayment_after_n_years
                             * WEEKS_IN_YEAR - 1 &&
        std::find(pay_on_weeks.begin(), pay_on_weeks.end(),
                  Utils::weekOfTheYear(week))
        != pay_on_weeks.end()) {

        return level_debt_service_payment /
               pow(1. + discount_rate, round((week - week_issued)/WEEKS_IN_YEAR_ROUND - 1));
    } else {
        return 0.;
    }
}

