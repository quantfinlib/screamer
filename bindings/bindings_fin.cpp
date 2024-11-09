#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"
#include "screamer/return.h"
#include "screamer/log_return.h"
#include "screamer/rsi.h"

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

    py::class_<screamer::RSI, screamer::ScreamerBase>(m, "RSI")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RSI::operator(), py::arg("value"))
        .def("reset", &screamer::RSI::reset, "Reset to the initial state.");

}