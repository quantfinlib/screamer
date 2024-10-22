#ifndef SCREAMER_SMA_H
#define SCREAMER_SMA_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class SMA  {
public:
    SMA(int N) : sum(0), sum_count(0), buffer(N, std::numeric_limits<double>::quiet_NaN()) {}

    double operator()(double newValue) 
    {
        double oldValue = buffer.append(newValue);

        if (!std::isnan(oldValue)) {
            sum -= oldValue;
            sum_count--;
        }

        if (!std::isnan(newValue)) {
            sum += newValue;
            sum_count++;
        }

        if (sum_count > 0) {
            return sum / sum_count;
        } else {
            return std::numeric_limits<double>::quiet_NaN();
        }
    }

    void reset() 
    {
        buffer.reset(std::numeric_limits<double>::quiet_NaN());
        sum = 0.0;
        sum_count = 0;
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    FixedSizeBuffer buffer;
    double sum;
    int sum_count;
};

} // namespace screamer

#endif // SCREAMER_SMA_H
