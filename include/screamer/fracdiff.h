#ifndef SCREAMER_FRACDIFF_H
#define SCREAMER_FRACDIFF_H

#include <vector>
#include <deque>
#include <stdexcept>
#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class FracDiff {

public:
    FracDiff(double frac_order, int window_size, double threshold=1e-5)
        : frac_order(frac_order), window_size(window_size), threshold(threshold) {
        compute_weights();
    }

    // Method to compute the weights for the fractional differentiation
    void compute_weights() {
        weights.push_back(1.0);
        for (int k = 1; k < window_size; ++k) {
            double weight = -weights.back() * (frac_order - k + 1) / k;
            if (std::abs(weight) < threshold) {
                break;
            }
            weights.push_back(weight);
        }
    }

    // Operator to be used as the function call operator for applying the fractional differentiation
    double operator()(double newValue) 
    {
        // Add the new value to the buffer
        if (buffer.size() == window_size) {
            buffer.pop_front();
        }
        buffer.push_back(newValue);

        // Compute the fractionally differenced value
        double result = 0.0;
        for (int i = 0; i < std::min(buffer.size(), weights.size()); ++i) {
            result += weights[i] * buffer[buffer.size() - i - 1];
        }

        return result;
    }

    // reset the internal state
    void reset() 
    {
        buffer.clear();
    }

    // Method to transform a NumPy array, applying the lag transformation
    py::array_t<double> transform(py::array_t<double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    double frac_order;
    int window_size;
    double threshold;
    std::deque<double> buffer;
    std::vector<double> weights;
};
} // namespace screamer

#endif // SCREAMER_FRACDIFF_H