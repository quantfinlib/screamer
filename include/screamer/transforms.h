#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

// Define a template function to accept any callable object
template <typename Func>
py::array_t<double> transform_1(Func&& func, py::array_t<double> input_array) {
    // Request a buffer (to manipulate the NumPy array)
    py::buffer_info buf_info = input_array.request();
    
    // Ensure that the input is a 1D array of doubles
    if (buf_info.ndim != 1 || buf_info.itemsize != sizeof(double)) {
        throw std::runtime_error("Input array must be a 1D array of doubles");
    }
    
    // Cast input data to double*
    double* input_data = static_cast<double*>(buf_info.ptr);
    size_t length = buf_info.size;
    
    // Create an output array
    py::array_t<double> result(length);
    py::buffer_info result_buf = result.request();
    double* result_data = static_cast<double*>(result_buf.ptr);
    
    // Apply the functor or callable to each element
    for (size_t i = 0; i < length; ++i) {
        result_data[i] = func(input_data[i]);  // Call the provided functor on each element
    }
    
    return result;
}
