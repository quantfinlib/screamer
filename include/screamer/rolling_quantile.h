#ifndef SCREAMER_ROLLING_QUANTILE_H
#define SCREAMER_ROLLING_QUANTILE_H

#include <set>
#include <deque>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class RollingQuantile {
public:
    RollingQuantile(int window_size, double quantile) : window_size(window_size), quantile(quantile)
    {
        if (window_size <= 0) {
            throw std::invalid_argument("Window size must be positive.");
        }
        if (quantile < 0.0 || quantile > 1.0) {
            throw std::invalid_argument("Quantile must be between 0.0 and 1.0.");
        }
    }

    double operator()(const double newValue)
    {
        // Remove the oldest value if the window is full
        if (values.size() == window_size) {
            double old_value = values.front();
            values.pop_front();
            removeValue(old_value);
        }

        insertValue(newValue);
        values.push_back(newValue);

        // Return the current quantile
        return getQuantile();
    }

    void reset()
    {
        window.clear();
        values.clear();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array)
    {
        return transform_1(*this, input_array);
    }

private:
    const int window_size;
    const double quantile;
    std::deque<double> values; // To keep track of the order of elements
    std::multiset<double> window; // Stores window elements in sorted order

    void insertValue(double value)
    {
        window.insert(value); // Insert the new value into the sorted window
    }

    void removeValue(double value)
    {
        auto it = window.find(value);
        if (it != window.end()) {
            window.erase(it); // Remove the value from the sorted window
        } else {
            throw std::runtime_error("Value not found in window during removal.");
        }
    }

    double getQuantile() const
    {
        if (window.empty()) {
            throw std::runtime_error("Window is empty, cannot compute quantile.");
        }

        // Find the position corresponding to the quantile
        size_t rank = static_cast<size_t>(quantile * (window.size() - 1));
        auto it = window.begin();
        std::advance(it, rank); // Move iterator to the quantile position

        return *it;
    }
};

} // namespace screamer

#endif // SCREAMER_ROLLING_QUANTILE_H
