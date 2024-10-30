#ifndef SCREAMER_ROLLING_POLY1_H
#define SCREAMER_ROLLING_POLY1_H

#include <screamer/buffer.h>
#include "screamer/base.h"
#include <stdexcept>
#include <tuple>

namespace screamer {

    class RollingPoly1 : public ScreamerBase {
    public:
        RollingPoly1(int window_size, int derivative_order = 0) : 
            window_size_(window_size),
            derivative_order_(derivative_order),
            sum_y_buffer(window_size)
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
            if (derivative_order != 0 && derivative_order != 1) {
                throw std::invalid_argument("Derivative order must be 0 (endpoint) or 1 (slope).");
            }

            double N = window_size;
            // the sum of x-values [0, 1, 2, ..., window_size - 1]
            sum_x = (N - 1.0) * N / 2.0;

            // the population variance of x-values multiplied by n (window_size).
            n_var_x = (N - 1.0) * N * (N + 1.0) / 12.0;

        }

        void reset() override {
            sum_y_buffer.reset();
            sum_y = 0.0;
            sum_xy = 0.0;
        }

        double process_scalar(double y) override {
            sum_y = sum_y_buffer.process_scalar(y);
            sum_xy += y * window_size_ - sum_y;
            double n_covar_xy = sum_xy - sum_x * sum_y / window_size_;

            double slope = n_covar_xy / n_var_x;
            double intercept = (sum_y - slope * sum_x) / window_size_;
            double endpoint = intercept + slope * (window_size_ - 1.0);
            // Return based on mode: endpoint if mode == 0, or slope if mode == 1
            return derivative_order_ == 0 ? endpoint : slope;
        }

    private:
        const size_t window_size_;
        double sum_x, sum_y, sum_xy, n_var_x;
        RollingSum sum_y_buffer;

        const int derivative_order_;

    };

} // end namespace screamer

#endif // SCREAMER_ROLLING_POLY1_H
