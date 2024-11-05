#ifndef SCREAMER_BUTTERWORTH_H
#define SCREAMER_BUTTERWORTH_H

#include <array>
#include <cmath>
#include "screamer/common/base.h"
#include "screamer/common/math.h"
#include "screamer/signal/signal.h"
#include "screamer/signal/buttern.h"

namespace screamer {

    class Butter : public ScreamerBase {
    public:


        Butter(int order, double cutoff_freq) :
            order_(order), cutoff_freq_(cutoff_freq)
        {
            std::vector<double> b, a;
            butterworth_filter(order, cutoff_freq, b, a);
            irr.init(b, a);
        }

        void reset() override {
            irr.reset();
        }

        double process_scalar(double newValue) override {
            return irr.process_scalar(newValue);
        }

        void process_array_no_stride(
            double* y, 
            const double* x,
            size_t size) override
        {
            irr.process_array_no_stride(y, x, size);
        }

        void process_array_stride(
            double* y, 
            size_t dyi,
            const double* x, 
            size_t dxi,
            size_t size) override
        {
            irr.process_array_stride(y, dyi, x, dxi, size);
        }        

    private:
        const int order_;
        const double cutoff_freq_;
        IIRFilter irr;
    };

} // namespace screamer

#endif
