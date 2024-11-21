#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"
#include "screamer/return.h"
#include "screamer/log_return.h"
#include "screamer/rolling_fracdiff.h"
#include "screamer/expanding_smpoly.h"
#include "screamer/expanding_smexp.h"
#include "screamer/expanding_smpower.h"


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

    py::class_<screamer::ExpandingSMPoly, screamer::ScreamerBase>(m, "ExpandingSMPoly")
        .def(py::init<double, int, bool>(), py::arg("phi"), py::arg("tau"), py::arg("log")=true)
        .def("__call__", &screamer::ExpandingSMPoly::operator(), py::arg("value"))
        .def("reset", &screamer::ExpandingSMPoly::reset, "Reset to the initial state.");

    py::class_<screamer::ExpandingSMExp, screamer::ScreamerBase>(m, "ExpandingSMExp")
        .def(py::init<double, int>(), py::arg("phi"), py::arg("tau"))
        .def("__call__", &screamer::ExpandingSMExp::operator(), py::arg("value"))
        .def("reset", &screamer::ExpandingSMExp::reset, "Reset to the initial state.");

    py::class_<screamer::ExpandingSMPower, screamer::ScreamerBase>(m, "ExpandingSMPower")
        .def(py::init<double, int>(), py::arg("phi"), py::arg("tau"))
        .def("__call__", &screamer::ExpandingSMPower::operator(), py::arg("value"))
        .def("reset", &screamer::ExpandingSMPower::reset, "Reset to the initial state.");

}