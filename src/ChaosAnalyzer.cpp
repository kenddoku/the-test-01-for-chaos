#include "ChaosAnalyzer.h"
#include "LinearRegression.h"

#include <fstream>
#include <stdexcept>
#include <cmath>

// ===== TimeSeries definitions =====
size_t TimeSeries::len() const {
    return data.size();
}// TimeSeries::len()


// ===== Analyzer definitions =====
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
AnalysisResult ChaosAnalyzer::run(const TimeSeries *ts, size_t mode) {
    
    if (ts == nullptr) {
        throw std::invalid_argument("ERROR: Time series pointer is a nullptr");
    }
    if (ts->len() != apar.N) {
        throw std::invalid_argument("ERROR: Time series length doesn't match the N size in AnalysisParameters");
    }

    // Creating an object of class AnalysisResult that will store all the results
    AnalysisResult ares;
    // Assigning appropriate N/N0 values to ares so that the ares object gets full information
    ares.N = apar.N;
    ares.N0 = apar.N0;

    // Resizing intermediate_results to store all control results in final result
    ares.intermediate_results.resize(apar.c_num);
    
    ares.ts_ptr = ts; // assigning time series pointer to be visible from AnalysisResult class object
    c_counter = 0;

    // Vector to store only KWI (KWithIndex) objects used to later calculate final result by calling MathHelper::median()
    std::vector<KWithIndex> KWI_corr_vec;
    KWI_corr_vec.resize(apar.c_num);

    std::vector<KWithIndex> KWI_linreg_vec;
    KWI_linreg_vec.resize(apar.c_num);

    for(size_t i=0; i<apar.c_num; i++) {
        AnalysisControlResult acor;
        acor.p.resize(apar.N);
        acor.q.resize(apar.N);
        acor.M.resize(apar.N0);

        double c = generateC();
        acor.c = c;
        acor.p[0] = ts->data[0] * std::cos(c);
        acor.q[0] = ts->data[0] * std::sin(c);
        for(size_t j=1; j<apar.N; j++) {
            acor.p[j] = acor.p[j-1] + ts->data[j] * std::cos((j+1)*c);
            acor.q[j] = acor.q[j-1] + ts->data[j] * std::sin((j+1)*c);
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
        acor.KWI_corr.index = i;
        acor.KWI_corr.K = corrMethod(acor, mode);
        KWI_corr_vec[i] = acor.KWI_corr;

        acor.KWI_linreg.index = i;
        acor.KWI_linreg.K = linregMethod(acor, mode);
        KWI_linreg_vec[i] = acor.KWI_linreg;

        ares.intermediate_results[i] = std::move(acor);
    }
    ares.KWI_corr = KWithIndex::median(KWI_corr_vec);
    ares.KWI_linreg = KWithIndex::median(KWI_linreg_vec);
    
    return ares;
}// ChaosAnalyzer::run()


double ChaosAnalyzer::corrMethod(AnalysisControlResult &acor, size_t mode) {
    // filling ksi vector only once when first c value is being considered
    if(c_counter == 1) {
        ksi.resize(acor.M.size());
        for(size_t i=0; i<acor.M.size(); i++) {
            ksi[i] = i+1;
        }
    }
    
    const std::vector<double>& Delta = acor.M;

    double K = MathHelper::corr(ksi, Delta);
    return K;
}//ChaosAnalyzer::corrMethod()

double ChaosAnalyzer::linregMethod(AnalysisControlResult &acor, size_t mode) {
    if(c_counter == 1) {
        ksi.resize(acor.M.size());
        for(size_t i=0; i<acor.M.size(); i++) {
            ksi[i] = i+1;
        }
    }

    std::vector<double> M_log(acor.M.size());
    std::vector<double> ksi_log(ksi.size());

    for(size_t i=0; i<M_log.size(); i++) {
        // Safeguard since first value of M is equal to zero
        if(acor.M[i] < 1E-6)
            M_log[i] = std::log10(1E-6);
        else
            M_log[i] = std::log10(acor.M[i]);
        ksi_log[i] = std::log10(ksi[i]);
    }

    linreg_result_t result = huber_irls_linreg(ksi_log, M_log);

    return result.beta1;
}


