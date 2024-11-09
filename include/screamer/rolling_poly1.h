#ifndef SCREAMER_ROLLING_POLY1_H
#define SCREAMER_ROLLING_POLY1_H

#include "screamer/common/base.h"
#include "screamer/common/float_info.h"
#include <stdexcept>
#include <tuple>
#include "screamer/detail/delay_buffer.h"

/*
## OLS Fit

Define the following summations:
Sx = x0 + x1 + ... + x(n-1)
Sy = y0 + y1 + ... + y(n-1)
Sxy = x0*y0 + x1*y1 + ... + x(n-1)*y(n-1)
Sxx = x0*x0 + x1*x1 + ... + x(n-1)*x(n-1)

The slope and intercept of OLS fit of y = a * x + b is:

a = [n * Sxy - Sx * Sy] / [n * Sxx - Sx * Sx]
b = [Sy - a * Sx] / n

## Sliding the window forward one step
We one slide y forward, we keep x = [0, 1, ..., n-1]
then 
Sx = n (n - 1) / 2
Sxx = (n - 1) n (2*n - 1) / 6

### Sliding Sy
The new value we recieve is yn

Sy` <- Sy + yn - y0
    = y1 + y2 + ... + y(n-1) + yn

### Sliding Sxy
before sliding we have: 
Sxy  = 0*y0 + 1*y1 + 2*y2 + ... + (n-1)*y(n-1)
after slidng we need
Sxy` =        0*y1 + 1*y2 + ... + (n-2)*y(n-1) + (n-1)*yn
thus
Sxy` = Sxy - [y1 + y2 + ... + y(n-1)     ] + (n-1)*yn
     = Sxy - [y1 + y2 + ... + y(n-1) + yn] + n*yn
     = Sxy - Sy` + n*yn

*/
namespace screamer {

    class RollingPoly1 : public ScreamerBase {
    public:
        RollingPoly1(int window_size, int derivative_order = 0, const std::string& start_policy = "strict") : 
            window_size_(window_size),
            derivative_order_(derivative_order),
            start_policy_(detail::parse_start_policy(start_policy)),
            n_(0),
            delay_buffer_(window_size, "zero")
        {
            if (window_size < 2) {
                throw std::invalid_argument("Window size must be 2 or more.");
            }
            if (derivative_order != 0 && derivative_order != 1) {
                throw std::invalid_argument("Derivative order must be 0 (endpoint) or 1 (slope).");
            }

            reset();
        }

        void reset() override {
            delay_buffer_.reset();
            sum_y = 0.0;
            sum_xy = 0.0;    

            if (start_policy_ != detail::StartPolicy::Zero)  {
                n_ = 0;
                sum_x = 0.0;
                sum_xx = 0.0;
            } else {
                n_ = window_size_;
                sum_x = (n_ - 1.0) * n_ / 2.0;
                sum_xx = (n_ - 1.0) * n_ * (2*n_ - 1.0) / 6.0;            
            }
        }

        double process_scalar(double yn) override {

            if (n_ < window_size_) {
                delay_buffer_.append(yn); // we need to fill the buffer for later
                sum_y += yn;

                // update sum_x and sum_xx if the windows is expanding
                if (start_policy_ != detail::StartPolicy::Zero) { // expanding (and strict uses n_ from this)
                    sum_x += n_;
                    sum_xx += n_ * n_;
                    sum_xy += n_ * yn; // expanding uses a regular sum
                    n_++;
                } else {
                    sum_xy += window_size_ * yn - sum_y; // the zero policy assumes zero padding and full window size        
                }
                
            } else {
                double y0 = delay_buffer_.append(yn);
                sum_y += yn - y0;
                sum_xy += window_size_ * yn - sum_y;
            }

            if (n_ < window_size_) {
                if (start_policy_ == detail::StartPolicy::Strict) return std::numeric_limits<double>::quiet_NaN();
                if (n_ < 2) return std::numeric_limits<double>::quiet_NaN();
            }
            
            double slope = (n_ * sum_xy - sum_x * sum_y) / (n_ * sum_xx - sum_x * sum_x);
            double intercept = (sum_y - slope * sum_x) / n_;
            double endpoint = intercept + (n_ - 1) * slope;

            return derivative_order_ == 0 ? endpoint : slope;
        }

    private:
        const size_t window_size_;
        const int derivative_order_;
        const detail::StartPolicy start_policy_;
        size_t n_;
        detail::DelayBuffer delay_buffer_;
        double sum_x, sum_xx, sum_y, sum_xy;
    };

} // end namespace screamer

#endif // SCREAMER_ROLLING_POLY1_H
