#include <pybind11/pybind11.h>
#include "screamer/common/base.h"
#include "screamer/common/transform.h"
#include "screamer/transform_functions.h"
#include "screamer/linear.h"
#include "screamer/power.h"

namespace py = pybind11;

void init_bindings_math(py::module& m) {

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

     py::class_<screamer::Transform<(double (*)(double))screamer::signum<double> >>(m, "Sign")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) screamer::signum<double>>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) screamer::signum<double>>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) std::tanh>>(m, "Tanh")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) std::tanh>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) std::tanh>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) screamer::relu>>(m, "Relu")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) screamer::relu>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) screamer::relu>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) screamer::selu>>(m, "Selu")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) screamer::selu>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) screamer::selu>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) screamer::elu>>(m, "Elu")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) screamer::elu>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) screamer::elu>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) screamer::softsign>>(m, "Softsign")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) screamer::softsign>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) screamer::softsign>::reset, "Reset to the initial state.");

     py::class_<screamer::Transform<(double (*)(double)) screamer::sigmoid>>(m, "Sigmoid")
        .def(py::init<>())
        .def("__call__", &screamer::Transform<(double (*)(double)) screamer::sigmoid>::operator(), py::arg("value"))
        .def("reset", &screamer::Transform<(double (*)(double)) screamer::sigmoid>::reset, "Reset to the initial state.");

     py::class_<screamer::Linear, screamer::ScreamerBase>(m, "Linear")
        .def(py::init<double, double>(), py::arg("scale"), py::arg("shift"))
        .def("__call__", &screamer::Linear::operator(), py::arg("value"))
        .def("reset", &screamer::Linear::reset, "Reset to the initial state.");

     py::class_<screamer::Power, screamer::ScreamerBase>(m, "Power")
        .def(py::init<double>(), py::arg("p"))
        .def("__call__", &screamer::Power::operator(), py::arg("value"))
        .def("reset", &screamer::Power::reset, "Reset to the initial state.");


}