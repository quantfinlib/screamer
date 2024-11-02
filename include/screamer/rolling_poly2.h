#ifndef SCREAMER_ROLLING_POLY2_H
#define SCREAMER_ROLLING_POLY2_H

#include <screamer/common/buffer.h>
#include "screamer/common/base.h"
#include <stdexcept>
#include <tuple>

namespace screamer {

    class RollingPoly2 : public ScreamerBase {
    public:
        RollingPoly2(int window_size, int derivative_order = 0) : 
            window_size_(window_size),
            derivative_order_(derivative_order),
            sum_y_buffer(window_size)
        {
            if (window_size <= 0) {
                throw std::invalid_argument("Window size must be positive.");
            }
            if (derivative_order < 0 || derivative_order > 2) {
                throw std::invalid_argument("Derivative order must be 0 (endpoint), 1 (slope), or 2 (curvature).");
            }

            double N = window_size;

            // Precompute sums and moments for x-values [0, 1, 2, ..., window_size - 1]
            Sx = (N - 1) * N / 2;
            Sxx = (N - 1) * N * (2 * N - 1) / 6;
            Sxxx = N * N * (N - 1) * (N - 1) / 4;
            Sxxxx = (N - 1) * N * (2*N - 1) * (3*N*N - 3*N - 1) / 30;
         
            // derived stats without y
            Zxx = Sxx - Sx * Sx / N;
            Zxx2 = Sxxx - Sxx * Sx / N;
            Zx2x2 = Sxxxx - Sxx * Sxx / N;
            d = Zxx * Zx2x2 - Zxx2 * Zxx2;

            // reset the dynamic variables
            reset();
        }

        void reset() override {
            sum_y_buffer.reset();
            Sy = 0.0;
            Sxy = 0.0;
            Sxxy = 0.0;
        }

        double process_scalar(double y) override {
            double N = window_size_;


            // update the sums with "y" terms
            // the order of 3 eqs is important!
            Sy = sum_y_buffer.process_scalar(y);
            Sxxy += N * (N - 2) * y + Sy - 2 * Sxy;
            Sxy += N * y - Sy;

            // compute derived stats with y terms
            double Zxy = Sxy - Sx * Sy / N;
            double Zx2y = Sxxy - Sxx * Sy / N;

            // Compute a,b,c
            double a = (Zx2y * Zxx - Zxy * Zxx2) / d;
            double b = (Zxy * Zx2x2 - Zx2y * Zxx2) / d;
            double c = Sy / N - b * Sx / N - a * Sxx / N;
            
            // Calculating endpoint, slope, and curvature based on derivative_order_
            double endpoint = a * (N - 1) * (N - 1) + b * (N - 1) + c;
            double slope = 2 * a * (N - 1) + b;
            double curvature = 2 * a;

            /*
            std::cout << "RollingPoly2::process_scalar(" << y << ")" << std::endl;

            std::cout 
                << "Sx=" << Sx << ", " 
                << "Sxx=" << Sxx << ", " 
                << "Sxxx=" << Sxxx << ", " 
                << "Sxxxx=" << Sxxxx  << std::endl;

            std::cout 
                << "Sx=" << Sx << ", " 
                << "Sxx=" << Sxx << ", " 
                << "Sxxx=" << Sxxx << ", " 
                << "Sxxxx=" << Sxxxx  << std::endl;

            std::cout 
                << "Sy=" << Sy << ", " 
                << "Sxy=" << Sxy << ", " 
                << "Sxxy=" << Sxxy  << std::endl;

            std::cout 
                << "Zxx=" << Zxx << ", " 
                << "Zxy=" << Zxy << ", " 
                << "Zxx2=" << Zxx2 << ", " 
                << "Zx2y=" << Zx2y << ", " 
                << "Zx2x2=" << Zx2x2 << ", " 
                << "d=" << d << std::endl;

            std::cout 
                << "a=" << a << ", " 
                << "b=" << b << ", " 
                << "c=" << c << ", " 
                << "endpoint=" << endpoint << ", " 
                << "slope=" << slope << ", " 
                << "curvature=" << curvature << std::endl;
            */
           
            // Return based on derivative_order_
            return derivative_order_ == 0 ? endpoint : (derivative_order_ == 1 ? slope : curvature);
        }

    private:
        const size_t window_size_;
        double Sx, Sxx, Sxxx, Sxxxx;
        double Sy, Sxy, Sxxy;
        double Zxx, Zxy, Zxx2, Zx2y, Zx2x2, d;
        RollingSum sum_y_buffer;

        const int derivative_order_;
    };

} // end namespace screamer

#endif // SCREAMER_ROLLING_POLY2_H
