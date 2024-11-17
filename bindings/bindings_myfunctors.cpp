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


    py::class_<MyFunctor>(m, "MyFunctor")
        .def(py::init<>())
        .def("__call__", [](MyFunctor& self, double a, double b, double c) {
            return self(a, b, c);
        })
        .def("__call__", [](MyFunctor& self, const MyFunctor::InputTuple& inputs) {
            return self(inputs);
        })
        .def("__call__", [](MyFunctor& self, py::object obj) {
            return self.process_object(obj);
        });

    bind_functor_iterator<MyFunctor>(m, "MyFunctorIterator");



    py::class_<MyFunctor1>(m, "MyFunctor1")
        .def(py::init<>())
        .def("__call__", [](MyFunctor1& self, double a) {
            return self(a);
        })
        .def("__call__", [](MyFunctor1& self, const MyFunctor1::InputTuple& inputs) {
            return self(inputs);
        })
        .def("__call__", [](MyFunctor1& self, py::object obj) {
            return self.process_object(obj);
        });

    bind_functor_iterator<MyFunctor1>(m, "MyFunctor1Iterator");



    py::class_<My2ndFunctor>(m, "My2ndFunctor")
        .def(py::init<>())
        .def("__call__", [](My2ndFunctor& self, double a, double b) {
            return self(a, b);
        })
        .def("__call__", [](My2ndFunctor& self, const My2ndFunctor::InputTuple& inputs) {
            return self(inputs);
        })
        .def("__call__", [](My2ndFunctor& self, py::object obj) {
            return self.process_object(obj);
        });

    bind_functor_iterator<My2ndFunctor>(m, "My2ndFunctorIterator");


    py::class_<My3rdFunctor>(m, "My3rdFunctor")
        .def(py::init<>())
        .def("__call__", [](My3rdFunctor& self, double a, double b) {
            return self(a, b);
        })
        .def("__call__", [](My3rdFunctor& self, const My3rdFunctor::InputTuple& inputs) {
            return self(inputs);
        })
        .def("__call__", [](My3rdFunctor& self, py::object obj) {
            return self.process_object(obj);
        });

    bind_functor_iterator<My3rdFunctor>(m, "My3rdFunctorIterator");
    
}


/*
in a statement like:

.def("__call__", [](MyFunctor& self, py::object obj) {
    return self.process_object(obj);
}, py::keep_alive<0, 1>());


py::keep_alive<0, 1> is used to ensure that the lifetime of the obj (the second argument in 
this directive) is tied to the lifetime of the return value (the first argument in this directive).

*/
