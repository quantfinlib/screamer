#ifndef SCREAMER_BASE_N_H
#define SCREAMER_BASE_N_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <array>
#include <utility>  // For std::index_sequence
//#include "screamer/common/function_traits.h"

namespace py = pybind11;

namespace screamer {

// Forward declare LazyIterator
class LazyIteratorN;

template <typename Derived, size_t N>
class ScreamerBaseN {
public:
    virtual ~ScreamerBaseN() = default;
    virtual void reset() {}

    template <typename... PyArgs>
    py::object operator()(PyArgs... py_args);

protected:
    template <size_t... Is>
    double expand_and_process(const std::array<double, N>& values, std::index_sequence<Is...>);

    template <typename... Args>
    double process_scalar_base(Args... args);

    // Provide LazyIterator with callable for process_scalar
    auto get_lazy_processor();

    template <size_t... Is>
    std::array<double, N> convert_args_to_doubles(const std::array<py::object, N>& args, std::index_sequence<Is...>);

}; // class

} // namespace

#include "base_n.tpp"  // Include implementation file

#endif  // SCREAMER_BASE_H
