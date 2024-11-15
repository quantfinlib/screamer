#ifndef FUNCTOR_BASE_H
#define FUNCTOR_BASE_H

#include <tuple>
#include <type_traits>
#include <utility>

namespace screamer {

template <typename OutputType, typename... InputTypes>
class FunctorBase {
public:
    using InputTuple = std::tuple<InputTypes...>;
    using ResultType = OutputType;

    virtual ~FunctorBase() = default;

    // Unpacked arguments - calls the renamed `call` method
    virtual ResultType call(InputTypes... args) = 0;

    // Tuple-based operator() forwards to `call`
    ResultType operator()(const InputTuple& inputs) {
        return std::apply([this](auto&&... args) { return call(args...); }, inputs);
    }

    // Unpacked operator() also forwards to `call`
    ResultType operator()(InputTypes... args) {
        return call(args...);
    }
};


}
#endif