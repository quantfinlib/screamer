
#ifndef SCREAMER_ROLLING_KURT_H
#define SCREAMER_ROLLING_KURT_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingKurt : public ScreamerBase {
    public:

        RollingKurt(int window_size) : 
            window_size_(window_size), 
            sum_x_buffer(window_size),
            sum_xx_buffer(window_size),
            sum_xxx_buffer(window_size),
            sum_xxxx_buffer(window_size)
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
            double N = window_size;
            c0 = N * (N + 1);
            c1 = (N - 1) * (N - 2) * (N - 3);
            c2 = (3 * (N - 1) * (N - 1)) / ((N - 2) * (N - 3));
        }

        void reset() override {
            sum_x_buffer.reset();
            sum_xx_buffer.reset();
            sum_xxx_buffer.reset();
            sum_xxxx_buffer.reset();    
        }
        
    private:

        double process_scalar(double newValue) override {
            double N = window_size_;

            // Update the rolling sums
            double newValue2 = newValue*newValue;
            double sum_x = sum_x_buffer.process_scalar(newValue);
            double sum_xx = sum_xx_buffer.process_scalar(newValue2);
            double sum_xxx = sum_xxx_buffer.process_scalar(newValue2 * newValue);
            double sum_xxxx = sum_xxxx_buffer.process_scalar(newValue2 * newValue2);

            // Calculate the mean
            double mean = sum_x / N;
            double mean2 = mean * mean;

            // Calculate the sample variance (unbiased)
            double variance = (sum_xx - N * mean2) / (N - 1);
            double std_dev = std::sqrt(variance);

            if (std_dev > 0 && N > 3) {
                // Calculate m4 (fourth central moment)
                double m4 = sum_xxxx - 4 * mean * sum_xxx + 6 * mean2 * sum_xx - 3 * N * mean2 * mean2;

                // Calculate numerator and denominator for kurtosis
                double numerator = c0 * m4;
                double denominator = c1 * variance * variance;

                // Calculate kurtosis
                double excess_kurtosis = (numerator / denominator) - c2;
                return excess_kurtosis;

            } else {
                return std::numeric_limits<double>::quiet_NaN();  // Undefined kurtosis
            }

        }

    private:
        RollingSum sum_x_buffer;
        RollingSum sum_xx_buffer;
        RollingSum sum_xxx_buffer;
        RollingSum sum_xxxx_buffer;
        const int window_size_;

        double c0;
        double c1;
        double c2;

    }; // end of class

} // end of namespace

#endif // end of include guards
