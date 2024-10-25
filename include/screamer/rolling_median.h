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
