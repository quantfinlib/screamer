#ifndef SCREAMER_ROLLING_MAX_H
#define SCREAMER_ROLLING_MAX_H

#include <deque>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/buffer.h>
#include "screamer/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingMax : public ScreamerBase {
    public:

        RollingMax(int window_size) : 
            window_size_(window_size), 
            index(0)
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        void reset() override {
            max_deque.clear();
            index = 0;    
        }
        
    private:

        double process_scalar(double newValue) override {
            // Remove elements from the back that are smaller than the new value
            while (!max_deque.empty() && max_deque.back().first <= newValue) {
                max_deque.pop_back();
            }

            // Add the new value and its index to the deque
            max_deque.emplace_back(newValue, index);

            // Remove the front element if it's outside the window
            if (max_deque.front().second <= index - window_size_) {
                max_deque.pop_front();
            }

            index++;

            // The front of the deque contains the maximum value in the window
            return max_deque.front().first;
        }

    private:
    const int window_size_;
    int index; // Current index
    std::deque<std::pair<double, int>> max_deque; // Stores pairs of (value, index)

    }; // end of class

} // end of namespace

#endif // end of include guards
