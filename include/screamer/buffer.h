#ifndef SCREAMER_BUFFER_H
#define SCREAMER_BUFFER_H

#include <vector>
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

} // namespace screamer

#endif // SCREAMER_BUFFER_H
