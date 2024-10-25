#ifndef SCREAMER_ROLLING_MEAN_H
#define SCREAMER_ROLLING_MEAN_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms_mean.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class RollingMean  {
public:
    RollingMean(int N) : N(N), one_over_N(1.0 / N), sum(0.0), buffer(N, 0.0) {}

    double operator()(double newValue) 
    {
        if (!std::isnan(newValue)) {
            double oldValue = buffer.append(newValue);
            sum -= oldValue;
            sum += newValue;
        }

        return sum * one_over_N;
    }

    void reset()
    {
        buffer.reset(0.0);
        sum = 0.0;
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_mean(N, input_array);
    }

private:
    FixedSizeBuffer buffer;
    double sum;
    const double one_over_N;
    const int N;
};

} // namespace screamer

#endif // SCREAMER_ROL_MEAN_H
