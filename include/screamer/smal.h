#ifndef SCREAMER_SMAL_H
#define SCREAMER_SMAL_H

#include <vector>
#include <stdexcept>
#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/lag.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

// Simple Moving Average
class SMAL {
public:
    // Constructor with an integer windows size (N)
    SMAL(int N)
        : lag(N), sum(0.0), sum_count(0)
    { 
    }

    // Operator to be used as the function call operator for applying the lag
    double operator()(double newValue) 
    {
        double oldValue = lag(newValue);

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

    // reset the internal state
    void reset() 
    {
        lag.reset();
    }

    // Method to transform a NumPy array, applying the lag transformation
    py::array_t<double> transform(py::array_t<double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    double sum;
    int sum_count; 
    Lag lag;
};

} // namespace screamer

#endif // SCREAMER_SMAL_H
