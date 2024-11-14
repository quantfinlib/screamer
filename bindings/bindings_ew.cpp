#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"
#include "screamer/ew_mean.h"
#include "screamer/ew_var.h"
#include "screamer/ew_std.h"
#include "screamer/ew_zscore.h"
#include "screamer/ew_skew.h"
#include "screamer/ew_kurt.h"
#include "screamer/ew_rms.h"

namespace py = pybind11;

void init_bindings_ew(py::module& m) {

     py::class_<screamer::EwMean, screamer::ScreamerBase>(m, "EwMean")
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

     py::class_<screamer::EwVar, screamer::ScreamerBase>(m, "EwVar")
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

     
     py::class_<screamer::EwStd, screamer::ScreamerBase>(m, "EwStd")
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

     
     py::class_<screamer::EwZscore, screamer::ScreamerBase>(m, "EwZscore")
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


     py::class_<screamer::EwSkew, screamer::ScreamerBase>(m, "EwSkew")
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


     py::class_<screamer::EwKurt, screamer::ScreamerBase>(m, "EwKurt")
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


     py::class_<screamer::EwRms, screamer::ScreamerBase>(m, "EwRms")
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

}
