#ifndef SCREAMER_EXPANDINGSM_EXP_H
#define SCREAMER_EXPANDINGSM_EXP_H

#include "screamer/common/base.h"
#include <vector>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <iostream>

namespace screamer {

    class ExpandingSMExp : public ScreamerBase {
    public:
        ExpandingSMExp(double phi, int tau)
            : phi(phi), tau(tau){
            if (tau <= 0) throw std::invalid_argument("Tau must be positive.");
            if (phi < 0 || phi > 1) throw std::invalid_argument("Phi must be between 0 and 1.");
        }

        double process_scalar(double newValue) override {
            // If the new value is not positive, return NaN
            if (newValue <= 0){
                return std::numeric_limits<double>::quiet_NaN(); 
            }
            // Apply log transformation to the new value
            buffer.push_back(std::log(newValue));
            // If the buffer size is less than tau, return NaN
            if (buffer.size() <= static_cast<size_t>(tau)) {
                return std::numeric_limits<double>::quiet_NaN();
            }
            return calculate_maximum_explosiveness();
        }

        void reset() override {
            buffer.clear();
        }

    private:
        double compute_explosiveness(
            const std::vector<double>& y,
            const std::vector<double>& t) {
            
            size_t n = y.size();
            if (n < 3) return std::numeric_limits<double>::quiet_NaN();

            // Perform linear regression to estimate intercept and slope
            double sum_y_ = std::accumulate(y.begin(), y.end(), 0.0);
            double sum_t_ = std::accumulate(t.begin(), t.end(), 0.0);
            double sum_t2_ = std::accumulate(t.begin(), t.end(), 0.0, 
                [](double sum, double t) { return sum + t * t; });
            double sum_t_y_ = std::inner_product(t.begin(), t.end(), y.begin(), 0.0);

            // Calculate slope and intercept using the normal equations for linear regression
            double denominator = n * sum_t2_ - sum_t_ * sum_t_;
            if (std::abs(denominator) < 1e-12 || n <= 1) return std::numeric_limits<double>::quiet_NaN();

            double beta = (n * sum_t_y_ - sum_t_ * sum_y_) / denominator;
            double log_alpha = (sum_y_ * sum_t2_ - sum_t_ * sum_t_y_) / denominator;

            // Compute residuals and sum of squared errors (SSE)
            double sse = 0.0;
            for (size_t i = 0; i < n; ++i) {
                double y_hat = log_alpha + beta * t[i];  // Predicted log(y_t) based on the model
                sse += std::pow(y[i] - y_hat, 2);        // Residuals squared
            }
            
            // Compute variance of beta
            double variance_beta = sse / (n - 2) / denominator;
            // Return explossiveness
            return (std::abs(beta) / std::sqrt(variance_beta)) / std::pow(n, phi);
        }

        double calculate_maximum_explosiveness() {
            if (buffer.size() < 3) return std::numeric_limits<double>::quiet_NaN();

            double max_explosiveness = -std::numeric_limits<double>::infinity();
            size_t n = buffer.size();
            for (size_t t0 = 0; t0 < (n > tau ? n - tau : 0); ++t0) {
                auto y_sub_start = buffer.begin() + t0;
                auto y_sub_end = buffer.end();
                std::vector<double> y_sub(y_sub_start, y_sub_end);

                size_t sub_size = y_sub.size();
                std::vector<double> t_sub(sub_size);
                std::iota(t_sub.begin(), t_sub.end(), 0);  // Generate t as the index vector

                try {
                    double explosiveness = compute_explosiveness(y_sub, t_sub);
                    if (!std::isnan(explosiveness)) {
                        max_explosiveness = std::max(max_explosiveness, explosiveness);
                    }
                } catch (const std::exception& e) {
                    continue;
                }
            }
            return std::isinf(max_explosiveness) ? std::numeric_limits<double>::quiet_NaN() : max_explosiveness;
        }

        double phi;
        int tau;
        std::vector<double> buffer;
    };
}

#endif // SCREAMER_EXPANDINGSM_EXP_H
