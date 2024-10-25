#ifndef SCREAMER_TRANSFORMS_SUM_H
#define SCREAMER_TRANSFORMS_SUM_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/transforms.h"
#include <vector>
#include <limits>

namespace py = pybind11;


// Optimized version for 1D contiguous arrays
py::array_t<double> transform_1d_contiguous_sum(int window_size, py::array_t<double> input_array) {
    auto [result, input_data, result_data, size] = transform_1d_contiguous_setup(input_array);

    // first elements
    double sum = 0.0;
    for (size_t i = 0; i < window_size; ++i) {
        sum += input_data[i];
        result_data[i] = sum;
    }

    // 2nd sweep: remaining
    for (size_t i = window_size; i < size; ++i) {
        result_data[i] = result_data[i-1] + input_data[i] - input_data[i - window_size];
    } 

    return result;
}

py::array_t<double> transform_sum(int window_size, py::array_t<double> input_array) {
    // Check if the array is 1D and contiguous
    py::buffer_info buf_info = input_array.request();
    if (buf_info.ndim == 1 && buf_info.strides[0] == sizeof(double)) {
        // Call the optimized version for 1D contiguous arrays
        return transform_1d_contiguous_sum(window_size, input_array);
    }

    // Call setup function to initialize variables
    auto [result, input_data, result_data, first_dim, rest_size, col_input_offsets, col_result_offsets, input_strides, result_strides] 
        = transform_1_multidim_setup(input_array);

    // Apply the function to each "slice" along the first axis
    for (size_t col = 0; col < rest_size; ++col) {

        // Initialize indices with precomputed offsets
        size_t i0, i1, r1;
        double sum;

        // First rows need to be filled with cumsum
        r1 = col_result_offsets[col];
        i1 = col_input_offsets[col];
        sum = 0.0;      
        for (size_t row = 0; row < window_size; ++row) {
            sum += input_data[i1];
            result_data[r1] = sum;

            i1 += input_strides[0];
            r1 += result_strides[0];
        }

        // Consecutive rows need to be windowed

        // Initialize indices with precomputed offsets
        i0 = col_input_offsets[col];
        i1 = col_input_offsets[col] + window_size * input_strides[0];
        r1 = col_result_offsets[col] + window_size * result_strides[0];
        sum = result_data[r1 - result_strides[0]];
        for (size_t row = window_size; row < first_dim; ++row) {
            sum = sum + input_data[i1] - input_data[i0];
            result_data[r1] = sum;

            // Increment indices by the stride
            i0 += input_strides[0];
            i1 += input_strides[0];
            r1 += result_strides[0];
        }
    }
    return result;
}

#endif
