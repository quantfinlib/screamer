#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/lag.h"
#include "screamer/diff.h"
#include "screamer/rolling_sum.h"
#include "screamer/rolling_mean.h"
#include "screamer/rolling_std.h"
#include "screamer/rolling_var.h"
#include "screamer/rolling_skew.h"
#include "screamer/rolling_kurt.h"
#include "screamer/rolling_zscore.h"
#include "screamer/return.h"
#include "screamer/log_return.h"
#include "screamer/ewma.h"
#include "screamer/ew_var.h"
#include "screamer/ew_std.h"

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
             "Apply the lag transformation to a NumPy array.")
        .doc() = "Lag indicator, a delayed version of values in a sequence.";

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
             "Apply the diff transformation to a NumPy array.")
        .doc() = "Diff indicator, relative compared to past values.";

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
             "Apply the rolling sum transformation to a NumPy array.")
        .doc() = "Rolling sum, the sum of the most recent values in a sequence.";

    py::class_<screamer::RollingMean>(m, "RollingMean")
        .def(py::init<int>(), py::arg("window_size"),
             "Initialize the simple moving average indicator.\n\n"
             ":param window_size: The window_size parameter.")
        .def("__call__", &screamer::RollingMean::operator(), py::arg("value"),
             "Update and return the simple moving average.\n\n"
             ".. math::\n"
             "    f(X[i]) = \\sum_{i=0}^{w-1}X[i-w] / w\n\n"
             "This applies a simple moving average to the stream of input values `X`.")
        .def("reset", &screamer::RollingMean::reset, "Reset the simple moving average indicator to its initial state.")
        .def("transform", &screamer::RollingMean::transform, py::arg("input_array"),
             "Apply the simple moving average transformation to a NumPy array.")
        .doc() = "Simple moving average, the average of the most recent values in a sequence.";


    py::class_<screamer::RollingStd>(m, "RollingStd")
        .def(py::init<int>(), py::arg("window_size"),
             "Initialize the rolling standard deviation indicator.\n\n"
             ":param window_size: The window_size parameter.")
        .def("__call__", &screamer::RollingStd::operator(), py::arg("value"),
             "This applies a rolling standard deviation the stream of input values `X`.")
        .def("reset", &screamer::RollingStd::reset, "Reset the indicator to its initial state.")
        .def("transform", &screamer::RollingStd::transform, py::arg("input_array"),
             "Apply the indicator to a NumPy array.")
        .doc() = "Rolling standard deviation, the standard deviation of the most recent values in a sequence.";

    py::class_<screamer::RollingVar>(m, "RollingVar")
        .def(py::init<int>(), py::arg("window_size"),
             "Initialize the rolling variance indicator.\n\n"
             ":param window_size: The window_size parameter.")
        .def("__call__", &screamer::RollingVar::operator(), py::arg("value"),
             "This applies a rolling variance to the stream of input values `X`.")
        .def("reset", &screamer::RollingVar::reset, "Reset the indicator to its initial state.")
        .def("transform", &screamer::RollingVar::transform, py::arg("input_array"),
             "Apply the indicator to a NumPy array.")
        .doc() = "Rolling variance, the variance of the most recent values in a sequence.";

    py::class_<screamer::RollingSkew>(m, "RollingSkew")
        .def(py::init<int>(), py::arg("window_size"),
             "Initialize the rolling skew indicator.\n\n"
             ":param window_size: The window_size parameter.")
        .def("__call__", &screamer::RollingSkew::operator(), py::arg("value"),
             "This applies a rolling skew to the stream of input values `X`.")
        .def("reset", &screamer::RollingSkew::reset, "Reset the indicator to its initial state.")
        .def("transform", &screamer::RollingSkew::transform, py::arg("input_array"),
             "Apply the indicator to a NumPy array.")
        .doc() = "Rolling skew, the skew of the most recent values in a sequence.";

    py::class_<screamer::RollingKurt>(m, "RollingKurt")
        .def(py::init<int>(), py::arg("window_size"),
             "Initialize the rolling kurtosis indicator.\n\n"
             ":param window_size: The window_size parameter.")
        .def("__call__", &screamer::RollingKurt::operator(), py::arg("value"),
             "This applies a rolling kurtosis to the stream of input values `X`.")
        .def("reset", &screamer::RollingKurt::reset, "Reset the indicator to its initial state.")
        .def("transform", &screamer::RollingKurt::transform, py::arg("input_array"),
             "Apply the indicator to a NumPy array.")
        .doc() = "Rolling kurtosis, the kurtosis of the most recent values in a sequence.";

    py::class_<screamer::RollingZscore>(m, "RollingZscore")
        .def(py::init<int>(), py::arg("window_size"),
             "Initialize the rolling zscore indicator.\n\n"
             ":param window_size: The window_size parameter.")
        .def("__call__", &screamer::RollingZscore::operator(), py::arg("value"),
             "This applies a rolling zscore the stream of input values `X`.")
        .def("reset", &screamer::RollingZscore::reset, "Reset the indicator to its initial state.")
        .def("transform", &screamer::RollingZscore::transform, py::arg("input_array"),
             "Apply the indicator to a NumPy array.")
        .doc() = "Rolling zscore, the zscore of the most recent values in a sequence.";

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
             "Apply the simple return transformation to a NumPy array.")
        .doc() = "Simple return, the percentage change compared to a previous value";

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
             "Apply the log return transformation to a NumPy array.")
        .doc() = "Log return, the logarithmic change compared to a previous value.";

    py::class_<screamer::EWMA>(m, "EWMA")
        .def(py::init<int>(), py::arg("weight"),
             "Initialize the exponentially weighted moving average indicator.\n\n"
             ":param weight: Weight of new values.")
        .def("__call__", &screamer::EWMA::operator(), py::arg("value"),
             "Update and return the simple moving average.\n\n"
             ".. math::\n"
             "    f(X[i]) = w * X[i] + (1 - w) * X[i-1]\n\n"
             "This applies an exponentially weighted moving average to the stream of input values `X`.")
        .def("reset", &screamer::EWMA::reset, "Reset the exponentially weighted moving average indicator to its initial state.")
        .def("transform", &screamer::EWMA::transform, py::arg("input_array"),
             "Apply the exponentially weighted moving average transformation to a NumPy array.")
        .doc() = "Exponentially weighted moving average ofvalues in a sequence.";

    py::class_<screamer::EwStd>(m, "EwStd")
        .def(py::init<int>(), py::arg("weight"),
             "Initialize the exponentially weighted standard deviation indicator.\n\n"
             ":param weight: Weight of new values.")
        .def("__call__", &screamer::EwStd::operator(), py::arg("value"),
             "Update and return exponentially weighted standard deviation indicator.")
        .def("reset", &screamer::EwStd::reset, 
        "Reset the exponentially weighted moving standard deviation indicator to its initial state.")
        .def("transform", &screamer::EwStd::transform, py::arg("input_array"),
             "Apply the exponentially weighted standard deviation transformation to a NumPy array.")
        .doc() = "Exponentially weighted standard deviation of values in a sequence.";

    py::class_<screamer::EwVar>(m, "EwVar")
        .def(py::init<int>(), py::arg("weight"),
             "Initialize the exponentially weighted variance indicator.\n\n"
             ":param weight: Weight of new values.")
        .def("__call__", &screamer::EwVar::operator(), py::arg("value"),
             "Update and return exponentially weighted variance indicator.")
        .def("reset", &screamer::EwVar::reset, 
        "Reset the exponentially weighted moving standard deviation indicator to its initial state.")
        .def("transform", &screamer::EwVar::transform, py::arg("input_array"),
             "Apply the exponentially weighted variance transformation to a NumPy array.")
        .doc() = "Exponentially weighted variance of values in a sequence.";


}