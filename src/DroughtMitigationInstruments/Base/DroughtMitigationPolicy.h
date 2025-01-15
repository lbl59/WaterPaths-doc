/**
 * @class DroughtMitigationPolicy
 * @brief The `DroughtMitigationPolicy` class is an abstract class that defines the interface for drought mitigation policies.
 * Created by bernardo on 2/6/17.
 */

#ifndef TRIANGLEMODEL_DROUGHTMITIGATIONPOLICY_H
#define TRIANGLEMODEL_DROUGHTMITIGATIONPOLICY_H

#include "../../SystemComponents/Utility/Utility.h"
#include "../../Utils/Constants.h"
#include "../../Utils/Graph/Graph.h"
#include "../../Utils/Matrices.h"
#include "../../Controls/Base/MinEnvFlowControl.h"

class DroughtMitigationPolicy {
private:
    /**
     * @brief A vector of 2D matrices that maps the storage of a utility to the ROF of that utility.
     */
    vector<Matrix2D<double>> *storage_to_rof_table_;

protected:

    /**
     * @brief Constructs a new DroughtMitigationPolicy object by copying an existing one.
     * This function initializes a new DroughtMitigationPolicy object by copying the `id` and `type`
     * attributes from another instance.
     *
     * @param drought_mitigation_policy The DroughtMitigationPolicy object to be copied.
     *
     * @return None. This is a constructor.
     */
    DroughtMitigationPolicy(const DroughtMitigationPolicy &drought_mitigation_policy);

    /**
     * @brief A vector of the utility IDs associated with the DroughtMitigationPolicy.
     */
    vector<int> utilities_ids;
    /** 
     * @brief A vector of pointers to the utilities associated with the DroughtMitigationPolicy in a given realization. 
     */
    vector<Utility *> realization_utilities;
    /** 
     * @brief A vector of pointers to the water sources associated with the DroughtMitigationPolicy in a given realization. 
     */
    vector<WaterSource *> realization_water_sources;
    /** 
     * @brief A vector of pointers to the MinEnvFlowControls associated with the DroughtMitigationPolicy in a given realization. 
     */
    vector<MinEnvFlowControl *> realization_min_env_flow_controls;
    /** 
     * @brief A 2D vector of pointers to the RDM factors for all utilities associated with the DroughtMitigationPolicy in a given realization. 
     */
    vector<vector<double>> *rdm_factors_all;
    /** 
     * @brief A pointer to the RDM factors for the current realization. 
     */
    double *rdm_factors_realization;
    /**
     * @brief A boolean flag indicating whether to use imported ROF tables.
     */
    bool use_imported_tables;

    /**
     * @brief Retrieves the risk-of-failure (ROF) value for a given utility, week, and tier from a given realization.
     * This function accesses the storage-to-ROF table to fetch the corresponding ROF value.
     *
     * @param utility_id The ID of the utility for which the ROF value is to be retrieved.
     * @param week The week for which the ROF value is needed.
     * @param tier The tier for which the ROF value is required.
     *
     * @return The ROF value as a double for the specified utility, week, and tier from a given realization.
     */
    double getRofFromRealizationTable(int utility_id, int week, int tier);

public:
    /**
     * @brief The unique ID of the DroughtMitigationPolicy object. 
     */
    const int id;
    /**
     * @brief The type of DroughtMitigationPolicy object.
     */
    const int type;

    /**
     * @brief Constructs a new DroughtMitigationPolicy object with specified attributes.
     * This constructor initializes a DroughtMitigationPolicy object using the provided `id` and `type`.
     *
     * @param id The unique identifier for the DroughtMitigationPolicy.
     * @param type The type of the DroughtMitigationPolicy.
     */
    DroughtMitigationPolicy(const int id, const int type);

    /**
     * @brief Applies the Drought Mitigation Policy for a given week.
     * 
     * This is a purely virtual function that must be implemented by derived classes.
     * 
     * @param week The week for which to extract the Drought Mitigation Policy.
     * 
     * @return  None
     */
    virtual void applyPolicy(int week)= 0;

    /**
     * @brief Adds system components (utilities, water sources, and minimum environment flow controls) to the Drought Mitigation Policy.
     * 
     * This function is a purely virtual function that must be implemented by derived classes.
     * 
     * @param utilities A vector of pointers to the utilities associated with the Drought Mitigation Policy.
     * @param water_sources A vector of pointers to the water sources associated with the Drought Mitigation Policy. 
     * @param min_env_flow_controls A vector of pointers to the minimum environment flow controls associated with the Drought Mitigation Policy.
     * 
     * @return None
     */ 
    virtual void addSystemComponents(vector<Utility *> utilities,
                                         vector<WaterSource *> water_sources,
                                         vector<MinEnvFlowControl *> min_env_flow_controls)= 0;

    /**
     * @brief Retrieves the list of utility IDs associated with this DroughtMitigationPolicy.
     * This function returns a reference to a vector containing the utility IDs.
     *
     * @return A constant reference to a vector of integers representing utility IDs.
     */
    const vector<int> &getUtilities_ids() const;

    /**
     * @brief Compares two DroughtMitigationPolicy objects based on their IDs.
     * This operator checks whether the current object's ID is less than the ID of another DroughtMitigationPolicy object.
     *
     * @param other A pointer to the DroughtMitigationPolicy object to compare with.
     *
     * @return `true` if the current object's ID is less than the ID of the other object; otherwise, `false`.
     */
    bool operator<(const DroughtMitigationPolicy *other);

    /**
     * @brief Compares two DroughtMitigationPolicy objects based on their IDs.
     * This operator checks whether the current object's ID is greater than the ID of another DroughtMitigationPolicy object.
     *
     * @param other A pointer to the DroughtMitigationPolicy object to compare with.
     *
     * @return `true` if the current object's ID is greater than the ID of the other object; otherwise, `false`.
     */
    bool operator>(const DroughtMitigationPolicy *other);

    /**
     * @brief Destructor for the DroughtMitigationPolicy class.
     * This function cleans up resources used by the DroughtMitigationPolicy object.
     *
     * @return None
     */
    virtual ~DroughtMitigationPolicy();

    /**
     * @brief Sets the storage-to-ROF table and configuration for imported tables.
     * This function updates the reference to the storage-to-ROF table and specifies whether imported tables are used.
     *
     * @param storage_to_rof_table_ A reference to a vector of `Matrix2D<double>` representing the storage-to-ROF table.
     * @param use_imported_tables An integer flag indicating whether imported tables should be used. 
     *        This is compared with `IMPORT_ROF_TABLES`.
     *
     * @return None.
     */
    void setStorage_to_rof_table_(vector<Matrix2D<double>> &storage_to_rof_table_, int use_imported_tables);

    /**
     * @brief Retrieves the risk-of-failure (ROF) value for a specified utility, week, and tier.
     * This function accesses the storage-to-ROF table to fetch the corresponding ROF value based on the input parameters.
     *
     * @param utility_id The ID of the utility for which the ROF value is to be retrieved.
     * @param week The week for which the ROF value is needed.
     * @param tier The ROF tier required.
     *
     * @return The ROF value as a `double` for the specified utility, week, and tier.
     */
    virtual void setRealization(unsigned long realization_id, vector<double> &utilities_rdm,
                                vector<double> &water_sources_rdm, vector<double> &policy_rdm)= 0;

};


#endif //TRIANGLEMODEL_DROUGHTMITIGATIONPOLICY_H
