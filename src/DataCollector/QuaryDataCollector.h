/**
 * @class QuaryDataCollector
 * @brief The `QuaryDataCollector` class is a subclass of the `ReservoirDataCollector` class, which in turn is a subclass of the main `DataCollector` class. 
 * It implements the data collection of quarry objects.
 * FIXME: Needs to be renamed to `QuarryDataCollector`.
 * 
 * Created by bernardoct on 8/26/17.
 */

#ifndef TRIANGLEMODEL_QUARYDATACOLLECTOR_H
#define TRIANGLEMODEL_QUARYDATACOLLECTOR_H

#include "ReservoirDataCollector.h"

class QuaryDataCollector : public ReservoirDataCollector {
public:
    /**
     * @brief Construct a new Quary Data Collector object.
     * 
     * @param reservoir The reservoir through which this quarry is routing..
     * @param realization The current realization number.
     * 
     */
    QuaryDataCollector(Reservoir *reservoir, unsigned long realization);

};


#endif //TRIANGLEMODEL_QUARYDATACOLLECTOR_H
