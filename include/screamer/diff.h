#ifndef SCREAMER_DIFF_H
#define SCREAMER_DIFF_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class Diff : public ScreamerBase {
    public:

        Diff(int window_size) : 
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

        double process_scalar(double newValue) override {
            double oldValue = buffer_.append(newValue);
            return newValue - oldValue;     
        }

        void process_array_no_stride(double* y,  const double* x, size_t size) override {
            if (size > window_size_) {
                // Copy the first `window_size_` elements directly
                std::memcpy(y, x, window_size_ * sizeof(double));

                // Process elements in chunks of 4, starting from `window_size_`
                size_t i = window_size_;
                for (; i + 4 <= size; i += 4) {
                    y[i]     = x[i]     - x[i - window_size_];
                    y[i + 1] = x[i + 1] - x[i + 1 - window_size_];
                    y[i + 2] = x[i + 2] - x[i + 2 - window_size_];
                    y[i + 3] = x[i + 3] - x[i + 3 - window_size_];
                }

                // Process any remaining elements one by one
                for (; i < size; i++) {
                    y[i] = x[i] - x[i - window_size_];
                }

            } else {
                // If `size` is smaller than `window_size_`, copy everything directly
                std::memcpy(y, x, size * sizeof(double));
            }
        }       

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {


            // the elements < window_size don't have a x[i - window_size], we set them to zero
            size_t split = std::min(size, window_size_);

            size_t yi = 0;
            size_t xi = 0;

            for (size_t i=0; i<split; i++) { 
                y[yi] = x[xi];
                xi += dxi;
                yi += dyi;
            }

            // all other elements
            size_t shift = window_size_ * dxi;
            for (size_t i=split; i<size; i++) {
                y[yi] =  x[xi] - x[xi - shift];
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
