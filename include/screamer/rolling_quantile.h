#ifndef SCREAMER_ROLLING_QUANTILE_H
#define SCREAMER_ROLLING_QUANTILE_H

#include <deque>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/buffer.h>
#include "screamer/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingQuantile : public ScreamerBase {
    public:

        RollingQuantile(int window_size, double quantile) : 
            window_size(window_size), 
            quantile(quantile),
            buffer(window_size, std::numeric_limits<double>::quiet_NaN()) 
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
            if (quantile < 0.0 || quantile > 1.0) {
                throw std::invalid_argument("Quantile must be between 0 and 1.");
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

            if (!std::isnan(oldValue)) {
                remove(oldValue);
            }

            if (!std::isnan(newValue)) {
                add(newValue);
            }

            if (low.empty()) {
                return std::numeric_limits<double>::quiet_NaN();
            } else {
                return getQuantile();
            }
        }

    private:
        int window_size;
        double quantile;
        FixedSizeBuffer buffer;
        std::multiset<double> low;  // Lower part
        std::multiset<double> high; // Upper part

        void add(double x)
        {
            if (low.empty() || x <= *low.rbegin()) {
                low.insert(x);
            } else {
                high.insert(x);
            }

            rebalanceAfterInsert();
        }

        void remove(double x)
        {
            auto it = low.find(x);
            if (it != low.end()) {
                low.erase(it);
            } else {
                it = high.find(x);
                if (it != high.end()) {
                    high.erase(it);
                }
            }

            rebalanceAfterRemove();
        }

        void rebalanceAfterInsert()
        {
            size_t k = getK();

            // Rebalance heaps to maintain low.size() == k
            if (low.size() > k) {
                high.insert(*low.rbegin());
                low.erase(--low.end());
            } else if (low.size() < k && !high.empty()) {
                low.insert(*high.begin());
                high.erase(high.begin());
            }
        }

        void rebalanceAfterRemove()
        {
            size_t k = getK();

            // Rebalance heaps to maintain low.size() == k
            if (low.size() > k) {
                high.insert(*low.rbegin());
                low.erase(--low.end());
            } else if (low.size() < k && !high.empty()) {
                low.insert(*high.begin());
                high.erase(high.begin());
            }
        }

        size_t getK() const
        {
            size_t window_elements = low.size() + high.size();
            return static_cast<size_t>(std::floor(quantile * window_elements));
        }

        double getQuantile()
        {
            if (low.empty()) {
                return std::numeric_limits<double>::quiet_NaN();
            } else {
                return *low.rbegin();
            }
        }
    
    }; // end of class

} // end of namespace

#endif // end of include guards
