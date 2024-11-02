#ifndef SCREAMER_LAG_H
#define SCREAMER_LAG_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class Lag : public ScreamerBase {
    public:

        Lag(int window_size) : 
            window_size_(window_size), 
             buffer_(window_size, 0.0) 
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        void reset() override {
            buffer_.reset(0.0);
        }
        
    private:

        double process_scalar(double newValue) override {

            return buffer_.append(newValue);         
        }

        void process_array_no_stride(double* y,  const double* x, size_t size) override {
            if (size > window_size_) {
                // Use memset for zero-initialization (often faster than std::fill_n for large blocks)
                std::memset(y, 0, window_size_ * sizeof(double));

                // Copy remaining elements from x to y, starting after the zeros
                std::memcpy(y + window_size_, x, (size - window_size_) * sizeof(double));
            } else {
                // If size <= window_size_, only fill with zeros up to `size`
                std::memset(y, 0, size * sizeof(double));
            }
        }       

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {


            // the elements < window_size don't have a x[i - window_size], we set them to zero
            size_t split = std::min(size, window_size_);

            size_t xi = 0;
            size_t yi = 0;

            for (size_t i=0; i<split; i++) { // start at 1
                y[yi] = 0.0;
                yi += dyi;
            }

            // all other elements
            for (size_t i=split; i<size; i++) {
                y[yi] =  x[xi];
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
