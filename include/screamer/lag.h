#ifndef SCREAMER_LAG_H
#define SCREAMER_LAG_H

#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class Lag {
public:
    // Constructor with an integer delay (N) and an optional default value
    Lag(int N, double defaultValue = 0.0);

    // Operator to be used as the function call operator for applying the lag
    double operator()(double newValue);

    // reset the internal state
    void reset();

    // Method to transform a NumPy array, applying the lag transformation
    py::array_t<double> transform(py::array_t<double> input_array);

private:
    size_t index; // Tracks the current position in the buffer
    size_t N;     // The lag value (number of steps to delay)
    double defaultValue; // we need to memorize this for the reset()
    std::vector<double> buffer; // Circular buffer for storing lagged values
};

} // namespace screamer

#endif // SCREAMER_LAG_H
