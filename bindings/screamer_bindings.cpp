#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/lag.h"

namespace py = pybind11;

PYBIND11_MODULE(screamer_bindings, m) {
    py::class_<screamer::Lag>(m, "Lag")
        .def(py::init<int, double>(), py::arg("delay"), py::arg("initial") = 0.0)
        .def("__call__", &screamer::Lag::operator(), "Apply lag to the input value")
        .def("reset", &screamer::Lag::reset, "Reset the indicator")
        .def("transform", &screamer::Lag::transform, "Apply the lag transformation to a NumPy array");
}
