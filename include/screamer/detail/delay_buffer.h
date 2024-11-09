#ifndef SCREAMER_DETAIL_DELAY_BUFFER_H
#define SCREAMER_DETAIL_DELAY_BUFFER_H

#include <vector>
#include <stdexcept>
#include <algorithm>
#include "screamer/detail/start_policy.h"

namespace screamer {
namespace detail {


class DelayBuffer {
public:
    DelayBuffer(size_t size, const std::string& start_policy = "strict") 
        : 
        capacity_(size),
        start_policy_(parse_start_policy(start_policy)),
        index_(0),
        size_(0)
    {
        if (size < 1) {
            throw std::invalid_argument("Size must be at least 1.");
        }

        buffer_.resize(size);
        reset();
    }

    void reset() 
    {
        size_ = 0;
        index_ = 0;
        std::fill(buffer_.begin(), buffer_.end(), std::numeric_limits<double>::quiet_NaN());
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
            return oldValue;
        }
        
        // all other case, increment size
        size_++;

        // if we haven't reached capacity yet the it depends on the policy
        if (start_policy_ == StartPolicy::Strict) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        if (start_policy_ == StartPolicy::Zero) {
            return 0.0;
        }
        // else: StartPolicy::Expanding
        return buffer_[0];
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
    std::vector<double> buffer_;

}; // class

} // namespace detail
} // namespace screamer
#endif // include guards