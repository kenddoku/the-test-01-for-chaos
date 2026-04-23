#include "ChaosAnalyzer.h"

#include <fstream>

// ===== TimeSeries definitions =====
size_t TimeSeries::len() const {
    return data.size();
}// TimeSeries::len()


// ===== Analyzer definitions =====
// !!!!! consider making AnalysisParameters an attribute of ChaosAnalyzer since it makes more sense
// to pass it once upon creation of ChaosAnalyzer instance
ChaosAnalyzer::ChaosAnalyzer(const AnalysisParameters& apar_, unsigned int seed) : apar(apar_), rng(seed) {}

double ChaosAnalyzer::generateC() {
    if( apar.randomize_c ) {
        std::uniform_real_distribution<double> dist(apar.c_min, apar.c_max);
        double c = dist(rng);
        while( fabs(c) < 1E-6 ) c = dist(rng);
        c_counter++;

        return c;
    } else {
        // c_max is never achieved unless c_min == c_max
        double c = apar.c_min + c_counter * (apar.c_max - apar.c_min) / apar.c_num;
        if( fabs(c) < 1E-6 ) c = 1E-6;
        c_counter++;

        return c;
    }
}// ChaosAnalyzer::generateC()


// Method performing final computation of intermediate results and K parameter values
AnalysisResult ChaosAnalyzer::run(const TimeSeries &ts, size_t mode) {
    AnalysisResult ares;
    
    AnalysisControlResult acor;
    std::vector<AnalysisControlResult> intermediate_results; // where all runs are stored
    intermediate_results.reserve(apar.c_num);

    acor.p.resize(apar.N);
    acor.q.resize(apar.N);
    acor.M.resize(apar.N0);

    for(size_t i=0; i<apar.c_num; i++) {
        double c = generateC();
        acor.c = c;
        acor.p[0] = ts.data[0] * std::cos(c);
        acor.q[0] = ts.data[0] * std::sin(c);
        for(size_t j=1; j<apar.N; j++) {
            acor.p[j] = acor.p[j-1] + ts.data[j] * std::cos((j+1)*c);
            acor.q[j] = acor.q[j-1] + ts.data[j] * std::sin((j+1)*c);
        }

        for(size_t n=0; n<apar.N0; n++) {
            size_t j_max = apar.N - n; // ensures that while iterating we do not go beyond the p and q size = N
            
            double sum {0.0};

            for(size_t j=0; j<j_max; j++) {
                double dp = acor.p[j+n] - acor.p[j];
                double dq = acor.q[j+n] - acor.q[j];
                sum += dp*dp + dq*dq;
            }
            acor.M[n] = sum / j_max;
        }
        corrMethod(acor, mode);
        intermediate_results.push_back(acor);
    }
    ares.intermediate_results = std::move(intermediate_results);
    
    return ares;
}// ChaosAnalyzer::run()


double ChaosAnalyzer::corrMethod(AnalysisControlResult &acor, size_t mode) {
    // filling ksi vector only once after first c value is being considered
    if(c_counter == 1) {
        ksi.resize(acor.M.size());
        for(size_t i=0; i<acor.M.size(); i++) {
            ksi[i] = i+1;
        }
    }
    
    const std::vector<double>& Delta =
        (mode == 0) ? acor.M : acor.D;

    acor.K_corr = MathHelper::corr(ksi, Delta);
    return acor.K_corr;
}//ChaosAnalyzer::corrMethod()


