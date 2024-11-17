#ifndef FUNCTOR_ITERATOR_H
#define FUNCTOR_ITERATOR_H

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace screamer {

template <typename Functor>
class FunctorIterator {
public:
    using InputTuple = typename Functor::InputTuple;

    Functor& functor;
    py::object iterator;

    FunctorIterator(Functor& func, py::object iterable)
        : functor(func), iterator(iterable.attr("__iter__")()) {}

    FunctorIterator& __iter__() {
        return *this;
    }

    py::object __next__() {
        try {
            // Get the next item from the iterator
            py::object item = iterator.attr("__next__")();

            InputTuple tuple;

            if constexpr (std::tuple_size<InputTuple>::value == 1) {
                // Single-element tuple handling
                using ElementType = typename std::tuple_element<0, InputTuple>::type;

                try {
                    // Try casting item directly to the argument type
                    ElementType value = item.cast<ElementType>();
                    tuple = std::make_tuple(value);
                } catch (const py::cast_error&) {
                    // Try casting item to InputTuple
                    tuple = item.cast<InputTuple>();
                }
            } else {
                // Multi-element tuple handling
                tuple = item.cast<InputTuple>();
            }

            auto result = functor(tuple);
            return py::cast(result);
        } catch (py::stop_iteration&) {
            throw py::stop_iteration();
        }
    }
};


// Binding the iterator class template
template <typename Functor>
void bind_functor_iterator(py::module& m, const char* name) {
    py::class_<FunctorIterator<Functor>>(m, name)
        .def(py::init<Functor&, py::object>())
        .def("__iter__", &FunctorIterator<Functor>::__iter__, py::return_value_policy::reference_internal)
        .def("__next__", &FunctorIterator<Functor>::__next__);
}


}
#endif