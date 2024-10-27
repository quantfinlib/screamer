#ifndef SCREAMER_FFILL_H
#define SCREAMER_FFILL_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/base.h"

namespace py = pybind11;

namespace screamer {

    class Ffill : public ScreamerBase {
    public:

        Ffill() : lastValidValue(std::numeric_limits<double>::quiet_NaN()) {}

        void reset() override {
            lastValidValue = std::numeric_limits<double>::quiet_NaN();
        }

    private:

        double process_scalar(double newValue) override {
            if (!std::isnan(newValue)) {
                lastValidValue = newValue;
            }
            return lastValidValue;   
        }

        void process_array_no_stride(double* y, double* x, size_t size) override {
            for (size_t i=0; i<size; i++) {
                if (!std::isnan(x[i])) {
                    lastValidValue = x[i];
                }
                y[i] = lastValidValue;
            } 
        }

        void process_array_stride(double* y, size_t dyi, double* x, size_t dxi, size_t size) override {

            size_t yi = 0;
            size_t xi = 0;

            for (size_t i=0; i<size; i++) {
                if (!std::isnan(x[xi])) {
                    lastValidValue = x[xi];
                }
                y[yi] = lastValidValue;
                xi += dxi;
                yi += dyi;
            } 

        }

    private:

        double lastValidValue;

    }; // end of class

} // end of namespace

#endif // end of include guards
