#ifndef SCREAMER_ROLLING_STD_H
#define SCREAMER_ROLLING_STD_H

#include <limits>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/rolling_sum.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class RollingStd {
public:

    RollingStd(int N) : 
        N(N), 
        std(std::numeric_limits<double>::quiet_NaN()),
        sum_x_buffer(N),
        sum_xx_buffer(N)
    {}
    
    double operator()(const double newValue) 
    {
        if (!std::isnan(newValue)) {
            double sum_x = sum_x_buffer(newValue);
            double sum_xx = sum_xx_buffer(newValue * newValue);
            std = std::sqrt((N * sum_xx - sum_x * sum_x) / (N * (N - 1)));
        } 

        return std;
    }

    void reset() 
    {
        sum_x_buffer.reset();
        sum_xx_buffer.reset();        
        std = std::numeric_limits<double>::quiet_NaN();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    RollingSum sum_x_buffer;
    RollingSum sum_xx_buffer;
    double std;
    const int N;
};

} // namespace screamer

#endif // SCREAMER_ROLLING_STD_H
