#ifndef CHAOSANALYZER_H
#define CHAOSANALYZER_H

#include "TimeSeries.h"
#include "MathHelper.h"
#include "KWithIndex.h"

#include <algorithm>
#include <random>

struct AnalysisParameters {
    size_t N;
    size_t N0;
    // NOTE: still thinking about making multi-time-series analysis possible but seems like
    // overkill right now
    // size_t ts_num = 1; // number of time series data on which analysis shall be performed
    size_t c_num = 11; // number of c values used for analysis
    double c_min = 0.0;
    double c_max = 2*MathHelper::pi;
    bool randomize_c = true;
}; // struct AnalysisParameters

// Test 0-1 for chaos analysis results for a single c value
struct AnalysisControlResult {
    double c;
    std::vector<double> p;
    std::vector<double> q;
    std::vector<double> M;

    KWithIndex KWI_corr = {0};
    KWithIndex KWI_linreg = {0};
}; // struct AnalysisControlResult

// Final test 0-1 for chaos analysis results
struct AnalysisResult {
    size_t N  = 0;
    size_t N0 = 0;
    KWithIndex KWI_corr  = {0};      // K value (with index->position in intermediate_results) computed using correlation method
    KWithIndex KWI_linreg= {0};    // K value (with index->position in intermediate_results) computed using linear regression method
    const TimeSeries *ts_ptr;     // pointer to time series for which analysis was performed
    std::vector<AnalysisControlResult> intermediate_results;
}; // struct AnalysisResult

class ChaosAnalyzer {
public:
    AnalysisParameters apar;

    explicit ChaosAnalyzer(const AnalysisParameters& apar_, unsigned int seed = std::random_device{}());

    double generateC();

    AnalysisResult run(const TimeSeries *ts, size_t mode = 0);

    double corrMethod(AnalysisControlResult &acor, size_t mode = 0);

    double linregMethod(AnalysisControlResult &acor, size_t mode = 0);

private:
    std::mt19937 rng;   // rng engine
    size_t c_counter = 0;
    std::vector<double> ksi;
}; // class ChaosAnalyzer

#endif