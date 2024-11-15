#include "screamer/common/base_n.h"
#include "screamer/common/iterator_n.h"

namespace screamer {


template <typename Derived, size_t N>
template <size_t... Is>
double ScreamerBaseN<Derived, N>::expand_and_process(const std::array<double, N>& values, std::index_sequence<Is...>) {
    return process_scalar_base(values[Is]...);
}

template <typename Derived, size_t N>
template <typename... Args>
double ScreamerBaseN<Derived, N>::process_scalar_base(Args... args) {
    static_assert(sizeof...(Args) == N, "Number of arguments must match N");
    return static_cast<Derived*>(this)->process_scalar(args...);
}

template <typename Derived, size_t N>
template <typename... PyArgs>
py::object ScreamerBaseN<Derived, N>::operator()(PyArgs... py_args) {
    static_assert(sizeof...(PyArgs) >= 1, "At least one argument is required.");

    // Collect the PyArgs into an array of py::object
    std::array<py::object, sizeof...(PyArgs)> args = {py_args...};
    py::object first_arg = args[0];

    if (can_cast_to_double(first_arg)) {
        // Ensure the number of arguments matches N
        if constexpr (sizeof...(PyArgs) != N) {
            throw std::invalid_argument("Number of arguments must match N when using scalar inputs.");
        }

        // Create a sequence of indices for the arguments
        constexpr auto indices = std::make_index_sequence<N>{};

        // Convert arguments to doubles and process them
        std::array<double, N> values = convert_args_to_doubles(args, indices);
        return py::float_(expand_and_process(values, indices));
    }

    if (py::isinstance<py::iterable>(first_arg)) {
        if constexpr (sizeof...(PyArgs) > 1) {
            throw std::invalid_argument("Extra arguments are not allowed when using an iterable.");
        }

        return py::cast(LazyIteratorN(first_arg.cast<py::iterable>(), *this, get_lazy_processor()));
    }

    throw std::invalid_argument("First argument must be either a scalar or an iterable.");
}


// Helper function to convert arguments to doubles using index sequence
template <typename Derived, size_t N>
template <size_t... Is>
std::array<double, N> ScreamerBaseN<Derived, N>::convert_args_to_doubles(
    const std::array<py::object, N>& args, std::index_sequence<Is...>) {
    return {py::cast<double>(args[Is])...};
}

template <typename Derived, size_t N>
auto ScreamerBaseN<Derived, N>::get_lazy_processor() {
    return [this](const py::object& item) -> double {
        auto values = item.cast<std::array<double, N>>();
        return expand_and_process(values, std::make_index_sequence<N>{});
    };
}

} //namespace