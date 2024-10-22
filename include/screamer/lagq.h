#ifndef SCREAMER_LAGQ_H
#define SCREAMER_LAGQ_H

#include <deque>
#include <stdexcept>
#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class Lagq {
public:
    // Constructor with an integer delay (N)
    Lagq(int N)
        : N(N)
    {
        if (N < 1) {
            throw std::invalid_argument("Delay must be an integer >= 1.");
        }
        // Initialize the deque with N NaN values
        buffer.resize(N, std::numeric_limits<double>::quiet_NaN());    
    }

    // Operator to be used as the function call operator for applying the lag
    double operator()(double newValue) 
    {
        double oldValue = buffer.front();  // Get the oldest value (front of the deque)
        buffer.pop_front();                // Remove the oldest value
        buffer.push_back(newValue);        // Add the new value to the back
        return oldValue;
    }

    // Reset the internal state
    void reset() 
    {
        std::fill(buffer.begin(), buffer.end(), std::numeric_limits<double>::quiet_NaN());
    }

    // Method to transform a NumPy array, applying the lag transformation
    py::array_t<double> transform(py::array_t<double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    size_t N;     // The lag value (number of steps to delay)
    std::deque<double> buffer; // Used as deque for storing lagged values
};

} // namespace screamer

#endif // SCREAMER_LAGQ_H
