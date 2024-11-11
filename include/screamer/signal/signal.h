#ifndef SCREAMER_SIGNAL
#define SCREAMER_SIGNAL

/*
struct ZPK

functions:
* relative_degree
* roots_are_conjugate_pairs
* poly_mult
* poly
* lp2lp_zpk
* bilinear_zpk
* zpk2tf

classes:
* IIRFilter

*/

#include <vector>
#include <complex>
#include <numeric>


namespace screamer {


struct ZPK {
    std::vector<std::complex<double>> zeros;
    std::vector<std::complex<double>> poles;
    double gain;
};

// Function to compute the degree (difference in the number of poles and zeros)
int relative_degree(const std::vector<std::complex<double>>& z,
                    const std::vector<std::complex<double>>& p) {
    return static_cast<int>(p.size()) - static_cast<int>(z.size());
}

// Function to check if all complex roots come in conjugate pairs
bool roots_are_conjugate_pairs(const std::vector<std::complex<double>>& roots) {
    std::vector<std::complex<double>> roots_sorted = roots;
    std::vector<std::complex<double>> roots_conj = roots;

    // Conjugate the roots
    for (auto& root : roots_conj) {
        root = std::conj(root);
    }

    // Sort roots and their conjugates
    auto complex_compare = [](const std::complex<double>& a, const std::complex<double>& b) {
        if (std::abs(a.real() - b.real()) > 1e-8) {
            return a.real() < b.real();
        } else {
            return a.imag() < b.imag();
        }
    };

    std::sort(roots_sorted.begin(), roots_sorted.end(), complex_compare);
    std::sort(roots_conj.begin(), roots_conj.end(), complex_compare);

    // Compare the sorted roots and their conjugates
    for (size_t i = 0; i < roots.size(); ++i) {
        if (std::abs(roots_sorted[i] - roots_conj[i]) > 1e-8) {
            return false;
        }
    }
    return true;
}

// Function to multiply two polynomials with complex coefficients
std::vector<std::complex<double>> poly_mult(const std::vector<std::complex<double>>& p1,
                                            const std::vector<std::complex<double>>& p2) {
    std::vector<std::complex<double>> result(p1.size() + p2.size() - 1, std::complex<double>(0.0, 0.0));
    for (size_t i = 0; i < p1.size(); ++i) {
        for (size_t j = 0; j < p2.size(); ++j) {
            result[i + j] += p1[i] * p2[j];
        }
    }
    return result;
}

// Function to compute polynomial coefficients from roots
std::vector<std::complex<double>> poly(const std::vector<std::complex<double>>& roots) {
    std::vector<std::complex<double>> coeffs = {1.0}; // Start with leading coefficient as 1

    for (const auto& root : roots) {
        std::vector<std::complex<double>> factor = {1.0, -root};
        coeffs = poly_mult(coeffs, factor);
    }

    // Check if coefficients can be converted to real numbers
    bool is_all_conjugate_pairs = roots_are_conjugate_pairs(roots);

    if (is_all_conjugate_pairs) {
        // Convert coefficients to real numbers
        for (auto& coeff : coeffs) {
            coeff = coeff.real();
        }
    }

    return coeffs;
}

// Low-pass to low-pass transformation for ZPK representation
ZPK lp2lp_zpk(const std::vector<std::complex<double>>& z,
              const std::vector<std::complex<double>>& p,
              double k, double wo = 1.0) {
    // Scale zeros and poles by the cutoff frequency wo
    std::vector<std::complex<double>> z_lp, p_lp;
    std::transform(z.begin(), z.end(), std::back_inserter(z_lp),
                   [wo](const std::complex<double>& zero) { return wo * zero; });
    std::transform(p.begin(), p.end(), std::back_inserter(p_lp),
                   [wo](const std::complex<double>& pole) { return wo * pole; });

    // Compute the degree of the filter
    int degree = relative_degree(z, p);

    // Adjust the gain by wo raised to the power of the filter degree
    double k_lp = k * std::pow(wo, degree);

    return {z_lp, p_lp, k_lp};
}


//  Return a digital IIR filter from an analog one using a bilinear transform.
ZPK bilinear_zpk(const std::vector<std::complex<double>>& z,
                 const std::vector<std::complex<double>>& p,
                 double k, double fs) {
    if (fs <= 0.0) {
        throw std::invalid_argument("Sample rate fs must be positive.");
    }

    double fs2 = 2.0 * fs;
    std::vector<std::complex<double>> z_z, p_z;

    // Bilinear transform the poles and zeros
    for (const auto& zero : z) {
        z_z.push_back((fs2 + zero) / (fs2 - zero));
    }
    for (const auto& pole : p) {
        p_z.push_back((fs2 + pole) / (fs2 - pole));
    }

    // Calculate the relative degree (difference in length between p and z)
    int degree = static_cast<int>(p.size()) - static_cast<int>(z.size());

    // Move any zeros at infinity to the Nyquist frequency (-1 in the z-plane)
    for (int i = 0; i < degree; ++i) {
        z_z.push_back(-1.0);
    }

    // Compensate for gain change due to the bilinear transform
    std::complex<double> num_product = std::accumulate(
        z.begin(), z.end(),
        std::complex<double>(1.0, 0.0),
        [fs2](std::complex<double> prod, std::complex<double> zero) {
            return prod * (fs2 - zero);
        });

    std::complex<double> den_product = std::accumulate(
        p.begin(), p.end(),
        std::complex<double>(1.0, 0.0),
        [fs2](std::complex<double> prod, std::complex<double> pole) {
            return prod * (fs2 - pole);
        });

    double k_z = k * std::real(num_product / den_product);

    return {z_z, p_z, k_z};
}


// Main function to convert ZPK to TF
void zpk2tf(const std::vector<std::complex<double>>& zeros,
            const std::vector<std::complex<double>>& poles,
            double k,
            std::vector<double>& b,
            std::vector<double>& a) {
    // Compute numerator polynomial coefficients
    std::vector<std::complex<double>> b_complex = poly(zeros);
    // Multiply by gain
    for (auto& coeff : b_complex) {
        coeff *= k;
    }

    // Compute denominator polynomial coefficients
    std::vector<std::complex<double>> a_complex = poly(poles);

    // Convert to real coefficients if possible
    bool b_real = true;
    for (const auto& coeff : b_complex) {
        if (std::abs(coeff.imag()) > 1e-8) {
            b_real = false;
            break;
        }
    }
    if (b_real) {
        b.reserve(b_complex.size());
        for (const auto& coeff : b_complex) {
            b.push_back(coeff.real());
        }
    } else {
        throw std::runtime_error("Numerator coefficients are complex.");
    }

    bool a_real = true;
    for (const auto& coeff : a_complex) {
        if (std::abs(coeff.imag()) > 1e-8) {
            a_real = false;
            break;
        }
    }
    if (a_real) {
        a.reserve(a_complex.size());
        for (const auto& coeff : a_complex) {
            a.push_back(coeff.real());
        }
    } else {
        throw std::runtime_error("Denominator coefficients are complex.");
    }
}



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

} // namespace

#endif // include guards