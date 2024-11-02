#ifndef SCREAMER_ROLLING_RMS_H
#define SCREAMER_ROLLING_RMS_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingRms : public ScreamerBase {
    public:

        RollingRms(int window_size) : 
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

            sum_ -= buffer_.append(newValue * newValue);
            sum_ += newValue * newValue;

            return std::sqrt(sum_ * one_over_w_);            
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {
            double one_over_w_ = this->one_over_w_;
            size_t window_size_ = this->window_size_;
            size_t split = std::min(size, window_size_);

            y[0] = x[0] * x[0] * one_over_w_;
            for (size_t i=1; i<split; i++) {
                y[i] = y[i - 1] + x[i] * x[i] * one_over_w_;
            }
            for (size_t i=split; i<size; i++) {
                double x_prev = x[i - window_size_];
                y[i] = y[i - 1] + (x[i] * x[i] - x_prev * x_prev) * one_over_w_;
            }
            for (size_t i=0; i<size; i++) {
                y[i] = std::sqrt(y[i]);
            }
        }

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {

            // the first element, we don't have a previous y value
            y[0] = x[0] * x[0] * one_over_w_;

            // the elements < window_size don't have a x[i - window_size]
            size_t split = std::min(size, window_size_);

            // start at the 2nd element, i=1 in the loop below
            size_t xi = dxi;
            size_t yi = dyi;

            for (size_t i=1; i<split; i++) { // start at 1
                y[yi] = y[yi - dyi] + x[xi] * x[xi] * one_over_w_;
                xi += dxi;
                yi += dyi;
            }

            // all other elements
            size_t window_size_shift = window_size_ * dyi;
            for (size_t i=split; i<size; i++) {
                double x_prev = x[xi - window_size_];
                y[i] = y[i - dyi] + (x[xi] * x[xi] - x_prev * x_prev) * one_over_w_;
                xi += dxi;
                yi += dyi;                
            }

            yi = 0;
            for (size_t i=0; i<size; i++) {
                y[yi] = std::sqrt(y[yi]);
                yi += dyi;                
            }            
           
        }

    private:
        FixedSizeBuffer buffer_;
        double sum_;
        const size_t window_size_;
        const double one_over_w_;


    }; // end of class

} // end of namespace

#endif // end of include guards
