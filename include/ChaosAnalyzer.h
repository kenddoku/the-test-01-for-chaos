#ifndef CHAOSANALYZER_H
#define CHAOSANALYZER_H

#include "TimeSeries.h"
#include "MathHelper.h"

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

// KWithIndex class created to keep track of the position (index inside the intermediate_results vector) of K value 
// being the final result of test01 analysis even after sorting
// The KWithIndex::median() method was introduced since MathHelper::median() does not support such inputs
class KWithIndex {
public:
    double K;
    size_t index;

    bool operator<(const KWithIndex& other) const {
        return (K < other.K);
    }

    // Returns KWithIndex object with .K being mean value of both inputs K values
    // but .index is set to the index of first input (since we still need to navigate
    // inside of the intermediate_results vector). Choosing .index equal to the index
    // of second output would also be valid
    static KWithIndex mean(const KWithIndex& KWI1, const KWithIndex& KWI2) {
        KWithIndex mean_result = {
            .K = (KWI1.K + KWI2.K) / 2.0,
            .index = KWI1.index 
        };

        return mean_result;
    } 

    static KWithIndex median(std::vector<KWithIndex> KWI_vec) {
        std::sort(KWI_vec.begin(), KWI_vec.end());

        size_t n = KWI_vec.size();

        if (n % 2 == 0) {
            return KWithIndex::mean(KWI_vec[n/2 - 1], KWI_vec[n/2]);
        } else {
            return KWI_vec[n/2];
        }
    }
};//class KWithIndex

// Test 0-1 for chaos analysis results for a single c value
struct AnalysisControlResult {
    double c;
    std::vector<double> p;
    std::vector<double> q;
    std::vector<double> M;
    std::vector<double> Vosc;
    std::vector<double> D;
    std::vector<double> D_prime; // D_prime is the same as D but with added constant to make all entries positive
    KWithIndex KWI_corr = {0};
    KWithIndex KWI_linreg = {0};
}; // struct AnalysisControlResult

// Final test 0-1 for chaos analysis results
struct AnalysisResult {
    KWithIndex KWI_corr = {0};      // K value (with index->position in intermediate_results) computed using correlation method
    KWithIndex KWI_linreg = {0};    // K value (with index->position in intermediate_results) computed using linear regression method
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

private:
    std::mt19937 rng;   // rng engine
    size_t c_counter = 0;
    std::vector<double> ksi;
}; // class ChaosAnalyzer

#endif