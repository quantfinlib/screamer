// #define PYBIND11_DETAILED_ERROR_MESSAGES

#include <pybind11/pybind11.h>

namespace py = pybind11;

// Function declarations of submodules
void init_bindings_core(py::module& m);
void init_bindings_math(py::module& m);
void init_bindings_rolling(py::module& m);
void init_bindings_ew(py::module& m);
void init_bindings_preprocessing(py::module& m);
void init_bindings_signal(py::module& m);
void init_bindings_fin(py::module& m);
void init_bindings_misc(py::module& m);

PYBIND11_MODULE(screamer_bindings, m) {
    init_bindings_core(m);
    init_bindings_math(m);
    init_bindings_rolling(m);
    init_bindings_ew(m);
    init_bindings_preprocessing(m);
    init_bindings_signal(m);
    init_bindings_fin(m);
    init_bindings_misc(m);
}