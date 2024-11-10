#ifndef SCREAMER_ROLLING_RSI_H
#define SCREAMER_ROLLING_RSI_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/common/base.h"
#include "screamer/detail/rolling_sum.h"
#include "screamer/common/float_info.h"

namespace py = pybind11;

namespace screamer {

    class RollingRSI : public ScreamerBase {
    public:

        RollingRSI(int window_size, const std::string& start_policy = "strict") : 
        window_size_(window_size),
        start_policy_(detail::parse_start_policy(start_policy)),
        rolling_gain_sum(window_size, start_policy), 
        rolling_loss_sum(window_size, start_policy)
        {
            if (window_size_ < 2) {
                throw std::invalid_argument("Window size must be 2 or more.");
            }

            reset();
        }

        void reset() override {
            rolling_gain_sum.reset();
            rolling_loss_sum.reset();    
            prev_x = std::numeric_limits<double>::quiet_NaN();
            if (start_policy_ != detail::StartPolicy::Zero)  {
                n_ = 0;
            } else {
                n_ = window_size_;        
            }
        }

        double process_scalar(double x) override {
            double dx = x - prev_x;
            prev_x = x;

            if (isnan2(dx)) {
                dx = 0.0;
            } else {
                if (n_ < window_size_) {
                    n_++;
                }
            }

            double gain_sum = rolling_gain_sum.append(dx > 0.0 ? dx : 0.0);
            double loss_sum = rolling_loss_sum.append(dx < 0.0 ? -dx : 0.0);
            if (start_policy_ == detail::StartPolicy::Strict) {
                return std::numeric_limits<double>::quiet_NaN();
            }
            double rsi = 100.0 * gain_sum / (gain_sum + loss_sum);
            return rsi;
        }

    private:
        const int window_size_;
        const detail::StartPolicy start_policy_;
        detail::RollingSum rolling_gain_sum;
        detail::RollingSum rolling_loss_sum;
        double prev_x;
        int n_;

    }; // end of class

} // end of namespace

#endif // end of include guards
