#ifndef SCREAMER_ROLLING_MIN_H
#define SCREAMER_ROLLING_MIN_H

#include <limits>
#include <deque>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class RollingMin {
public:

    RollingMin(int window_size) : window_size(window_size), index(0)
    {
        if (window_size <= 0) {
            throw std::invalid_argument("Window size must be positive.");
        }
    }
    
    double operator()(const double newValue) 
    {
        // Remove elements from the back that are larger than the new value
        while (!min_deque.empty() && min_deque.back().first >= newValue) {
            min_deque.pop_back();
        }

        // Add the new value and its index to the deque
        min_deque.emplace_back(newValue, index);

        // Remove the front element if it's outside the window
        if (min_deque.front().second <= index - window_size) {
            min_deque.pop_front();
        }

        index++;

        // The front of the deque contains the minimum value in the window
        return min_deque.front().first;
    }

    void reset() 
    {
        min_deque.clear();
        index = 0;
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    const int window_size;
    int index; // Current index
    std::deque<std::pair<double, int>> min_deque; // Stores pairs of (value, index)
};

} // namespace screamer

#endif // SCREAMER_ROLLING_MIN_H
