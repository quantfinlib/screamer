#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"
#include "screamer/butter.h"

namespace py = pybind11;

void init_bindings_signal(py::module& m) {

    py::class_<screamer::Butter, screamer::ScreamerBase>(m, "Butter")
        .def(py::init<int,double>(),  py::arg("order"), py::arg("cutoff_freq"))
        .def("__call__", &screamer::Butter::operator(), py::arg("value"))
        .def("reset", &screamer::Butter::reset, "Reset to the initial state.");

}