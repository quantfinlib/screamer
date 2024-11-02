#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Required for std::optional support
#include "screamer/common/base.h"
#include "screamer/ffill.h"
#include "screamer/fillna.h"
#include "screamer/clip.h"

namespace py = pybind11;

void init_bindings_preprocessing(py::module& m) {

    py::class_<screamer::Ffill, screamer::ScreamerBase>(m, "Ffill")
        .def(py::init<>())
        .def("__call__", &screamer::Ffill::operator(), py::arg("value"))
        .def("reset", &screamer::Ffill::reset, "Reset to the initial state.");

    py::class_<screamer::FillNa, screamer::ScreamerBase>(m, "FillNa")
        .def(py::init<double>(), py::arg("fill"))
        .def("__call__", &screamer::FillNa::operator(), py::arg("value"))
        .def("reset", &screamer::FillNa::reset, "Reset to the initial state.");


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

}
