#ifndef SCREAMER_ROLLING_MEDIAN2_H
#define SCREAMER_ROLLING_MEDIAN2_H
// Ekstrom's Algorithm, adjust to handle floating-point numbers effectively by using a balanced binary search tree
#include <map>
#include <set>
#include <deque>
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

// Phil Ekstrom's Fast Median Filter Algorithm Implementation
class RollingMedian2 {
public:
    RollingMedian2(int window_size) : window_size(window_size)
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
        counts.clear();
        total_counts = 0;
        values.clear();
        median_it = counts.end();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array)
    {
        return transform_1(*this, input_array);
    }

private:
    const int window_size;
    std::deque<double> values; // To keep track of the order of elements
    std::map<double, int> counts; // Corrected std::map usage
    std::map<double, int>::iterator median_it; // Corrected iterator usage
    int total_counts = 0;

    void insertValue(double value)
    {
        // Insert or increment the count of the new value
        auto it = counts.find(value);
        if (it != counts.end()) {
            it->second += 1;
        } else {
            it = counts.insert({value, 1}).first;
        }
        total_counts += 1;

        // Adjust median_it
        if (counts.size() == 1) {
            median_it = counts.begin();
        } else {
            if (value < median_it->first) {
                if (isTotalCountsEven()) {
                    // Median moves left
                    if (total_counts > 2) {
                        decrementMedianIt();
                    }
                }
            } else {
                if (!isTotalCountsEven()) {
                    // Median moves right
                    incrementMedianIt();
                }
            }
        }
    }

    void removeValue(double value)
    {
        // Decrement the count or remove the value
        auto it = counts.find(value);
        if (it != counts.end()) {
            it->second -= 1;
            if (it->second == 0) {
                // Adjust median_it if necessary before erasing
                if (it == median_it) {
                    if (isTotalCountsEven()) {
                        // Median moves to next higher value
                        auto next_it = std::next(it);
                        counts.erase(it);
                        median_it = next_it;
                    } else {
                        // Median moves to next lower value
                        if (it != counts.begin()) {
                            auto prev_it = std::prev(it);
                            counts.erase(it);
                            median_it = prev_it;
                        } else {
                            counts.erase(it);
                            median_it = counts.begin();
                        }
                    }
                } else {
                    counts.erase(it);
                }
            } else {
                // Adjust median_it if necessary
                if (value < median_it->first) {
                    if (!isTotalCountsEven()) {
                        incrementMedianIt();
                    }
                } else {
                    if (isTotalCountsEven()) {
                        decrementMedianIt();
                    }
                }
            }
            total_counts -= 1;
        } else {
            throw std::runtime_error("Value not found in counts during removal.");
        }
    }

    double getMedian() const
    {
        if (counts.empty()) {
            throw std::runtime_error("Window is empty, cannot compute median.");
        }

        if (isTotalCountsEven()) {
            // Need to find the two middle values
            auto next_it = median_it;
            incrementIterator(next_it);
            return (median_it->first + next_it->first) / 2.0;
        } else {
            return median_it->first;
        }
    }

    // Helper functions
    bool isTotalCountsEven() const
    {
        return (total_counts % 2) == 0;
    }

    void incrementMedianIt()
    {
        incrementIterator(median_it);
    }

    void decrementMedianIt()
    {
        if (median_it != counts.begin()) {
            --median_it;
        }
    }

    void incrementIterator(std::map<double, int>::iterator& it) const
    {
        int remaining = it->second;
        while (remaining <= 0) {
            ++it;
            if (it == counts.end()) break;
            remaining = it->second;
        }
    }
};

} // namespace screamer

#endif // SCREAMER_ROLLING_MEDIAN2_H
