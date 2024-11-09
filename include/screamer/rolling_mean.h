#ifndef SCREAMER_ROLLING_MEAN_H
#define SCREAMER_ROLLING_MEAN_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/detail/rolling_mean.h"
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingMean : public ScreamerBase {
    public:

        RollingMean(int window_size, const std::string& start_policy = "strict") : 
            rolling_mean_(window_size, start_policy)
        {
        }

        void reset() override {
            rolling_mean_.reset();
        }
        
        double process_scalar(double newValue) override {
            return rolling_mean_.append(newValue);        
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {
            size_t window_size_ = rolling_mean_.capacity();
            double one_over_w_ = 1.0 / window_size_;

            size_t split = std::min(size, window_size_);

            for (size_t i=0; i<split; i++) {
                y[i] = rolling_mean_.append(x[i]);
            }
            
            for (size_t i=split; i<size; i++) {
                y[i] = y[i - 1] + (x[i] - x[i - window_size_]) * one_over_w_;
            }
        }

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {
            size_t window_size_ = rolling_mean_.capacity();
            double one_over_w_ = 1.0 / window_size_;

            size_t split = std::min(size, window_size_);

            for (size_t i = 0, xi = 0, yi = 0; i < split; ++i, xi += dxi, yi += dyi) {
                y[yi] = rolling_mean_.append(x[xi]);
            }

            size_t shift_x_forward_ = window_size_ * dxi;

            for (size_t i = split, xi = 0, yi = window_size_ * dyi; i < size; ++i, xi += dxi, yi += dyi) {
                y[yi] = y[yi - dyi] + (x[xi + shift_x_forward_] - x[xi]) * one_over_w_;
            }
           
        }

    private:
        screamer::detail::RollingMean rolling_mean_;

    }; // end of class

} // end of namespace

#endif // end of include guards
