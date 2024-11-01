#ifndef SCREAMER_BUFFER_H
#define SCREAMER_BUFFER_H

#include <vector>
#include <set>
#include <iostream>
#include <limits>
#include <stdexcept>


namespace screamer {

class FixedSizeBuffer final {
public:

    // Constructor
    FixedSizeBuffer(size_t N, double defaultValue = std::numeric_limits<double>::quiet_NaN()) 
        : index(0), N(N)
    {
        if (N <= 0) {
            throw std::invalid_argument("N must be positive.");
        }
        buffer.resize(N, defaultValue);
    }

    // Append an element to the buffer and return the old value that gets kicked out
    double append(double newValue) 
    {
        double oldValue = buffer[index];
        buffer[index] = newValue;
        index++;
        if (index == N) {
            index = 0;
        }
        return oldValue;
    }

    // Random read-only access element in the buffer, pos is the oldest element
    const double& operator[](size_t pos) const {
        size_t actualPos = index + pos;
        if (actualPos >= N) {
            actualPos -= N;
        }
        return buffer[actualPos];
    }
    
    // write access
    double& operator[](size_t pos) {
        size_t actualPos = index + pos;
        if (actualPos >= N) {
            actualPos -= N;
        }
        return buffer[actualPos];
    }

    // Reset the internal state
    void reset(double defaultValue) 
    {
        std::fill(buffer.begin(), buffer.end(), defaultValue);
        index = 0;

    }

private:
    const size_t N;     // The lag value (number of steps to delay), marked as const
    size_t index; // Tracks the current position in the buffer
    std::vector<double> buffer; // Used as circular buffer for storing lagged values
};


template<typename T>
class SortedFixedSizeBuffer {
public:
    SortedFixedSizeBuffer(size_t size)
    {
        if (size <= 0) {
            throw std::invalid_argument("N must be positive.");
        }
        // Now that size is validated, we can initialize the members
        buffer.resize(size);  // Initialize buffer with the specified size
        capacity = size;
        write_head = 0;
        filled_size = 0;
    }

    void insert(T value) {
        // If the buffer is full, remove the element at the current write head from the multiset
        if (filled_size == capacity) {
            auto it = sorted_values.find(buffer[write_head]);
            sorted_values.erase(it); // Remove old value from multiset
        } else {
            ++filled_size;
        }

        // Add the new value to the cyclic buffer
        buffer[write_head] = value;
        sorted_values.insert(value);  // Add new value to the multiset

        // Update the write head
        write_head = (write_head + 1) % capacity;
    }

    T get_min() const {
        return *sorted_values.begin();
    }

    T get_max() const {
        return *sorted_values.rbegin();
    }

    T get_median() const {
        auto it = sorted_values.begin();
        std::advance(it, filled_size / 2);
        if (filled_size % 2 == 0) {
            // If even, take average of two middle values
            auto it_prev = std::prev(it);
            return (*it + *it_prev) / 2;
        }
        return *it; // Odd number of elements
    }

    T get_quantile(double q) const {
        // q should be between 0 and 1 (e.g., 0.5 for median)
        size_t rank = q * (filled_size - 1);  // Get rank position for quantile
        auto it = sorted_values.begin();
        std::advance(it, rank);
        return *it;
    }

    // Reset the buffer to its initial state
    void reset() {
        buffer.clear();                  // Clear the buffer
        buffer.resize(capacity);          // Resize to the original capacity
        sorted_values.clear();            // Clear the sorted multiset
        write_head = 0;                   // Reset the write head
        filled_size = 0;                  // Reset the filled size
    }

private:
    std::vector<T> buffer;           // Cyclic buffer to store the elements
    std::multiset<T> sorted_values;  // Sorted multiset to maintain current values
    size_t capacity;                 // Maximum size of the cyclic buffer
    size_t write_head;               // Write head index
    size_t filled_size = 0;          // Number of valid elements currently in buffer
};



} // namespace screamer

#endif // SCREAMER_BUFFER_H

