#ifndef SCREAMER_DETAIL_ROLLING_MEAN_H
#define SCREAMER_DETAIL_ROLLING_MEAN_H

#include <vector>
#include <stdexcept>
#include <algorithm>
#include "screamer/detail/start_policy.h"

namespace screamer {
namespace detail {


class RollingMean {
public:
    RollingMean(size_t size, const std::string& start_policy = "strict") 
        : 
        capacity_(size),
        start_policy_(parse_start_policy(start_policy)),
        index_(0),
        size_(0),
        sum_(0)
    {
        if (size < 1) {
            throw std::invalid_argument("Size must be at least 1.");
        }

        buffer_.resize(size);
        reset();
    }

    void reset() 
    {
        sum_ = 0;
        size_ = 0;
        index_ = 0;
        std::fill(buffer_.begin(), buffer_.end(), 0.0);
    }

    double append(double newValue) 
    {
        double oldValue = buffer_[index_];
        buffer_[index_] = newValue;

        index_++;
        if (index_ == capacity_) {
            index_ = 0;
        }

        // the most common case, we are past the start period
        if (size_ == capacity_) {
            sum_ += newValue - oldValue;
            return sum_ / capacity_;
        }
        
        // all other case, add the new element to the sum, increment size
        sum_ += newValue;
        size_++;

        // if we reached capacity then we have a valid sum
        if (size_ == capacity_) {
            return sum_ / capacity_;
        }

        // if we haven't reached capacity yet the it depends on the policy
        if (start_policy_ == StartPolicy::Strict) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        if (start_policy_ == StartPolicy::Expanding) {
            return sum_ / size_;
        } 
        // else: StartPolicy::Zero
        return sum_ / capacity_;
    }        

    const size_t size() {
        return size_;
    }

    const size_t capacity() {
        return capacity_;
    }

    const StartPolicy start_policy() {
        return start_policy_;
    }

private:
    const size_t capacity_;
    const StartPolicy start_policy_; 
    size_t index_;
    size_t size_;
    double sum_;
    std::vector<double> buffer_;

}; // class

} // namespace detail
} // namespace screamer
#endif // include guards