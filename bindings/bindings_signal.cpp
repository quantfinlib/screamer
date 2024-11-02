#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"
#include "screamer/butter2.h"

namespace py = pybind11;

void init_bindings_signal(py::module& m) {

    py::class_<screamer::Butter2, screamer::ScreamerBase>(m, "Butter2")
        .def(py::init<double>(), py::arg("cutoff_freq"))
        .def("__call__", &screamer::Butter2::operator(), py::arg("value"))
        .def("reset", &screamer::Butter2::reset, "Reset to the initial state.");


}