
#ifndef SCREAMER_ROLLING_SKEW_H
#define SCREAMER_ROLLING_SKEW_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/buffer.h>
#include "screamer/base.h"

namespace py = pybind11;

namespace screamer {

    class RollingSkew : public ScreamerBase {
    public:

        RollingSkew(int window_size) : 
            window_size_(window_size), 
            sum_x_buffer(window_size),
            sum_xx_buffer(window_size),
            sum_xxx_buffer(window_size)
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        void reset() override {
            sum_x_buffer.reset();
            sum_xx_buffer.reset();        
            sum_xxx_buffer.reset();        
        }
        
    private:

        double process_scalar(double newValue) override {
            double N = this->window_size_;

            // Update the rolling sums
            double sum_x = sum_x_buffer.process_scalar(newValue);
            double sum_xx = sum_xx_buffer.process_scalar(newValue * newValue);
            double sum_xxx = sum_xxx_buffer.process_scalar(newValue * newValue * newValue);

            // Calculate the rolling mean
            double mean = sum_x / N;

            // Sample variance
            double variance = (sum_xx - (sum_x * mean)) / (N - 1);

            if (variance > 0 && N > 2) {

                // Sample std
                double std_dev = std::sqrt(variance);

                // Calculate the third central moment
                // double m3 = (sum_xxx / N) - 3 * mean * (sum_xx / N) + 2 * mean * mean * mean;
                double m3 = sum_xxx - 3 * mean * sum_xx + 2 * N * mean * mean * mean;

                // Calculate skewness using Pandas formula
                double g1 = m3 / (std_dev * std_dev * std_dev);
                double skew = (N * g1) / ((N - 1) * (N - 2));
                return skew;
            } else {
                return std::numeric_limits<double>::quiet_NaN();  // Undefined skewness
            }

        }

    private:
        RollingSum sum_x_buffer;
        RollingSum sum_xx_buffer;
        RollingSum sum_xxx_buffer;
        const int window_size_;

    }; // end of class

} // end of namespace

#endif // end of include guards













#ifndef SCREAMER_ROLLING_SKEW_H
#define SCREAMER_ROLLING_SKEW_H

#include <limits>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/rolling_sum.h"
#include "float_info.h"

namespace py = pybind11; // Alias for pybind11 namespace

namespace screamer {

class RollingSkew {
public:

    RollingSkew(int N) : 
        N(N), 
        skew(0.0),
        sum_x_buffer(N),
        sum_xx_buffer(N),
        sum_xxx_buffer(N)
    {}
    
    double operator()(const double newValue) 
    {
        if (!isnan2(newValue)) {
            // Update the rolling sums
            double sum_x = sum_x_buffer(newValue);
            double sum_xx = sum_xx_buffer(newValue * newValue);
            double sum_xxx = sum_xxx_buffer(newValue * newValue * newValue);

            // Calculate the rolling mean
            double mean = sum_x / N;

            // Sample variance
            double variance = (sum_xx - (sum_x * mean)) / (N - 1);

            if (variance > 0 && N > 2) {

                // Sample std
                double std_dev = std::sqrt(variance);

                // Calculate the third central moment
                // double m3 = (sum_xxx / N) - 3 * mean * (sum_xx / N) + 2 * mean * mean * mean;
                double m3 = sum_xxx - 3 * mean * sum_xx + 2 * N * mean * mean * mean;

                // Calculate skewness using Pandas formula
                double g1 = m3 / (std_dev * std_dev * std_dev);
                skew = (N * g1) / ((N - 1) * (N - 2));
            } else {
                skew = std::numeric_limits<double>::quiet_NaN();  // Undefined skewness
            }
            
        }

        return skew;
    }

    void reset() 
    {
        sum_x_buffer.reset();
        sum_xx_buffer.reset();
        sum_xxx_buffer.reset();
        skew = std::numeric_limits<double>::quiet_NaN();
    }

    py::array_t<double> transform(const py::array_t<const double> input_array) 
    {
        return transform_1(*this, input_array);
    }

private:
    RollingSum sum_x_buffer;
    RollingSum sum_xx_buffer;
    RollingSum sum_xxx_buffer;
    double skew;
    const int N;
};

} // namespace screamer

#endif // SCREAMER_ROLLING_SKEW_H
