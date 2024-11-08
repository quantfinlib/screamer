#ifndef SCREAMER_LOG_RETURN_H
#define SCREAMER_LOG_RETURN_H

#include <limits>
#include <algorithm>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class LogReturn : public ScreamerBase {
    public:

        LogReturn(int window_size) : 
            window_size_(window_size), 
             buffer_(window_size, std::numeric_limits<double>::quiet_NaN()) 
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        void reset() override {
            buffer_.reset(std::numeric_limits<double>::quiet_NaN());
        }
        
        double process_scalar(double newValue) override {
            double oldValue = buffer_.append(newValue);
            return std::log(newValue / oldValue);     
        }          

        void process_array_no_stride(double* y,  const double* x, size_t size) override {
            size_t window_size_ = this->window_size_;
            
            size_t split = std::min(size, window_size_);

            // The first window_size_ elements are NaN
            for (size_t i=0; i<split; i++) { 
                y[i] = std::numeric_limits<double>::quiet_NaN();
            }

            // Process elements in chunks of 4, starting from `window_size_`
            size_t i = split;
            for (; i + 4 <= size; i += 4) {

                y[i]     = x[i]     / x[i - window_size_];
                y[i + 1] = x[i + 1] / x[i + 1 - window_size_];
                y[i + 2] = x[i + 2] / x[i + 2 - window_size_];
                y[i + 3] = x[i + 3] / x[i + 3 - window_size_];

                y[i]     = std::log(y[i]);
                y[i + 1] = std::log(y[i + 1]);
                y[i + 2] = std::log(y[i + 2]);
                y[i + 3] = std::log(y[i + 3]);
            }    
            
            // Process any remaining elements one by one
            for (; i < size; i++) {
                y[i] = std::log(x[i] / x[i - window_size_]);
            }     

        }       


        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {
            const size_t window_size_ = this->window_size_;
            size_t split = std::min(size, window_size_);

            size_t yi = 0;
            size_t xi = 0;

            for (size_t i=0; i<split; i++) { 
                y[yi] = std::numeric_limits<double>::quiet_NaN();
                yi += dyi;                
            }

            // all other elements
            size_t shift = window_size_ * dxi;
            xi = shift;
            for (size_t i=split; i<size; i++) {
                double old_x = x[xi - shift];
                y[yi] = std::log(x[xi] / old_x);
                xi += dxi;
                yi += dyi;                
            }
           
        }

    private:
        FixedSizeBuffer buffer_;
        const size_t window_size_;

    }; // end of class

} // end of namespace

#endif // end of include guards
