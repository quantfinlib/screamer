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

    inline void var_from_stats(double sum_x, double sum_xx, int n, double& var) {
        var = (sum_xx - (sum_x * sum_x) / n) / (n - 1);
    }

    inline void skew_n_const(int n_, double& c0) {
        double n = n_;
        c0 = n / ((n - 1) * (n - 2));
    }

    inline void skew_from_stats(double sum_x, double sum_xx, double sum_xxx, double c0, int n, double& skew) {
        double mean = sum_x / n;

        double var;
        var_from_stats(sum_x, sum_xx, n, var);
        double std_dev = std::sqrt(var);

        double m3 = sum_xxx - 3 * mean * sum_xx + 2 * n * mean * mean * mean;
        double g1 = m3 / (var * std_dev);
        skew = g1 * c0;     
    }

    inline void kurt_n_const(int n_, double& c0, double& c1, double& c2) {
        double n = n_; // cast to double
        c0 = n * (n + 1);
        c1 = (n - 1) * (n - 2) * (n - 3);
        c2 = (3 * (n - 1) * (n - 1)) / ((n - 2) * (n - 3));
    }

    inline void kurt_from_stats(
        double sum_x, double sum_xx, double sum_xxx, double sum_xxxx, 
        double c0, double c1, double c2,
        int n, 
        double& kurt) {

        double mean = sum_x / n;

        double var;
        var_from_stats(sum_x, sum_xx, n, var);

        double std_dev = std::sqrt(var);

        double mean2 = mean * mean;
        double m4 = sum_xxxx - 4 * mean * sum_xxx + 6 * mean2 * sum_xx - 3 * n * mean2 * mean2;

        double numerator = c0 * m4;
        double denominator = c1 * var * var;

        kurt = (numerator / denominator) - c2;
    }
    

} // namespace
#endif