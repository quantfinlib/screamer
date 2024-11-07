
#ifndef SCREAMER_ROLLING_OU_H
#define SCREAMER_ROLLING_OU_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"
#include "screamer/rolling_sum.h"

namespace py = pybind11;

namespace screamer {

    class RollingOU : public ScreamerBase {
    public:

        RollingOU(int window_size, std::optional<int> output = std::nullopt) : 
            n(window_size - 1), 
            sum_y_buffer(window_size - 1), // the sum buffer have length n-1
            sum_yy_buffer(window_size - 1),
            sum_xy_buffer(window_size - 1),
            output_(output.value_or(0))

        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }

            if (output_ < 0 || output_ > 3) {
                throw std::invalid_argument("Output order must be 0 (mean reversion rate), 1 (mean), 2 (relative mean), or 3 (std).");
            }

            reset();
        }

        void reset() override {
            sum_y_buffer.reset();
            sum_yy_buffer.reset();
            sum_xy_buffer.reset();
            x = 0;
            y = 0;
            sum_x = 0;
            sum_y = 0;
            sum_xx = 0;
            sum_yy = 0;
            sum_xy = 0;
        }
        
        double process_scalar(double newValue) override {
            // the x values is the previous y
            x = y;
            y = newValue;

            sum_x = sum_y;
            sum_y = sum_y_buffer.process_scalar(y);

            sum_xx = sum_yy;
            sum_yy = sum_yy_buffer.process_scalar(y*y);

            sum_xy = sum_xy_buffer.process_scalar(x*y);

            double a_denominator = (n * sum_xx - sum_x * sum_x);
            if (a_denominator == 0) return std::numeric_limits<double>::quiet_NaN();

            double a_numerator = (n * sum_xy - sum_x * sum_y);
            double a = a_numerator / a_denominator;

            // continuous: double mrr = -std::log(a);
            double mrr = 1.0 - a;
            if (output_ == 0) return mrr;

            double b = (sum_y - a * sum_x) / n;
            double mu = b / (1.0 - a);
            if (output_ == 1) return mu;

            if (output_ == 2) return mu - y;

            double e = std::sqrt((n * sum_yy - sum_y * sum_y - a * a_numerator) / (n * (n-2)));
            // continuous: double sigma = e * std::sqrt(-2 * mrr / (1 - a*a));
            //if (output_ == 2) return sigma;
            if (output_ == 3) return e;

            return std::numeric_limits<double>::quiet_NaN();
        }


    private:
        RollingSum sum_y_buffer;
        RollingSum sum_yy_buffer;
        RollingSum sum_xy_buffer;
        const int n;
        const int output_;
        double x, y, sum_x, sum_y, sum_xx, sum_yy, sum_xy;


    }; // end of class

} // end of namespace

#endif // end of include guards

