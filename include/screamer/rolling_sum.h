#ifndef SCREAMER_ROLLING_SUM_H
#define SCREAMER_ROLLING_SUM_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingSum : public ScreamerBase {
    public:

        RollingSum(int window_size) : 
            window_size_(window_size), 
            sum_(0.0),
             buffer_(window_size, 0.0) 
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        void reset() override {
            buffer_.reset(0.0);
            sum_ = 0.0;
        }
        
        double process_scalar(double newValue) override {

            sum_ -= buffer_.append(newValue);
            sum_ += newValue;

            return sum_;            
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {

            size_t window_size_ = this->window_size_;
            y[0] = x[0];

            size_t split = std::min(size, window_size_);

            for (size_t i=1; i<split; i++) {
                y[i] = y[i - 1] + x[i];
            }
            
            for (size_t i=split; i<size; i++) {
                y[i] = y[i - 1] + (x[i] - x[i - window_size_]);
            }
        }

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {

            // the first element, we don't have a previous y value
            y[0] = x[0];

            // the elements < window_size don't have a x[i - window_size]
            size_t split = std::min(size, window_size_);

            // start at the 2nd element, i=1 in the loop below
            size_t xi = dxi;
            size_t yi = dyi;

            for (size_t i=1; i<split; i++) { // start at 1
                y[yi] = y[yi - dyi] + x[xi];
                xi += dxi;
                yi += dyi;
            }

            // all other elements
            size_t window_size_shift = window_size_ * dxi;
            for (size_t i=split; i<size; i++) {
                y[yi] = y[yi - dyi] + (x[xi] - x[xi - window_size_shift]);
                xi += dxi;
                yi += dyi;                
            }
           
        }

    private:
        FixedSizeBuffer buffer_;
        double sum_;
        const size_t window_size_;

    }; // end of class

} // end of namespace

#endif // end of include guards
