#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"
#include "screamer/rolling_sum.h"
#include "screamer/rolling_mean.h"
#include "screamer/rolling_var.h"
#include "screamer/rolling_std.h"
#include "screamer/rolling_skew.h"
#include "screamer/rolling_kurt.h"
#include "screamer/rolling_zscore.h"
#include "screamer/rolling_min.h"
#include "screamer/rolling_max.h"
#include "screamer/rolling_median.h"
#include "screamer/rolling_quantile.h"
#include "screamer/rolling_rms.h"
#include "screamer/rolling_poly1.h"
#include "screamer/rolling_poly2.h"
#include "screamer/rolling_sigma_clip.h"
#include "screamer/rolling_ou.h"
#include "screamer/rolling_rsi.h"

namespace py = pybind11;

void init_bindings_rolling(py::module& m) {

    py::class_<screamer::RollingMean, screamer::ScreamerBase>(m, "RollingMean")
        .def(py::init<int, const std::string&>(), py::arg("window_size"), py::arg("start_policy") = "strict")
        .def("__call__", &screamer::RollingMean::operator(), py::arg("value"))
        .def("reset", &screamer::RollingMean::reset, "Reset to the initial state.");

    py::class_<screamer::RollingRms, screamer::ScreamerBase>(m, "RollingRms")
        .def(py::init<int, const std::string&>(),
            py::arg("window_size"),
            py::arg("start_policy") = "strict")
        .def("__call__", &screamer::RollingRms::operator(), py::arg("value"))
        .def("reset", &screamer::RollingRms::reset, "Reset to the initial state.");

    py::class_<screamer::RollingSum, screamer::ScreamerBase>(m, "RollingSum")
        .def(py::init<int, const std::string&>(), py::arg("window_size"), py::arg("start_policy") = "strict")
        .def("__call__", &screamer::RollingSum::operator(), py::arg("value"))
        .def("reset", &screamer::RollingSum::reset, "Reset to the initial state.");

    py::class_<screamer::RollingStd, screamer::ScreamerBase>(m, "RollingStd")
        .def(py::init<int, const std::string&>(),
            py::arg("window_size"),
            py::arg("start_policy") = "strict")
        .def("__call__", &screamer::RollingStd::operator(), py::arg("value"))
        .def("reset", &screamer::RollingStd::reset, "Reset to the initial state.");

    py::class_<screamer::RollingVar, screamer::ScreamerBase>(m, "RollingVar")
        .def(py::init<int, const std::string&>(),
            py::arg("window_size"),
            py::arg("start_policy") = "strict")
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
        .def(py::init<int, const std::string&>(),
            py::arg("window_size"),
            py::arg("start_policy") = "strict")
        .def("__call__", &screamer::RollingZscore::operator(), py::arg("value"))
        .def("reset", &screamer::RollingZscore::reset, "Reset to the initial state.");

    py::class_<screamer::RollingPoly1>(m, "RollingPoly1")
        .def(py::init<int, int, const std::string&>(),
            py::arg("window_size"),
            py::arg("derivative_order") = 0,
            py::arg("start_policy") = "strict")
        .def("__call__", &screamer::RollingPoly1::operator(), py::arg("value"))
        .def("reset", &screamer::RollingPoly1::reset, "Reset to the initial state.");


    py::class_<screamer::RollingPoly2>(m, "RollingPoly2")
        .def(py::init<int, int, const std::string&>(),
            py::arg("window_size"),
            py::arg("derivative_order") = 0,
            py::arg("start_policy") = "strict")
        .def("__call__", &screamer::RollingPoly2::operator(), py::arg("value"))
        .def("reset", &screamer::RollingPoly2::reset, "Reset to the initial state.");


     py::class_<screamer::RollingSigmaClip>(m, "RollingSigmaClip")
        .def(
          py::init<
            int,
            std::optional<double>,
            std::optional<double>,
            std::optional<int>
          >(),
          py::arg("window_size"),
          py::arg("lower") = std::nullopt,
          py::arg("upper") = std::nullopt,
          py::arg("output") = std::nullopt
        )
        .def("__call__", &screamer::RollingSigmaClip::operator(), py::arg("value"))
        .def("reset", &screamer::RollingSigmaClip::reset, "Reset to the initial state.");


     py::class_<screamer::RollingOU>(m, "RollingOU")
        .def(
          py::init<int, std::optional<int>>(),
          py::arg("window_size"),
          py::arg("output") = std::nullopt
        )
        .def("__call__", &screamer::RollingOU::operator(), py::arg("value"))
        .def("reset", &screamer::RollingOU::reset, "Reset to the initial state.");

    py::class_<screamer::RollingRSI, screamer::ScreamerBase>(m, "RollingRSI")
        .def(py::init<int, const std::string&>(),
            py::arg("window_size"),
            py::arg("start_policy") = "strict")
        .def("__call__", &screamer::RollingRSI::operator(), py::arg("value"))
        .def("reset", &screamer::RollingRSI::reset, "Reset to the initial state.");


}
