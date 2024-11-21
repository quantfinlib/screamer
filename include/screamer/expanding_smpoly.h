#ifndef SCREAMER_EXPANDINGSM_POLY_H
#define SCREAMER_EXPANDINGSM_POLY_H

#include "screamer/common/base.h"
#include <Eigen/Dense>
#include <vector>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <iostream>

namespace screamer {

    class ExpandingSMPoly : public ScreamerBase {
    public:
        ExpandingSMPoly(double phi, int tau, bool log = true)
            : phi(phi), tau(tau), log(log) {
            if (tau <= 0) throw std::invalid_argument("Tau must be positive.");
            if (phi < 0 || phi > 1) throw std::invalid_argument("Phi must be between 0 and 1.");
        }

        double process_scalar(double newValue) override {
            if (log){
                buffer.push_back(std::log(newValue));
            }
            else{
                buffer.push_back(newValue);
            }
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
            const std::vector<double>& t,
            const std::vector<double>& t_squared) {
            
            size_t n = y.size();
            if (n < 3) return std::numeric_limits<double>::quiet_NaN();

            double sum_y_ = std::accumulate(y.begin(), y.end(), 0.0);
            double sum_t_ = std::accumulate(t.begin(), t.end(), 0.0);
            double sum_t2_ = std::accumulate(t_squared.begin(), t_squared.end(), 0.0);
            double sum_t_y_ = std::inner_product(t.begin(), t.end(), y.begin(), 0.0);
            double sum_t2_y_ = std::inner_product(t_squared.begin(), t_squared.end(), y.begin(), 0.0);
            double sum_t_t2_ = std::inner_product(t.begin(), t.end(), t_squared.begin(), 0.0);
            double sum_tt_ = std::inner_product(t.begin(), t.end(), t.begin(), 0.0);
            double sum_t2_t2_ = std::inner_product(t_squared.begin(), t_squared.end(), t_squared.begin(), 0.0);

            Eigen::Matrix3d A;
            A << n,         sum_t_,     sum_t2_,
                 sum_t_,     sum_tt_,    sum_t_t2_,
                 sum_t2_,    sum_t_t2_,  sum_t2_t2_;
            Eigen::Vector3d b;
            b << sum_y_, sum_t_y_, sum_t2_y_;
            Eigen::Vector3d x = A.colPivHouseholderQr().solve(b);

            double beta = x(2);

            // Compute residuals and variance of beta
            double sse = 0.0;
            for (size_t i = 0; i < n; ++i) {
                double y_hat = x(0) + x(1) * t[i] + x(2) * t_squared[i];
                sse += std::pow(y[i] - y_hat, 2);
            }
            double denominator = sum_t2_t2_ - std::pow(sum_t2_, 2) / n;
            if (std::abs(denominator) < 1e-12 || n <= 3) return std::numeric_limits<double>::quiet_NaN();
            // Compute variance of beta
            double variance_beta = sse / (n - 3) / denominator;
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
                std::vector<double> t_squared_sub(sub_size);

                std::iota(t_sub.begin(), t_sub.end(), 0);
                std::transform(t_sub.begin(), t_sub.end(), t_squared_sub.begin(),
                               [](double t) { return t * t; });
                try {
                    double explosiveness = compute_explosiveness(y_sub, t_sub, t_squared_sub);
                    if (!std::isnan(explosiveness)) {
                        max_explosiveness = std::max(max_explosiveness, explosiveness);
                    }
                } catch (const std::exception& e) {
i                    continue;
                }
            }
            return std::isinf(max_explosiveness) ? std::numeric_limits<double>::quiet_NaN() : max_explosiveness;
        }

        double phi;
        int tau;
        bool log;
        std::vector<double> buffer;
    };
}

#endif // SCREAMER_EXPANDINGSM_POLY_H
