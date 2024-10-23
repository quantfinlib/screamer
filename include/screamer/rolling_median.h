#ifndef SCREAMER_ROLLING_MEDIAN_H
#define SCREAMER_ROLLING_MEDIAN_H

#include <set>
#include <deque>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class RollingMedian {
public:
    RollingMedian(int window_size) : window_size(window_size)
    {
        if (window_size <= 0) {
            throw std::invalid_argument("Window size must be positive.");
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

        // Return the current median
        return getMedian();
    }

    void reset()
    {
        lower.clear();
        upper.clear();
        values.clear();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array)
    {
        return transform_1(*this, input_array);
    }

private:
    const int window_size;
    std::deque<double> values; // To keep track of the order of elements
    std::multiset<double> lower; // Max-heap behavior (lower half)
    std::multiset<double> upper; // Min-heap behavior (upper half)

    void insertValue(double value)
    {
        if (lower.empty() || value <= *lower.rbegin()) {
            lower.insert(value);
        } else {
            upper.insert(value);
        }

        // Rebalance the multisets
        rebalance();
    }

    void removeValue(double value)
    {
        auto it_lower = lower.find(value);
        if (it_lower != lower.end()) {
            lower.erase(it_lower);
        } else {
            auto it_upper = upper.find(value);
            if (it_upper != upper.end()) {
                upper.erase(it_upper);
            } else {
                // Value not found in either multiset
                throw std::runtime_error("Value not found in multiset during removal.");
            }
        }

        // Rebalance the multisets
        rebalance();
    }

    void rebalance()
    {
        // Ensure sizes of lower and upper are balanced
        if (lower.size() > upper.size() + 1) {
            // Move one element from lower to upper
            upper.insert(*lower.rbegin());
            auto it = lower.end();
            --it;
            lower.erase(it);
        } else if (upper.size() > lower.size()) {
            // Move one element from upper to lower
            lower.insert(*upper.begin());
            upper.erase(upper.begin());
        }
    }

    double getMedian() const
    {
        if (lower.empty() && upper.empty()) {
            throw std::runtime_error("Window is empty, cannot compute median.");
        }

        if ((lower.size() + upper.size()) % 2 == 1) {
            // Odd number of elements, median is the largest of lower
            return *lower.rbegin();
        } else {
            // Even number of elements, median is the average of max of lower and min of upper
            return (*lower.rbegin() + *upper.begin()) / 2.0;
        }
    }
};

} // namespace screamer

#endif // SCREAMER_ROLLING_MEDIAN_H
