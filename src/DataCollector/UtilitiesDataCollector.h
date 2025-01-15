/**
 * @class UtilitiesDataCollector
 * 
 * @brief The `UtilitiesDataCollector` class is a subclass of the `DataCollector` class that implements the data collection of Utility objects.
 * It is the main subclass that results in the Utilities' output CSV files.
 * 
 * Created by bernardoct on 8/25/17.
 */

#ifndef TRIANGLEMODEL_UTILITIESDATACOLLECTOR_H
#define TRIANGLEMODEL_UTILITIESDATACOLLECTOR_H

#include "Base/DataCollector.h"
#include "../SystemComponents/Utility/Utility.h"

class UtilitiesDataCollector : public DataCollector {
private:
    /**
     * @brief A vector of weekly short-term ROFs for a utility taken as the max between the storage and treatment short-term ROFs.
     */
    vector<double> st_rof;
    /**
     * @brief A vector of weekly short-term storage ROFs for a utility.
     */
    vector<double> st_stor_rof;
    /**
     * @brief A vector of weekly short-term treatment ROFs for a utility.
     */
    vector<double> st_trmt_rof;
    /**
     * @brief A vector of weekly long-term ROFs for a utility taken as the max between the storage and treatment long-term ROFs.
     */
    vector<double> lt_rof;
    /**
     * @brief A vector of weekly long-term storage ROFs for a utility.
     */
    vector<double> lt_stor_rof;
    /**
     * @brief A vector of weekly long-term treatment ROFs for a utility.
     */
    vector<double> lt_trmt_rof;
    /**
     * @brief A vector of weekly combined storage for a utility.
     */
    vector<double> combined_storage;
    /**
     * @brief A vector of weekly unrestricted demand for a utility.
     */
    vector<double> unrestricted_demand;
    /**
     * @brief A vector of weekly restricted demand for a utility.
     */
    vector<double> restricted_demand;
    /**
     * @brief A vector of weekly cumulative contingency fund sizes for a utility.
     */
    vector<double> contingency_fund_size;
    /**
     * @brief A vector of weekly gross revenues for a utility.
     */
    vector<double> gross_revenues;
    /**
     * @brief A vector of weekly contingency fund contributions for a utility.
     */
    vector<double> contingency_fund_contribution;
    /**
     * @brief A vector of weekly debt service payments for a utility.
     */
    vector<double> debt_service_payments;
    /**
     * @brief A vector of weekly present-valued debt service payments for a utility.
     */
    vector<double> present_valued_debt_service_payments;
    /**
     * @brief A vector of weekly insurance contract costs for a utility.
     */
    vector<double> insurance_contract_cost;
    /**
     * @brief A vector of weekly insurance payouts received by a utility from the insurance company.
     */
    vector<double> insurance_payout;
    /**
     * @brief A vector of weekly drought mitigation costs for a utility.
     */
    vector<double> drought_mitigation_cost;
    /**
     * @brief A vector of weekly net present infrastructure costs for a utility.
     */
    vector<double> capacity;
    /**
     * @brief A vector of weekly net present infrastructure costs for a utility.
     */
    vector<double> net_present_infrastructure_cost;
    /**
     * @brief A vector of weekly waste water discharge for a utility.
     */
    vector<double> waste_water_discharge;
    /**
     * @brief A vector of weekly unfulfilled demand for a utility.
     */
    vector<double> unfulfilled_demand;
    /**
     * @brief A vector of weekly net stream inflow for a utility.
     */
    vector<double> net_stream_inflow;
    /**
     * @brief A vector of weekly total treatment capacity for a utility.
     */
    vector<double> total_treatment_capacity;
    /**
     * @brief A vector of weekly final stored volume for a utility across all its water sources.
     */
    vector<double> final_stored_volume;
    /**
     * @brief A vector of weekly recorded annual demand for a utility.
     * This value stays constant for all weeks within the same year.
     */
    vector<double> recorded_annual_demand;
    /**
     * @brief A vector of annual projected demand estimates for a utility.
     */
    vector<double> projected_demand_estimate;
    /**
     * @brief A 2D vector of infrastructure pathways for a utility.
     */
    vector<vector<int>> pathways;
    /**
     * @brief A pointer to the utility object that this data collector is collecting data for.
     */
    const Utility *utility;

public:
    /**
     * @brief Constructs a `UtilitiesDataCollector` for managing and collecting data related to a specific utility.
     *        Initializes the data collector with utility-specific attributes.
     * 
     * @param utility Pointer to the `Utility` object for which the data collector is being created.
     * @param realization Unsigned long integer representing the realization ID.
     */
    explicit UtilitiesDataCollector(const Utility *utility, unsigned long realization);

    /**
     * @brief Overloads the assignment operator for the `UtilitiesDataCollector` class, allowing assignment of one data collector to another.
     *        This operator performs a deep copy of the data collector's attributes.
     * 
     * This function does not have an implementation in the .cpp file.
     * 
     * @param utility_data_collector A constant reference to the `UtilitiesDataCollector` object to be assigned.
     * 
     * @return A reference to the updated `UtilitiesDataCollector` object after the assignment.
     */
    UtilitiesDataCollector &operator=(const UtilitiesDataCollector &utility_data_collector);

    /**
     * @brief Generates a tabular string representation of utility data for a specified week.
     *        This includes data such as combined storage, capacity, net inflow, ROFs, demand, insurance, 
     *        contingency funds, debt service payments, and revenues.
     * 
     * This function overrides the `printTabularString` function in the `DataCollector` class.
     * 
     * @param week Integer representing the week for which the tabular data is generated.
     * 
     * @return A string containing the tabular representation of the utility data for the specified week.
     */
    string printTabularString(int week) override;

    /**
     * @brief Generates a compact, comma-delimited representation of utility data for a specified week.
     *        This includes data such as combined storage, capacity, net inflow, ROFs, demand, insurance, 
     *        contingency funds, debt service payments, and revenues.
     * 
     * This function overrides the `printCompactString` function in the `DataCollector` class.
     * 
     * @param week Integer representing the week for which the tabular data is generated.
     * 
     * @return A string containing the compact representation of the utility data for the specified week.
     */
    string printCompactString(int week) override;

    /**
     * @brief Generates the first header line for the tabular representation of utility data.
     *        This header provides general column titles for data such as combined storage, capacity, 
     *        net inflow, ROFs, demand, insurance, contingency funds, debt service payments, and revenues.
     * 
     * This function overrides the `printTabularStringHeaderLine1` function in the `DataCollector` class.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine1() override;

    /**
     * @brief Generates the second header line for the tabular representation of utility data.
     *        This header provides general column titles for data such as combined storage, capacity, 
     *        net inflow, ROFs, demand, insurance, contingency funds, debt service payments, and revenues.
     * 
     * This function overrides the `printTabularStringHeaderLine2` function in the `DataCollector` class.
     * 
     * @return A string containing the first header line for the tabular representation.
     */
    string printTabularStringHeaderLine2() override;

    /**
     * @brief Generates the header line for the compact, comma-delimited representation of utility data.
     *        This header provides general column titles for data such as combined storage, capacity, 
     *        net inflow, ROFs, demand, insurance, contingency funds, debt service payments, and revenues.
     * 
     * This function overrides the `printCompactStringHeader` function in the `DataCollector` class.
     * 
     * @return A string containing the header line for the compact, comma-delimited representation.
     */
    string printCompactStringHeader() override;

    /**
     * @brief Collects data for the utility and appends it to the respective data vectors.
     *        This includes combined storage, capacity, 
     *        net inflow, ROFs, demand, insurance, contingency funds, debt service payments, revenues, 
     *        and infrastructure net present costs.
     * 
     * This function overrides the `collect_data` function in the `DataCollector` class.
     * 
     * @return void
     */
    void collect_data() override;

    /**
     * @brief Retrieves the vector of combined storage data collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the combined storage data for the utility.
     */
    const vector<double> &getCombined_storage() const;

    /**
     * @brief Retrieves the vector of storage capacity data collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the storage capacity data for the utility.
     */
    const vector<double> &getCapacity() const;

    /**
     * @brief Retrieves the vector of gross revenue data collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the gross revenue data for the utility.
     */
    const vector<double> &getGross_revenues() const;

    /**
     * @brief Retrieves the vector of contingency fund contribution data collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the contingency fund contribution data for the utility.
     */
    const vector<double> &getContingency_fund_contribution() const;

    /**
     * @brief Retrieves the vector of weekly debt service payments collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the weekly debt service payments made by the utility.
     */
    const vector<double> &getDebt_service_payments() const;

    /**
     * @brief Retrieves the vector of weekly insurance contract costs paid by the utility collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the weekly insurance contract costs paid by the utility.
     */
    const vector<double> &getInsurance_contract_cost() const;

    /**
     * @brief Retrieves the vector of drought mitigation costs paid by the utility collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the contingency fund contribution data for the utility.
     */
    const vector<double> &getDrought_mitigation_cost() const;

    /**
     * @brief Retrieves the vector of a utility's net contingency fund sizes as it changes across all weeks collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the contingency fund size data for the utility.
     */
    const vector<double> &getContingency_fund_size() const;

    /**
     * @brief Retrieves the 2D vector of infrastructure pathways (infrastructure IDs and their associated realizations) build by the utility.
     * 
     * @return A constant reference to a 2D vector of integers a utility's infrastructure pathways.
     */
    const vector<vector<int>> &getPathways() const;

    /**
     * @brief Retrieves the vector of a utility's weekly net infrastructure net present cost collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the utility's infrastructure net present cost.
     */
    const vector<double> &getNet_present_infrastructure_cost() const;

    /**
     * @brief Retrieves the vector of a utility's weekly short-term ROF collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the utility's short-term ROF.
     */
    const vector<double> &getSt_rof() const;

    /**
     * @brief Retrieves the vector of a utility's weekly long-term ROF collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the utility's long-term ROF.
     */
    const vector<double> &getLt_rof() const;

    /**
     * @brief Retrieves the vector of a utility's weekly volumetric restricted demand collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the utility's weekly restricted demand.
     */
    const vector<double> &getRestricted_demand() const;

    /**
     * @brief Retrieves the vector of a utility's weekly volumetric unrestricted demand collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the utility's weekly unrestricted demand.
     */
    const vector<double> &getUnrestricted_demand() const;

    /**
     * @brief Retrieves the vector of a utility's recorded (historical) annual demand collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the utility's weekly restricted demand.
     */
    const vector<double> &getRecorded_annual_demand() const;

    /**
     * @brief Retrieves the vector of the present value of a utility's weekly debt service payments collected by the `UtilitiesDataCollector`.
     * 
     * @return A constant reference to a vector of doubles containing the utility's weekly restricted demand.
     */
    const vector<double> &getPresent_value_debt_service_payments() const;   

    /**
     * @brief Checks for `NaN` values in the most recent data entries collected by the `UtilitiesDataCollector`.
     *        Throws a runtime error if any `NaN` value is detected.
     * 
     * @return void
     * 
     * @throw std::runtime_error If a `NaN` value is detected in any of the data fields for the utility.
     *        Additionally, logs a warning if the net present infrastructure cost exceeds an absurdly high threshold.
     */
    void checkForNans() const;
};


#endif //TRIANGLEMODEL_UTILITIESDATACOLLECTOR_H
