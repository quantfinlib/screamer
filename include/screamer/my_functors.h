#ifndef MY_FUNCTORS_H
#define MY_FUNCTORS_H

#include <tuple>
#include "screamer/common/functor_base.h"

namespace screamer {

class MyFunctor31 : public FunctorBase<MyFunctor31, 3, 1> {
public:
    ResultTuple call(const InputArray& inputs) override {
        const auto& [a, b, c] = std::tie(inputs[0], inputs[1], inputs[2]);
        return a + b + c;
    }
};



class MyFunctor11 : public FunctorBase<MyFunctor11, 1, 1> {
public:
    ResultTuple call(const InputArray& inputs) override {
        const auto& [a] = std::tie(inputs[0]);
        return a * 2;
    }
};


class MyFunctor22 : public FunctorBase<MyFunctor22, 2, 2> {
private:
    double sum_state = 0.0;   // Example internal state
public:
    ResultTuple call(const InputArray& inputs) override {
        const auto& [a, b] = std::tie(inputs[0], inputs[1]);
        sum_state += a + b;           // Update state
        return {a - b, sum_state};    // Return computed values and state
    }
};


}
#endif

/*
# ---------------------------------
# MyFunctor11: single argument
#----------------------------------
from screamer import MyFunctor11
import numpy as np

obj11 = MyFunctor11() 
obj11(1)
obj11((1))
obj11((1,2,3))
obj11([1,2,3])
obj11([(1,), (2,), (3,)])
obj11(np.arange(5))
obj11(np.arange(10)[1::2])
obj11(np.arange(12).reshape(-1,3))
list(obj11(iter([1,2,3])))


# ----------------------------------------
# MyFunctor31: 3 arguments, 1 return value
#-----------------------------------------
from screamer import MyFunctor31
import numpy as np

obj31 = MyFunctor31()
obj31(1,2,3)
obj31((1,2,3))
obj31([(1,2,3), (4,5,6)])
obj31(np.arange(5), 2*np.arange(5), 3*np.arange(5))

obj31( [1,2,3,4], [1,2,3,4], [5,6,7,8] )





obj11((1))
obj11((1,2,3))
obj11([1,2,3])
obj11([(1,), (2,), (3,)])
obj11(np.arange(5))
obj11(np.arange(10)[1::2])
obj11(np.arange(12).reshape(-1,3))












obj(1.1, 2.2, 3.3)
obj( (1, 2, 3) )
obj( [(1, 2, 3), (4, 5, 6)] )

def tuple_gen():
	for _ in range(10):
		yield (1,2,3)

for a in obj(tuple_gen()):
	print(a)

list(obj(tuple_gen()))


# ---------------------------------
# MyFunctor: single input-output
#---------------------------------
from screamer import MyFunctor1

obj1 = MyFunctor1() 

obj1(1)
obj1(1.1)
obj1( (1) )
obj1( (1, 2, 3) )

def tuple_gen1():
	for _ in range(10):
		yield (1)

for a in obj1(tuple_gen1()):
	print(a)

list(obj1(tuple_gen1()))


---------------------------------
# My3rdFunctor: two outputs
---------------------------------
from screamer import My3rdFunctor

obj3 = My3rdFunctor()


obj3(1, 2)
obj3(1.1, 2.2)
obj3( (1, 2) )
obj3( [(1, 2), (4, 5)] )

def tuple_gen3():
	for _ in range(10):
		yield (1,2)

for a in obj3(tuple_gen3()):
	print(a)

list(obj3(tuple_gen3()))

*/