#ifndef SCREAMER_FILLNA_H
#define SCREAMER_FILLNA_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/common/base.h"
#include <algorithm>
#include <execution>
#include <cmath>
#include "screamer/common/float_info.h"

namespace py = pybind11;

namespace screamer {

    class FillNa : public ScreamerBase {
    public:

        FillNa(double fill) : fill_(fill) {}


    private:

        double process_scalar(double newValue) override {
            if (isnan2(newValue)) {
                return fill_;
            }
            return newValue;   
        }

        
        void process_array_no_stride(double* y, const double* x, size_t size) override {

            size_t i = 0;

            // Process elements in chunks of 4
            double fill_ = this->fill_;

            for (; i + 4 <= size; i += 4) {

                y[i]     = isnan2(x[i])     ? fill_ : x[i];
                y[i + 1] = isnan2(x[i + 1]) ? fill_ : x[i + 1];
                y[i + 2] = isnan2(x[i + 2]) ? fill_ : x[i + 2];
                y[i + 3] = isnan2(x[i + 3]) ? fill_ : x[i + 3];
                
            }

            // Process any remaining elements
            for (; i < size; i++) {
                y[i] = isnan2(x[i]) ? fill_ : x[i];
            }                     
        }
        

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {

            size_t yi = 0;
            size_t xi = 0;

            for (size_t i=0; i<size; i++) {
                if (isnan2(y[yi])) {
                    y[yi] = fill_;
                } else {
                    y[yi] = x[xi];
                }
                xi += dxi;
                yi += dyi;
            } 

        }

    private:

        double fill_;

    }; // end of class

} // end of namespace

#endif // end of include guards
