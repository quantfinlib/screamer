#ifndef SCREAMER_PYTHON_TOOLS_H
#define SCREAMER_PYTHON_TOOLS_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <tuple>
#include <optional>
#include <vector>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

// Static array of numerical dtypes, initialized once
static const std::array<py::dtype, 10> numerical_dtypes = {
    py::dtype::of<float>(),
    py::dtype::of<double>(),
    py::dtype::of<std::int8_t>(),
    py::dtype::of<std::int16_t>(),
    py::dtype::of<std::int32_t>(),
    py::dtype::of<std::int64_t>(),
    py::dtype::of<std::uint8_t>(),
    py::dtype::of<std::uint16_t>(),
    py::dtype::of<std::uint32_t>(),
    py::dtype::of<std::uint64_t>()
};

inline bool is_numpy_numerical_scalar(const py::object& obj) {
    // Ensure the object is not a NumPy array
    if (py::isinstance<py::array>(obj)) {
        return false; // Exclude arrays
    }

    // Check if the object has a 'dtype' attribute
    if (py::hasattr(obj, "dtype")) {
        // Retrieve the dtype of the object
        py::dtype dtype = py::cast<py::dtype>(obj.attr("dtype"));

        // Check if the object's dtype matches any of the numerical dtypes
        for (const auto& num_dtype : numerical_dtypes) {
            if (dtype.equal(num_dtype)) {
                return true;
            }
        }
    }
    return false;
}

inline bool is_numpy_numerical_scalar_v2(const py::object& obj) {
    // numpy primitive types
    auto type_str = std::string(py::str(obj.get_type()));
    if (type_str == "<class \'numpy.uint32\'>" ||
        type_str == "<class \'numpy.uint64\'>" ||
        type_str == "<class \'numpy.int32\'>" ||
        type_str == "<class \'numpy.int64\'>" ||
        type_str == "<class \'numpy.float32\'>" ||
        type_str == "<class \'numpy.float64\'>") {
        return true;
    }
    return false;
}

inline bool is_numpy_numerical_scalar_v3(const py::object& obj) {
    const std::string numpy_prefix = "<class 'numpy.";
    const size_t prefix_length = numpy_prefix.size();

    auto type_str = std::string(py::str(py::type::of(obj)));

    // Check if the string starts with the common prefix
    if (type_str.compare(0, prefix_length, numpy_prefix) != 0) {
        return false; // If the prefix doesn't match, it's not a NumPy type
    }

    // Compare the remainder of the string with the specific types
    const std::string suffix = type_str.substr(prefix_length);
    if (suffix == "uint32'>" ||
        suffix == "uint64'>" ||
        suffix == "int32'>" ||
        suffix == "int64'>" ||
        suffix == "float32'>" ||
        suffix == "float64'>") {
            return true;
    }

    return false; // Not a numerical scalar
}


// Check if a py::object can be cast to a double
inline bool can_cast_to_double(const py::object& obj) {
    
    // Native Python types
    if (py::isinstance<py::float_>(obj) || 
        py::isinstance<py::int_>(obj) || 
        py::isinstance<py::bool_>(obj)) {
        return true;
    }

    if (is_numpy_numerical_scalar_v3(obj)) {
        return true;
    }

    return false;
}


} // namespace screamer
#endif // include guards