#ifndef SCREAMER_POWER_H
#define SCREAMER_POWER_H

#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class Power : public ScreamerBase {
    public:
        Power(double p) : p_(p) {}

        double process_scalar(double newValue) override {
            return std::pow(newValue, p_);
        }
        
        void process_array_no_stride(double* y, const double* x, size_t size) override {
            for (size_t i=0; i<size; i++) {
                y[i] = std::pow(x[i], p_);
            }                  
        }
        
        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {

            size_t yi = 0;
            size_t xi = 0;

            for (size_t i=0; i<size; i++) {
                y[yi] = std::pow(x[xi], p_);
                xi += dxi;
                yi += dyi;
            } 
        }

    private:
        double p_;

    }; // end of class

} // end of namespace

#endif // end of include guards