#ifndef SCREAMER_ROLLING_MEAN_H
#define SCREAMER_ROLLING_MEAN_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/buffer.h>
#include "screamer/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingMean : public ScreamerBase {
    public:

        RollingMean(int window_size) : 
            window_size_(window_size), 
            one_over_w_(1.0 / window_size), 
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
        
    private:

        double process_scalar(double newValue) override {

            sum_ -= buffer_.append(newValue);
            sum_ += newValue;

            return sum_ * one_over_w_;            
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {
            double one_over_w_ = this->one_over_w_;
            size_t window_size_ = this->window_size_;

            y[0] = x[0] * one_over_w_;

            size_t split = std::min(size, window_size_);

            for (size_t i=1; i<split; i++) {
                y[i] = y[i - 1] + x[i] * one_over_w_;
            }
            
            for (size_t i=split; i<size; i++) {
                y[i] = y[i - 1] + (x[i] - x[i - window_size_]) * one_over_w_;
            }
        }

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {

            // the first element, we don't have a previous y value
            y[0] = x[0] * one_over_w_;

            // the elements < window_size don't have a x[i - window_size]
            size_t split = std::min(size, window_size_);

            // start at the 2nd element, i=1 in the loop below
            size_t xi = dxi;
            size_t yi = dyi;

            for (size_t i=1; i<split; i++) { // start at 1
                y[yi] = y[yi - dyi] + x[xi] * one_over_w_;
                xi += dxi;
                yi += dyi;
            }

            // all other elements
            size_t window_size_shift = window_size_ * dxi;
            for (size_t i=split; i<size; i++) {
                y[yi] = y[yi - dyi] + (x[xi] - x[xi - window_size_shift]) * one_over_w_;
                xi += dxi;
                yi += dyi;                
            }
           
        }

    private:
        FixedSizeBuffer buffer_;
        double sum_;
        const size_t window_size_;
        const double one_over_w_; // multiplication is faster than dividsion


    }; // end of class

} // end of namespace

#endif // end of include guards
