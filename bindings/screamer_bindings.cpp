#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/lag.h"
#include "screamer/diff.h"
#include "screamer/rolling_sum.h"
#include "screamer/rolling_skew.h"
#include "screamer/rolling_kurt.h"
#include "screamer/rolling_zscore.h"
#include "screamer/rolling_min.h"
#include "screamer/rolling_max.h"
#include "screamer/rolling_median.h"
#include "screamer/return.h"
#include "screamer/log_return.h"
#include "screamer/ewma.h"
#include "screamer/ew_var.h"
#include "screamer/ew_std.h"

#include "screamer/base.h"
#include "screamer/rolling_mean.h"
#include "screamer/rolling_std.h"
#include "screamer/rolling_var.h"
#include "screamer/ffill.h"
#include "screamer/fillna.h"
#include "screamer/rolling_quantile.h"

namespace py = pybind11;

PYBIND11_MODULE(screamer_bindings, m) {

    py::class_<screamer::ScreamerBase>(m, "_ScreamerBase");

    py::class_<screamer::ScreamerBase::LazyIterator>(m, "_LazyIterator")
        .def("__iter__", &screamer::ScreamerBase::LazyIterator::__iter__, py::return_value_policy::reference_internal)
        .def("__next__", &screamer::ScreamerBase::LazyIterator::__next__);

    py::class_<screamer::RollingMean, screamer::ScreamerBase>(m, "RollingMean")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingMean::operator(), py::arg("value"))
        .def("reset", &screamer::RollingMean::reset, "Reset to the initial state.");

    py::class_<screamer::RollingSum, screamer::ScreamerBase>(m, "RollingSum")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingSum::operator(), py::arg("value"))
        .def("reset", &screamer::RollingSum::reset, "Reset to the initial state.");

    py::class_<screamer::Diff, screamer::ScreamerBase>(m, "Diff")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::Diff::operator(), py::arg("value"))
        .def("reset", &screamer::Diff::reset, "Reset to the initial state.");

    py::class_<screamer::Lag, screamer::ScreamerBase>(m, "Lag")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::Lag::operator(), py::arg("value"))
        .def("reset", &screamer::Lag::reset, "Reset to the initial state.");

    py::class_<screamer::RollingStd, screamer::ScreamerBase>(m, "RollingStd")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingStd::operator(), py::arg("value"))
        .def("reset", &screamer::RollingStd::reset, "Reset to the initial state.");

    py::class_<screamer::RollingVar, screamer::ScreamerBase>(m, "RollingVar")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingVar::operator(), py::arg("value"))
        .def("reset", &screamer::RollingVar::reset, "Reset to the initial state.");

    py::class_<screamer::RollingSkew, screamer::ScreamerBase>(m, "RollingSkew")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingSkew::operator(), py::arg("value"))
        .def("reset", &screamer::RollingSkew::reset, "Reset to the initial state.");

    py::class_<screamer::RollingKurt, screamer::ScreamerBase>(m, "RollingKurt")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingKurt::operator(), py::arg("value"))
        .def("reset", &screamer::RollingKurt::reset, "Reset to the initial state.");

    py::class_<screamer::Ffill, screamer::ScreamerBase>(m, "Ffill")
        .def(py::init<>())
        .def("__call__", &screamer::Ffill::operator(), py::arg("value"))
        .def("reset", &screamer::Ffill::reset, "Reset to the initial state.");

    py::class_<screamer::FillNa, screamer::ScreamerBase>(m, "FillNa")
        .def(py::init<double>(), py::arg("fill"))
        .def("__call__", &screamer::FillNa::operator(), py::arg("value"))
        .def("reset", &screamer::FillNa::reset, "Reset to the initial state.");

    py::class_<screamer::RollingMin, screamer::ScreamerBase>(m, "RollingMin")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingMin::operator(), py::arg("value"))
        .def("reset", &screamer::RollingMin::reset, "Reset to the initial state.");

    py::class_<screamer::RollingMax, screamer::ScreamerBase>(m, "RollingMax")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingMax::operator(), py::arg("value"))
        .def("reset", &screamer::RollingMax::reset, "Reset to the initial state.");

    py::class_<screamer::RollingMedian, screamer::ScreamerBase>(m, "RollingMedian")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingMedian::operator(), py::arg("value"))
        .def("reset", &screamer::RollingMedian::reset, "Reset to the initial state.");

    py::class_<screamer::RollingQuantile, screamer::ScreamerBase>(m, "RollingQuantile")
        .def(py::init<int, double>(), py::arg("window_size"), py::arg("quantile"))
        .def("__call__", &screamer::RollingQuantile::operator(), py::arg("value"))
        .def("reset", &screamer::RollingQuantile::reset, "Reset to the initial state.");

    py::class_<screamer::RollingZscore, screamer::ScreamerBase>(m, "RollingZscore")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingZscore::operator(), py::arg("value"))
        .def("reset", &screamer::RollingZscore::reset, "Reset to the initial state.");

    py::class_<screamer::Return, screamer::ScreamerBase>(m, "Return")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::Return::operator(), py::arg("value"))
        .def("reset", &screamer::Return::reset, "Reset to the initial state.");

    py::class_<screamer::LogReturn, screamer::ScreamerBase>(m, "LogReturn")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::LogReturn::operator(), py::arg("value"))
        .def("reset", &screamer::LogReturn::reset, "Reset to the initial state.");
/*
    py::class_<screamer::EWMA>(m, "EWMA")
        .def(py::init<double>(), py::arg("weight"),
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
        .def(py::init<double>(), py::arg("weight"),
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
        .def(py::init<double>(), py::arg("weight"),
             "Initialize the exponentially weighted variance indicator.\n\n"
             ":param weight: Weight of new values.")
        .def("__call__", &screamer::EwVar::operator(), py::arg("value"),
             "Update and return exponentially weighted variance indicator.")
        .def("reset", &screamer::EwVar::reset, 
        "Reset the exponentially weighted moving standard deviation indicator to its initial state.")
        .def("transform", &screamer::EwVar::transform, py::arg("input_array"),
             "Apply the exponentially weighted variance transformation to a NumPy array.")
        .doc() = "Exponentially weighted variance of values in a sequence.";

*/

}