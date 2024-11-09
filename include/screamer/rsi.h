#ifndef SCREAMER_RSI_H
#define SCREAMER_RSI_H

#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/common/base.h"
#include "screamer/rolling_sum_nan.h"
#include "screamer/common/float_info.h"

namespace py = pybind11;

namespace screamer {

    class RSI : public ScreamerBase {
    public:

        RSI(int window_size) : 
        n(window_size), 
        rolling_gain_sum(window_size), 
        rolling_loss_sum(window_size),
        prev_x(std::numeric_limits<double>::quiet_NaN())
        {
        }


        double process_scalar(double x) override {
            double dx = x - prev_x;
            if (isnan2(dx)) dx = 0.0;

            double gain_sum = rolling_gain_sum.process_scalar(dx > 0.0 ? dx : 0.0);
            double loss_sum = rolling_loss_sum.process_scalar(dx < 0.0 ? -dx : 0.0);

            double rsi = 100.0 - 100.0 / (1.0 + gain_sum / loss_sum);
            // double rsi = 100.0 * gain_sum / (gain_sum + loss_sum + 1E-8);

            // double denominator = gain_sum + loss_sum;
            // double rsi = (denominator == 0.0) ? 0.5 : (100.0 * gain_sum / denominator);

            prev_x = x;
            return rsi;
        }
/*
        
        void process_array_no_stride(double* y, const double* x, size_t size) override {
            for (size_t i=0; i<size; i++) {
                y[i] = x[i];
        }
        

        void process_array_stride(double* y, size_t dyi, const double* x, size_t dxi, size_t size) override {

            size_t yi = 0;
            size_t xi = 0;

            for (size_t i=0; i<size; i++) {
                y[yi] = xx[xi];
                xi += dxi;
                yi += dyi;
            } 

        }
*/
    private:
        RollingSumNan rolling_gain_sum;
        RollingSumNan rolling_loss_sum;
        double prev_x;
        const int n;

    }; // end of class

} // end of namespace

#endif // end of include guards
