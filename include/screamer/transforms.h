#ifndef SCREAMER_TRANSFORMS_H
#define SCREAMER_TRANSFORMS_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <vector>

namespace py = pybind11;


// Optimized version for 1D contiguous arrays
template <typename Func>
py::array_t<double> transform_1d_contiguous(Func&& func, py::array_t<double> input_array) {
    // Request a buffer
    py::buffer_info buf_info = input_array.request();

    size_t size = buf_info.shape[0];  // The size of the 1D array

    // Cast input data to double*
    double* input_data = static_cast<double*>(buf_info.ptr);
    
    // Create an output array of the same shape
    py::array_t<double> result(buf_info.shape);
    py::buffer_info result_buf = result.request();
    double* result_data = static_cast<double*>(result_buf.ptr);

    // Reset the function before processing
    func.reset();

    // Process the entire array in a single loop (no need for complex indexing)
    for (size_t i = 0; i < size; ++i) {
        result_data[i] = func(input_data[i]);
    }

    return result;
}

// Define a template function to accept any callable object with reset() and operator()
template <typename Func>
py::array_t<double> transform_1(Func&& func, py::array_t<double> input_array) {
    // Request a buffer (to manipulate the NumPy array)
    py::buffer_info buf_info = input_array.request();

    // Ensure that the input array has at least one dimension
    if (buf_info.ndim < 1 || buf_info.itemsize != sizeof(double)) {
        throw std::runtime_error("Input array must have at least one dimension and contain doubles");
    }

    // Check if the array is 1D and contiguous
    if (buf_info.ndim == 1 && buf_info.strides[0] == sizeof(double)) {
        // Call the optimized version for 1D contiguous arrays
        return transform_1d_contiguous(func, input_array);
    }

    // General case for multi-dimensional arrays
    size_t first_dim = buf_info.shape[0];  // The size along the first dimension (e.g., rows)
    size_t rest_size = 1;                  // Total size of the rest of the dimensions
    for (int i = 1; i < buf_info.ndim; ++i) {
        rest_size *= buf_info.shape[i];
    }

    // Cast input data to double*
    double* input_data = static_cast<double*>(buf_info.ptr);
    
    // Create an output array of the same shape
    py::array_t<double> result(buf_info.shape);
    py::buffer_info result_buf = result.request();
    double* result_data = static_cast<double*>(result_buf.ptr);

    // Vector to store the stride steps for each dimension in terms of the number of elements
    std::vector<size_t> input_strides(buf_info.ndim);
    std::vector<size_t> result_strides(buf_info.ndim);
    
    for (int i = 0; i < buf_info.ndim; ++i) {
        input_strides[i] = buf_info.strides[i] / sizeof(double);
        result_strides[i] = result_buf.strides[i] / sizeof(double);
    }

    // Apply the function to each "slice" along the first axis
    for (size_t col = 0; col < rest_size; ++col) {
        func.reset();  // Reset the function for each slice

        for (size_t row = 0; row < first_dim; ++row) {
            // Calculate the multi-dimensional index
            size_t input_index = row * input_strides[0];
            size_t result_index = row * result_strides[0];
            size_t temp_col = col;

            // For each remaining dimension, adjust the input and result indices by considering strides
            for (int dim = buf_info.ndim - 1; dim > 0; --dim) {
                size_t index_in_dim = temp_col % buf_info.shape[dim];
                input_index += index_in_dim * input_strides[dim];
                result_index += index_in_dim * result_strides[dim];
                temp_col /= buf_info.shape[dim];
            }

            // Apply the function to the correct element in the input array
            result_data[result_index] = func(input_data[input_index]);
        }
    }

    return result;
}

#endif
