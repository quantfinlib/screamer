#ifndef MY_FUNCTORS_H
#define MY_FUNCTORS_H

#include <tuple>
#include "screamer/common/functor_base.h"

namespace screamer {

class MyFunctor : public FunctorBase<double, double, double, double> {
private:
    double state = 0.0;  // Example internal state

public:
    double call(double a, double b, double c) override {
        state += a + b + c;  // Update internal state
        return state;        // Return updated state
    }
};


class My2ndFunctor : public FunctorBase<double, double, double> {
private:
    double multiplier = 1.0;  // Example internal state

public:
    double call(double a, double b) override {
        multiplier *= a * b;  // Update multiplier
        return multiplier;    // Return updated state
    }
};


class My3rdFunctor : public FunctorBase<std::tuple<double, double>, double, double> {
private:
    double sum_state = 0.0;   // Example internal state

public:
    std::tuple<double, double> call(double a, double b) override {
        sum_state += a + b;           // Update state
        return {a - b, sum_state};    // Return computed values and state
    }
};


}
#endif

/*

from screamer import MyFunctor, My2ndFunctor, My3rdFunctor
obj1 = MyFunctor()
obj2 = My2ndFunctor()
obj3 = My3rdFunctor()

print(obj1(1,2,3))
print(obj2(1,2))
print(obj3(1,2))

def gen1():
    for _ in range(10):
        yield (1,2,3)

list(obj1(gen1()))

def gen3():
    for _ in range(10):
        yield (1,3)

list(obj3(gen3()))

*/