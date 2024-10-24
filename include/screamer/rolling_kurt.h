#ifndef SCREAMER_ROLLING_KURT_H
#define SCREAMER_ROLLING_KURT_H

#include <limits>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class RollingKurt {
public:

    RollingKurt(int N) : 
        N(N), 
        kurt(std::numeric_limits<double>::quiet_NaN()),
        sum_x_buffer(N),
        sum_xx_buffer(N),
        sum_xxx_buffer(N),
        sum_xxxx_buffer(N)
    {}
    
    double operator()(const double newValue) 
    {
        if (!std::isnan(newValue)) {
            // Update the rolling sums
            double sum_x = sum_x_buffer(newValue);
            double sum_xx = sum_xx_buffer(newValue * newValue);
            double sum_xxx = sum_xxx_buffer(newValue * newValue * newValue);
            double sum_xxxx = sum_xxxx_buffer(newValue * newValue * newValue * newValue);


            // Calculate the mean
            double mean = sum_x / N;

            // Calculate the sample variance (unbiased)
            double variance = (sum_xx - N * mean * mean) / (N - 1);
            double std_dev = std::sqrt(variance);

            if (std_dev > 0 && N > 3) {
                // Calculate m4 (fourth central moment)
                double m4 = sum_xxxx - 4 * mean * sum_xxx + 6 * mean * mean * sum_xx - 3 * N * mean * mean * mean * mean;

                // Calculate numerator and denominator for kurtosis
                double numerator = N * (N + 1) * m4;
                double denominator = (N - 1) * (N - 2) * (N - 3) * std_dev * std_dev * std_dev * std_dev;

                // Calculate kurtosis
                double excess_kurtosis = (numerator / denominator) - (3 * (N - 1) * (N - 1)) / ((N - 2) * (N - 3));

                kurt = excess_kurtosis;
            } else {
                kurt = std::numeric_limits<double>::quiet_NaN();  // Undefined kurtosis
            }
        }

        return kurt;
    }

    void reset() 
    {
        sum_x_buffer.reset();
        sum_xx_buffer.reset();
        sum_xxx_buffer.reset();
        sum_xxxx_buffer.reset();
        kurt = std::numeric_limits<double>::quiet_NaN();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    RollingSum sum_x_buffer;
    RollingSum sum_xx_buffer;
    RollingSum sum_xxx_buffer;
    RollingSum sum_xxxx_buffer;
    double kurt;
    const int N;
};

} // namespace screamer

#endif // SCREAMER_ROLLING_KURT_H
