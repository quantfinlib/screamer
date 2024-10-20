#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/lag.h"
#include "screamer/diff.h"

namespace py = pybind11;

PYBIND11_MODULE(screamer_bindings, m) {

    py::class_<screamer::Lag>(m, "Lag")
        .def(py::init<int>(), py::arg("delay"),
             "Initialize the Lag indicator.\n\n"
             ":param delay: The delay parameter, d.")
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

    py::class_<screamer::Diff>(m, "Diff")
        .def(py::init<int>(), py::arg("delay"),
             "Initialize the Diff indicator.\n\n"
             ":param delay: The delay parameter, d.")
        .def("__call__", &screamer::Diff::operator(), py::arg("value"),
             "Apply diff to the input value.\n\n"
             ".. math::\n"
             "    f(X[i]) = X[i[] - X[i-d]\n\n"
             "This applies a diff of `d` to the stream of input values `X`.")
        .def("reset", &screamer::Diff::reset, "Reset the diff indicator to its initial state.")
        .def("transform", &screamer::Diff::transform, py::arg("input_array"),
             "Apply the diff transformation to a NumPy array.\n\n"
             ".. math::\n"
             "    Y[i] = X[i] - X[i-d]\n\n"
             "This method returns the array `Y` where each value at index `i` is change over `d` steps.");


}