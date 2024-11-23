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

    py::class_<MyFunctor11>(m, "MyFunctor11")
        .def(py::init<>())
        .def("__call__", &MyFunctor11::handle_input);

    bind_functor_iterator<MyFunctor11>(m, "MyFunctorIterator11");

    py::class_<MyFunctor22>(m, "MyFunctor22")
        .def(py::init<>())
        .def("__call__", &MyFunctor22::handle_input);

    bind_functor_iterator<MyFunctor22>(m, "MyFunctorIterator22");

    py::class_<MyFunctor31>(m, "MyFunctor31")
        .def(py::init<>())
        .def("__call__", &MyFunctor31::handle_input);

/**
    py::class_<MyFunctor31>(m, "MyFunctor31")
        .def(py::init<>())
        .def("__call__",
            [](MyFunctor31& self, py::args args) {
                if (args.size() == 1) {
                    return self.handle_input(args[0]);
                } else {
                    py::tuple packed_args(args.size());
                    for (size_t i = 0; i < args.size(); ++i) {
                        packed_args[i] = args[i];
                    }
                    return self.handle_input(packed_args);                    
                }
            }
        );
*/
    bind_functor_iterator<MyFunctor31>(m, "MyFunctorIterator31");
    
}