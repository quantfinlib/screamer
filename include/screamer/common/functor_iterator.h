#ifndef FUNCTOR_ITERATOR_H
#define FUNCTOR_ITERATOR_H

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace screamer {

// Template for the iterator class
template <typename Functor>
class FunctorIterator {
public:
    Functor& functor;
    py::object iterator;

    FunctorIterator(Functor& functor, py::object iterable)
        : functor(functor), iterator(iterable.attr("__iter__")()) {}

    FunctorIterator(const FunctorIterator& other)
        : functor(other.functor), iterator(other.iterator) {}

    py::object __iter__() {
        return py::cast(this);
    }

    typename Functor::ResultType __next__() {
        py::object item;
        try {
            item = iterator.attr("__next__")();
        } catch (py::stop_iteration&) {
            throw py::stop_iteration();
        }
        auto args = item.cast<typename Functor::InputTuple>();
        return functor(args);
    }
};

// Binding the iterator class template
template <typename Functor>
void bind_functor_iterator(py::module& m, const char* name) {
    py::class_<FunctorIterator<Functor>>(m, name)
        .def(py::init<Functor&, py::object>())
        .def("__iter__", &FunctorIterator<Functor>::__iter__)
        .def("__next__", &FunctorIterator<Functor>::__next__);
}

}
#endif