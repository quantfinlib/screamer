#include "screamer/lag.h"

namespace screamer {


Lag::Lag(int N, double defaultValue)
    : index(0), N(N)
{
    if (N < 1) {
        throw std::invalid_argument("Delay must be an integer >= 1.");
    }
    buffer.resize(N, defaultValue);    
}

double Lag::operator()(double newValue) {

    double oldValue = buffer[index];
    buffer[index] = newValue;
    index++;
    if (index == N) {
        index = 0;
    }
    return oldValue;
}

py::array_t<double> Lag::transform(py::array_t<double> input_array) {
    // Get buffer info from the input NumPy array
    py::buffer_info buf_info = input_array.request();

    // Check that the input is a 1D array
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1D");
    }

    // Create an output array of the same size as the input
    py::array_t<double> result_array(buf_info.size);
    py::buffer_info result_buf_info = result_array.request();

    // Get pointers to input and output data
    double* input_ptr = static_cast<double*>(buf_info.ptr);
    double* result_ptr = static_cast<double*>(result_buf_info.ptr);

    // Apply the transformation for each element
    for (size_t i = 0; i < buf_info.size; ++i) {
        result_ptr[i] = (*this)(input_ptr[i]);
    }

    return result_array;
}

} // namespace screamer
