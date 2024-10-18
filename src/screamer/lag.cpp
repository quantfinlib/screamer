#include "screamer/lag.h"

namespace screamer {

Lag::Lag(int N, double defaultValue)
    : index(0), N(N)
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

} // namespace screamer
