#ifndef SCREAMER_MATH_H
#define SCREAMER_MATH_H

#include <cmath>
#include <iostream>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifndef SQRT_2
    #define SQRT_2 1.41421356237309504880  // Approximation of sqrt(2)
#endif

#ifndef SQRT_2_PI
    #define SQRT_2_PI 2.50662827463100050241  // Approximation of sqrt(2 * pi)
#endif

namespace screamer {

    // Helper functions for CDF and PDF of the standard normal distribution
    inline double standard_normal_cdf(double x) {
        return 0.5 * (1.0 + std::erf(x / SQRT_2));
    }

    inline double standard_normal_pdf(double x) {
        return std::exp(-0.5 * x * x) / SQRT_2_PI;
    }

    // Function to compute the mean and variance of a standard truncated normal distribution
    void standard_truncated_normal_mean_variance(double a, double b, double& mu_trunc, double& sigma_trunc);

    // Function to estimate true mean and std from observed truncated data
    void estimate_true_mean_std(
        double mu_obs, double sigma_obs, 
        double mu_trunc_std, double sigma_trunc_std,
        double& mu_true, double& sigma_true
    );

} // namespace
#endif