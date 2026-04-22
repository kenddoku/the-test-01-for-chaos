#ifndef TIMESERIES_H
#define TIMESERIES_H

#include <vector>
#include <string>

// Struct designed to store a single time series data
struct TimeSeries {
    int         nu;
    int         cp_val; // control parameter value
    std::string cp_name;// control parameter name
    
    std::vector<double> data;

    size_t len() const;
};

#endif