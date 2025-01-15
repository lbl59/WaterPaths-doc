/**
 * @class ReservoirExpansion
 * @brief The `ReservoirExpansion` class is a subclass of the main `WaterSource` class. It represents a general reservoir object scheduled for expansion.
 * 
 * Created by bernardoct on 5/3/17.
 */
#ifndef TRIANGLEMODEL_RESERVOIREXPANSION_H
#define TRIANGLEMODEL_RESERVOIREXPANSION_H


#include "Base/WaterSource.h"

class ReservoirExpansion : public WaterSource {
public:
        /**
         * @brief The ID of the parent reservoir that is being expanded.
         */
        const unsigned int parent_reservoir_ID;

        /**
         * @brief Constructor for creating a ReservoirExpansion object.
         * 
         * This constructor initializes a `ReservoirExpansion` object with the specified parameters 
         * and sets the parent reservoir ID. It also calls the constructor of the base class `WaterSource`
         * to initialize the shared attributes.
         * 
         * @param name The name of the reservoir expansion.
         * @param id The unique identifier for the reservoir expansion.
         * @param parent_reservoir_ID The ID of the parent reservoir.
         * @param capacity The capacity of the reservoir expansion.
         * @param construction_time_range The time range for construction of the reservoir expansion.
         * @param permitting_period The permitting period for the reservoir expansion.
         * @param bond A reference to the bond object associated with the reservoir expansion.
         */
        ReservoirExpansion(const char *name, const int id, const unsigned int parent_reservoir_ID,
                       const double capacity, const vector<double> &construction_time_range,
                       double permitting_period, Bond &bond);

        /**
         * @brief Copy constructor for the ReservoirExpansion class.
         * 
         * This constructor creates a copy of an existing `ReservoirExpansion` object by copying 
         * the values of its attributes, including calling the copy constructor of the base class `WaterSource`.
         * 
         * @param reservoir_expansion The `ReservoirExpansion` object to copy.
         */
        ReservoirExpansion(const ReservoirExpansion &reservoir_expansion);

        /**
         * @brief Assignment operator for the ReservoirExpansion class.
         * 
         * This operator assigns the values of one `ReservoirExpansion` object to another.
         * 
         * @param reservoir_expansion The `ReservoirExpansion` object to assign from.
         * 
         * @return A reference to the current `ReservoirExpansion` object after assignment.
         */
        ReservoirExpansion &operator=(
            const ReservoirExpansion &reservoir_expansion);

        /**
         * @brief Throws an error indicating that continuity cannot be applied to a reservoir expansion.
         * 
         * This function throws a `logic_error` because a reservoir expansion only adds storage volume 
         * to the reservoir it is assigned to. Continuity calculations should be applied to the reservoir 
         * being expanded, not the expansion itself.
         * 
         * @param week The week for which continuity would be applied.
         * @param upstream_source_inflow The inflow from upstream sources (not used).
         * @param wastewater_discharge The inflow from wastewater discharge (not used).
         * @param demand_outflow A vector of demand outflows (not used).
         * 
         * @throws logic_error This function always throws a `logic_error` because continuity cannot be applied 
         * to a reservoir expansion.
         */
        void applyContinuity(int week, double upstream_source_inflow,
                             double wastewater_discharge,
                             vector<double> &demand_outflow) override;

};


#endif //TRIANGLEMODEL_RESERVOIREXPANSION_H
