#ifndef SCREAMER_EW_SKEW_H
#define SCREAMER_EW_SKEW_H

#include <optional>
#include <stdexcept>
#include <cmath>
#include "screamer/base.h"

namespace screamer {

    class EwSkew : public ScreamerBase {
    public:
        explicit EwSkew(
            std::optional<double> com = std::nullopt,
            std::optional<double> span = std::nullopt,
            std::optional<double> halflife = std::nullopt,
            std::optional<double> alpha = std::nullopt)
        {
            // Same setup for alpha as in EwStd
            int provided_args = (com.has_value() ? 1 : 0) +
                                (span.has_value() ? 1 : 0) +
                                (halflife.has_value() ? 1 : 0) +
                                (alpha.has_value() ? 1 : 0);

            if (provided_args != 1) {
                throw std::invalid_argument("Exactly one of com, span, halflife, or alpha must be provided");
            }

            if (alpha.has_value()) {
                alpha_ = alpha.value();
            } else if (com.has_value()) {
                alpha_ = 1.0 / (1.0 + com.value());
            } else if (span.has_value()) {
                alpha_ = 2.0 / (span.value() + 1.0);
            } else if (halflife.has_value()) {
                alpha_ = 1.0 - std::exp(-std::log(2.0) / halflife.value());
            }

            if (alpha_ <= 0.0 || alpha_ >= 1.0) {
                throw std::invalid_argument("Alpha must be between 0 and 1 (exclusive)");
            }
            one_minus_alpha_ = 1.0 - alpha_;
            one_minus_alpha2_ = one_minus_alpha_ * one_minus_alpha_;

            reset();
        }

        void reset() override {
            sum_x_ = 0.0;
            sum_xx_ = 0.0;
            sum_xxx_ = 0.0;
            sum_w_ = 0.0;
            sum_w2_ = 0.0;
        }

        double process_scalar(double newValue) override {
            sum_x_ *= one_minus_alpha_;
            sum_xx_ *= one_minus_alpha_;
            sum_xxx_ *= one_minus_alpha_;

            sum_w_ *= one_minus_alpha_;
            sum_w2_ *= one_minus_alpha2_;

            sum_x_ += newValue;
            sum_xx_ += newValue * newValue;
            sum_xxx_ += newValue * newValue * newValue;

            sum_w_ += 1.0;
            sum_w2_ += 1.0;

            double n_eff = sum_w_ * sum_w_ / sum_w2_;

            // Compute the weighted mean
            double mean = sum_x_ / sum_w_;

            // Compute the weighted variance
            double variance = (sum_xx_ / sum_w_) - (mean * mean);
            variance *= n_eff / (n_eff - 1.0);
            double std_dev = std::sqrt(variance);

            // Compute the third central moment (m3)
            double m3 = (sum_xxx_ / sum_w_) - 3 * mean * (sum_xx_ / sum_w_) + 2 * mean * mean * mean;

            // Adjust skewness using Pandas-like bias correction
            double g1 = m3 / (std_dev * std_dev * std_dev);
            double skew = (n_eff * g1) / ((n_eff - 1.0) * (n_eff - 2.0));

            return skew;
        }

    private:
        double alpha_;
        
        double one_minus_alpha_;
        double one_minus_alpha2_;
        double sum_x_;
        double sum_xx_;
        double sum_xxx_;
        double sum_w_;
        double sum_w2_;
    };

} // namespace screamer

#endif // SCREAMER_EW_SKEW_H
