#include <iostream>
#include <iomanip>
#include "screamer/detail/rolling_sum.h"
#include "screamer/detail/rolling_mean.h"

 
int main() {

    {
        std::cout <<  "INPUT:    ";
        for (int i=0; i<8; ++i) {
            double k = i+1;
            std::cout << std::setprecision(0) << std::fixed << std::setw(4) << k << " ";
        }
        std::cout << std::endl;
    }

    for (int n=1; n<4; n++) {
        std::cout << std::endl << "RollingSum N=" << n  << std::endl;
        {
            std::cout <<  "strict:   ";
            screamer::detail::RollingSum acc(n, "strict");
            for (int i=0; i<8; ++i) {
                double k = i+1;
                std::cout << std::setprecision(0) << std::fixed << std::setw(4) << acc.append(k) << " ";
            }
            std::cout << std::endl;
        }
        {
            std::cout << "expanding ";
            screamer::detail::RollingSum acc(n, "expanding");
            for (int i=0; i<8; ++i) {
                double k = i+1;
                std::cout << std::setprecision(0) << std::fixed << std::setw(4) << acc.append(k) << " ";
            }
            std::cout << std::endl;
        }
        {
            std::cout  << "zero      ";
            screamer::detail::RollingSum acc(n, "zero");
            for (int i=0; i<8; ++i) {
                double k = i+1;
                std::cout << std::setprecision(0) << std::fixed << std::setw(4) << acc.append(k) << " ";
            }
            std::cout << std::endl;
        }

    }
 
    return 0;
}
