#ifndef SCREAMER_INSPECT_NUM_ARGS_H
#define SCREAMER_INSPECT_NUM_ARGS_H

#include <tuple>
#include <type_traits>

namespace screamer {

// Helper to extract the type of a member function
template <typename T>
struct FunctionTraits;

// Specialization for member functions
template <typename C, typename Ret, typename... Args>
struct FunctionTraits<Ret (C::*)(Args...)> {
    using return_type = Ret;
    using class_type = C;
    using args_tuple = std::tuple<Args...>;
    static constexpr size_t arity = sizeof...(Args);  // Number of arguments
};

// Specialization for const member functions
template <typename C, typename Ret, typename... Args>
struct FunctionTraits<Ret (C::*)(Args...) const> {
    using return_type = Ret;
    using class_type = C;
    using args_tuple = std::tuple<Args...>;
    static constexpr size_t arity = sizeof...(Args);  // Number of arguments
};

}
#endif