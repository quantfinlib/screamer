#ifndef SCREAMER_ROLLING_OFI_H
#define SCREAMER_ROLLING_OFI_H

#include "screamer/common/base_n.h"

namespace screamer {

class RollingOFI : public ScreamerBaseN<RollingOFI, 4> {
public:
    RollingOFI(int window_size, const std::string& start_policy = "strict")
        : window_size_(window_size), start_policy_(start_policy) {}

    double process_scalar(double bid_price, double bid_volume, double ask_price, double ask_volume) {
        return bid_price + bid_volume + ask_price + ask_volume;
    }

private:
    int window_size_;
    std::string start_policy_;
};

}
#endif