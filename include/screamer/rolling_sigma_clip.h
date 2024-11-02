
#ifndef SCREAMER_ROLLING_SIGMA_CLIP_H
#define SCREAMER_ROLLING_SIGMA_CLIP_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"
#include "screamer/rolling_sum_nan.h"
#include "screamer/common/float_info.h"
#include "screamer/common/math.h"

namespace py = pybind11;

namespace screamer {

    class RollingSigmaClip : public ScreamerBase {
    public:

        RollingSigmaClip(
            int window_size,
            std::optional<double> lower = std::nullopt, 
            std::optional<double> upper = std::nullopt,
            std::optional<int> output = std::nullopt
        ) : 
            sum_x_buffer(window_size),
            sum_xx_buffer(window_size),
            lower_bound_(lower.value_or(std::numeric_limits<double>::lowest())),
            upper_bound_(upper.value_or(std::numeric_limits<double>::max())),
            c0(1.0 / (window_size * (window_size - 1))),
            window_size_(window_size), 
            output_(output.value_or(0))
        {
            if (window_size_ <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }

            if (output_ < 0 || output_ > 3) {
                throw std::invalid_argument("Output order must be 0 (clipped value), 1 (mean est.), or 2 (std est.) or 3 clipped as NaN.");
            }

            if (lower_bound_ >= upper_bound_ ) {
                throw std::invalid_argument("Lower bound must be below the upper bound.");
            }

            standard_truncated_normal_mean_variance(
                lower_bound_,
                upper_bound_,
                mu_trunc,
                sigma_trunc
            );
            reset();
        }

        void reset() override {
            sum_x_buffer.reset();
            sum_xx_buffer.reset();       
            mean_ = std::numeric_limits<double>::quiet_NaN();
            std_ = std::numeric_limits<double>::quiet_NaN(); 
        }
        
        void _update_mean_std(double newValue) {
            double sum_x = sum_x_buffer.process_scalar(newValue);
            double sum_xx = sum_xx_buffer.process_scalar(newValue * newValue);

            // Compute the raw mean and std
            double observed_mean = sum_x / window_size_;
            double observed_std = std::sqrt((window_size_ * sum_xx - sum_x * sum_x) * c0);

            // Correct to the mean_ and std_ for truncation bias
            estimate_true_mean_std(
                observed_mean, 
                observed_std, 
                mu_trunc, 
                sigma_trunc, 
                mean_, 
                std_
            );
        }

        double process_scalar(double newValue) override {

            double zscore, clippedValue;

            // If newValue is NaN then there will be no clipping and/or updating
            if (isnan2(newValue)) {
                if (output_ == 1) return mean_;
                if (output_ == 2) return std_;
                return std::numeric_limits<double>::quiet_NaN();
            }

            // if we have a mean and std then we can do clipping
            clippedValue = newValue;
            if ( (!isnan2(mean_)) && (!isnan2(std_))) {
                zscore = (newValue - mean_) / std_;
                if (zscore < lower_bound_) {
                    clippedValue =  mean_ + lower_bound_ * std_;
                }
                if (zscore > upper_bound_) {
                    clippedValue = mean_ + upper_bound_ * std_;
                }
            }

            // did we clip?
            if (clippedValue != newValue) {
                if (output_ == 0) return clippedValue;
                if (output_ == 1) return mean_;
                if (output_ == 2) return std_;
                return std::numeric_limits<double>::quiet_NaN()  ;              
            }

            // we didn't clip: update 
            _update_mean_std(newValue);

            // return the final result
            if (output_ == 1) return mean_;
            if (output_ == 2) return std_;
            return newValue;
        }


    private:
        RollingSumNan sum_x_buffer;
        RollingSumNan sum_xx_buffer;

        const double c0;
        const double lower_bound_;
        const double upper_bound_;
        const int window_size_;
        const int output_;
        double mu_trunc;
        double sigma_trunc;

        double mean_;
        double std_;


    }; // end of class

} // end of namespace

#endif // end of include guards

