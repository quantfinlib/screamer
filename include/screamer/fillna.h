#ifndef SCREAMER_FILLNA_H
#define SCREAMER_FILLNA_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/base.h"
#include <algorithm>
#include <execution>
#include <cmath>

namespace py = pybind11;

namespace screamer {

    class FillNa : public ScreamerBase {
    public:

        FillNa(double fill) : fill_(fill) {}


    private:

        double process_scalar(double newValue) override {
            if (std::isnan(newValue)) {
                return fill_;
            }
            return newValue;   
        }

        
        void process_array_no_stride(double* y, const double* x, size_t size) override {
            /*
            std::transform(x, x + size, y, [this](double val) {
                return std::isnan(val) ? fill_ : val;
            });  
            */
            size_t i = 0;

            // Process elements in chunks of 4
            double fill_ = this->fill_;

            for (; i + 4 <= size; i += 4) {
                /*
                y[i]     = x[i];
                y[i + 1] = x[i + 1];
                y[i + 2] = x[i + 2];
                y[i + 3] = x[i + 3];

                if (std::isnan(y[i]))   y[i]   = fill_;
                if (std::isnan(y[i+1])) y[i+1] = fill_;
                if (std::isnan(y[i+2])) y[i+2] = fill_;
                if (std::isnan(y[i+3])) y[i+3] = fill_;
                */
                
                y[i]     = std::isnan(x[i])     ? fill_ : x[i];
                y[i + 1] = std::isnan(x[i + 1]) ? fill_ : x[i + 1];
                y[i + 2] = std::isnan(x[i + 2]) ? fill_ : x[i + 2];
                y[i + 3] = std::isnan(x[i + 3]) ? fill_ : x[i + 3];
                
            }

            // Process any remaining elements
            for (; i < size; i++) {
                y[i] = std::isnan(x[i]) ? fill_ : x[i];
            }                     
        }
        

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {

            size_t yi = 0;
            size_t xi = 0;

            for (size_t i=0; i<size; i++) {
                if (std::isnan(y[yi])) {
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
