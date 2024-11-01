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
    double standard_normal_cdf(double x) {
        return 0.5 * (1.0 + std::erf(x / SQRT_2));
    }

    double standard_normal_pdf(double x) {
        return std::exp(-0.5 * x * x) / SQRT_2_PI;
    }
void untruncated_mean_and_std_estimate(double observed_mean, double observed_std, double a, double b, 
                                       double& adjusted_mean, double& adjusted_std) {
    // Standardize truncation bounds
    double alpha = (a - observed_mean) / observed_std;
    double beta = (b - observed_mean) / observed_std;

    // Calculate Z (probability within [a, b])
    double Z = standard_normal_cdf(beta) - standard_normal_cdf(alpha);

    // Calculate Delta
    double delta = (standard_normal_pdf(alpha) - standard_normal_pdf(beta)) / Z;

    // Adjust the mean to account for truncation
    adjusted_mean = observed_mean + observed_std * delta;

    // Adjust the variance upwards to approximate the untruncated distribution's variance
    double variance_inflation_factor = 1.0 + delta * delta + 
                                       (alpha * standard_normal_pdf(alpha) - beta * standard_normal_pdf(beta)) / Z;

    double adjusted_variance = observed_std * observed_std * variance_inflation_factor;

    // Adjusted standard deviation
    adjusted_std = std::sqrt(adjusted_variance);
}

// Function to compute the mean and variance of a standard truncated normal distribution
void standard_truncated_normal_mean_variance(double a, double b, double& mu_trunc, double& sigma_trunc) {
    // Compute Z
    double Z = standard_normal_cdf(b) - standard_normal_cdf(a);

    // Compute phi(a) and phi(b)
    double phi_a = standard_normal_pdf(a);
    double phi_b = standard_normal_pdf(b);

    // Compute truncated mean
    double lambda = (phi_a - phi_b) / Z;
    mu_trunc = lambda;

    // Compute truncated variance
    double sigma_trunc_squared = 1 + (a * phi_a - b * phi_b) / Z - lambda * lambda;
    sigma_trunc = std::sqrt(sigma_trunc_squared);
}

// Function to estimate true mean and std from observed truncated data
void estimate_true_mean_std(double mu_obs, double sigma_obs, double a, double b,
                            double& mu_true, double& sigma_true) {
    // Step 1: Compute mean and std of standard truncated normal
    double mu_trunc_std, sigma_trunc_std;
    standard_truncated_normal_mean_variance(a, b, mu_trunc_std, sigma_trunc_std);

    // Step 2: Compute scaling factor
    double sigma_factor = sigma_obs / sigma_trunc_std;

    // Step 3: Compute shift
    double mu_shift = mu_obs - sigma_factor * mu_trunc_std;

    // Step 4: Estimated true mean and std
    mu_true = mu_shift;
    sigma_true = sigma_factor;
}

// Function to estimate true mean and std from observed truncated data
void estimate_true_mean_std_v2(
        double mu_obs, double sigma_obs, 
        double mu_trunc_std, double sigma_trunc_std,
        double& mu_true, double& sigma_true) {
    // Step 1: Compute mean and std of standard truncated normal

    // Step 2: Compute scaling factor
    double sigma_factor = sigma_obs / sigma_trunc_std;

    // Step 3: Compute shift
    double mu_shift = mu_obs - sigma_factor * mu_trunc_std;

    // Step 4: Estimated true mean and std
    mu_true = mu_shift;
    sigma_true = sigma_factor;
}


} // namespace
#endif