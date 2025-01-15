/**
 * @class LevelDebtServiceBond
 * 
 * @brief The `LevelDebtServiceBond` class is a subclass of the main `Bond` class. 
 * This class represents a bond where the total amount of interest paid each year remains relatively constant.
 * 
 * Created by bernardo on 4/12/18.
 */

#ifndef TRIANGLEMODEL_LEVELDEBTSERVICEBOND_H
#define TRIANGLEMODEL_LEVELDEBTSERVICEBOND_H


#include "Base/Bond.h"

class LevelDebtServiceBond : public Bond {
private:
    /**
     * @brief The level debt service payment. This amount remains constant over the life of the bond.
     */
    double level_debt_service_payment;
    /**
     * @brief Number of payments made so far.
     */
    int n_payments_made = 0;

public:
    /**
     * @brief Constructs a LevelDebtServiceBond object with specified attributes.
     *        This function initializes a level debt service bond with the provided parameters.
     * 
     * @param id The unique identifier for the bond.
     * @param cost_of_capital The cost of capital associated with the bond. Must be non-negative.
     * @param n_payments The total number of payments for the bond.
     * @param coupon_rate The fixed coupon rate for the bond.
     * @param pay_on_weeks A vector specifying the weeks when payments are due.
     * @param begin_repayment_at_issuance Flag indicating whether repayment should begin at issuance.
     * 
     * @return None
     */
    LevelDebtServiceBond(const int id, const double cost_of_capital, const int n_payments,
                             const double coupon_rate, vector<int> pay_on_weeks, bool begin_repayment_at_issuance = false);

    /**
     * @brief Destructor for the LevelDebtServiceBond class.
     *        This destructor cleans up the resources associated with the LevelDebtServiceBond object.
     * 
     * @param None
     * 
     * @return None
     */
    ~LevelDebtServiceBond() override;

    /**
     * @brief Calculates the debt service for the bond at a specific week.
     *        This function computes the bond's debt service payment based on a level debt service structure, where payments are the same amount for each period.
     * 
     * @param week The current week for which to calculate the debt service.
     * 
     * @return double The debt service amount for the given week, or `0.0` if no payment is due.
     */
    double getDebtService(int week) override;

    /**
     * @brief Calculates the present value of the debt service for the bond at a specific week.
     *        This function computes the present value of the debt service payment based on the level debt service structure and a discount rate for the given week.
     * 
     * @param week The current week for which to calculate the present value of the debt service.
     * @param discount_rate The discount rate used to calculate the present value of future payments.
     * 
     * @return double The present value of the debt service for the given week, or `0.0` if no payment is due.
     * 
     * @throws None
     */
    double getPresentValueDebtService(int week, double discount_rate) override;

    /**
     * @brief Calculates the net present value (NPV) of the bond at the time of issuance.
     *        This function computes the NPV of the bond based on its level debt service payments, discount rate, and the number of payments.
     * 
     * @param yearly_discount_rate The yearly discount rate used to calculate the present value.
     * @param week The week for which to calculate the NPV. This is used to adjust for the time of the first payment.
     * 
     * @return double The net present value of the bond at the time of issuance.
     * 
     * @throws None
     */
    double getNetPresentValueAtIssuance(double yearly_discount_rate, int week) const override;

    /**
     * @brief Issues a level debt service bond and calculates the level debt service payment.
     *        This function issues the bond and calculates the fixed level debt service payment based on the cost of capital, coupon rate, and number of payments.
     * 
     * @param week The week when the bond is issued.
     * @param construction_time The duration of construction in weeks, used to determine when repayment begins.
     * @param bond_term_multiplier A multiplier to adjust the total number of payments (`n_payments`).
     * @param bond_interest_rate_multiplier A multiplier to adjust the bond's coupon rate.
     * 
     * @return None
     */
    void issueBond(int week, int construction_time, double bond_term_multiplier, double bond_interest_rate_multiplier) override;
};


#endif //TRIANGLEMODEL_LEVELDEBTSERVICEBOND_H
