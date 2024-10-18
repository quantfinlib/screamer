#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "screamer/lag.h"

namespace py = pybind11;
using namespace screamer;

PYBIND11_MODULE(screamer_bindings, m) {
    py::class_<Lag>(m, "Lag")
        .def(py::init<int, double>(), py::arg("delay"), py::arg("initial") = 0.0)
        .def("__call__", &Lag::operator(), "Apply lag to the input value");
}
