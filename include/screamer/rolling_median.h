#ifndef SCREAMER_ROLLING_MEDIAN_H
#define SCREAMER_ROLLING_MEDIAN_H

#include <deque>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/buffer.h>
#include "screamer/base.h"
#include "float_info.h"

namespace py = pybind11;

namespace screamer {

    class RollingMedian : public ScreamerBase {
    public:

        RollingMedian(int window_size) : 
            window_size(window_size), 
            buffer(window_size, std::numeric_limits<double>::quiet_NaN()) 
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        void reset() override
        {
            buffer.reset(std::numeric_limits<double>::quiet_NaN());
            low.clear();
            high.clear();
        }
        
    private:

        double process_scalar(double newValue) override 
        {
            double oldValue = buffer.append(newValue);

            if (!isnan2(oldValue)) {
                remove(oldValue);
            }

            if (!isnan2(newValue)) {
                add(newValue);
            }

            if (low.empty() && high.empty()) {
                return std::numeric_limits<double>::quiet_NaN();
            } else {
                return getMedian();
            }
        }

    private:
        int window_size;
        FixedSizeBuffer buffer;
        std::multiset<double> low;  // Max heap (lower half)
        std::multiset<double> high; // Min heap (upper half)

        void add(double x)
        {
            // Insert into appropriate half
            if (low.empty() || x <= *low.rbegin()) {
                low.insert(x);
            } else {
                high.insert(x);
            }

            // Rebalance the two halves
            rebalance();
        }

        void remove(double x)
        {
            // Remove from the appropriate half
            auto it = low.find(x);
            if (it != low.end()) {
                low.erase(it);
            } else {
                it = high.find(x);
                if (it != high.end()) {
                    high.erase(it);
                }
            }

            // Rebalance after removal
            rebalance();
        }

        void rebalance()
        {
            // Ensure size properties: low.size() >= high.size()
            if (low.size() > high.size() + 1) {
                high.insert(*low.rbegin());
                low.erase(--low.end());
            } else if (high.size() > low.size()) {
                low.insert(*high.begin());
                high.erase(high.begin());
            }
        }

        double getMedian()
        {
            if (low.empty()) {
                return std::numeric_limits<double>::quiet_NaN();
            }

            if (low.size() == high.size()) {
                // Median is the average of max of low and min of high
                return (*low.rbegin() + *high.begin()) / 2.0;
            } else {
                // Median is max of low
                return *low.rbegin();
            }
        }
    
    }; // end of class

} // end of namespace

#endif // end of include guards