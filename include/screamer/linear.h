#ifndef SCREAMER_LINEAR_H
#define SCREAMER_LINEAR_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/base.h"

namespace py = pybind11;

namespace screamer {

    class Linear : public ScreamerBase {
    public:

        Linear(double scale, double shift) : scale_(scale), shift_(shift) {}


    private:

        double process_scalar(double newValue) override {
            return scale_ * newValue + shift_;
        }

        
        void process_array_no_stride(double* y, const double* x, size_t size) override {
            for (size_t i=0; i<size; i++) {
                y[i] = scale_ * x[i] + shift_;
            }                  
        }
        

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {

            size_t yi = 0;
            size_t xi = 0;

            for (size_t i=0; i<size; i++) {
                y[yi] = scale_ * x[xi] + shift_;
                xi += dxi;
                yi += dyi;
            } 
        }

    private:

        double scale_, shift_;

    }; // end of class

} // end of namespace

#endif // end of include guards
