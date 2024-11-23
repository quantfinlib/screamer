#ifndef FUNCTOR_ITERATOR_H
#define FUNCTOR_ITERATOR_H

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace screamer {

template <typename Functor>
class FunctorIterator {
public:
    using InputArray = typename Functor::InputArray;
    using ResultTuple = typename Functor::ResultTuple;

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

            // Cast the item to the expected InputArray type
            InputArray input = item.cast<InputArray>();

            // Call the functor and process the result
            auto result = functor.call(input);

            // Cast and return the result as a Python object
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

} // namespace screamer

#endif
