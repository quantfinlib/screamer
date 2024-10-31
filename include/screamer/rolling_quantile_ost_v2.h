#ifndef SCREAMER_ROLLING_QUANTILE_H
#define SCREAMER_ROLLING_QUANTILE_H

#include <deque>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/buffer.h"
#include "screamer/base.h"
#include "order_statistic_tree_v2.h"
#include "float_info.h"

namespace py = pybind11;

namespace screamer {

    class RollingQuantile : public ScreamerBase {
    public:

        RollingQuantile(int window_size, double quantile) :
            window_size(window_size),
            quantile(quantile),
            buffer(window_size, std::numeric_limits<double>::quiet_NaN())
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
            if (quantile < 0.0 || quantile > 1.0) {
                throw std::invalid_argument("Quantile must be between 0 and 1.");
            }
        }

        void reset() override
        {
            buffer.reset(std::numeric_limits<double>::quiet_NaN());
            ost.clear();
        }

    private:

        double process_scalar(double newValue) override
        {
            double oldValue = buffer.append(newValue);

            if (!isnan2(oldValue)) {
                remove(oldValue);
            }

            if (!isnan2(newValue)) {
                add(newValue);
            }

            if (ost.size() < static_cast<size_t>(window_size)) {
                return std::numeric_limits<double>::quiet_NaN();
            } else {
                return getQuantile();
            }
        }

    private:
        int window_size;
        double quantile;
        FixedSizeBuffer buffer;
        OrderStatisticTree ost;

        void add(double x)
        {
            ost.insert(x);
        }

        void remove(double x)
        {
            ost.erase(x);
        }

        double getQuantile()
        {
            if (ost.size() == 0) {
                return std::numeric_limits<double>::quiet_NaN();
            }

            int n = ost.size();
            double pos = quantile * (n - 1);
            int index = static_cast<int>(std::floor(pos));
            double fraction = pos - index;

            double lower = ost.kth_element(index);
            double upper = lower;

            if (fraction > 0.0 && index + 1 < n) {
                upper = ost.kth_element(index + 1);
            }

            return lower + fraction * (upper - lower);
        }

    }; // end of class

} // end of namespace

#endif // SCREAMER_ROLLING_QUANTILE_H
