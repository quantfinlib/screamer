#ifndef SCREAMER_DIFF_H
#define SCREAMER_DIFF_H

#include <vector>
#include <stdexcept>
#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class Diff {
public:
    // Constructor with an integer delay (N)
    Diff(int N)
        : index(0), N(N)
    {
        if (N < 1) {
            throw std::invalid_argument("Delay must be an integer >= 1.");
        }
        buffer.resize(N, std::numeric_limits<double>::quiet_NaN());    
    }

    // Operator to be used as the function call operator for applying the diff
    double operator()(double newValue) 
    {
        double oldValue = buffer[index];
        buffer[index] = newValue;
        index++;
        if (index == N) {
            index = 0;
        }
        return newValue - oldValue;
    }

    // reset the internal state
    void reset() 
    {
        std::fill(buffer.begin(), buffer.end(), std::numeric_limits<double>::quiet_NaN());
        index = 0;
    }

    // Method to transform a NumPy array, applying the diff transformation
    py::array_t<double> transform(py::array_t<double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    size_t index; // Tracks the current position in the buffer
    size_t N;     // The delay value (number of steps to compage the difference with)
    std::vector<double> buffer; // Used as circular buffer for storing lagged values
};

} // namespace screamer

#endif // SCREAMER_DIFF_H
