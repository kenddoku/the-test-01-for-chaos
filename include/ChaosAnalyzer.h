#ifndef CHAOSANALYZER_H
#define CHAOSANALYZER_H

#include "TimeSeries.h"
#include "MathHelper.h"

#include <random>

struct AnalysisParameters {
    size_t N;
    size_t N0;
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
    std::vector<double> Vosc;
    std::vector<double> D;
    std::vector<double> D_prime; // D_prime is the same as D but with added constant to make all entries positive
    double K_corr;
    double K_linreg;
}; // struct AnalysisControlResult

// Final test 0-1 for chaos analysis results
struct AnalysisResult {
    double K_corr;      // K value computed using correlation method
    double K_linreg;    // K value computed using linear regression method

    std::vector<AnalysisControlResult> intermediate_results;
}; // struct AnalysisResult

class ChaosAnalyzer {
public:
    explicit ChaosAnalyzer(unsigned int seed = std::random_device{}());

    double generateC(const AnalysisParameters &apar);

    AnalysisResult run(const TimeSeries &ts, const AnalysisParameters &apar);

    double corrMethod(AnalysisControlResult &acor, size_t mode = 0);

private:
    std::mt19937 rng;
    size_t c_counter = 0;
    std::vector<double> ksi;
}; // class ChaosAnalyzer

#endif