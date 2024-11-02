#ifndef SCREAMER_BUTTERWORTH2_H
#define SCREAMER_BUTTERWORTH2_H

#include <array>
#include <cmath>
#include "screamer/common/base.h"
#include "screamer/common/math.h"

namespace screamer {

    class Butter2 : public ScreamerBase {
    public:
        Butter2() {
            a.fill(0.0);
            b.fill(0.0);
            x.fill(0.0);
            y.fill(0.0);
        }

        Butter2(double cutoff_freq) {
            calculateCoefficients(cutoff_freq);
        }

        double process_scalar(double newValue) override {
            // Shift previous samples and outputs
            x[2] = x[1];
            x[1] = x[0];
            x[0] = newValue;

            y[2] = y[1];
            y[1] = y[0];

            // Calculate output using IIR filter difference equation
            y[0] = b[0] * x[0] + b[1] * x[1] + b[2] * x[2]
                - a[1] * y[1] - a[2] * y[2];

            return y[0];
        }

        void setCoefficients(const std::array<double, 3>& b_coeffs, const std::array<double, 3>& a_coeffs) {
            b = b_coeffs;
            a = a_coeffs;
        }

    private:
        void calculateCoefficients(double cutoffFreq) {
            // Angular frequency and pre-warping for bilinear transform
            double omega = tan(M_PI * cutoffFreq);
            double omegaSquared = omega * omega;

            // Butterworth filter parameters (for a second-order low-pass filter)
            double sqrt2 = std::sqrt(2.0);

            double denom = 1.0 + sqrt2 * omega + omegaSquared;

            // Feedforward (numerator) coefficients
            b[0] = omegaSquared / denom;
            b[1] = 2.0 * omegaSquared / denom;
            b[2] = omegaSquared / denom;

            // Feedback (denominator) coefficients, with a[0] normalized to 1
            a[0] = 1.0;
            a[1] = 2.0 * (omegaSquared - 1.0) / denom;
            a[2] = (1.0 - sqrt2 * omega + omegaSquared) / denom;
        }

        std::array<double, 3> a{};  // Feedback (denominator) coefficients
        std::array<double, 3> b{};  // Feedforward (numerator) coefficients
        std::array<double, 3> x{};  // Input history
        std::array<double, 3> y{};  // Output history
    };


} // namespace screamer

#endif
