#ifndef SCREAMER_ROLLINGSUM_H
#define SCREAMER_ROLLINGSUM_H

#include <vector>
#include <stdexcept>
#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

// RollingSum, skip NaNs
class RollingSum {
public:
    // Constructor with an integer windows size (N)
    RollingSum(int N)
        : index(0), N(N), sum(0.0)
    {
        if (N < 1) {
            throw std::invalid_argument("Window size must be an integer >= 1.");
        }
        buffer.resize(N, std::numeric_limits<double>::quiet_NaN());    
    }

    // Operator to be used as the function call operator for applying the lag
    double operator()(double newValue) 
    {
        double oldValue = buffer[index];
        buffer[index] = newValue;

        index++;
        if (index == N) {
            index = 0;
        }

        if (!std::isnan(oldValue)) {
            sum -= oldValue;
        }

        if (!std::isnan(newValue)) {
            sum += newValue;
        }  

        return sum;

    }

    // reset the internal state
    void reset() 
    {
        std::fill(buffer.begin(), buffer.end(), std::numeric_limits<double>::quiet_NaN());
        index = 0;
        sum = 0.0;
    }

    // Method to transform a NumPy array, applying the lag transformation
    py::array_t<double> transform(py::array_t<double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    size_t index; // Tracks the current position in the buffer
    size_t N;     // The window size value
    std::vector<double> buffer; // Used as circular buffer for storing lagged values
    double sum;
};

} // namespace screamer

#endif // SCREAMER_ROLLINGSUM_H
