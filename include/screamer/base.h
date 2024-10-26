#ifndef SCREAMER_BASE_H
#define SCREAMER_BASE_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <iostream>

namespace py = pybind11;

namespace screamer {

    class ScreamerBase {
    public:

        virtual ~ScreamerBase() = default;

        // virtual function with empty default implementation to reset state
        virtual void reset() {};

        py::object operator()(py::object input) {
            try {

                // Handle scalar types like float, double, int
                double value = input.cast<double>();
                return py::float_(process_scalar(value));

            } catch (const py::cast_error&) {

                // Handle numpy array types
                if (py::isinstance<py::array>(input)) {
                    py::array array = input.cast<py::array>();
                    ssize_t size = array.size();

                    if (size == 1) {
                        double value = py::cast<double>(array[0]);
                        return py::float_(process_scalar(value));
                    } else if (size > 1) {
                        py::array_t<double> double_array = py::cast<py::array_t<double>>(array);
                        return process_python_array(double_array);
                    }
                }          

                // Handle Python list types      
                if (py::isinstance<py::list>(input)) {
                    py::list input_list = input.cast<py::list>();
                    py::array_t<double> double_array = py::cast<py::array_t<double>>(input_list);
                    return process_python_array(double_array);
                }

                // Handle itterator / generatore types
                if (py::isinstance<py::iterable>(input)) {
                    return py::cast(LazyIterator(input.cast<py::iterable>(), *this));
                }

                throw std::invalid_argument("Unsupported input type for call");
            }
        }

        class LazyIterator {
        public:
            LazyIterator(py::iterable iterable, ScreamerBase& processor)
                : iterator_(py::iter(iterable)), processor_(processor) {}

            // __iter__ method
            LazyIterator& __iter__() { return *this; }

            // __next__ method
            py::object __next__() {
                try {
                    // Get the next item by calling the Python iterator's __next__ method
                    py::object item = iterator_.attr("__next__")();
                    double value = item.cast<double>();
                    return py::float_(processor_.process_scalar(value));
                } catch (py::error_already_set &e) {
                    if (e.matches(PyExc_StopIteration)) {
                        throw py::stop_iteration();
                    } else {
                        throw;  // Re-throw other exceptions
                    }
                }
            }

        private:
            py::iterator iterator_;
            ScreamerBase& processor_;

            // Make LazyIterator friend, for potential access
            friend class LazyIterator;            
        };

    protected:

        // Pure virtual function to process a single scalar
        virtual double process_scalar(double value) = 0;

        // Virtual function to process an array in contiguous memory (no strides)
        // defaulting to looping with process_scalar.
        virtual void process_array_no_stride(
        double* result_data, 
        double* input_data,
         size_t size) {

            for (size_t i=0; i<size; i++) {
                result_data[i] = process_scalar(input_data[i]);
            }
        }

        // Virtual function to process an array with strided elements, typially a
        // srided view on a numpy array like x[1::2, ::4, ...]
        // defaulting to looping with process_scalar.
        virtual void process_array_stride(
            double* result_data, 
            size_t result_stride,
            double* input_data, 
            size_t input_stride,
            size_t size) {

            size_t result_start = 0;
            size_t input_start = 0;

            for (size_t i = 0; i < size; i++) {
                result_data[result_start] = process_scalar(input_data[input_start]);
                result_start += result_stride;
                input_start += input_stride;
            }
           
        }


        // function for numpy array processing
        py::array_t<double> process_python_array(py::array_t<double> input_array) {

            // Inspect the input
            py::buffer_info buf_info = input_array.request();

            // Ensure that the input array has at least one dimension
            if (buf_info.ndim < 1 || buf_info.itemsize != sizeof(double)) {
                throw std::runtime_error("Input array must have at least one dimension and contain doubles");
            }
            
            // Get access to the data of the input
            double* input_data = static_cast<double*>(buf_info.ptr);

            // Create an output array of the same shape as the input
            py::array_t<double> result(buf_info.shape);
            py::buffer_info result_buf = result.request();
            double* result_data = static_cast<double*>(result_buf.ptr);

            // The size along the first dimension
            size_t size = buf_info.shape[0];

            // If this is a contiguous 1d array then we have optimized code!
            if (buf_info.ndim == 1 && buf_info.strides[0] == sizeof(double)) {

                reset();

                process_array_no_stride(result_data, input_data, size);

                return result;
            }

            // We have multidimensional and/or strided data

            // Total size of the rest of the dimensions
            size_t rest_size = 1;  
            for (int i = 1; i < buf_info.ndim; ++i) {
                rest_size *= buf_info.shape[i];
            }

            // Compute the strides in terms of the number of elements
            std::vector<size_t> input_strides(buf_info.ndim);
            std::vector<size_t> result_strides(buf_info.ndim);

            for (int i = 0; i < buf_info.ndim; ++i) {
                input_strides[i] = buf_info.strides[i] / sizeof(double);
                result_strides[i] = result_buf.strides[i] / sizeof(double);
            }

            // Precompute column offsets
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

            // Apply the function to each column
            for (size_t col = 0; col < rest_size; ++col) {

                // Initialize indices with precomputed offsets
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

            return result;

        }


    };

}

#endif