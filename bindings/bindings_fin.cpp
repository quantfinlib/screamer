#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"
#include "screamer/return.h"
#include "screamer/log_return.h"
#include "screamer/rolling_fracdiff.h"

namespace py = pybind11;

void init_bindings_fin(py::module& m) {

    py::class_<screamer::Return, screamer::ScreamerBase>(m, "Return")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::Return::operator(), py::arg("value"))
        .def("reset", &screamer::Return::reset, "Reset to the initial state.");

    py::class_<screamer::LogReturn, screamer::ScreamerBase>(m, "LogReturn")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::LogReturn::operator(), py::arg("value"))
        .def("reset", &screamer::LogReturn::reset, "Reset to the initial state.");
    
    py::class_<screamer::RollingFracDiff, screamer::ScreamerBase>(m, "RollingFracDiff")
        .def(py::init<double, int, double>(), py::arg("frac_order"), py::arg("window_size"), py::arg("threshold")=1e-5)
        .def("__call__", &screamer::RollingFracDiff::operator(), py::arg("value"))
        .def("reset", &screamer::RollingFracDiff::reset, "Reset to the initial state.");
}