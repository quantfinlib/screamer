#ifndef SCREAMER_DIFF_H
#define SCREAMER_DIFF_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms_diff.h>
#include <screamer/buffer.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class Diff {
public:

    Diff(int N) : N(N), buffer(N, std::numeric_limits<double>::quiet_NaN()) {}

    double operator()(const double newValue) 
    {
        double oldValue = buffer.append(newValue);
        return newValue - oldValue;
    }

    void reset() 
    {
        buffer.reset(std::numeric_limits<double>::quiet_NaN());
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_diff(N, input_array);
    }

private:
    FixedSizeBuffer buffer;
    const int N;
};

} // namespace screamer

#endif // SCREAMER_DIFF_H
