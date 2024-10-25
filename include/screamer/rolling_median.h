#ifndef SCREAMER_ROLLING_MEDIAN_H
#define SCREAMER_ROLLING_MEDIAN_H

#include <set>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class RollingMedian {
public:
    RollingMedian(int N) : window_size(N), buffer(N, std::numeric_limits<double>::quiet_NaN()) {}

    double operator()(double newValue) 
    {
        double oldValue = buffer.append(newValue);

        if (!std::isnan(oldValue)) {
            remove(oldValue);
        }

        if (!std::isnan(newValue)) {
            add(newValue);
        }

        if (low.empty() && high.empty()) {
            return std::numeric_limits<double>::quiet_NaN();
        } else {
            return getMedian();
        }
    }

    void reset() 
    {
        buffer.reset(std::numeric_limits<double>::quiet_NaN());
        low.clear();
        high.clear();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
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
};

} // namespace screamer

#endif // SCREAMER_ROLLING_MEDIAN_H

/*
#ifndef SCREAMER_ROLLING_MEDIAN_H
#define SCREAMER_ROLLING_MEDIAN_H

#include <queue>
#include <vector>
#include <map>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/transforms.h>
#include <screamer/buffer.h>

namespace py = pybind11;

namespace screamer {

class RollingMedian {
public:
    RollingMedian(int N) : window_size(N), buffer(N, std::numeric_limits<double>::quiet_NaN()) {}

    double operator()(double newValue) 
    {
        double oldValue = buffer.append(newValue);

        if (!std::isnan(oldValue)) {
            remove(oldValue);
        }

        if (!std::isnan(newValue)) {
            add(newValue);
        }

        rebalanceHeaps();

        if (maxHeapSize + minHeapSize == 0) {
            return std::numeric_limits<double>::quiet_NaN();
        } else {
            return getMedian();
        }
    }

    void reset() 
    {
        buffer.reset(std::numeric_limits<double>::quiet_NaN());
        maxHeap = std::priority_queue<double>();
        minHeap = std::priority_queue<double, std::vector<double>, std::greater<double>>();
        delayed.clear();
        maxHeapSize = 0;
        minHeapSize = 0;
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    int window_size;
    FixedSizeBuffer buffer;
    std::priority_queue<double> maxHeap; // Max heap for lower half
    std::priority_queue<double, std::vector<double>, std::greater<double>> minHeap; // Min heap for upper half
    std::map<double, int> delayed; // Map to handle delayed deletions
    int maxHeapSize = 0;
    int minHeapSize = 0;

    void add(double num)
    {
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
            maxHeapSize++;
        } else {
            minHeap.push(num);
            minHeapSize++;
        }
    }

    void remove(double num)
    {
        delayed[num]++;
        if (!maxHeap.empty() && num <= maxHeap.top()) {
            maxHeapSize--;
            if (num == maxHeap.top()) {
                pruneHeap(maxHeap);
            }
        } else {
            minHeapSize--;
            if (num == minHeap.top()) {
                pruneHeap(minHeap);
            }
        }
    }

    void rebalanceHeaps()
    {
        // Balance the two heaps so that their sizes differ at most by 1
        while (maxHeapSize > minHeapSize + 1) {
            pruneHeap(maxHeap);
            double num = maxHeap.top();
            maxHeap.pop();
            maxHeapSize--;
            minHeap.push(num);
            minHeapSize++;
        }
        while (maxHeapSize < minHeapSize) {
            pruneHeap(minHeap);
            double num = minHeap.top();
            minHeap.pop();
            minHeapSize--;
            maxHeap.push(num);
            maxHeapSize++;
        }
    }

    template <typename HeapType>
    void pruneHeap(HeapType& heap)
    {
        while (!heap.empty()) {
            double num = heap.top();
            if (delayed.count(num)) {
                delayed[num]--;
                if (delayed[num] == 0) {
                    delayed.erase(num);
                }
                heap.pop();
            } else {
                break;
            }
        }
    }

    double getMedian()
    {
        pruneHeap(maxHeap);
        pruneHeap(minHeap);

        if ((maxHeapSize + minHeapSize) % 2 != 0) {
            return maxHeap.top();
        } else {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
    }
};

} // namespace screamer

#endif // SCREAMER_ROLLING_MEDIAN_H
*/