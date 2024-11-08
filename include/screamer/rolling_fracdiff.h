

#ifndef SCREAMER_FRACDIFF_H
#define SCREAMER_FRACDIFF_H

#include <vector>
#include <deque>
#include <stdexcept>
#include <limits>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <screamer/common/buffer.h>
#include "screamer/common/base.h"


namespace py = pybind11; 

namespace screamer {

    class RollingFracDiff : public ScreamerBase {
    public:
    
        RollingFracDiff(double frac_order, int window_size, double threshold=1e-5):
            frac_order(frac_order), window_size(window_size), threshold(threshold) 
        {
            compute_weights();
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
        }

        // Method to compute the weights for the fractional differentiation
        void compute_weights() {
            weights.push_back(1.0);
            for (int k = 1; k < window_size; ++k) {
                double weight = -weights.back() * (frac_order - k + 1) / k;
                if (std::abs(weight) < threshold) {
                    break;
                }
                weights.push_back(weight);
            }
        }
    
        double process_scalar(double newValue) override {
            if (buffer.size() == window_size) {
                buffer.pop_front();
            }
            buffer.push_back(newValue);
            double result = 0.0;
            for (int i = 0; i < std::min(buffer.size(), weights.size()); ++i) {
                result += weights[i] * buffer[buffer.size() - i - 1];
            }
            return result;
        }
        // reset the internal state
        void reset() override {
            buffer.clear();
        }
        // Method to transform a NumPy array
        //py::array_t<double> transform(py::array_t<double> input_array) {
        //    return transform_1(*this, input_array);
        //}

    private:
        double frac_order;
        int window_size;
        double threshold;
        std::vector<double> weights;
        std::deque<double> buffer;  
    };
}
#endif // SCREAMER_FRACDIFF_H

