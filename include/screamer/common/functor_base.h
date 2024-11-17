#ifndef FUNCTOR_BASE_H
#define FUNCTOR_BASE_H

#include <tuple>
#include <type_traits>
#include <utility>

namespace screamer {

template <typename Derived, typename ResultType, typename... InputTypes>
class FunctorBase {
public:
    using InputTuple = std::tuple<InputTypes...>;

    // Keep the pure virtual function
    virtual ResultType call(InputTypes... args) = 0;

    // Process individual arguments
    ResultType operator()(InputTypes... args) {
        return call(args...);
    }

    // Process a single tuple
    ResultType operator()(const InputTuple& inputs) {
        return std::apply([this](auto&&... args) { return call(args...); }, inputs);
    }

    // Process an iterable
    py::list operator()(py::iterable iterable) {
        py::list results;

        for (auto item : iterable) {
            InputTuple args_tuple;

            if constexpr (std::tuple_size<InputTuple>::value == 1) {
                // For single-argument functors
                try {
                    // Try casting item directly to the argument type
                    using ArgType = typename std::tuple_element<0, InputTuple>::type;
                    ArgType arg = item.cast<ArgType>();
                    args_tuple = std::make_tuple(arg);
                } catch (const py::cast_error&) {
                    // If it fails, try casting to InputTuple (tuple with one element)
                    args_tuple = item.cast<InputTuple>();
                }
            } else {
                // For multiple-argument functors
                args_tuple = item.cast<InputTuple>();
            }

            results.append((*this)(args_tuple));
        }

        return results;
    }

    // Process a py::object
    py::object process_object(py::object obj) {
        // Check if the object is an iterator
        if (py::hasattr(obj, "__next__")) {
            // Return a FunctorIterator for iterators
            return py::cast(
                FunctorIterator<Derived>(*static_cast<Derived*>(this), obj)
            );
        }

        // Check if the object is a tuple
        if (py::isinstance<py::tuple>(obj)) {
            if constexpr (std::tuple_size<InputTuple>::value > 1) {
                if (py::len(obj) == std::tuple_size<InputTuple>::value) {
                    // Treat as a single tuple of arguments
                    return py::cast((*this)(obj.cast<InputTuple>()));
                } else {
                    // Treat as an iterable of tuples
                    return (*this)(obj.cast<py::iterable>());
                }
            } else {
                // For single-argument functors, treat as an iterable
                return (*this)(obj.cast<py::iterable>());
            }
        }

        // Check if the object is an iterable
        if (py::isinstance<py::iterable>(obj)) {
            return (*this)(obj.cast<py::iterable>());
        }

        // Try casting to InputTuple
        try {
            auto args_tuple = obj.cast<InputTuple>();
            return py::cast((*this)(args_tuple));
        } catch (const py::cast_error&) {
            // For single-argument functors, try casting directly to the argument type
            if constexpr (std::tuple_size<InputTuple>::value == 1) {
                using ArgType = typename std::tuple_element<0, InputTuple>::type;
                try {
                    ArgType arg = obj.cast<ArgType>();
                    return py::cast(call(arg));
                } catch (const py::cast_error&) {
                    throw py::type_error("Invalid input type for the functor.");
                }
            } else {
                throw py::type_error("Invalid input type for the functor.");
            }
        }
    }
};


}
#endif