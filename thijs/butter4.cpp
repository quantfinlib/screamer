#include <iostream>
#include <vector>
#include <cmath>
#include <complex>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// Function to apply a direct form II transposed IIR filter
std::vector<double> lfilter(const std::vector<double>& b, const std::vector<double>& a, const std::vector<double>& x) {
    if (a.empty() || b.empty()) {
        throw std::invalid_argument("Coefficient vectors a and b must not be empty.");
    }
    if (a[0] != 1.0) {
        // Normalize the coefficients by a[0]
        double a0_inv = 1.0 / a[0];
        for (double& coeff : const_cast<std::vector<double>&>(a)) {
            coeff *= a0_inv;
        }
        for (double& coeff : const_cast<std::vector<double>&>(b)) {
            coeff *= a0_inv;
        }
    }

    size_t n = std::max(a.size(), b.size());
    std::vector<double> y(x.size(), 0.0);
    std::vector<double> z(n - 1, 0.0); // Internal states (filter delay values)

    for (size_t m = 0; m < x.size(); ++m) {
        y[m] = b[0] * x[m] + z[0];
        for (size_t i = 0; i < n - 2; ++i) {
            z[i] = b[i + 1] * x[m] + z[i + 1] - a[i + 1] * y[m];
        }
        z[n - 2] = b[n - 1] * x[m] - a[n - 1] * y[m];
    }

    return y;
}

// Function to multiply two polynomials
std::vector<double> poly_mult(const std::vector<double>& p1, const std::vector<double>& p2) {
    std::vector<double> result(p1.size() + p2.size() - 1, 0.0);
    for (size_t i = 0; i < p1.size(); ++i) {
        for (size_t j = 0; j < p2.size(); ++j) {
            result[i + j] += p1[i] * p2[j];
        }
    }
    return result;
}

// Function to compute the Butterworth filter coefficients
void butterworth_4th_order(double cutoff_freq, std::vector<double>& b, std::vector<double>& a) {
    if (cutoff_freq <= 0.0 || cutoff_freq >= 0.5) {
        throw std::invalid_argument("cutoff_freq must be between 0 and 0.5.");
    }

    // Pre-warp the frequency
    double Wn = 2.0 * tan(M_PI * cutoff_freq / 2.0);

    // Order of the filter
    int N = 4;

    // Compute the poles of the analog filter
    std::vector<std::complex<double>> poles;
    for (int k = 1; k <= N; ++k) {
        double theta = (2.0 * k - 1.0) * M_PI / (2.0 * N) + M_PI / 2.0;
        std::complex<double> s_k = Wn * std::polar(1.0, theta);
        poles.push_back(s_k);
    }

    // Map the analog poles to the digital domain using the bilinear transform
    std::vector<std::complex<double>> z_poles;
    for (auto& s_k : poles) {
        std::complex<double> z_k = (1.0 + s_k / 2.0) / (1.0 - s_k / 2.0);
        z_poles.push_back(z_k);
    }

    // The zeros are all at z = -1 (for even order low-pass filter)
    std::vector<double> numerator = {1.0};
    for (int i = 0; i < N; ++i) {
        numerator = poly_mult(numerator, {1.0, 1.0});
    }

    // Compute denominator polynomial from digital poles
    std::vector<double> denominator = {1.0};
    for (size_t i = 0; i < z_poles.size(); i += 2) {
        // Handle complex conjugate pairs
        std::complex<double> p1 = z_poles[i];
        std::complex<double> p2 = z_poles[i + 1];

        // Form the quadratic polynomial (1 - 2*Re(p)*z^{-1} + |p|^2*z^{-2})
        double coef1 = -2.0 * p1.real();
        double coef2 = std::norm(p1); // |p|^2

        std::vector<double> quadratic = {1.0, coef1, coef2};
        denominator = poly_mult(denominator, quadratic);
    }

    // Normalize the filter coefficients so that the gain at DC is 1
    double gain_num = 0.0;
    double gain_den = 0.0;
    for (size_t i = 0; i < numerator.size(); ++i) {
        gain_num += numerator[i];
        gain_den += denominator[i];
    }

    double gain = gain_den / gain_num;
    for (double& coeff : numerator) {
        coeff *= gain;
    }

    b = numerator;
    a = denominator;
}


int main() {
    double cutoff_freq = 0.1;

    // Example: coefficients for a 4th-order Butterworth lowpass filter with a given cutoff
    //std::vector<double> b = {0.00362168, 0.01448674, 0.02173012, 0.01448674, 0.00362168}; // Example numerator coefficients
    //std::vector<double> a = {1.0, -3.005135, 3.623284, -2.162366, 0.501205}; // Example denominator coefficients

    std::vector<double> b, a;
    butterworth_4th_order(cutoff_freq, b, a);

    // Print numerator coefficients (b)
    std::cout << "Numerator coefficients (b): ";
    for (const double& coeff : b) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;

    // Print denominator coefficients (a)
    std::cout << "Denominator coefficients (a): ";
    for (const double& coeff : a) {
        std::cout << coeff << " ";
    }
    std::cout << std::endl;


    // Example input vector (signal)
    std::vector<double> x(1000);
    for (size_t i = 0; i < 1000; ++i) {
        double t = i * 0.02;
        x[i] = std::sin(t * t);
    }

    // Apply the filter
    try {
        std::vector<double> y = lfilter(b, a, x);

        for (size_t i = 0; i < 10; ++i) {
            std::cout << i << " " << x[i] << " " << y[i] << std::endl;
        }   
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

