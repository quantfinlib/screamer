#ifndef SCREAMER_ROLLING_MAX_H
#define SCREAMER_ROLLING_MAX_H

#include <limits>
#include <deque>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class RollingMax {
public:

    RollingMax(int window_size) : window_size(window_size), index(0)
    {
        if (window_size <= 0) {
            throw std::invalid_argument("Window size must be positive.");
        }
    }
    
    double operator()(const double newValue) 
    {
        // Remove elements from the back that are smaller than the new value
        while (!max_deque.empty() && max_deque.back().first <= newValue) {
            max_deque.pop_back();
        }

        // Add the new value and its index to the deque
        max_deque.emplace_back(newValue, index);

        // Remove the front element if it's outside the window
        if (max_deque.front().second <= index - window_size) {
            max_deque.pop_front();
        }

        index++;

        // The front of the deque contains the maximum value in the window
        return max_deque.front().first;
    }

    void reset() 
    {
        max_deque.clear();
        index = 0;
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    const int window_size;
    int index; // Current index
    std::deque<std::pair<double, int>> max_deque; // Stores pairs of (value, index)
};

} // namespace screamer

#endif // SCREAMER_ROLLING_MAX_H
