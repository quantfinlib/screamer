
#ifndef SCREAMER_ROLLING_VAR_H
#define SCREAMER_ROLLING_VAR_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/buffer.h>
#include "screamer/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingVar : public ScreamerBase {
    public:

        RollingVar(int window_size) : 
            window_size_(window_size), 
            sum_x_buffer(window_size),
            sum_xx_buffer(window_size)
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        void reset() override {
            sum_x_buffer.reset();
            sum_xx_buffer.reset();        
        }
        
    private:

        double process_scalar(double newValue) override {
            double sum_x = sum_x_buffer(newValue);
            double sum_xx = sum_xx_buffer(newValue * newValue);
            return (window_size_ * sum_xx - sum_x * sum_x) / (window_size_ * (window_size_ - 1));
        }

    private:
        RollingSum sum_x_buffer;
        RollingSum sum_xx_buffer;
        double std_;
        const int window_size_;

    }; // end of class

} // end of namespace

#endif // end of include guards

