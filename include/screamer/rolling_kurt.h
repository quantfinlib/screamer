
#ifndef SCREAMER_ROLLING_KURT_H
#define SCREAMER_ROLLING_KURT_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/common/base.h"
#include "screamer/common/math.h"
#include "screamer/detail/rolling_sum.h"

namespace py = pybind11;

namespace screamer {

    class RollingKurt : public ScreamerBase {
    public:

        RollingKurt(
            int window_size,
            const std::string& start_policy = "strict"
        ) : 
            window_size_(window_size), 
            start_policy_(detail::parse_start_policy(start_policy)),
            sum_x_buffer(window_size, start_policy),
            sum_xx_buffer(window_size, start_policy),
            sum_xxx_buffer(window_size, start_policy),
            sum_xxxx_buffer(window_size, start_policy)
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
            
            reset();
        }

        void reset() override {
            sum_x_buffer.reset();
            sum_xx_buffer.reset();
            sum_xxx_buffer.reset();
            sum_xxxx_buffer.reset();   

            n_ = (start_policy_ != detail::StartPolicy::Zero) ? 0 : window_size_;

            kurt_n_const(n_, c0, c1, c2);
        }
        
    private:

        double process_scalar(double newValue) override {

            if ((n_ < window_size_) && (start_policy_ != detail::StartPolicy::Zero) ) {
                n_++;
                kurt_n_const(n_, c0, c1, c2);
            }

            // Update the rolling sums
            double newValue2 = newValue*newValue;
            double sum_x = sum_x_buffer.append(newValue);
            double sum_xx = sum_xx_buffer.append(newValue2);
            double sum_xxx = sum_xxx_buffer.append(newValue2 * newValue);
            double sum_xxxx = sum_xxxx_buffer.append(newValue2 * newValue2);

            double kurt;
            kurt_from_stats(sum_x, sum_xx, sum_xxx, sum_xxxx, c0, c1, c2, n_, kurt);
            return kurt;
        }

    private:
        const int window_size_;
        const detail::StartPolicy start_policy_;
        detail::RollingSum sum_x_buffer;
        detail::RollingSum sum_xx_buffer;
        detail::RollingSum sum_xxx_buffer;
        detail::RollingSum sum_xxxx_buffer;

        double c0;
        double c1;
        double c2;
        int n_;

    }; // end of class

} // end of namespace

#endif // end of include guards
