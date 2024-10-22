#ifndef SCREAMER_ROLLING_STD_H
#define SCREAMER_ROLLING_STD_H

#include <limits>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class RollingStd {
public:

    RollingStd(int N) : 
        N(N), 
        std(std::numeric_limits<double>::quiet_NaN()),
        sum_x_buffer(N, std::numeric_limits<double>::quiet_NaN()),
        sum_xx_buffer(N, std::numeric_limits<double>::quiet_NaN())
    {}
    
    double operator()(const double newValue) 
    {
        if (!std::isnan(newValue)) {
            double sum_x = sum_x_buffer.append(newValue);
            double sum_xx = sum_x_buffer.append(newValue * newValue);
            std = std::sqrt((sum_xx - sum_x * sum_x / N) / (N - 1));
        } 

        return std;
    }

    void reset() 
    {
        sum_x_buffer.reset(std::numeric_limits<double>::quiet_NaN());
        sum_xx_buffer.reset(std::numeric_limits<double>::quiet_NaN());        
        std = std::numeric_limits<double>::quiet_NaN();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    FixedSizeBuffer sum_x_buffer;
    FixedSizeBuffer sum_xx_buffer;
    double std;
    const int N;
};

} // namespace screamer

#endif // SCREAMER_ROLLING_STD_H
