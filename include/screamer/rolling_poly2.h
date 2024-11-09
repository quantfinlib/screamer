#ifndef SCREAMER_ROLLING_POLY2_H
#define SCREAMER_ROLLING_POLY2_H

#include <screamer/common/buffer.h>
#include "screamer/common/base.h"
#include <stdexcept>
#include <tuple>
#include "screamer/detail/delay_buffer.h"


namespace screamer {

    class RollingPoly2 : public ScreamerBase {
    public:
        RollingPoly2(int window_size, int derivative_order = 0, const std::string& start_policy = "strict") : 
            window_size_(window_size),
            derivative_order_(derivative_order),
            start_policy_(detail::parse_start_policy(start_policy)),
            n_(0),
            delay_buffer_(window_size, "zero")
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
            if (derivative_order < 0 || derivative_order > 2) {
                throw std::invalid_argument("Derivative order must be 0 (endpoint), 1 (slope), or 2 (curvature).");
            }


            // derived stats without y
            Zxx = sum_xx - sum_x * sum_x / n_;
            Zxx2 = sum_xxx - sum_xx * sum_x / n_;
            Zx2x2 = sum_xxxx - sum_xx * sum_xx / n_;
            d = Zxx * Zx2x2 - Zxx2 * Zxx2;

            // reset the dynamic variables
            reset();
        }

        void reset() override {
            delay_buffer_.reset();
            sum_y = 0.0;
            sum_xy = 0.0;
            sum_xxy = 0.0;

            if (start_policy_ == screamer::detail::StartPolicy::Expanding) {
                n_ = 0;
                sum_x = 0.0;
                sum_xx = 0.0;
                sum_xxx = 0.0;
                sum_xxxx = 0.0;
            } else {
                n_ = window_size_;
                sum_x = (n_ - 1.0) * n_ / 2.0;
                sum_xx = (n_ - 1.0) * n_ * (2*n_ - 1.0) / 6.0;
                sum_xxx = n_ * n_ * (n_ - 1) * (n_ - 1) / 4;
                sum_xxxx = (n_ - 1) * n_ * (2*n_ - 1) * (3*n_*n_ - 3*n_ - 1) / 30;                      
            }    

            // derived statistics that have no y term
            Zxx = sum_xx - sum_x * sum_x / n_;
            Zxx2 = sum_xxx - sum_xx * sum_x / n_;
            Zx2x2 = sum_xxxx - sum_xx * sum_xx / n_;
            d = Zxx * Zx2x2 - Zxx2 * Zxx2;

        }

        double process_scalar(double yn) override {

            if (n_ < window_size_) {
                if (start_policy_ == detail::StartPolicy::Expanding) {
                    sum_x += n_;
                    sum_xx += n_ * n_;
                    sum_xxx += n_ * n_ * n_;
                    sum_xxxx += n_ * n_ * n_ * n_;

                    // derived statistics that have no y term
                    Zxx = sum_xx - sum_x * sum_x / n_;
                    Zxx2 = sum_xxx - sum_xx * sum_x / n_;
                    Zx2x2 = sum_xxxx - sum_xx * sum_xx / n_;
                    d = Zxx * Zx2x2 - Zxx2 * Zxx2;

                    n_++;
                } // else we have set things to n=window_size in the reset()
            }

            double y0 = delay_buffer_.append(yn);
            sum_y += yn - y0;

            // update the sums with "y" terms
            // the order of 3 eqs is important!
            sum_xxy += n_ * (n_ - 2) * yn + sum_y - 2 * sum_xy;
            sum_xy += n_ * yn - sum_y;

            if (n_ < window_size_) {
                if (start_policy_ == detail::StartPolicy::Strict) {
                    return std::numeric_limits<double>::quiet_NaN();
                }
            }

            // compute derived stats with y terms
            double Zxy = sum_xy - sum_x * sum_y / n_;
            double Zx2y = sum_xxy - sum_xx * sum_y / n_;

            // Compute a,b,c
            double a = (Zx2y * Zxx - Zxy * Zxx2) / d;
            double b = (Zxy * Zx2x2 - Zx2y * Zxx2) / d;
            double c = sum_y / n_ - b * sum_x / n_ - a * sum_xx / n_;
            
            // Calculating endpoint, slope, and curvature based on derivative_order_
            double endpoint = a * (n_ - 1) * (n_ - 1) + b * (n_ - 1) + c;
            double slope = 2 * a * (n_ - 1) + b;
            double curvature = 2 * a;
           
            // Return based on derivative_order_
            return derivative_order_ == 0 ? endpoint : (derivative_order_ == 1 ? slope : curvature);
        }

    private:
        const size_t window_size_;
        const int derivative_order_;
        const detail::StartPolicy start_policy_;
        detail::DelayBuffer delay_buffer_;
        size_t n_;
        double sum_x, sum_xx, sum_xxx, sum_xxxx;
        double sum_y, sum_xy, sum_xxy;
        double Zxx, Zxy, Zxx2, Zx2y, Zx2x2, d;

    };

} // end namespace screamer

#endif // SCREAMER_ROLLING_POLY2_H
