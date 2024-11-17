#ifndef MY_FUNCTORS_H
#define MY_FUNCTORS_H

#include <tuple>
#include "screamer/common/functor_base.h"

namespace screamer {

class MyFunctor : public FunctorBase<MyFunctor, double, double, double, double> {
public:
    double call(double a, double b, double c) override {
        return a + b + c;
    }
};


class MyFunctor1 : public FunctorBase<MyFunctor1, double, double> {
public:
    double call(double a) override {
        return a * 2; 
    }
};

class My2ndFunctor : public FunctorBase<My2ndFunctor, double, double, double> {
public:
    double call(double a, double b) override {
        return a * b;
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
from screamer import MyFunctor1
obj1 = MyFunctor1()
obj1(1) # works

tuple_of_double = (1.1, 2.2, 3.3)
list_of_double = [1.1, 2.2, 3.3]
tuple_of_tuple_of_double = ( (1.1), (2.2), (3.3) )
list_of_tuple_of_double = [ (1.1), (2.2), (3.3) ]

obj1(tuple_of_double) # fails
obj1(list_of_double) # fails
obj1(tuple_of_tuple_of_double) # fails
obj1(list_of_tuple_of_double) # fails


obj1((1,2,3))

from screamer import MyFunctor, MyFunctor1, My2ndFunctor, My3rdFunctor

obj0 = MyFunctor()
obj1 = MyFunctor1()
obj2 = My2ndFunctor()
obj3 = My3rdFunctor()

print(obj0(1,2,3))
print(obj1(1))
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