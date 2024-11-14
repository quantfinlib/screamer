#ifndef SCREAMER_BASE_H
#define SCREAMER_BASE_H

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <vector>
#include <iostream>
#include <sstream>
#include "screamer/common/cast_double.h"
#include "screamer/common/async_generator.h"

namespace py = pybind11;

namespace screamer {

class ScreamerBase {
public:
    virtual ~ScreamerBase() = default;

    virtual void reset() {}

    py::object operator()(py::object obj);

    virtual double process_scalar(double value) = 0;

    virtual void process_array_no_stride(double* result_data, const double* input_data, size_t size);

    virtual void process_array_stride(
        double* result_data, 
        size_t result_stride,
        const double* input_data, 
        size_t input_stride,
        size_t size
    );

protected:
    py::array_t<double> process_python_array(py::array_t<double> input_array);
};

} // namespace screamer

#endif // SCREAMER_BASE_H
