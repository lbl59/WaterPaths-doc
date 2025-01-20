/**
 * @class VariableDebtServiceBond
 * 
 * @brief The `VariableDebtServiceBond` class is a subclass of the main `Bond` class. 
 * This class represents a bond (typically a municipal bond) where the floating coupon payments are 
 * adjusted at specified intervals. This bond is associated with a specific water source.
 * 
 */

// Created by dgorelic on 11/4/2019.
#ifndef TRIANGLEMODEL_VARIABLEDEBTSERVICEBOND_H
#define TRIANGLEMODEL_VARIABLEDEBTSERVICEBOND_H


#include "Base/Bond.h"

class VariableDebtServiceBond : public Bond {
private:
    /**
     * @brief The value of variable debt service for this bond.
     */
    double variable_debt_service_payment;
    /**
     * @brief The initial fraction of allocated debt.
     */
    double initial_debt_fraction;
    /**
     * @brief The ID of the water source associated with this bond.
     */
    int water_source_id;
    /**
     * @brief The number of payments made.
     */
    int n_payments_made = 0;

public:
    /**
     * @brief Constructs a variable debt service bond with specified parameters.
     *        This constructor initializes the bond with a set of parameters specific to variable debt service bonds,
     *        including the bond's debt allocation and associated water source ID.
     * 
     * @param id The unique identifier for the bond.
     * @param water_source_id The identifier for the water source associated with the bond.
     * @param total_unallocated_cost_of_capital The total cost of capital for the bond that has not been allocated yet.
     * @param initial_fraction_of_allocated_debt The initial fraction of the allocated debt for the bond.
     * @param n_payments The number of payments to be made on the bond.
     * @param coupon_rate The interest rate to be applied to the bond's payments.
     * @param pay_on_weeks A vector of weeks in each calendar year when payments are due.
     * @param begin_repayment_at_issuance A boolean indicating whether repayment begins at the issuance date.
     * 
     * @return None
     */
    VariableDebtServiceBond(const int id, const int water_source_id,
                            const double total_unallocated_cost_of_capital,
                            const double initial_fraction_of_allocated_debt,
                            const int n_payments,
                            const double coupon_rate,
                            vector<int> pay_on_weeks,
                            bool begin_repayment_at_issuance = false);

    /**
     * @brief Destroys the variable debt service bond object.
     *        This destructor cleans up resources associated with the variable debt service bond. 
     *        It is automatically called when an object of the `VariableDebtServiceBond` class goes out of scope or is deleted.
     * 
     * @return None
     */
    ~VariableDebtServiceBond() override;

    /**
     * @brief Calculates and returns the debt service payment for a given week.
     *        This function calculates the debt service for the bond, checking if repayments are due and handling
     *        any corrections if the calculated debt service value is NaN.
     * 
     * @param week The week number for which the debt service is to be calculated.
     * 
     * @return double The calculated debt service for the specified week. If no payment is due for the week, returns 0.
     */
    double getDebtService(int week) override;

    /**
     * @brief Calculates the present value of the debt service for a given week.
     * 
     * This function calculates the present value of the debt service payment for a specific week,
     * taking into account the discount rate. It checks if the bond is in its repayment phase,
     * and if the current week is a designated payment week.
     * 
     * @param week The current week for which the debt service present value is calculated.
     * @param discount_rate The annual discount rate used for present value calculations.
     * 
     * @return double The present value of the debt service for the given week.
     */
    double getPresentValueDebtService(int week, double discount_rate) override;

    /**
     * @brief Calculates the net present value (NPV) at issuance for the variable debt service bond.
     *        This calculation is generally not applicable for a variable rate bond and might require an additional
     *        calculation at the end of realization. This method is intended as a placeholder.
     * 
     * @param yearly_discount_rate The annual discount rate used for the NPV calculation.
     * @param week The week number at which the NPV is to be calculated.
     * 
     * @return double The calculated NPV at issuance.
     */
    double getNetPresentValueAtIssuance(double yearly_discount_rate, int week) const override;

    /**
     * @brief Issues the bond, initializes bond details, and calculates the variable debt service payment.
     *        This method calculates the initial variable debt service payment for the bond based on the bond's
     *        cost of capital, debt fraction, coupon rate, and the number of payments.
     * 
     * @param week The week when the bond is issued.
     * @param construction_time The time (in weeks) it takes to complete construction.
     * @param bond_term_multiplier A multiplier that adjusts the term of the bond.
     * @param bond_interest_rate_multiplier A multiplier that adjusts the interest rate of the bond.
     * 
     * @return void
     */
    void issueBond(int week, int construction_time, double bond_term_multiplier,
                   double bond_interest_rate_multiplier) override;

    /**
     * @brief Calculates and updates the variable debt service payment based on the provided allocated fraction of annual debt service.
     *      Overrides the virtual base class method.
     * 
     * @param updated_allocated_fraction_of_annual_debt_service The new fraction of the annual debt service to be allocated
     *        to the bond.
     * 
     * @return void
     */
    void setDebtService(double updated_allocated_fraction_of_annual_debt_service) override;

    /**
     * @brief Retrieves the water source ID associated with the bond.
     *      Overrides the virtual base class method.
     * 
     * This function returns the water source ID that is associated with the bond. The ID can be used
     * to track or link the bond to a specific water source.
     * 
     * @return int The water source ID associated with the bond.
     */
    int getWaterSourceID() override;
};


#endif //TRIANGLEMODEL_VARIABLEDEBTSERVICEBOND_H
