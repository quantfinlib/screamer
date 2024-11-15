#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <tuple>
#include <iterator>
#include "screamer/common/functor_iterator.h"
#include "screamer/my_functors.h"

namespace py = pybind11;

using namespace screamer;

void init_bindings_myfunctors(py::module& m) {

    // MyFunctor bindings
    py::class_<MyFunctor> my_functor(m, "MyFunctor");
    my_functor
        .def(py::init<>())
        .def("__call__", [](MyFunctor& self, double a, double b, double c) {
            return self(a, b, c);
        })
        .def("__call__", [](MyFunctor& self, const MyFunctor::InputTuple& inputs) {
            return self(inputs);
        })
        .def("__call__", [](MyFunctor& self, py::object iterable) {
            return FunctorIterator<MyFunctor>(self, iterable);
        }, py::keep_alive<0, 1>());

    bind_functor_iterator<MyFunctor>(m, "MyFunctorIterator");

    // My2ndFunctor bindings
    py::class_<My2ndFunctor> my_2nd_functor(m, "My2ndFunctor");
    my_2nd_functor
        .def(py::init<>())
        .def("__call__", [](My2ndFunctor& self, double a, double b) {
            return self(a, b);
        })
        .def("__call__", [](My2ndFunctor& self, const My2ndFunctor::InputTuple& inputs) {
            return self(inputs);
        })
        .def("__call__", [](My2ndFunctor& self, py::object iterable) {
            return FunctorIterator<My2ndFunctor>(self, iterable);
        }, py::keep_alive<0, 1>());

    bind_functor_iterator<My2ndFunctor>(m, "My2ndFunctorIterator");

    // My3rdFunctor bindings
    py::class_<My3rdFunctor> my_3rd_functor(m, "My3rdFunctor");
    my_3rd_functor
        .def(py::init<>())
        .def("__call__", [](My3rdFunctor& self, double a, double b) {
            return self(a, b);
        })
        .def("__call__", [](My3rdFunctor& self, const My3rdFunctor::InputTuple& inputs) {
            return self(inputs);
        })
        .def("__call__", [](My3rdFunctor& self, py::object iterable) {
            return FunctorIterator<My3rdFunctor>(self, iterable);
        }, py::keep_alive<0, 1>());

    bind_functor_iterator<My3rdFunctor>(m, "My3rdFunctorIterator");
    
}

