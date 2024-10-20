#ifndef SCREAMER_LAG_H
#define SCREAMER_LAG_H

#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class Lag {
public:
    // Constructor with an integer delay (N) and an optional default value
    Lag(int N, double defaultValue = 0.0)
        : index(0), N(N), defaultValue(defaultValue)
    {
        if (N < 1) {
            throw std::invalid_argument("Delay must be an integer >= 1.");
        }
        buffer.resize(N, defaultValue);    
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
        return oldValue;
    }

    // reset the internal state
    void reset() 
    {
        std::fill(buffer.begin(), buffer.end(), defaultValue);
        index = 0;
    }

    // Method to transform a NumPy array, applying the lag transformation
    py::array_t<double> transform(py::array_t<double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    size_t index; // Tracks the current position in the buffer
    size_t N;     // The lag value (number of steps to delay)
    double defaultValue; // we need to memorize this for the reset()
    std::vector<double> buffer; // Used as circular buffer for storing lagged values
};

} // namespace screamer

#endif // SCREAMER_LAG_H
