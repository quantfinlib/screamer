#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/lag.h"
#include "screamer/diff.h"
#include "screamer/rolling_sum.h"
#include "screamer/return.h"
#include "screamer/log_return.h"
#include "screamer/sma.h"
#include "screamer/fracdiff.h"

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
             "Apply the lag transformation to a NumPy array.");

    py::class_<screamer::Diff>(m, "Diff")
        .def(py::init<int>(), py::arg("delay"),
             "Initialize the Diff indicator.\n\n"
             ":param delay: The delay parameter, d.")
        .def("__call__", &screamer::Diff::operator(), py::arg("value"),
             "Apply diff to the input value.\n\n"
             ".. math::\n"
             "    f(X[i]) = X[i] - X[i-d]\n\n"
             "This applies a diff of `d` to the stream of input values `X`.")
        .def("reset", &screamer::Diff::reset, "Reset the diff indicator to its initial state.")
        .def("transform", &screamer::Diff::transform, py::arg("input_array"),
             "Apply the diff transformation to a NumPy array.");

    py::class_<screamer::RollingSum>(m, "RollingSum")
        .def(py::init<int>(), py::arg("window_size"),
             "Initialize the rolling sum indicator.\n\n"
             ":param window_size: The window_size parameter.")
        .def("__call__", &screamer::RollingSum::operator(), py::arg("value"),
             "Update and return the rolling sum.\n\n"
             ".. math::\n"
             "    f(X[i]) = \\sum_{i=0}^{w-1}X[i-w]\n\n"
             "This applies a rolling sum the stream of input values `X`.")
        .def("reset", &screamer::RollingSum::reset, "Reset the rolling sum indicator to its initial state.")
        .def("transform", &screamer::RollingSum::transform, py::arg("input_array"),
             "Apply the rolling sum transformation to a NumPy array.");

    py::class_<screamer::Return>(m, "Return")
        .def(py::init<int>(), py::arg("delay"),
             "Initialize the simple return indicator.\n\n"
             ":param window_size: The return delay parameter.")
        .def("__call__", &screamer::Return::operator(), py::arg("value"),
             "Update and return the simple return.\n\n"
             ".. math::\n"
             "    f(X[i]) = X[i] / X[i-d] - 1\n\n"
             "This applies a simple return to the stream of input values `X`.")
        .def("reset", &screamer::Return::reset, "Reset the simple return indicator to its initial state.")
        .def("transform", &screamer::Return::transform, py::arg("input_array"),
             "Apply the simple return transformation to a NumPy array.");

    py::class_<screamer::LogReturn>(m, "LogReturn")
        .def(py::init<int>(), py::arg("delay"),
             "Initialize the log return indicator.\n\n"
             ":param window_size: The return delay parameter.")
        .def("__call__", &screamer::LogReturn::operator(), py::arg("value"),
             "Update and return the log return.\n\n"
             ".. math::\n"
             "    f(X[i]) = ln( X[i] / X[i-d] )\n\n"
             "This applies a log return to the stream of input values `X`.")
        .def("reset", &screamer::LogReturn::reset, "Reset the log return indicator to its initial state.")
        .def("transform", &screamer::LogReturn::transform, py::arg("input_array"),
             "Apply the log return transformation to a NumPy array.");

    py::class_<screamer::SMA>(m, "SMA")
        .def(py::init<int>(), py::arg("window_size"),
             "Initialize the simple moving average indicator.\n\n"
             ":param window_size: The window_size parameter.")
        .def("__call__", &screamer::SMA::operator(), py::arg("value"),
             "Update and return the simple moving average.\n\n"
             ".. math::\n"
             "    f(X[i]) = \\sum_{i=0}^{w-1}X[i-w] / w\n\n"
             "This applies a simple moving average to the stream of input values `X`.")
        .def("reset", &screamer::SMA::reset, "Reset the simple moving average indicator to its initial state.")
        .def("transform", &screamer::SMA::transform, py::arg("input_array"),
             "Apply the simple moving average transformation to a NumPy array.");

     py::class_<screamer::FracDiff>(m, "FracDiff")
          .def(py::init<double, int, double>(), py::arg("frac_order"), py::arg("window_size"), py::arg("threshold")=1e-5,
                "Initialize the fractional difference indicator.\n\n"
                ":param frac_order: The fractional order parameter, d.\n"
                ":param window_size: The window_size parameter.\n"
                ":param threshold: The threshold parameter.")
          .def("__call__", &screamer::FracDiff::operator(), py::arg("value"),
                "Update and return the fractionally differenced value.\n\n"
                ".. math::\n"
                "    f(X[i]) = \\sum_{i=0}^{w-1}w[i] * X[i-w]\n\n"
                "This applies a fractional difference to the stream of input values `X`.")
          .def("transform", &screamer::FracDiff::transform, py::arg("input_array"),
                "Apply the fractional difference transformation to a NumPy array.");

}