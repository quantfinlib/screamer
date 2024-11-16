#ifndef FUNCTOR_ITERATOR_H
#define FUNCTOR_ITERATOR_H

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace screamer {

template <typename Functor>
class FunctorIterator {
public:
    Functor& functor;
    py::object iterator; // Holds a reference to the Python iterator

    FunctorIterator(Functor& functor, py::object iterable)
        : functor(functor), iterator(iterable) {
        // Ensure we have an iterator
        if (!py::hasattr(iterator, "__next__")) {
            iterator = py::iter(iterator);
        }
    }

    FunctorIterator& __iter__() {
        return *this;
    }

    py::object __next__() {
        py::object item;
        try {
            item = iterator.attr("__next__")();
        } catch (py::stop_iteration&) {
            throw py::stop_iteration();
        }
        auto args = item.cast<typename Functor::InputTuple>();
        auto result = functor(args);
        return py::cast(result);
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