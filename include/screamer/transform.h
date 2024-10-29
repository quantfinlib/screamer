#ifndef SCREAMER_TRANSFORM_H
#define SCREAMER_TRANSFORM_H

#include <optional>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/base.h"
#include <algorithm>
#include <execution>
#include <cmath>
#include <functional>

namespace py = pybind11;

namespace screamer {

    template <double (*TransformFunc)(double)>
    class Transform : public ScreamerBase {
    public:
        Transform() {}

        double process_scalar(double newValue) override {
            return TransformFunc(newValue);
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {
            size_t i = 0;

            for (size_t i=0; i < size; i ++) {
                y[i] = TransformFunc(x[i]);
            }

        }

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {
            size_t yi = 0;
            size_t xi = 0;

            for (size_t i = 0; i < size; i++) {
                y[yi] = TransformFunc(x[xi]);
                xi += dxi;
                yi += dyi;
            }
        }

    };

} // namespace screamer

#endif // SCREAMER_TRANSFORM_H
