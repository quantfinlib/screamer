#ifndef SCREAMER_ROLLING_SIGMA_CLIP_H
#define SCREAMER_ROLLING_SIGMA_CLIP_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/common/base.h"
#include "screamer/detail/rolling_sum.h"
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
            std::optional<int> output = std::nullopt,
            const std::string& start_policy = "strict"
        ) : 
            window_size_(window_size), 
            lower_bound_(lower.value_or(std::numeric_limits<double>::lowest())),
            upper_bound_(upper.value_or(std::numeric_limits<double>::max())),
            output_(output.value_or(0)),
            start_policy_(detail::parse_start_policy(start_policy)),
            sum_x_buffer(window_size, start_policy),
            sum_xx_buffer(window_size, start_policy)           
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
            if (start_policy_ != detail::StartPolicy::Zero)  {
                n_ = 0;
            } else {
                n_ = window_size_;        
            }            
        }
        
        void _update_mean_std(double newValue,size_t n_) {
            double sum_x = sum_x_buffer.append(newValue);
            double sum_xx = sum_xx_buffer.append(newValue * newValue);

            // Compute the raw mean and std
            double observed_mean = sum_x / n_;
            double observed_std = std::sqrt((sum_xx - sum_x * sum_x / n_) / (n_ - 1));

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

            // did we clip? If so we don't update stats, we return
            if (clippedValue != newValue) {
                if (n_ < window_size_) {
                    if (start_policy_ == detail::StartPolicy::Strict) {
                        return std::numeric_limits<double>::quiet_NaN();
                    }
                    if (n_ < 2) return std::numeric_limits<double>::quiet_NaN();
                }
                if (output_ == 0) return clippedValue;
                if (output_ == 1) return mean_;
                if (output_ == 2) return std_;
                return std::numeric_limits<double>::quiet_NaN();
            }

            // we didn't clip: update 
            if (n_ < window_size_) {
                n_++;
            } 
            _update_mean_std(newValue, n_);

            // return the final result
            if (n_ < window_size_) {
                if (start_policy_ == detail::StartPolicy::Strict) {
                    return std::numeric_limits<double>::quiet_NaN();
                }
                if (n_ < 2) return std::numeric_limits<double>::quiet_NaN();
            }        
            if (output_ == 1) return mean_;
            if (output_ == 2) return std_;
            return newValue;
        }


    private:
        detail::RollingSum sum_x_buffer;
        detail::RollingSum sum_xx_buffer;
        const detail::StartPolicy start_policy_;

        const double lower_bound_;
        const double upper_bound_;
        const int window_size_;
        const int output_;
        int n_;
        double mu_trunc;
        double sigma_trunc;

        double mean_;
        double std_;


    }; // end of class

} // end of namespace

#endif // end of include guards

