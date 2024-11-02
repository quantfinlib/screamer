
#ifndef SCREAMER_ROLLING_MIN_H
#define SCREAMER_ROLLING_MIN_H

#include <deque>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingMin : public ScreamerBase {
    public:

        RollingMin(int window_size) : 
            window_size_(window_size), 
            index(0)
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        void reset() override {
            min_deque.clear();
            index = 0;    
        }
        
    private:

        double process_scalar(double newValue) override {
            // Remove elements from the back that are larger than the new value
            while (!min_deque.empty() && min_deque.back().first >= newValue) {
                min_deque.pop_back();
            }

            // Add the new value and its index to the deque
            min_deque.emplace_back(newValue, index);

            // Remove the front element if it's outside the window
            if (min_deque.front().second <= index - window_size_) {
                min_deque.pop_front();
            }

            index++;

            // The front of the deque contains the minimum value in the window
            return min_deque.front().first;
        }

    private:
    const int window_size_;
    int index; // Current index
    std::deque<std::pair<double, int>> min_deque; // Stores pairs of (value, index)

    }; // end of class

} // end of namespace

#endif // end of include guards
