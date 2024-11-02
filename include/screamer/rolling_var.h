
#ifndef SCREAMER_ROLLING_VAR_H
#define SCREAMER_ROLLING_VAR_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"

/*
todo: this implementation might  suffer from numerical instability
      https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm
*/
namespace py = pybind11;

namespace screamer {

    class RollingVar : public ScreamerBase {
    public:

        RollingVar(int window_size) : 
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
            double N = this->window_size_;
            double sum_x = sum_x_buffer.process_scalar(newValue);
            double sum_xx = sum_xx_buffer.process_scalar(newValue * newValue);
            return (N * sum_xx - sum_x * sum_x) * c0;
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {

            size_t window_size_ = this->window_size_;
            double sum_x = 0.0;
            double sum_xx = 0.0;

            size_t split = std::min(size, window_size_);

            for (size_t i=0; i<split; i++) {
                sum_x += x[i];
                sum_xx += x[i] * x[i];
                y[i] = (window_size_ * sum_xx - sum_x * sum_x) * c0;
            }
            
            for (size_t i=split; i<size; i++) {
                sum_x = sum_x + x[i] - x[i - window_size_];
                sum_xx = sum_xx + x[i] * x[i] - x[i - window_size_] * x[i - window_size_];
                y[i] = (window_size_ * sum_xx - sum_x * sum_x) * c0;            
            }
        }
    private:
        double std_;
        const double c0;
        const int window_size_;
        RollingSum sum_x_buffer;
        RollingSum sum_xx_buffer;


    }; // end of class

} // end of namespace

#endif // end of include guards

