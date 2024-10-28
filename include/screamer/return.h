#ifndef SCREAMER_RETURN_H
#define SCREAMER_RETURN_H

#include <limits>
#include <algorithm>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/buffer.h>
#include "screamer/base.h"

namespace py = pybind11;

namespace screamer {

    class Return : public ScreamerBase {
    public:

        Return(int window_size) : 
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
            return (newValue - oldValue) / oldValue;     
        }          

        void process_array_no_stride(double* y,  const double* x, size_t size) override {
            const size_t window_size_ = this->window_size_;
            size_t split = std::min(size, window_size_);

            for (size_t i=0; i<split; i++) { 
                y[i] = std::numeric_limits<double>::quiet_NaN();
            }

            // all other elements
            for (size_t i=split; i < size; i++) {
                y[i]     = x[i]     / x[i     - window_size_] - 1.0;
              
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
                y[yi] = (x[xi] - old_x) / old_x;
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
