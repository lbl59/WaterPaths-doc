//
// Created by bernardo on 4/12/18.
//

#ifndef TRIANGLEMODEL_BONDFINANCING_H
#define TRIANGLEMODEL_BONDFINANCING_H

#include <vector>
#include <stdexcept>
#include "../../../Utils/Constants.h"

using namespace std;
using namespace Constants;

class Bond {
private:
    /**
     * @brief A boolean indicating whether the bond has been issued.
     * 
     */
    bool issued = false;
protected:
    /**
     * @brief The week the bond was issued.
     */
    int week_issued;
    /**
     * @brief The cost of capital for the bond.
     */
    double cost_of_capital;
    /**
     * @brief The fixed interest rate paid to bondholders each year, as a percentage of the bond's face value.
     */
    double coupon_rate;
    /**
     * @brief The number of payments to be made on the bond.
     */
    int n_payments;
    /**
     * @brief The number of years after issuance that the bond will start being repaid.
     */
    int begin_repayment_after_n_years = NON_INITIALIZED;
public:
    /**
     * @brief The type of bond.
     */
    const int type;
    /**
     * @brief A vector specifying the weeks when payments are due.
     */
    const vector<int> pay_on_weeks;
    /**
     * @brief The unique identifier of the bond.
     */
    const int id;

    /**
     * @brief Constructs a Bond object with specified attributes.
     *        This function initializes a bond and validates its cost of capital.
     * 
     * @param id The unique identifier for the bond.
     * @param cost_of_capital The cost of capital associated with the bond. Must be non-negative.
     * @param n_payments The total number of payments for the bond.
     * @param pay_on_weeks A vector specifying the weeks when payments are due.
     * @param type The type of bond.
     * @param begin_repayment_at_issuance Boolean indicating whether repayment starts immediately upon issuance.
     * 
     * @throws std::invalid_argument If `cost_of_capital` is NaN or negative.
     */ 
    Bond(const int id, const double cost_of_capital, const int n_payments, vector<int> pay_on_weeks,
                  const int type, bool begin_repayment_at_issuance = false);

    /**
     * @brief Constructs a Bond object with specified attributes.
     *        This function initializes a bond with a coupon rate and validates its cost of capital.
     * 
     * @param id The unique identifier for the bond.
     * @param cost_of_capital The cost of capital associated with the bond. Must be non-negative.
     * @param n_payments The total number of payments for the bond.
     * @param pay_on_weeks A vector specifying the weeks when payments are due.
     * @param coupon_rate The fixed interest rate paid to bondholders each year associated with the bond.
     * @param type The type identifier for the bond.
     * @param begin_repayment_at_issuance Boolean indicating whether repayment starts immediately upon issuance.
     * 
     * @throws std::invalid_argument If `cost_of_capital` is NaN or negative.
     */
    Bond(const int id, const double cost_of_capital, const int n_payments, vector<int> pay_on_weeks,
         const double coupon_rate, const int type, bool begin_repayment_at_issuance = false);

    /**
     * @brief Constructs a default Bond object with uninitialized values.
     *        This function initializes a Bond object with placeholder values.
     */
    Bond();

    /**
     * @brief Destroys the Bond object.
     *        This function cleans up resources used by the Bond object.
     */
    virtual ~Bond();

    Bond(const Bond&) = default;

    /**
     * @brief Gets the debt service for the bond.
     *      This function is intended to be overridden in derived classes and is not meant to be called directly on the base `Bond` class.
     * 
     * @param week The week for which to calculate total remaining value of the debt service (accounting for inflation).
     * @return double The debt service for the bond.
     */
    virtual double getDebtService(int week) = 0;

    /**
     * @brief Get the Present Value Debt Service object.
     *      This function is intended to be overridden in derived classes and is not meant to be called directly on the base `Bond` class.
     * 
     * @param week The week for which to calculate the present value of debt service.
     * @param discount_rate The discount rate to use in the calculation.
     * @return double The present value of the debt service.
     */
    virtual double getPresentValueDebtService(int week, double discount_rate) = 0;

    /**
     * @brief Sets the debt service for the bond.
     *        This function is intended to be overridden in derived classes and is not meant to be called directly on the base `Bond` class.
     * 
     * @param updated_allocated_fraction_of_annual_debt_service The updated fraction of the annual debt service allocation.
     * 
     * @return None
     * 
     * @throws std::logic_error Always throws an exception when called on the base `Bond` class.
     */
    virtual void setDebtService(double updated_allocated_fraction_of_annual_debt_service);

    /**
     * @brief Returns the bond's unique identifier.
     *        This function retrieves the `id` of the bond, which serves as its water source ID.
     * 
     * @param None
     * 
     * @return int The unique identifier (`id`) of the bond.
     */
    virtual int getWaterSourceID();

    virtual double getNetPresentValueAtIssuance(double discount_rate, int week) const = 0;

    /**
     * @brief Issues a bond and adjusts its parameters based on given multipliers.
     *        This function sets the repayment start date and modifies bond parameters upon issuance.
     * 
     * @param week The week when the bond is issued.
     * @param construction_time The duration of construction in weeks, used to calculate the repayment start date.
     * @param bond_term_multiplier A multiplier to adjust the total number of payments.
     * @param bond_interest_rate_multiplier A multiplier to adjust the bond's coupon rate.
     * 
     * @return None
     * 
     * @see Bond::setIssued
     */
    virtual void
    issueBond(int week, int construction_time, double bond_term_multiplier, double bond_interest_rate_multiplier);

    /**
     * @brief Adjusts the bond's cost of capital based on a realization factor.
     * This function modifies the bond's cost of capital using a random factor.
     * 
     * @param r An unsigned long integer representing the realization identifier (not used in the function logic).
     * @param rdm_factors A vector of random factors used to adjust the bond's cost of capital. The first element is used.
     * 
     * @return None
     * 
     * @throws std::out_of_range If `rdm_factors` is empty or does not contain at least one element.
     * 
     * @see Bond::issueBond
     */
    virtual void setRealizationWaterSource(unsigned long r, vector<double> &rdm_factors);

    /**
     * @brief Checks if the bond has been issued.
     *        This function returns whether the bond has been marked as issued.
     * 
     * @param None
     * 
     * @return bool `true` if the bond has been issued; `false` otherwise.
     */
    bool isIssued() const;

    /**
     * @brief Marks the bond as issued.
     *        This function sets the `issued` status of the bond to `true`.
     * 
     * @param None
     * 
     * @return None
     */ 
    void setIssued();

    /**
     * @brief Returns the cost of capital required to issue the bond.
     *        This function retrieves the `cost_of_capital` of the bond.
     * 
     * @param None
     * 
     * @return double The cost of capital (`cost_of_capital`) of the bond.
     */
    double getCostOfCapital();

    /**
     * @brief Adjusts the bond's cost of capital by a specified reduction.
     *        This function decreases the bond's `cost_of_capital` by the given reduction value.
     * 
     * @param reduction The amount by which to reduce the bond's `cost_of_capital`.
     * 
     * @return None
     * 
     * @throws std::logic_error If the adjusted `cost_of_capital` becomes negative.
     */
    void adjustCostOfCapital(double reduction);
};


#endif //TRIANGLEMODEL_BONDFINANCING_H
