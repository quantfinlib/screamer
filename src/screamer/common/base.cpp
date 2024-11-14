#include "screamer/common/base.h"
#include "screamer/common/async_generator.h"
#include "screamer/common/iterator.h"

namespace screamer {

py::object ScreamerBase::operator()(py::object obj) {
    if (can_cast_to_double(obj)) {
        double value = py::cast<double>(obj);
        return py::float_(process_scalar(value));
    }

    if (py::isinstance<py::array>(obj) || py::isinstance<py::list>(obj) || py::isinstance<py::tuple>(obj)) {
        py::array_t<double> double_array_t = py::cast<py::array_t<double>>(obj);
        int size = double_array_t.size();

        if (size > 1) {
            return process_python_array(double_array_t);
        } else {
            py::buffer_info buf_info = double_array_t.request();
            double* input_data_ptr = static_cast<double*>(buf_info.ptr);
            double value = input_data_ptr[0];
            return py::float_(process_scalar(value));
        }
    }

    if (py::isinstance<py::iterable>(obj)) {
        return py::cast(LazyIterator(obj.cast<py::iterable>(), *this));
    }

    if (is_async_generator(obj)) {
        std::cout << "we have an async_generator" << std::endl;
    }

    auto type_str = std::string(py::str(py::type::of(obj)));
    std::ostringstream oss;
    oss << "Unsupported input type for call: [" << type_str << "]";
    throw std::invalid_argument(oss.str());
}

void ScreamerBase::process_array_no_stride(double* result_data, const double* input_data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        result_data[i] = process_scalar(input_data[i]);
    }
}

void ScreamerBase::process_array_stride(
    double* result_data, 
    size_t result_stride,
    const double* input_data, 
    size_t input_stride,
    size_t size
) {
    size_t result_start = 0;
    size_t input_start = 0;

    for (size_t i = 0; i < size; i++) {
        result_data[result_start] = process_scalar(input_data[input_start]);
        result_start += result_stride;
        input_start += input_stride;
    }
}

py::array_t<double> ScreamerBase::process_python_array(py::array_t<double> input_array) {
    py::buffer_info buf_info = input_array.request();

    if (buf_info.ndim < 1 || buf_info.itemsize != sizeof(double)) {
        throw std::runtime_error("Input array must have at least one dimension and contain doubles");
    }

    double* input_data = static_cast<double*>(buf_info.ptr);
    py::array_t<double> result(buf_info.shape);
    py::buffer_info result_buf = result.request();
    double* result_data = static_cast<double*>(result_buf.ptr);
    size_t size = buf_info.shape[0];

    if (size == 0) {
        return result;
    }

    if (buf_info.ndim == 1 && buf_info.strides[0] == sizeof(double)) {
        reset();
        process_array_no_stride(result_data, input_data, size);
        reset();
        return result;
    }

    size_t rest_size = 1;  
    for (int i = 1; i < buf_info.ndim; ++i) {
        rest_size *= buf_info.shape[i];
    }

    std::vector<size_t> input_strides(buf_info.ndim);
    std::vector<size_t> result_strides(buf_info.ndim);

    for (int i = 0; i < buf_info.ndim; ++i) {
        input_strides[i] = buf_info.strides[i] / sizeof(double);
        result_strides[i] = result_buf.strides[i] / sizeof(double);
    }

    std::vector<size_t> col_input_offsets(rest_size);
    std::vector<size_t> col_result_offsets(rest_size);

    for (size_t col = 0; col < rest_size; ++col) {
        size_t temp_col = col;
        size_t col_input_offset = 0;
        size_t col_result_offset = 0;

        for (int dim = buf_info.ndim - 1; dim > 0; --dim) {
            size_t index_in_dim = temp_col % buf_info.shape[dim];
            col_input_offset += index_in_dim * input_strides[dim];
            col_result_offset += index_in_dim * result_strides[dim];
            temp_col /= buf_info.shape[dim];
        }

        col_input_offsets[col] = col_input_offset;
        col_result_offsets[col] = col_result_offset;
    }

    for (size_t col = 0; col < rest_size; ++col) {
        size_t input_index = col_input_offsets[col];
        size_t result_index = col_result_offsets[col];

        reset();

        process_array_stride(
            &result_data[result_index], 
            result_strides[0],
            &input_data[input_index], 
            input_strides[0],
            size
        );
    }

    reset();
    return result;
}

} // namespace screamer
