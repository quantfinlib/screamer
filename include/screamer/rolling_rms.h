
#ifndef SCREAMER_ROLLING_RMS_H
#define SCREAMER_ROLLING_RMS_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/detail/rolling_sum.h>
#include "screamer/common/base.h"

/*
todo: this implementation might  suffer from numerical instability
      https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm
*/
namespace py = pybind11;

namespace screamer {

    class RollingRms : public ScreamerBase {
    public:

        RollingRms(int window_size, const std::string& start_policy = "strict") : 
            window_size_(window_size), 
            start_policy_(detail::parse_start_policy(start_policy)),
            sum_y2_buffer(window_size, start_policy)
        {
            if (window_size_ < 2) {
                throw std::invalid_argument("Window size must be 2 or more.");
            }

            reset();
        }

        void reset() override {
            sum_y2_buffer.reset();    
            if (start_policy_ != detail::StartPolicy::Zero)  {
                n_ = 0;
            } else {
                n_ = window_size_;        
            }
        }
        
        double process_scalar(double newValue) override {
            if ((n_ < window_size_) && (start_policy_ != detail::StartPolicy::Zero) ) {
                n_++;
            } 
            double sum_y2 = sum_y2_buffer.append(newValue * newValue);
            return std::sqrt(sum_y2 / n_);
        }

        void process_array_no_stride(double* y, const double* x, size_t size) override {

            double sum_x = 0.0;
            double sum_xx = 0.0;

            size_t split = std::min<int>(size, window_size_);

            for (size_t i=0; i<split; i++) {
                y[i] = process_scalar(x[i]);
                sum_xx += x[i] * x[i];
            }
            
            for (size_t i=split; i<size; i++) {
                sum_xx = sum_xx + x[i] * x[i] - x[i - window_size_] * x[i - window_size_];
                y[i] = std::sqrt(sum_xx / n_);
            }

        }
    private:
        const int window_size_;
        const detail::StartPolicy start_policy_;
        size_t n_;
        screamer::detail::RollingSum sum_y2_buffer;



    }; // end of class

} // end of namespace

#endif // end of include guards

