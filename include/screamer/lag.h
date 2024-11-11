#ifndef SCREAMER_LAG_H
#define SCREAMER_LAG_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/detail/delay_buffer.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class Lag : public ScreamerBase {
    public:

        Lag(int window_size, const std::string& start_policy = "strict") : 
            delay_buffer_(window_size, start_policy)
        {
        }

        void reset() override {
            delay_buffer_.reset();
        }
        
        double process_scalar(double newValue) override {

            return delay_buffer_.append(newValue);         
        }

        void process_array_no_stride(double* y,  const double* x, size_t size) override {
            size_t window_size_ = delay_buffer_.capacity();
            size_t split = std::min<size_t>(window_size_, size);

            for (int i=0; i < split; ++i) {
                y[i] = delay_buffer_.append(x[i]);
            }

            if (size > window_size_) {
                std::memcpy(y + window_size_, x, (size - window_size_) * sizeof(double));
            }
        }       

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {
            size_t window_size_ = delay_buffer_.capacity();
            size_t split = std::min<size_t>(window_size_, size);

            for (size_t i = 0, xi = 0, yi = 0; i < split; ++i, xi += dxi, yi += dyi) {
                y[yi] = delay_buffer_.append(x[xi]);
            }

            for (size_t i = split, xi = 0, yi = window_size_ * dyi; i < size; ++i, xi += dxi, yi += dyi) {
                y[yi] = x[xi];
            }           
        }

    private:
        screamer::detail::DelayBuffer delay_buffer_;

    }; // end of class

} // end of namespace

#endif // end of include guards
