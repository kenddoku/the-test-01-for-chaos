#ifndef LINEARREGRESSION_H
#define LINEARREGRESSION_H

#include <vector>
#include <cstdlib>
#include <cmath>
#include <gsl/gsl_multifit.h>

typedef struct {
    double beta0;
    double beta1;
    int iterations;
    int converged;
} linreg_result_t;

// Iteratively Reweighted Least Squares (IRLS) linear regression uising Huber weight
linreg_result_t huber_irls_linreg(const std::vector<double>& x,
                              const std::vector<double>& y,
                              size_t max_iter = 1000,    // Maximum number of iterations to fit a line
                              double tol = 1E-6,        // Toleration for differences of fit params beetween two iteration (if < tol => break)
                              double delta = 0.5);      // Parameter in huber weight (large values => Least Squares Method essentially)
                                                             
#endif