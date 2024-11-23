#ifndef FUNCTOR_BASE_H
#define FUNCTOR_BASE_H

#include <tuple>
#include <type_traits>
#include <utility>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <iostream>

namespace screamer {

namespace detail {
    // Helper struct to create a tuple of M doubles
    template <size_t M, typename T = double, typename... Ts>
    struct TupleOfDoublesHelper {
        using type = typename TupleOfDoublesHelper<M - 1, T, T, Ts...>::type;
    };

    // Specialization for 0
    template <typename T, typename... Ts>
    struct TupleOfDoublesHelper<0, T, Ts...> {
        using type = std::tuple<Ts...>;
    };

    // Alias for easier use
    template <size_t M>
    using TupleOfDoubles = typename TupleOfDoublesHelper<M>::type;
}

template <class Derived, size_t N, size_t M>
class FunctorBase {
public:
    using InputArray = std::array<double, N>;
    using ResultTuple = std::conditional_t<M == 1, double, typename detail::TupleOfDoubles<M>>;

    virtual ResultTuple call(const InputArray& inputs) = 0;
    virtual void reset() {};


    size_t numpy_num_cols(const py::buffer_info& buf_info) {
        size_t num_cols = 1;  
        for (int i = 1; i < buf_info.ndim; ++i) {
            num_cols *= buf_info.shape[i];
        }
        return num_cols;
    }

    size_t numpy_col_start_pos(const size_t column, const py::buffer_info& buf_info) {
        size_t start_pos = 0;
        size_t temp = column;
        for (size_t dim = 1; dim < buf_info.ndim; ++dim) {
            size_t index = temp % buf_info.shape[dim];
            start_pos += index * (buf_info.strides[dim] / buf_info.itemsize);
            temp /= buf_info.shape[dim];
        }
        return start_pos;
    }


    template <size_t TN = N, size_t TM = M, typename = std::enable_if_t<(TN == 1) && (TM == 1)>>
    py::object handle_input_1i_1o_numpy(py::array_t<double>& input) {
        
        py::buffer_info input_info = input.request();

        if (input_info.ndim < 1 || input_info.itemsize != sizeof(double)) {
            throw std::runtime_error("Input array must have at least one dimension and contain doubles");
        }

        // Allocate output storage
        py::array_t<double> output(input_info.shape);
        py::buffer_info output_info = output.request();

        // Get pointers to input and output memory
        double* input_data = static_cast<double*>(input_info.ptr);
        double* output_data = static_cast<double*>(output_info.ptr);

        // get number of elements in a column, and the stepsize
        size_t size = input_info.shape[0];
        ssize_t input_stride = input_info.strides[0] / input_info.itemsize;
        ssize_t output_stride = output_info.strides[0] / input_info.itemsize;

        // get the number of columns in this ndarray
        auto num_cols = numpy_num_cols(input_info);

        // loop over all columns
        for (size_t col = 0; col < num_cols; ++col) {
            
            // find the start positions in memory of this column
            size_t input_index = numpy_col_start_pos(col, input_info);
            size_t output_index = numpy_col_start_pos(col, output_info);

            reset(); // reset before processing this column

            for (size_t i = 0; i < size; i++) {
                output_data[output_index] = call({input_data[input_index]});

                input_index += input_stride;
                output_index += output_stride;
            }

        }

        reset(); // after we have processed all columns

        return output;
    }

    template <size_t TN = N, size_t TM = M, typename = std::enable_if_t<(TN > 1) && (TM == 1)>>
    py::object handle_input_Ni_1o_numpy(py::tuple& inputs) {

        std::array<py::array_t<double>, TN> inputs_array;
        std::array<py::buffer_info, TN> inputs_info;
        // Check that the first input is a numpy array
        if (!py::isinstance<py::array>(inputs[0])) {
            throw py::type_error("Incompatible input type, a mix of numpy arrays and other.");
        }
        inputs_array[0] = py::cast<py::array_t<double>>(inputs[0]);


        // Get shape info of the first numpy array
        inputs_info[0] = inputs_array[0].request();

        // basic cjeck
        if (inputs_info[0].ndim < 1) {
            throw std::runtime_error("Input array must have at least one dimension");
        }

        // Check alignment between the first numpy array, and all others
        for (size_t i = 1; i < TN; ++i) {
            // Check that that the type is a numpy array
            if (!py::isinstance<py::array>(inputs[i])) {
                throw py::type_error("Incompatible input type, a mix of numpy arrays and other.");
            }
            inputs_array[i] = py::cast<py::array_t<double>>(inputs[i]);

            // Check for the same number of dims
            inputs_info[i] = inputs_array[i].request();
            if (inputs_info[0].ndim != inputs_info[i].ndim) {
                throw py::type_error("Incompatible input numpy arrays, dimensions mismatch.");
            }
            // Check for the same number of elements per dim
            for (size_t d=0; d<inputs_info[0].ndim; ++d) {
                if (inputs_info[0].shape[d] != inputs_info[i].shape[d]) {
                    throw py::type_error("Incompatible input numpy arrays, shape mismatch.");
                }
            }
        }
        // we have N numpy arrays of matching shape!

        // Allocate output storage
        py::array_t<double> output(inputs_info[0].shape);
        py::buffer_info output_info = output.request();
        double* output_data = static_cast<double*>(output_info.ptr);
        ssize_t output_stride = output_info.strides[0] / output_info.itemsize;

        // Get input info
        std::array<double*, TN> inputs_data;
        std::array<int64_t, TN> inputs_stride;
        size_t size = inputs_info[0].shape[0];
        for (size_t i = 0; i < TN; ++i) {
            inputs_data[i] = static_cast<double*>(inputs_info[i].ptr);
            inputs_stride[i] = inputs_info[i].strides[0] / inputs_info[i].itemsize;
        }

        // get the number of columns in this ndarray
        auto num_cols = numpy_num_cols(inputs_info[0]);

        // loop over all columns
        std::array<size_t, TN> inputs_index; 
        for (size_t col = 0; col < num_cols; ++col) {
            
            // find the start positions in memory of this column for all input arguments
            for (size_t i = 0; i < TN; ++i) {
                inputs_index[i] = numpy_col_start_pos(col, inputs_info[i]);
            }
            size_t output_index = numpy_col_start_pos(col, output_info);

            reset(); // reset before processing this column

            InputArray call_array;
            for (size_t i = 0; i < size; i++) {
                for (size_t i = 0; i < TN; ++i) {
                    call_array[i] = inputs_data[i][inputs_index[i]];
                }
                output_data[output_index] = call(call_array);

                for (size_t i = 0; i < TN; ++i) {
                    inputs_index[i] += inputs_stride[i];
                }
                output_index += output_stride;
            }

        }

        reset(); // after we have processed all columns

        return output;
    }    

    // one input, one output
    template <size_t TN = N, size_t TM = M, typename = std::enable_if_t<(TN == 1) && (TM == 1)>>
    py::object handle_input_1i_1o(py::object input) {
        
        // Case 1: Scalar input
        try {
            InputArray input_array = {input.cast<double>()};
            return py::cast(call(input_array));
        } catch (const py::cast_error&) {
            // If not a scalar, fall through to further checks
        }

        // Case 2: Numpy array
        if (py::isinstance<py::array>(input)) {
            py::array_t<double> input_pyarray = py::cast<py::array_t<double>>(input);
            return handle_input_1i_1o_numpy(input_pyarray);
        }

        // Case 3: Iterable
        if (py::isinstance<py::iterable>(input)) {
            std::vector<ResultTuple> results;

            for (auto item : input) {

                try {
                    // Case 2.1: Scalar input item
                    InputArray input_array = {item.cast<double>()};
                    results.push_back(call(input_array));
                    continue;
                } catch (const py::cast_error&) {
                    // If not a scalar, continue to further checks
                }

                if (py::isinstance<py::tuple>(item)) {
                    auto tuple = item.cast<py::tuple>();
                    if (tuple.size() == N) {
                        InputArray input_array = cast_to_array(tuple);
                        results.push_back(call(input_array));
                    } else {
                        throw py::type_error("Invalid tuple size in iterable.");
                    }
                } else {
                    throw py::type_error("Iterable must contain doubles or tuples of correct size.");
                }
            }

            return py::cast(results);
        }

        // Case no match:
        throw py::type_error("Unsupported input type. Supperted types are double, or iterables.");
    }


    py::tuple args_to_tuple_n(const py::args args) const {

        if (args.size() == 1) { // a container of N

            auto arg = args[0];
            
            // we only support tuples and lists
            if (!(py::isinstance<py::list>(arg) || py::isinstance<py::tuple>(arg))) {
                throw py::type_error("Unsupported single argument input type. Supperted types are lists or tuples.");
            }

            // convert to tuple
             py::tuple inputs = py::cast<py::tuple>(arg);

            // validate size
            if (inputs.size() != N) {
                throw py::type_error("Wrong number of elements in the single argument input list / tuple.");
            }

            return inputs;
        } 
        
        if (args.size() == N) {
            return py::cast<py::tuple>(args);
        }
        
        throw py::type_error("Wrong number of arguments.");
    }
    

    // ---------------------------------------------------------
    // MULTIPLE INPUTS, ONE OUTPUT HANDELER
    // ---------------------------------------------------------
    template <size_t TN = N, size_t TM = M, typename = std::enable_if_t<(TN > 1) && (TM == 1)>>
    py::object handle_input_Ni_1o(const py::args args) {

       // Case 1: we need to get his out of the way first. 
       // A single argument, list/tuple with N-tuples inside: [ (1,2,3), (4,5,6), ...]
        if (args.size() == 1) {
            auto input = args[0];
            if (py::isinstance<py::list>(input) || py::isinstance<py::tuple>(input)) {
                bool valid = true;
                std::vector<ResultTuple> results;
                for (auto item : input) {
                    if (!py::isinstance<py::tuple>(item)) {
                        valid = false;
                        break;
                    }
                    auto tuple = item.cast<py::tuple>();
                    if (tuple.size() != N) {
                        valid = false;
                        break;
                    }
                    InputArray input_array = cast_to_array(tuple);
                    results.push_back(call(input_array));
                }
                if (valid) {       
                    return py::cast(results);         
                }
            }
        }

        // after this, now we handle cases where we have N arguments
        py::tuple inputs = args_to_tuple_n(args);

        // Case 2: try a tuple of N scalar input: (0.3, 1.2, 4.0)
        try {
            InputArray array;
            for (size_t i = 0; i < N; ++i) {
                array[i] = inputs[i].cast<double>();
            }
            return py::cast(call(array));
        } catch (const py::cast_error&) {
            // If not a scalar, fall through to further checks
        }

        // Case 3: try a tuple of N numpy arrays, all of the same size.( nparray, nparray, nparray)
        if (py::isinstance<py::array>(inputs[0])) {
            return handle_input_Ni_1o_numpy(inputs);
        }

        // Case 4: a tuple of N iterables: ( [... != ], [...], [...] )
        bool all_iterable = true;
        for (auto input : inputs) {
            all_iterable = all_iterable && py::isinstance<py::iterable>(input);
            if (!all_iterable) {
                break;
            }
        }

        if (all_iterable) {

            // Initialize iterators for each input iterable
            std::array<py::iterator, N> iterators;

            for (size_t i = 0; i < N; ++i) {
                iterators[i] = py::iter(inputs[i]);
            }

            std::vector<ResultTuple> results;

            // Loop until any of the iterators is exhausted
            while (true) {
                
                InputArray array;
                try {
                    // Advance all iterators and collect the next items
                    for (size_t i = 0; i < N; ++i) {

                        // Check if the iterator is valid
                        if (iterators[i] == py::iterator()) {
                            throw py::stop_iteration();
                        }

                        auto val = *iterators[i];
                        array[i] = val.template cast<double>();
                        ++iterators[i];
                    }
                } catch (py::stop_iteration&) {
                    // One of the iterators is exhausted; exit the loop
                    break;
                }

                // store the call
                results.push_back(call(array));

            }

            return py::cast(results);

        }

        // Case no match:
        throw py::type_error("Unsupported input type.");
    }

    // ---------------------------------------------------------
    // Main dispatcher
    // ---------------------------------------------------------
    py::object handle_input(py::args args) {
        if constexpr (N == 1) {
            if (args.size() != 1) {
                throw py::type_error("Wrong number of in puts");
            }
        }
        if constexpr ((N == 1) && (M == 1)) {
            return handle_input_1i_1o(args[0]);
        } else if constexpr ((N > 1) && (M == 1)) {
            return handle_input_Ni_1o(args);
        } else if constexpr ((N == 1) && (M > 1)) {
            throw py::type_error("Unsupported functor type: N == 1, M > 1");
        } else if constexpr ((N > 1) && (M > 1)) {
            throw py::type_error("Unsupported functor type: N > 1, M > 1");
        } else {
            throw py::type_error("Unknown configuration.");
        }
    }

    

private:
    InputArray cast_to_array(const py::tuple& tuple) {
        if (tuple.size() != N) {
            throw py::type_error("Tuple size does not match the number of expected inputs.");
        }
        InputArray array;
        for (size_t i = 0; i < N; ++i) {
            array[i] = tuple[i].cast<double>();
        }
        return array;
    }
};




}
#endif