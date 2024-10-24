#ifndef SCREAMER_ROLLINGSUM_H
#define SCREAMER_ROLLINGSUM_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class RollingSum {
public:

    RollingSum(int N) : sum(0), buffer(N, std::numeric_limits<double>::quiet_NaN()) {}
    
    double operator()(const double newValue) 
    {
        double oldValue = buffer.append(newValue);

        if (!std::isnan(oldValue)) {
            sum -= oldValue;
        }

        if (!std::isnan(newValue)) {
            sum += newValue;
        }  

        return sum;

    }

    void reset() 
    {
        buffer.reset(std::numeric_limits<double>::quiet_NaN());
        sum = 0.0;
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    FixedSizeBuffer buffer;
    double sum;
};

} // namespace screamer

#endif // SCREAMER_ROLLINGSUM_H
