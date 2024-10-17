#include "screamer/indicators/lag.h"

namespace screamer {
namespace indicators {

Lag::Lag(int delay, double initial)
    : delay_(delay), initial_(initial)
{
    if (delay_ < 1) {
        throw std::invalid_argument("Delay must be an integer >= 1.");
    }
    buffer_.resize(delay_, initial_);
}


double Lag::operator()(double value) {
    double delayed = buffer_.front();
    buffer_.pop_front();
    buffer_.push_back(value);
    return delayed;
}

} // namespace indicators
} // namespace screamer
