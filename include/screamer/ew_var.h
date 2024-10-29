// rolling_mean.h
#ifndef SCREAMER_EW_VAR_H
#define SCREAMER_EW_VAR_H

#include <optional>
#include <stdexcept>
#include <cmath>
#include "screamer/base.h"

/*
Info about the bias correction: https://osquant.com/papers/replicating-pandas-ewm-var/ 

Exponetially weighted statistics have a sum of weights that's a geometric series

Geometric series:
sum_w = 1 + a + a^2 + ... + a^n 
      = (1 - a^(n+1)) / (1 - a)

The bias correction term for exponentially weighted variance is:

b = sum_w^2 / ( sum_w^2 - sum_w2)

with 
sum_w2 = 1 + a^2 + a^4 + ... + a^2n
       = 1 - a^(2*(n+1))) / (1 - a^2)

and we should get
b = (1 - w^{n+1}) (1 + w) / [ 2w (1-w^n) ]

A simpler way is to compute "effective sample size" for exponentialy weighted 
statistical indicators, and plug that value in for N in the unbiased sample
estimates. Like for the variance estimate: sum (x - mu)^2 / (N - 1)

n_eff = S_1^2 / S_2

S_1 <- (1 - alpha) S_1 + 1
S_2 <- (1 - alpha)^2 S_2 + 1
*/

namespace screamer {

    class EwVar : public ScreamerBase {
    public:
        explicit EwVar(
            std::optional<double> com = std::nullopt,
            std::optional<double> span = std::nullopt,
            std::optional<double> halflife = std::nullopt,
            std::optional<double> alpha = std::nullopt)
        {
            // Count the number of provided arguments
            int provided_args = (com.has_value() ? 1 : 0) +
                                (span.has_value() ? 1 : 0) +
                                (halflife.has_value() ? 1 : 0) +
                                (alpha.has_value() ? 1 : 0);

            if (provided_args != 1) {
                throw std::invalid_argument("Exactly one of com, span, halflife, or alpha must be provided");
            }

            // Map provided argument to alpha
            if (alpha.has_value()) {
                alpha_ = alpha.value();
            } else if (com.has_value()) {
                alpha_ = 1.0 / (1.0 + com.value());
            } else if (span.has_value()) {
                alpha_ = 2.0 / (span.value() + 1.0);
            } else if (halflife.has_value()) {
                alpha_ = 1.0 - std::exp(-std::log(2.0) / halflife.value());
            }

            // Validate alpha
            if (alpha_ <= 0.0 || alpha_ >= 1.0) {
                throw std::invalid_argument("Alpha must be between 0 and 1 (exclusive)");
            }
            one_minus_alpha_ = 1.0 - alpha_;
            one_minus_alpha2_ = one_minus_alpha_*one_minus_alpha_;

            reset();
        }

        void reset() override {
            sum_x_ = 0.0;
            sum_xx_ = 0.0;
            sum_w_ = 0.0;
            sum_w2_ = 0.0;
        }

        double process_scalar(double newValue) override {
            sum_x_ *= one_minus_alpha_;
            sum_xx_ *= one_minus_alpha_;

            sum_w_ *= one_minus_alpha_;
            sum_w2_ *= one_minus_alpha2_;

            sum_x_ += newValue;
            sum_xx_ += newValue * newValue;

            sum_w_ += 1.0;
            sum_w2_ += 1.0;

            double n_eff = sum_w_* sum_w_ / sum_w2_; 
    
            // Compute the weighted mean
            double mean = sum_x_ / sum_w_;

            // Compute the weighted variance
            double variance = (sum_xx_ / sum_w_) - (mean * mean);
            variance *= n_eff / (n_eff - 1.0);
            return variance;
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {
            double one_minus_alpha_ = this->one_minus_alpha_;
            double sum_x_ = 0.0;
            double sum_xx_ = 0.0;
            double sum_w_ = 0.0;
            double sum_w2_ = 0.0;

            for (size_t i=0; i<size; i++) {
                sum_x_ *= one_minus_alpha_;
                sum_xx_ *= one_minus_alpha_;

                sum_w_ *= one_minus_alpha_;
                sum_w2_ *= one_minus_alpha2_;

                sum_x_ += x[i];
                sum_xx_ += x[i] * x[i];

                sum_w_ += 1.0;
                sum_w2_ += 1.0;
                
                double n_eff = sum_w_ * sum_w_ / sum_w2_; 
                double mean = sum_x_ / sum_w_;
                double variance = (sum_xx_ / sum_w_) - (mean * mean);
                variance *= n_eff / (n_eff - 1.0);
                y[i] = variance;      
            }
        }

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {
            
            double one_minus_alpha_ = this->one_minus_alpha_;
            double sum_x_ = 0.0;
            double sum_xx_ = 0.0;
            double sum_w_ = 0.0;
            double sum_w2_ = 0.0;
            
            size_t xi = 0;
            size_t yi = 0;

            for (size_t i=0; i<size; i++) { // start at 1
                sum_x_ *= one_minus_alpha_;
                sum_xx_ *= one_minus_alpha_;

                sum_w_ *= one_minus_alpha_;
                sum_w2_ *= one_minus_alpha2_;

                sum_x_ += x[xi];
                sum_xx_ += x[xi]*x[xi];

                sum_w_ += 1.0;
                sum_w2_ += 1.0;

                double n_eff = sum_w_* sum_w_ / sum_w2_; 
                double mean = sum_x_ / sum_w_;
                double variance = (sum_xx_ / sum_w_) - (mean * mean);
                variance *= n_eff / (n_eff - 1.0);
                y[yi] = variance;
                xi += dxi;
                yi += dyi;                
            }
        }  

    private:
        double alpha_;
        
        double one_minus_alpha_;
        double one_minus_alpha2_;
        double sum_x_;
        double sum_xx_;
        double sum_w_;
        double sum_w2_;
        double w2_;
    };

} // namespace screamer

#endif // SCREAMER_EW_VAR_H
