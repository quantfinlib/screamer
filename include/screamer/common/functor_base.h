#ifndef FUNCTOR_BASE_H
#define FUNCTOR_BASE_H

#include <tuple>
#include <type_traits>
#include <utility>

namespace screamer {

template <typename Derived, typename OutputType, typename... InputTypes>
class FunctorBase {
public:
    using InputTuple = std::tuple<InputTypes...>;
    using ResultType = OutputType;

    virtual ~FunctorBase() = default;

    // Method to be implemented in derived classes
    virtual ResultType call(InputTypes... args) = 0;

    // Process individual arguments like: func(double, double, double)
    ResultType operator()(InputTypes... args) {
        return call(args...);
    }
    // Process a single tuple like func((double, double))
    ResultType operator()(const InputTuple& inputs) {
        return std::apply([this](auto&&... args) { return call(args...); }, inputs);
    }

    // Process Python iterables of tuples
    py::list operator()(py::iterable iterable) {
        py::list results;

        for (auto item : iterable) {
            auto tuple = item.cast<InputTuple>();
            results.append((*this)(tuple));
        }

        return results;
    }

    py::object process_object(py::object obj) {
        // Check if the object is a tuple
        if (py::isinstance<py::tuple>(obj)) {
            bool all_elements_are_tuples = true;
            for (auto element : obj) {
                if (!py::isinstance<py::tuple>(element) ||
                    py::len(element) != std::tuple_size<InputTuple>::value) {
                    all_elements_are_tuples = false;
                    break;
                }
            }

            if (all_elements_are_tuples) {
                // Treat as an iterable of tuples
                return (*this)(obj.cast<py::iterable>()); // Returns py::list
            } else if (py::len(obj) == std::tuple_size<InputTuple>::value) {
                // Treat as a single tuple to unpack
                return py::cast((*this)(obj.cast<InputTuple>())); // Cast to py::object
            }
        }

        // Check if the object is an iterator
        if (py::hasattr(obj, "__next__")) {
            // Return a FunctorIterator for iterators
            return py::cast(
                FunctorIterator<Derived>(*static_cast<Derived*>(this), obj)
            );
        }

        // Otherwise, treat as a general iterable
        return (*this)(obj.cast<py::iterable>()); // Returns py::list
    }


};



}
#endif