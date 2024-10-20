#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

// Define a template function to accept any callable object with reset() and operator()
template <typename Func>
py::array_t<double> transform_1(Func&& func, py::array_t<double> input_array) {
    // Request a buffer (to manipulate the NumPy array)
    py::buffer_info buf_info = input_array.request();

    // Ensure that the input array has at least one dimension
    if (buf_info.ndim < 1 || buf_info.itemsize != sizeof(double)) {
        throw std::runtime_error("Input array must have at least one dimension and contain doubles");
    }
    
    size_t rows = buf_info.shape[0];  // The size along the first dimension
    size_t rest_size = 1;             // Total size of the rest of the dimensions (2nd axis onward)
    for (int i = 1; i < buf_info.ndim; ++i) {
        rest_size *= buf_info.shape[i];
    }

    // Cast input data to double*
    double* input_data = static_cast<double*>(buf_info.ptr);
    
    // Create an output array of the same shape
    py::array_t<double> result(buf_info.shape);
    py::buffer_info result_buf = result.request();
    double* result_data = static_cast<double*>(result_buf.ptr);

    // Apply the functor or callable to each column (defined by slicing along the first axis)
    for (size_t col = 0; col < rest_size; ++col) {
        func.reset();  // Call reset() before processing each column

        // Process each element along the first dimension for the current column
        for (size_t row = 0; row < rows; ++row) {
            size_t index = row * rest_size + col;  // Calculate the flat index in the data
            result_data[index] = func(input_data[index]);  // Apply the functor to each element
        }
    }
    
    return result;
}
