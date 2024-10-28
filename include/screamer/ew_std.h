/*
#ifndef SCREAMER_EWSTD_H
#define SCREAMER_EWSTD_H

#include <limits>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class EwStd {
public:
    // Constructor initializes the weight and anti-weight
    EwStd(double weight) : 
        weight(weight), 
        anti_weight(1.0 - weight), 
        mean(std::numeric_limits<double>::quiet_NaN()), 
        variance(std::numeric_limits<double>::quiet_NaN()) 
    {}

    // Function call operator for new values
    double operator()(double newValue) 
    {
        // Skip NaN values
        if (std::isnan(newValue)) {
            return std::sqrt(variance); // Return the current standard deviation (sqrt of variance)
        }

        // If mean is NaN, initialize mean and variance with the new value
        if (std::isnan(mean)) {
            mean = newValue;
            variance = 0.0; // Variance is zero with just one value
            return 0.0; // Standard deviation is zero at this point
        }

        // Update the EWMA (mean) as before
        double prev_mean = mean;
        mean = weight * newValue + anti_weight * mean;

        // Update the variance based on the new value and the previous EWMA (mean)
        double diff = newValue - prev_mean;
        variance = weight * diff * diff + anti_weight * variance;

        // Return the exponentially weighted moving standard deviation
        return std::sqrt(variance);
    }

    // Reset method to clear the state
    void reset() 
    {
        mean = std::numeric_limits<double>::quiet_NaN();
        variance = std::numeric_limits<double>::quiet_NaN();
    }

    // Transform method to process a NumPy array
    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    double mean;
    double variance;
    const double weight;
    const double anti_weight;
};

} // namespace screamer

#endif // SCREAMER_EWSTD_H
*/