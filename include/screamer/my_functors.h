#ifndef MY_FUNCTORS_H
#define MY_FUNCTORS_H

#include <tuple>
#include "screamer/common/functor_base.h"

namespace screamer {

class MyFunctor : public FunctorBase<MyFunctor, double, double, double, double> {
public:
    double call(double a, double b, double c) override {
        // Your implementation
        return a + b + c;  // Example
    }
};

class My2ndFunctor : public FunctorBase<My2ndFunctor, double, double, double> {
public:
    double call(double a, double b) override {
        // Your implementation
        return a * b;  // Example
    }
};

// Similarly for My3rdFunctor


class My3rdFunctor : public FunctorBase<My3rdFunctor, std::tuple<double, double>, double, double> {
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


x = [(1,2,3)]*5
print(obj1(x))

print(obj1((1,2,3)))

*/