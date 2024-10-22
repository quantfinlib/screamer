#ifndef SCREAMER_LAG_H
#define SCREAMER_LAG_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class Lag {
public:
 
    Lag(int N) : buffer(N, std::numeric_limits<double>::quiet_NaN()) {}

    double operator()(const double newValue) 
    {
        return buffer.append(newValue);
    }

    void reset() 
    {
        buffer.reset(std::numeric_limits<double>::quiet_NaN());
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    FixedSizeBuffer buffer;
};

} // namespace screamer

#endif // SCREAMER_LAG_H
