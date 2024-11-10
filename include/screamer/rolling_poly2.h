#ifndef SCREAMER_ROLLING_POLY2_H
#define SCREAMER_ROLLING_POLY2_H

#include <screamer/common/buffer.h>
#include "screamer/common/base.h"
#include <stdexcept>
#include <tuple>
#include "screamer/detail/delay_buffer.h"

/*
# Quadratic Regression Statistical Equations

## Coefficients (a, b, c)

### Coefficient \(a\):

$$
a = \frac{\left[ S(x^2y) \cdot S(xx) \right] - \left[ S(xy) \cdot S(xx^2) \right]}{\left[ S(xx) \cdot S(x^2x^2) \right] - \left[ S(xx^2) \right]^2}
$$

### Coefficient \(b\):

$$
b = \frac{\left[ S(xy) \cdot S(x^2x^2) \right] - \left[ S(x^2y) \cdot S(xx^2) \right]}{\left[ S(xx) \cdot S(x^2x^2) \right] - \left[ S(xx^2) \right]^2}
$$

### Coefficient \(c\):

$$
c = \frac{S(y)}{n} - \left[ b \cdot \frac{S(x)}{n} \right] - \left[ a \cdot \frac{S(x^2)}{n} \right]
$$

## Where:

### \( S(xx) \):

$$
S(xx) = \sum x_i^2 - \frac{\left( \sum x_i \right)^2}{n}
$$

### \( S(xy) \):

$$
S(xy) = \sum x_i y_i - \frac{\left( \sum x_i \right) \cdot \left( \sum y_i \right)}{n}
$$

### \( S(xx^2) \):

$$
S(xx^2) = \sum x_i^3 - \frac{\left( \sum x_i \right) \cdot \left( \sum x_i^2 \right)}{n}
$$

### \( S(x^2y) \):

$$
S(x^2y) = \sum x_i^2 y_i - \frac{\left( \sum x_i^2 \right) \cdot \left( \sum y_i \right)}{n}
$$

### \( S(x^2x^2) \):

$$
S(x^2x^2) = \sum x_i^4 - \frac{\left( \sum x_i^2 \right)^2}{n}
$$

*/

namespace screamer {

    // https://www.azdhs.gov/documents/preparedness/state-laboratory/lab-licensure-certification/technical-resources/calibration-training/12-quadratic-least-squares-regression-calib.pdf
    // this is a standalone function, we don't use it, instead we compute it in parts like below
    void _quadratic_regression_coef(
        int n, 
        double Sx, double Sx2, double Sx3, double Sx4, 
        double Sy, double Sxy, double Sx2y,
        double& a, double& b, double& c) 
    {
            double Zxx = Sx2 - Sx * Sx / n;
            double Zxy = Sxy - Sx * Sy / n;
            double Zxx2 = Sx3 - Sx * Sx2 / n;
            double Zx2y = Sx2y - Sx2 * Sy / n;            
            double Zx2x2 = Sx4 - Sx2 * Sx2 / n;

            double d = Zxx * Zx2x2 - Zxx2 * Zxx2;

            a = (Zx2y * Zxx - Zxy * Zxx2) / d;
            b = (Zxy * Zx2x2 - Zx2y * Zxx2) / d;
            c = Sy / n - b * Sx / n - a * Sx2 / n;            
    }

    // these are analytical formulae for sum_{i=0}^{n-1} i^p
    void _quadratic_regression_coef_part0(
        double n, 
        double& Sx, double& Sx2, double& Sx3, double& Sx4)
    {
        Sx = (n - 1) * n / 2;                                     // 0 + 1 + 2 + ... + (n-1)
        Sx2 = (n - 1) * n * (2*n - 1) / 6;                        // 0^2 + 1^2 + 2^2 + ... + (n-1)^2
        Sx3 = n * n * (n - 1) * (n - 1) / 4;                      // 0^3 + 1^3 + 2^3 + ... + (n-1)^3
        Sx4 = (n - 1) * n * (2*n - 1) * (3*n*n - 3*n - 1) / 30;   // 0^4 + 1^4 + 2^4 + ... + (n-1)^4

    }

    // these are the intermediate constants that only depend of x and not on y
    void _quadratic_regression_coef_part1(
        int n, 
        double Sx, double Sx2, double Sx3, double Sx4, // from part0
        double& Zxx, double& Zxx2, double& Zx2x2, double& d) 
    {
            Zxx = Sx2 - Sx * Sx / n;
            Zxx2 = Sx3 - Sx * Sx2 / n;
            Zx2x2 = Sx4 - Sx2 * Sx2 / n;
            d = Zxx * Zx2x2 - Zxx2 * Zxx2;          
    }

    // here we compute a,b,c of the quadratic equation OLS fit y = ax^2 + bx + c
    // the imputs are a mix of part0, part1 and cummulative terms that depend on y
    void _quadratic_regression_coef_part2(
        int n, 
        double Sx, double Sx2, // from part0
        double Sy, double Sxy, double Sx2y,
        double Zxx, double Zxx2, double Zx2x2, double d, // from part1
        double& a, double& b, double& c) 
    {
            double Zxy = Sxy - Sx * Sy / n;
            double Zx2y = Sx2y - Sx2 * Sy / n;            
            a = (Zx2y * Zxx - Zxy * Zxx2) / d;
            b = (Zxy * Zx2x2 - Zx2y * Zxx2) / d;
            c = Sy / n - b * Sx / n - a * Sx2 / n;            
    }

    class RollingPoly2 : public ScreamerBase {
    public:
        RollingPoly2(int window_size, int derivative_order = 0, const std::string& start_policy = "strict") : 
            window_size_(window_size),
            derivative_order_(derivative_order),
            start_policy_(detail::parse_start_policy(start_policy)),
            n_(0),
            delay_buffer_(window_size, "zero")
        {
            if (window_size < 3) {
                throw std::invalid_argument("Window size must be 3 or more.");
            }
            if (derivative_order < 0 || derivative_order > 2) {
                throw std::invalid_argument("Derivative order must be 0 (endpoint), 1 (slope), or 2 (curvature).");
            }

            // reset the dynamic variables
            reset();
        }

        void reset() override {
            delay_buffer_.reset();
            sum_y = 0.0;
            sum_xy = 0.0;
            sum_xxy = 0.0;

            if (start_policy_ != detail::StartPolicy::Zero)  {
                n_ = 0;
                sum_x = 0.0;
                sum_xx = 0.0;
                sum_xxx = 0.0;
                sum_xxxx = 0.0;
            } else {
                n_ = window_size_;
                _quadratic_regression_coef_part0(
                    n_, 
                    sum_x, sum_xx, sum_xxx, sum_xxxx
                );               
            }    

            _quadratic_regression_coef_part1(
                n_, sum_x, sum_xx, sum_xxx, sum_xxxx, 
                Zxx, Zxx2, Zx2x2, d
            ); 

        }

        double process_scalar(double yn) override {

            double y0 = delay_buffer_.append(yn);
            sum_y += yn - y0;

            if ((n_ < window_size_) && (start_policy_ != detail::StartPolicy::Zero) ) {
                double n2_ = n_ * n_;
                sum_x += n_;
                sum_xx += n2_;
                sum_xxx += n2_ * n_;
                sum_xxxx += n2_ * n2_;

                sum_xxy += n_ * n_ * yn;
                sum_xy += n_ * yn;

                n_++;

                // derived statistics that have no y term
                _quadratic_regression_coef_part1(
                    n_, 
                    sum_x, sum_xx, sum_xxx, sum_xxxx, 
                    Zxx, Zxx2, Zx2x2, d
                );


            } else {
                sum_xxy += n_ * (n_ - 2) * yn + sum_y - 2 * sum_xy;
                sum_xy += n_ * yn - sum_y;
            }

            if (1==2) { // debug
                std::cout << " n=" << n_; 
                std::cout << " yn=" << yn << " y0=" << y0; 
                std::cout << " Sx=" << sum_x << " Sx2=" << sum_xx << " Sx3=" << sum_xxx << " Sx4=" << sum_xxxx;
                std::cout << " Sxy=" << sum_xy << " Sxxy=" << sum_xxy;
                std::cout << " Zxx=" << Zxx << " Zxx2=" << Zxx2 << " Zx2x2=" << Zx2x2 << " d=" << d;
                std::cout << std::endl; 
            }

            if (n_ < window_size_) {
                if (start_policy_ == detail::StartPolicy::Strict) return std::numeric_limits<double>::quiet_NaN();
                if (n_ < 3)  return std::numeric_limits<double>::quiet_NaN();
            }

            double a, b, c;
            _quadratic_regression_coef_part2(
                n_, 
                sum_x, sum_xx, 
                sum_y, sum_xy, sum_xxy,
                Zxx, Zxx2, Zx2x2, d,
                a, b, c
            );
            std::cout << "a=" << a << " b=" << b << " c=" << c << std::endl;

            // Calculating endpoint, slope, and curvature based on derivative_order_
            double endpoint = a * (n_ - 1) * (n_ - 1) + b * (n_ - 1) + c;
            double slope = 2 * a * (n_ - 1) + b;
            double curvature = 2 * a;
           
            // Return based on derivative_order_
            return derivative_order_ == 0 ? endpoint : (derivative_order_ == 1 ? slope : curvature);
        }

    private:
        const size_t window_size_;
        const int derivative_order_;
        const detail::StartPolicy start_policy_;
        detail::DelayBuffer delay_buffer_;
        size_t n_;
        double sum_x, sum_xx, sum_xxx, sum_xxxx;
        double sum_y, sum_xy, sum_xxy;
        double Zxx, Zxy, Zxx2, Zx2y, Zx2x2, d;

    };

} // end namespace screamer

#endif // SCREAMER_ROLLING_POLY2_H
