#include "LinearRegression.h"

#include <vector>

// static so this function is only visible inside this source file
static double huber_weight(double r, double delta) {
    double abs_r = fabs(r);
    return (abs_r <= delta) ? 1.0 : (delta / abs_r);
}

// Iteratively Reweighted Least Squares (IRLS) linear regression uising Huber weight
linreg_result_t huber_irls_linreg(const std::vector<double>& x,
                              const std::vector<double>& y,
                              size_t max_iter,      // Maximum number of iterations to fit a line
                              double tol,           // Toleration for differences of fit params beetween two iteration (if < tol => break)
                              double delta) {       // Parameter in huber weight (large values => Least Squares Method essentially)
                            
    size_t data_size = x.size();
    linreg_result_t result = {0};

    if (data_size < 2) {
        result.converged = 0;
        return result;
    }

    gsl_matrix *X = gsl_matrix_alloc(data_size, 2);
    gsl_vector *vy = gsl_vector_alloc(data_size);
    gsl_vector *w  = gsl_vector_alloc(data_size);
    gsl_vector *c  = gsl_vector_alloc(2);
    gsl_matrix *cov = gsl_matrix_alloc(2, 2);
    gsl_multifit_linear_workspace *work =
        gsl_multifit_linear_alloc(data_size, 2);

    double beta0 = 0.0, beta1 = 0.0;
    double prev_beta0 = 0.0, prev_beta1 = 0.0;
    double chisq;

    // Initialize design matrix and vectors
    for (size_t i = 0; i < data_size; i++) {
        gsl_matrix_set(X, i, 0, 1.0);
        gsl_matrix_set(X, i, 1, x[i]);
        gsl_vector_set(vy, i, y[i]);
        gsl_vector_set(w, i, 1.0);
    }

    int converged = 0;
    int iter;

    for (iter = 0; iter < max_iter; iter++) {
        gsl_multifit_wlinear(X, w, vy, c, cov, &chisq, work);

        beta0 = gsl_vector_get(c, 0);
        beta1 = gsl_vector_get(c, 1);

        if (fabs(beta0 - prev_beta0) < tol &&
            fabs(beta1 - prev_beta1) < tol) {
            converged = 1;
            break;
        }

        prev_beta0 = beta0;
        prev_beta1 = beta1;

        // Update weights
        for (size_t i = 0; i < data_size; i++) {
            double r = y[i] - (beta0 + beta1 * x[i]);
            double wi = huber_weight(r, delta);
            gsl_vector_set(w, i, wi);
        }
    }

    result.beta0 = beta0;
    result.beta1 = beta1;
    result.iterations = iter;
    result.converged = converged;

    gsl_matrix_free(X);
    gsl_vector_free(vy);
    gsl_vector_free(w);
    gsl_vector_free(c);
    gsl_matrix_free(cov);
    gsl_multifit_linear_free(work);

    return result;
}
