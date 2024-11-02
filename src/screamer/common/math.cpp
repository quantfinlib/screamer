#include <cmath>
#include "screamer/common/math.h"

namespace screamer {

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
    void estimate_true_mean_std(
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