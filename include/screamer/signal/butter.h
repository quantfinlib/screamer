#ifndef SCREAMER_BUTTER_MATH
#define SCREAMER_BUTTER_MATH

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>
#include "screamer/signal/signal.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace screamer {

    ZPK ButterworthZPK(int N) {

        std::vector<std::complex<double>> zeros;

        std::vector<std::complex<double>> poles;
        for (int m = -N + 1; m < N; m += 2) {
            std::complex<double> pole = -std::exp(std::complex<double>(0, M_PI * m / (2.0 * N)));
            poles.push_back(pole);
        }

        return {zeros, poles, 1.0};
    }

    void butterworth_filter(int N, double cutoff_freq, std::vector<double>& b, std::vector<double>& a) {
        ZPK but = ButterworthZPK(N);

        // Pre-warp frequencies for digital filter design
        double fs = 2.0;
        double Wn = cutoff_freq;
        double warped = 2 * fs * std::tan(M_PI * Wn / fs);
        
        // lowpass
        ZPK but_lo = lp2lp_zpk(but.zeros, but.poles, but.gain, warped);

        // make digital
        ZPK digibut = bilinear_zpk(but_lo.zeros, but_lo.poles, but_lo.gain, 2.0);

        // convert (z,p,k) to (b,a)
        zpk2tf(digibut.zeros, digibut.poles, digibut.gain, b, a);

    }

} // namespace

#endif // include guards