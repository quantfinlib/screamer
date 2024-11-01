#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h> // Required for std::optional support

#include "screamer/base.h"

#include "screamer/ffill.h"
#include "screamer/fillna.h"
#include "screamer/clip.h"

#include "screamer/lag.h"
#include "screamer/diff.h"
#include "screamer/return.h"
#include "screamer/log_return.h"

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
#include "screamer/rolling_quantile_ost_v3.h"
#include "screamer/rolling_rms.h"

#include "screamer/ew_mean.h"
#include "screamer/ew_var.h"
#include "screamer/ew_std.h"
#include "screamer/ew_zscore.h"
#include "screamer/ew_skew.h"
#include "screamer/ew_kurt.h"
#include "screamer/ew_rms.h"

#include "screamer/transform.h"

#include "screamer/butter2.h"
#include "screamer/rolling_poly1.h"
#include "screamer/rolling_poly2.h"

#include "screamer/rolling_sigma_clip.h"

template <typename T> T signum(T val) {
    return (T(0) < val) - (val < T(0));
}

namespace py = pybind11;

PYBIND11_MODULE(screamer_bindings, m) {

    py::class_<screamer::ScreamerBase>(m, "_ScreamerBase");



     py::class_<screamer::Transform<(double (*)(double)) std::abs>>(m, "Abs")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) std::abs>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) std::abs>::reset, "Reset to the initial state.");


     py::class_<screamer::Transform<(double (*)(double)) std::log>>(m, "Log")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) std::log>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) std::log>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) std::exp>>(m, "Exp")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) std::exp>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) std::exp>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) std::sqrt>>(m, "Sqrt")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) std::sqrt>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) std::sqrt>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) std::erf>>(m, "Erf")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) std::erf>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) std::erf>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) std::erfc>>(m, "Erfc")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) std::erfc>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) std::erfc>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double))signum<double> >>(m, "Sign")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) signum<double>>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) signum<double>>::reset, "Reset to the initial state.");





    py::class_<screamer::ScreamerBase::LazyIterator>(m, "_LazyIterator")
        .def("__iter__", &screamer::ScreamerBase::LazyIterator::__iter__, py::return_value_policy::reference_internal)
        .def("__next__", &screamer::ScreamerBase::LazyIterator::__next__);

    py::class_<screamer::RollingMean, screamer::ScreamerBase>(m, "RollingMean")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingMean::operator(), py::arg("value"))
        .def("reset", &screamer::RollingMean::reset, "Reset to the initial state.");

    py::class_<screamer::RollingRms, screamer::ScreamerBase>(m, "RollingRms")
        .def(py::init<int>(), py::arg("window_size"))
        .def("__call__", &screamer::RollingRms::operator(), py::arg("value"))
        .def("reset", &screamer::RollingRms::reset, "Reset to the initial state.");

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

     py::class_<screamer::EwMean>(m, "EwMean")
        .def(
          py::init<
               std::optional<double>,
               std::optional<double>,
               std::optional<double>,
               std::optional<double>
          >(),
          py::arg("com") = std::nullopt,
          py::arg("span") = std::nullopt,
          py::arg("halflife") = std::nullopt,
          py::arg("alpha") = std::nullopt
        )
        .def("__call__", &screamer::EwMean::operator(), py::arg("value"))
        .def("reset", &screamer::EwMean::reset, "Reset to the initial state.");

     py::class_<screamer::EwVar>(m, "EwVar")
        .def(
          py::init<
               std::optional<double>,
               std::optional<double>,
               std::optional<double>,
               std::optional<double>
          >(),
          py::arg("com") = std::nullopt,
          py::arg("span") = std::nullopt,
          py::arg("halflife") = std::nullopt,
          py::arg("alpha") = std::nullopt
        )
        .def("__call__", &screamer::EwVar::operator(), py::arg("value"))
        .def("reset", &screamer::EwVar::reset, "Reset to the initial state.");

     
     py::class_<screamer::EwStd>(m, "EwStd")
        .def(
          py::init<
               std::optional<double>,
               std::optional<double>,
               std::optional<double>,
               std::optional<double>
          >(),
          py::arg("com") = std::nullopt,
          py::arg("span") = std::nullopt,
          py::arg("halflife") = std::nullopt,
          py::arg("alpha") = std::nullopt
        )
        .def("__call__", &screamer::EwStd::operator(), py::arg("value"))
        .def("reset", &screamer::EwStd::reset, "Reset to the initial state.");

     
     py::class_<screamer::EwZscore>(m, "EwZscore")
        .def(
          py::init<
               std::optional<double>,
               std::optional<double>,
               std::optional<double>,
               std::optional<double>
          >(),
          py::arg("com") = std::nullopt,
          py::arg("span") = std::nullopt,
          py::arg("halflife") = std::nullopt,
          py::arg("alpha") = std::nullopt
        )
        .def("__call__", &screamer::EwZscore::operator(), py::arg("value"))
        .def("reset", &screamer::EwZscore::reset, "Reset to the initial state.");


     py::class_<screamer::EwSkew>(m, "EwSkew")
        .def(
          py::init<
               std::optional<double>,
               std::optional<double>,
               std::optional<double>,
               std::optional<double>
          >(),
          py::arg("com") = std::nullopt,
          py::arg("span") = std::nullopt,
          py::arg("halflife") = std::nullopt,
          py::arg("alpha") = std::nullopt
        )
        .def("__call__", &screamer::EwSkew::operator(), py::arg("value"))
        .def("reset", &screamer::EwSkew::reset, "Reset to the initial state.");


     py::class_<screamer::EwKurt>(m, "EwKurt")
        .def(
          py::init<
               std::optional<double>,
               std::optional<double>,
               std::optional<double>,
               std::optional<double>
          >(),
          py::arg("com") = std::nullopt,
          py::arg("span") = std::nullopt,
          py::arg("halflife") = std::nullopt,
          py::arg("alpha") = std::nullopt
        )
        .def("__call__", &screamer::EwKurt::operator(), py::arg("value"))
        .def("reset", &screamer::EwKurt::reset, "Reset to the initial state.");


     py::class_<screamer::EwRms>(m, "EwRms")
        .def(
          py::init<
               std::optional<double>,
               std::optional<double>,
               std::optional<double>,
               std::optional<double>
          >(),
          py::arg("com") = std::nullopt,
          py::arg("span") = std::nullopt,
          py::arg("halflife") = std::nullopt,
          py::arg("alpha") = std::nullopt
        )
        .def("__call__", &screamer::EwRms::operator(), py::arg("value"))
        .def("reset", &screamer::EwRms::reset, "Reset to the initial state.");



     py::class_<screamer::Clip>(m, "Clip")
        .def(
          py::init<
               std::optional<double>,
               std::optional<double>
          >(),
          py::arg("lower") = std::nullopt,
          py::arg("upper") = std::nullopt
        )
        .def("__call__", &screamer::Clip::operator(), py::arg("value"))
        .def("reset", &screamer::Clip::reset, "Reset to the initial state.");


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
        .def("__call__", &screamer::Clip::operator(), py::arg("value"))
        .def("reset", &screamer::Clip::reset, "Reset to the initial state.");


    py::class_<screamer::Butter2, screamer::ScreamerBase>(m, "Butter2")
        .def(py::init<double>(), py::arg("cutoff_freq"))
        .def("__call__", &screamer::Butter2::operator(), py::arg("value"))
        .def("reset", &screamer::Butter2::reset, "Reset to the initial state.");

    py::class_<screamer::RollingPoly1>(m, "RollingPoly1")
        .def(
            py::init<int, int>(),
            py::arg("window_size"),
            py::arg("derivative_order") = 0
        )
        .def("__call__", &screamer::RollingPoly1::operator(), py::arg("value"))
        .def("reset", &screamer::RollingPoly1::reset, "Reset to the initial state.");


    py::class_<screamer::RollingPoly2>(m, "RollingPoly2")
        .def(
            py::init<int, int>(),
            py::arg("window_size"),
            py::arg("derivative_order") = 0
        )
        .def("__call__", &screamer::RollingPoly2::operator(), py::arg("value"))
        .def("reset", &screamer::RollingPoly2::reset, "Reset to the initial state.");


}