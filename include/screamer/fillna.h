#ifndef SCREAMER_FILLNA_H
#define SCREAMER_FILLNA_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/base.h"

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

        void process_array_no_stride(double* y, double* x, size_t size) override {
            std::transform(x, x + size, y, [this](double val) {
                return std::isnan(val) ? fill_ : val;
            });
            /*
            for (size_t i=0; i<size; i++) {
                if (std::isnan(y[i])) {
                    y[i] = fill_;
                } else {
                    y[i] = x[i];
                }
            } 
            */
        }

        void process_array_stride(double* y, size_t dyi, double* x, size_t dxi, size_t size) override {

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
