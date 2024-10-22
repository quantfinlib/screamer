#ifndef SCREAMER_ROLLING_SKEW_H
#define SCREAMER_ROLLING_SKEW_H

#include <limits>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class RollingSkew {
public:

    RollingSkew(int N) : 
        N(N), 
        skew(std::numeric_limits<double>::quiet_NaN()),
        sum_x_buffer(N, std::numeric_limits<double>::quiet_NaN()),
        sum_xx_buffer(N, std::numeric_limits<double>::quiet_NaN()),
        sum_xxx_buffer(N, std::numeric_limits<double>::quiet_NaN())
    {}
    
    double operator()(const double newValue) 
    {
        if (!std::isnan(newValue)) {
            // Update the rolling sums
            double sum_x = sum_x_buffer.append(newValue);
            double sum_xx = sum_xx_buffer.append(newValue * newValue);
            double sum_xxx = sum_xxx_buffer.append(newValue * newValue * newValue);

            // Calculate the rolling mean and variance
            double mean = sum_x / N;
            double variance = (sum_xx - sum_x * mean) / (N - 1);
            double std_dev = std::sqrt(variance);

            if (std_dev > 0) {
                // Calculate the rolling skewness
                skew = (sum_xxx - 3 * mean * sum_xx + 3 * mean * mean * sum_x - N * mean * mean * mean) 
                    / (N * std_dev * std_dev * std_dev);
            } else {
                skew = std::numeric_limits<double>::quiet_NaN(); // Undefined skewness when std dev is 0
            }
        }

        return skew;
    }

    void reset() 
    {
        sum_x_buffer.reset(std::numeric_limits<double>::quiet_NaN());
        sum_xx_buffer.reset(std::numeric_limits<double>::quiet_NaN());
        sum_xxx_buffer.reset(std::numeric_limits<double>::quiet_NaN());
        skew = std::numeric_limits<double>::quiet_NaN();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    FixedSizeBuffer sum_x_buffer;
    FixedSizeBuffer sum_xx_buffer;
    FixedSizeBuffer sum_xxx_buffer;
    double skew;
    const int N;
};

} // namespace screamer

#endif // SCREAMER_ROLLING_SKEW_H
