#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class IIRFilter {
    public:
        // Default constructor
        IIRFilter() = default;

        // Static helper function to calculate n
        static size_t calculateN(const std::vector<double>& b, const std::vector<double>& a) {
            return std::max(b.size(), a.size());
        }

        void init(std::vector<double> bCoeffs, std::vector<double> aCoeffs) {
            if (bCoeffs.empty() || aCoeffs.empty()) {
                throw std::invalid_argument("Coefficient vectors b and a cannot be empty.");
            }

            // Set the internal coefficients using move semantics for efficiency
            b = std::move(bCoeffs);
            a = std::move(aCoeffs);

            // Calculate the maximum size for n
            n = std::max(b.size(), a.size());

            // Initialize z with zeros and size n - 1
            z.assign(n - 1, 0.0);
        }


        void reset() 
        {
            z.assign(n - 1, 0.0);
            //std::fill(z.begin(), z.end(), 0.0);
        }

        double process_scalar(double x) 
        {
            double y = b[0] * x + z[0];
            for (size_t i = 0; i < n - 2; ++i) {
                z[i] = b[i + 1] * x + z[i + 1] - a[i + 1] * y;
            }
            z[n - 2] = b[n - 1] * x - a[n - 1] * y;
            return y;
        }

        void process_array_no_stride(
            double* y, 
            const double* x,
            size_t size) 
        {
            for (size_t m = 0; m < size; ++m) {
                y[m] = b[0] * x[m] + z[0];
                for (size_t i = 0; i < n - 2; ++i) {
                    z[i] = b[i + 1] * x[m] + z[i + 1] - a[i + 1] * y[m];
                }
                z[n - 2] = b[n - 1] * x[m] - a[n - 1] * y[m];
            }
        }

        void process_array_stride(
            double* y, 
            size_t dyi,
            const double* x, 
            size_t dxi,
            size_t size)
        {
            int yi = 0;
            int xi = 0;
            for (size_t m = 0; m < size; ++m) {
                y[yi] = b[0] * x[xi] + z[0];
                for (size_t i = 0; i < n - 2; ++i) {
                    z[i] = b[i + 1] * x[xi] + z[i + 1] - a[i + 1] * y[yi];
                }
                z[n - 2] = b[n - 1] * x[xi] - a[n - 1] * y[yi];
                xi += dxi;
                yi += dyi;
            }
        }

    public:
        std::vector<double> b; // Numerator coefficients
        std::vector<double> a; // Denominator coefficients
        std::vector<double> z; // Internal states (filter delay values)
        size_t n;              // Maximum size of the coefficient vectors
};


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

void butterworth_filter(int N, double cutoff_freq, std::vector<double>& b, std::vector<double>& a) {
    if (cutoff_freq <= 0.0 || cutoff_freq >= 1.0) {
        throw std::invalid_argument("cutoff_freq must be between 0 and 1.0.");
    }
    if (N <= 0) {
        throw std::invalid_argument("Filter order N must be a positive integer.");
    }

    // Pre-warp the frequency
    double Wn = tan(M_PI * cutoff_freq / 2.0);

    // Compute the analog poles
    std::vector<std::complex<double>> poles;
    for (int k = 1; k <= N; ++k) {
        double theta = M_PI * (2.0 * k + N - 1.0) / (2.0 * N);
        std::complex<double> s_k = Wn * std::polar(1.0, theta);
        poles.push_back(s_k);
    }

    // Map poles to z-plane using bilinear transform
    std::vector<std::complex<double>> z_poles;
    for (auto& s_k : poles) {
        std::complex<double> z_k = (1.0 + s_k) / (1.0 - s_k);
        z_poles.push_back(z_k);
    }

    // Form denominator polynomial
    std::vector<double> denominator = {1.0};
    size_t i = 0;
    while (i < z_poles.size()) {
        if (i + 1 < z_poles.size() && std::abs(z_poles[i].imag()) > 1e-8) {
            // Complex conjugate pair
            std::complex<double> p = z_poles[i];
            double a1 = -2.0 * p.real();
            double a2 = p.real() * p.real() + p.imag() * p.imag();

            std::vector<double> quadratic = {1.0, a1, a2};
            denominator = poly_mult(denominator, quadratic);

            i += 2;
        } else {
            // Real pole
            double a1 = -z_poles[i].real();

            std::vector<double> linear = {1.0, a1};
            denominator = poly_mult(denominator, linear);

            i += 1;
        }
    }

    // Form numerator polynomial
    std::vector<double> numerator = {1.0};
    for (int i = 0; i < N; ++i) {
        numerator = poly_mult(numerator, {1.0, 1.0});
    }

    // Invert signs of denominator coefficients beyond a[0]
    //for (size_t i = 1; i < denominator.size(); ++i) {
    //    denominator[i] = -denominator[i];
    //}
    

    // Normalize coefficients
    double a0 = denominator[0];
    for (double& coeff : denominator) {
        coeff /= a0;
    }
    for (double& coeff : numerator) {
        coeff /= a0;
    }

    // Normalize numerator to ensure unity gain at DC
    double sum_num = 0.0;
    double sum_den = 0.0;
    for (double coeff : numerator) {
        sum_num += coeff;
    }
    for (double coeff : denominator) {
        sum_den += coeff;
    }
    double gain = sum_den / sum_num;
    for (double& coeff : numerator) {
        coeff *= gain;
    }

    b = numerator;
    a = denominator;
}




int main() {
    // Example usage with a normalized cutoff frequency of 0.1 and filter order N
    int N = 6; // Change N to any positive integer
    double cutoff_freq = 0.2;
    std::vector<double> b, a;

    try {
        butterworth_filter(N, cutoff_freq, b, a);

        // Print coefficients
        std::cout << "Numerator coefficients (b): ";
        for (const double& coeff : b) {
            std::cout << coeff << " ";
        }
        std::cout << std::endl;

        std::cout << "Denominator coefficients (a): ";
        for (const double& coeff : a) {
            std::cout << coeff << " ";
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }


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

    std::cout << "IRR ----------------------------" << std::endl;
    IIRFilter irr;
    irr.init(b, a);

    for (size_t i = 0; i < 10; ++i) {
        std::cout << i << " " << x[i] << " " << irr.process_scalar(x[i]) << std::endl;
    } 

    irr.reset();

    std::vector<double> y(1000);
    irr.process_array_no_stride(&y[0], &x[0], 1000);
    for (size_t i = 0; i < 10; ++i) {
        std::cout << i << " " << x[i] << " " << y[i] << std::endl;
    } 
    return 0;
}
