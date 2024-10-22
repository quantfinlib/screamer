#ifndef SCREAMER_LOG_RETURN_H
#define SCREAMER_LOG_RETURN_H

#include <limits>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class LogReturn {
public:

    LogReturn(int N) :  buffer(N, std::numeric_limits<double>::quiet_NaN()) {}

    double operator()(const double newValue) 
    {
        double oldValue = buffer.append(newValue);
        return std::log(newValue / oldValue);
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

#endif // SCREAMER_LOG_RETURN_H
