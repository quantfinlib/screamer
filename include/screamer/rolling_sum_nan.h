#ifndef SCREAMER_ROLLING_SUM_NAN_H
#define SCREAMER_ROLLING_SUM_NAN_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/buffer.h>
#include "screamer/base.h"
#include "float_info.h"

/*
This version initializes the buffer with NaN values and 
*/
namespace py = pybind11;

namespace screamer {

    class RollingSumNan : public ScreamerBase {
    public:

        RollingSumNan(int window_size) : 
            window_size_(window_size), 
            sum_(0.0),
            buffer_(window_size, std::numeric_limits<double>::quiet_NaN()) 
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
            buffer_[window_size_ - 1] = 0;
        }

        void reset() override {
            buffer_.reset(std::numeric_limits<double>::quiet_NaN());
            buffer_[window_size_ - 1] = 0;
            sum_ = 0.0;
        }
        
        double process_scalar(double newValue) override {
            if (isnan2(newValue)) {
                return std::numeric_limits<double>::quiet_NaN();
            }

            sum_ += newValue;
            double oldValue = buffer_.append(newValue);

            if (isnan2(oldValue)) {
                return std::numeric_limits<double>::quiet_NaN();
            } 
            sum_ -= oldValue;

            return sum_;            
        }

    private:
        FixedSizeBuffer buffer_;
        double sum_;
        const size_t window_size_;

    }; // end of class

} // end of namespace

#endif // end of include guards
