#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/lag.h"

namespace py = pybind11;

PYBIND11_MODULE(screamer_bindings, m) {
    py::class_<screamer::Lag>(m, "Lag")
        .def(py::init<int, double>(), py::arg("delay"), py::arg("initial") = 0.0,
             "Initialize the Lag indicator.\n\n"
             ":param delay: The delay parameter, d.\n"
             ":param initial: The initial value (default 0.0).")
        .def("__call__", &screamer::Lag::operator(), py::arg("value"),
             "Apply lag to the input value.\n\n"
             ".. math::\n"
             "    f(X[i]) = X[i-d]\n\n"
             "This applies a lag of `d` to the stream of input values `X`.")
        .def("reset", &screamer::Lag::reset, "Reset the Lag indicator to its initial state.")
        .def("transform", &screamer::Lag::transform, py::arg("input_array"),
             "Apply the lag transformation to a NumPy array.\n\n"
             ".. math::\n"
             "    Y[i] = X[i-d]\n\n"
             "This method returns the array `Y` where each value at index `i` is shifted by `d`.");
}