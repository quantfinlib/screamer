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

    // Function to adjust observed standard deviation for truncated normal distribution
    double untruncated_std_estimate(double observed_mean, double observed_std, double a, double b) {
        // Standardize truncation bounds
        double alpha = (a - observed_mean) / observed_std;
        double beta = (b - observed_mean) / observed_std;

        // Calculate Z (probability within [a, b])
        double Z = standard_normal_cdf(beta) - standard_normal_cdf(alpha);

        // Calculate Delta
        double delta = (standard_normal_pdf(alpha) - standard_normal_pdf(beta)) / Z;

        // Adjust the variance to account for truncation
        double adjusted_variance = observed_std * observed_std /
            (1.0 - delta * delta - (alpha * standard_normal_pdf(alpha) - beta * standard_normal_pdf(beta)) / Z);

        // Return the adjusted standard deviation
        return std::sqrt(adjusted_variance);
    }

/*
    int main() {
        // Example usage
        double observed_mean = 0.0;     // Mean of the truncated distribution
        double observed_std = 1.0;      // Observed (truncated) standard deviation
        double lower_bound = -1.5;      // Lower truncation point
        double upper_bound = 1.5;       // Upper truncation point

        double estimated_std = untruncated_std_estimate(observed_mean, observed_std, lower_bound, upper_bound);

        std::cout << "Estimated untruncated standard deviation: " << estimated_std << std::endl;

        return 0;
    }
*/


} // namespace
#endif