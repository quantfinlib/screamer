/*
#ifndef SCREAMER_EWMA_H
#define SCREAMER_EWMA_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class EWMA  {
public:
    EWMA(double weight) : weight(weight), anti_weight(1.0 - weight), value(std::numeric_limits<double>::quiet_NaN()) {}

    double operator()(double newValue) 
    {
        // if the internal value is NaN / not set, then set it to the current
        if (std::isnan(value)) { 
            value = newValue;
            return value;
        }

        // If the new value is NaN we skip it
        if (std::isnan(newValue)) {
            return value;
        }

        // Actual EWMA
        value = weight * newValue + anti_weight * value;
        return value;
    }

    void reset() 
    {
        value = std::numeric_limits<double>::quiet_NaN();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    double value;
    const double weight;
    const double anti_weight;
};

} // namespace screamer

#endif // SCREAMER_EWMA_H
*/