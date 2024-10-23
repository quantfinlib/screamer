#ifndef SCREAMER_ROLLING_MAX_H
#define SCREAMER_ROLLING_MAX_H

#include <limits>
#include <deque>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

// The descending maxima algorithm, similar to the ascending minima algorithm
class RollingMax {
public:

    RollingMax(int window_size) : window_size(window_size) 
    {
        if (window_size <= 0) {
            throw std::invalid_argument("N must be positive.");
        }
    }
    
    double operator()(const double newValue) 
    {
        // Add the new value to the list
        values.push_back(newValue);

        // Remove elements from the back of the deque that are smaller than the new value
        while (!deque.empty() && values[deque.back()] <= newValue) {
            deque.pop_back();
        }

        // Add the index of the new value to the deque
        deque.push_back(values.size() - 1);

        // Remove the front element if it's out of the window
        if (deque.front() <= static_cast<int>(values.size()) - window_size - 1) {
            deque.pop_front();
        }

        return values[deque.front()];
    }

    void reset() 
    {
        deque.clear();
        values.clear();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    const int window_size;
    std::deque<double> deque;
    std::vector<double> values;
};

} // namespace screamer

#endif // SCREAMER_ROLLING_MAX_H
