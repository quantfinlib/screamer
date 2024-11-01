
#ifndef SCREAMER_TRANSFORM_FUNCTIONS_H
#define SCREAMER_TRANSFORM_FUNCTIONS_H

#include <cmath>

namespace screamer {

    // ReLU function
    double relu(double x) {
        return x > 0 ? x : 0;
    }

    // ELU function (Exponential Linear Unit)
    double elu(double x) {
        return x > 0 ? x : (std::exp(x) - 1);
    }

    // SELU function (Scaled Exponential Linear Unit)
    double selu(double x) {
        const double lambda = 1.0507; // Scaling parameter
        const double alpha = 1.67326; // Exponential parameter
        return x > 0 ? lambda * x : lambda * alpha * (std::exp(x) - 1);
    }

    // Softsign function
    double softsign(double x) {
        return x / (1 + std::abs(x));
    }

    // Sigmoid function
    double sigmoid(double x) {
        return 1 / (1 + std::exp(-x));
    }

} // namespace

#endif