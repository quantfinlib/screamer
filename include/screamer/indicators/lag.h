#ifndef SCREAMER_INDICATORS_LAG_H
#define SCREAMER_INDICATORS_LAG_H

#include <deque>
#include <stdexcept>  // For std::invalid_argument

namespace screamer {
namespace indicators {

class Lag {
public:
    Lag(int delay, double initial = 0.0);
    double operator()(double value);

private:
    int delay_;
    double initial_;
    std::deque<double> buffer_;
};

} // namespace indicators
} // namespace screamer

#endif // SCREAMER_INDICATORS_LAG_H
