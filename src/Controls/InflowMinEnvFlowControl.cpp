//
// Created by bct52 on 6/29/17.
//

#include "InflowMinEnvFlowControl.h"

InflowMinEnvFlowControl::InflowMinEnvFlowControl(int water_source_id, const vector<double> &inflows,
                                                 const vector<double> &releases)
        : MinEnvFlowControl(water_source_id, vector<int>(1, water_source_id),
                                vector<int>(),
                                INFLOW_CONTROLS),
          inflows(inflows),
          releases(releases) {}

InflowMinEnvFlowControl::InflowMinEnvFlowControl(
        const InflowMinEnvFlowControl &min_env_control) :
        MinEnvFlowControl(min_env_control),
        inflows(min_env_control.inflows),
        releases(min_env_control.releases) {}

InflowMinEnvFlowControl::~InflowMinEnvFlowControl() = default;

double InflowMinEnvFlowControl::getRelease(int week) {
    // Retrieve the total inflow for the primary water source
    double inflow =
            water_sources[water_source_id]->getUpstream_source_inflow() +
            water_sources[water_source_id]->getUpstreamCatchmentInflow();

    double release = 0;
    for (unsigned long i = 0; i < inflows.size(); ++i) {
        // For each inflow threshold, check if the current inflow is greater than or equal to the threshold.
        // If true, update `release` with the corresponding value from the `releases` vector.
        // Done with ternary operator for improved performance.
        release = (inflow >= inflows[i] ? releases[i] : release);
    }
    return release;
}
