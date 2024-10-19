#include "screamer/lag.h"
#include <screamer/transforms.h>

namespace screamer {

Lag::Lag(int N, double defaultValue)
    : index(0), N(N), defaultValue(defaultValue)
{
    if (N < 1) {
        throw std::invalid_argument("Delay must be an integer >= 1.");
    }
    buffer.resize(N, defaultValue);    
}

double Lag::operator()(double newValue) {

    double oldValue = buffer[index];
    buffer[index] = newValue;
    index++;
    if (index == N) {
        index = 0;
    }
    return oldValue;
}

void Lag::reset() {
    std::fill(buffer.begin(), buffer.end(), defaultValue);
    index = 0;
}

py::array_t<double> Lag::transform(py::array_t<double> input_array) {
    return transform_1(*this, input_array);
}

} // namespace screamer
