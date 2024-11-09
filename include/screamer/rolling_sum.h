#ifndef SCREAMER_ROLLING_SUM_H
#define SCREAMER_ROLLING_SUM_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/detail/rolling_sum.h"
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingSum : public ScreamerBase {
    public:

        RollingSum(int window_size, const std::string& start_policy = "strict") : 
            rolling_sum_(window_size, start_policy)
        {
        }

        void reset() override {
            rolling_sum_.reset();
        }
        
        double process_scalar(double newValue) override {
            return rolling_sum_.append(newValue);        
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {
            size_t window_size_ = rolling_sum_.capacity();

            size_t split = std::min(size, window_size_);

            for (size_t i=0; i<split; i++) {
                y[i] = rolling_sum_.append(x[i]);
            }
            
            for (size_t i=split; i<size; i++) {
                y[i] = y[i - 1] + x[i] - x[i - window_size_];
            }
        }

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {
            size_t window_size_ = rolling_sum_.capacity();

            size_t split = std::min(size, window_size_);

            for (size_t i = 0, xi = 0, yi = 0; i < split; ++i, xi += dxi, yi += dyi) {
                y[yi] = rolling_sum_.append(x[xi]);
            }

            size_t shift_x_forward_ = window_size_ * dxi;
            for (size_t i = split, xi = 0, yi = window_size_ * dyi; i < size; ++i, xi += dxi, yi += dyi) {
                y[yi] = y[yi - dyi] + x[xi + shift_x_forward_] - x[xi];
            }
           
        }

    private:
        screamer::detail::RollingSum rolling_sum_;

    }; // end of class

} // end of namespace

#endif // end of include guards
