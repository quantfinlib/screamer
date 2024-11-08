#ifndef SCREAMER_CLIP_H
#define SCREAMER_CLIP_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/common/base.h"
#include <algorithm>
#include <execution>
#include <cmath>

namespace py = pybind11;

namespace screamer {

    class Clip : public ScreamerBase {
    public:
        Clip(std::optional<double> lower = std::nullopt, std::optional<double> upper = std::nullopt)
            : lower_(lower), upper_(upper) {}

    private:
        double process_scalar(double newValue) override {
            if (lower_.has_value() && newValue < lower_.value()) {
                newValue = lower_.value();
            }
            if (upper_.has_value() && newValue > upper_.value()) {
                newValue = upper_.value();
            }
            return newValue;
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {
            size_t i = 0;

            double lower_bound = lower_.value_or(std::numeric_limits<double>::lowest());
            double upper_bound = upper_.value_or(std::numeric_limits<double>::max());

            // Process elements in chunks of 4 for optimization
            for (; i + 4 <= size; i += 4) {
                y[i]     = std::clamp(x[i],     lower_bound, upper_bound);
                y[i + 1] = std::clamp(x[i + 1], lower_bound, upper_bound);
                y[i + 2] = std::clamp(x[i + 2], lower_bound, upper_bound);
                y[i + 3] = std::clamp(x[i + 3], lower_bound, upper_bound);
            }

            // Process any remaining elements
            for (; i < size; i++) {
                y[i] = std::clamp(x[i], lower_bound, upper_bound);
            }
        }

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {
            size_t yi = 0;
            size_t xi = 0;

            double lower_bound = lower_.value_or(std::numeric_limits<double>::lowest());
            double upper_bound = upper_.value_or(std::numeric_limits<double>::max());

            for (size_t i = 0; i < size; i++) {
                y[yi] = std::clamp(x[xi], lower_bound, upper_bound);
                xi += dxi;
                yi += dyi;
            }
        }

    private:
        std::optional<double> lower_;
        std::optional<double> upper_;
    };

} // namespace screamer

#endif // SCREAMER_CLIP_H
