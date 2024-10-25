#ifndef SCREAMER_TRANSFORMS_LAG_H
#define SCREAMER_TRANSFORMS_LAG_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "screamer/transforms.h"
#include <vector>
#include <limits>

namespace py = pybind11;


py::array_t<double> transform_1d_contiguous_lag(int lag, py::array_t<double> input_array) {

    auto [result, input_data, result_data, size] = transform_1d_contiguous_setup(input_array);

    std::fill_n(result_data, lag, std::numeric_limits<double>::quiet_NaN());

    size_t num_elements = size - lag;
    std::memcpy(result_data + lag, input_data, num_elements * sizeof(double));

    return result;
}


py::array_t<double> transform_lag(int lag, py::array_t<double> input_array) {
    // Check if the array is 1D and contiguous
    py::buffer_info buf_info = input_array.request();
    if (buf_info.ndim == 1 && buf_info.strides[0] == sizeof(double)) {
        // Call the optimized version for 1D contiguous arrays
        return transform_1d_contiguous_lag(lag, input_array);
    }

    // Call setup function to initialize variables
    auto [result, input_data, result_data, first_dim, rest_size, col_input_offsets, col_result_offsets, input_strides, result_strides] 
        = transform_1_multidim_setup(input_array);

    // Apply the function to each "slice" along the first axis
    for (size_t col = 0; col < rest_size; ++col) {

        // Initialize indices with precomputed offsets
        size_t input_index;
        size_t result_index;

        // First rows need to be filled with NaN
        result_index = col_result_offsets[col];
        
        for (size_t row = 0; row < lag; ++row) {
            result_data[result_index] = std::numeric_limits<double>::quiet_NaN();;
            result_index += result_strides[0];
        }

        // Initialize indices with precomputed offsets
        input_index = col_input_offsets[col];
        result_index = col_result_offsets[col] + lag * result_strides[0];

        for (size_t row = lag; row < first_dim; ++row) {
            result_data[result_index] = input_data[input_index];

            // Increment indices by the stride
            input_index += input_strides[0];
            result_index += result_strides[0];
        }
    }
    return result;
}


#endif
