
#ifndef SCREAMER_ROLLING_STD_H
#define SCREAMER_ROLLING_STD_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingStd : public ScreamerBase {
    public:

        RollingStd(int window_size) : 
            window_size_(window_size), 
            sum_x_buffer(window_size),
            sum_xx_buffer(window_size),
            c0(1.0 / (window_size * (window_size - 1)))
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        void reset() override {
            sum_x_buffer.reset();
            sum_xx_buffer.reset();        
        }
        
        double process_scalar(double newValue) override {
            double sum_x = sum_x_buffer.process_scalar(newValue);
            double sum_xx = sum_xx_buffer.process_scalar(newValue * newValue);
            return std::sqrt((window_size_ * sum_xx - sum_x * sum_x) * c0);
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {

            size_t window_size_ = this->window_size_;
            double sum_x = 0.0;
            double sum_xx = 0.0;

            size_t split = std::min(size, window_size_);

            for (size_t i=0; i<split; i++) {
                sum_x += x[i];
                sum_xx += x[i] * x[i];
                y[i] = std::sqrt((window_size_ * sum_xx - sum_x * sum_x) * c0);
            }
            
            for (size_t i=split; i<size; i++) {
                sum_x = sum_x + x[i] - x[i - window_size_];
                sum_xx = sum_xx + x[i] * x[i] - x[i - window_size_] * x[i - window_size_];
                y[i] = std::sqrt((window_size_ * sum_xx - sum_x * sum_x) * c0);            
            }
        }

    private:
        RollingSum sum_x_buffer;
        RollingSum sum_xx_buffer;
        const double c0;
        const int window_size_;

    }; // end of class

} // end of namespace

#endif // end of include guards

