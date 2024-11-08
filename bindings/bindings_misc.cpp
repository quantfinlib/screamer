#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"
#include "screamer/lag.h"
#include "screamer/diff.h"

namespace py = pybind11;

void init_bindings_misc(py::module& m) {

    py::class_<screamer::Diff, screamer::ScreamerBase>(m, "Diff")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::Diff::operator(), py::arg("value"))
        .def("reset", &screamer::Diff::reset, "Reset to the initial state.");

    py::class_<screamer::Lag, screamer::ScreamerBase>(m, "Lag")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::Lag::operator(), py::arg("value"))
        .def("reset", &screamer::Lag::reset, "Reset to the initial state.");

}