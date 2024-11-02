#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"

namespace py = pybind11;

void init_bindings_core(py::module& m) {

    py::class_<screamer::ScreamerBase>(m, "_ScreamerBase");

    py::class_<screamer::ScreamerBase::LazyIterator>(m, "_LazyIterator")
        .def("__iter__", &screamer::ScreamerBase::LazyIterator::__iter__, py::return_value_policy::reference_internal)
        .def("__next__", &screamer::ScreamerBase::LazyIterator::__next__);

}