#ifndef SCREAMER_ROLLING_SUM_H
#define SCREAMER_ROLLING_SUM_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms_sum.h>
#include <screamer/buffer.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class RollingSum {
public:

    RollingSum(int N) : sum(0), N(N), buffer(N, 0.0) {}
    
    double operator()(const double newValue) 
    {
        if (!std::isnan(newValue)) {
            double oldValue = buffer.append(newValue);
            sum -= oldValue;
            sum += newValue;
        }

        return sum;
    }

    void reset() 
    {
        buffer.reset(0.0);
        sum = 0.0;
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_sum(N, input_array);
    }

private:
    FixedSizeBuffer buffer;
    double sum;
    const int N;
};

} // namespace screamer

#endif // SCREAMER_ROLLINGSUM_H
