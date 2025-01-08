//
// Created by dgorelic on 11/4/2019.
//

#include <cmath>
#include <algorithm>
#include "VariableDebtServiceBond.h"
#include "../../Utils/Utils.h"

VariableDebtServiceBond::VariableDebtServiceBond(const int id, const int water_source_id,
                                                 const double total_unallocated_cost_of_capital,
                                                 const double initial_fraction_of_allocated_debt,
                                                 const int n_payments, const double coupon_rate,
                                                 vector<int> pay_on_weeks, bool begin_repayment_at_issuance)
                : Bond(id, total_unallocated_cost_of_capital, n_payments, pay_on_weeks, coupon_rate,
                       VARIABLE_INTEREST, begin_repayment_at_issuance),
                       initial_debt_fraction(initial_fraction_of_allocated_debt),
                       water_source_id(water_source_id) {}

VariableDebtServiceBond::~VariableDebtServiceBond() = default;

double VariableDebtServiceBond::getDebtService(int week) {
    // If there are still payments to be made, repayment has begun,
    // and this is a payment week, issue payment.
    // June 2020: appears to be some way that debt service can go to -nan if the
    //  fraction of capital expenses a utility is responsible for is 0, so
    //  catch this and correct the value if it occurs
    if (isnan(variable_debt_service_payment))
        variable_debt_service_payment = 0;

    // If the number of payments made is less than the total payments, repayment has begun.
    // If the current week is a payment week (determined from `pay_on_weeks`), the debt service payment is issued
    if (n_payments_made < n_payments &&
        week > week_issued + begin_repayment_after_n_years
                             * WEEKS_IN_YEAR - 1 &&
        std::find(pay_on_weeks.begin(), pay_on_weeks.end(),
                  Utils::weekOfTheYear(week))
        != pay_on_weeks.end()) {

        n_payments_made++;
        return variable_debt_service_payment;
    } else {
        return 0.;
    }
}


double VariableDebtServiceBond::getNetPresentValueAtIssuance(double yearly_discount_rate, int week) const {
    // this is basically useless for a variable rate bond - need to have calculation at end of realization as well
    double npv_at_first_payment_date =
            variable_debt_service_payment
            * (1. - pow(1. + (yearly_discount_rate / pay_on_weeks.size()),
                        -n_payments)) / (yearly_discount_rate /
                                         pay_on_weeks.size());
    
    // NPV at the first payment date is discounted by the delay before repayment starts
    double npv = npv_at_first_payment_date
                 / pow(1. + yearly_discount_rate, begin_repayment_after_n_years);

    return npv;
}

void VariableDebtServiceBond::issueBond(int week, int construction_time, double bond_term_multiplier,
                                        double bond_interest_rate_multiplier) {
    Bond::issueBond(week, construction_time, bond_term_multiplier, bond_interest_rate_multiplier);

    /// Variable debt service payment value
    variable_debt_service_payment = cost_of_capital * initial_debt_fraction * (coupon_rate
                                                    * pow(1. + coupon_rate, n_payments)) /
                                 (pow(1. + coupon_rate, n_payments) - 1.);
}

void VariableDebtServiceBond::setDebtService(double updated_allocated_fraction_of_annual_debt_service) {
    variable_debt_service_payment =
            cost_of_capital * updated_allocated_fraction_of_annual_debt_service
            * (coupon_rate * pow(1. + coupon_rate, n_payments)) / (pow(1. + coupon_rate, n_payments) - 1.);
}

int VariableDebtServiceBond::getWaterSourceID() {
    return water_source_id;
}

double VariableDebtServiceBond::getPresentValueDebtService(int week, double discount_rate) {
    // NOTE: pay_on_weeks is a vector with the weeks of each CALENDAR YEAR when payments are due
    // to designate how many times (and when) per year debt service is due. default is a
    // length-one vector with value 0 (single annual payment in first (0th) week of year)
    
    // If there are still payments to be made, repayment has begun,
    // and this is a payment week, issue payment.
    if (n_payments_made < n_payments &&
        week > week_issued + begin_repayment_after_n_years
                             * WEEKS_IN_YEAR - 1 &&
        std::find(pay_on_weeks.begin(), pay_on_weeks.end(),
                  Utils::weekOfTheYear(week))
        != pay_on_weeks.end()) {

        return variable_debt_service_payment /
               pow(1. + discount_rate, round((week - week_issued)/WEEKS_IN_YEAR_ROUND - 1));
    } else {
        return 0.;
    }
}
