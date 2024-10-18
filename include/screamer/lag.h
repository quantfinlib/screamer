#ifndef SCREAMER_LAG_H
#define SCREAMER_LAG_H

#include <vector>
#include <stdexcept>

namespace screamer {

class Lag {
public:
    Lag(int N, double defaultValue = 0.0);
    double operator()(double newValue);

private:
    size_t index;
    size_t N;
    std::vector<double> buffer;    
};

} // namespace screamer

#endif // SCREAMER_LAG_H
