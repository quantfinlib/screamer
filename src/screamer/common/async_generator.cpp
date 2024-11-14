#include "screamer/common/async_generator.h"
#include "screamer/common/base.h" // Include full definition of ScreamerBase
#include <pybind11/functional.h>   // For py::cpp_function
//#include <pybind11/async.h>        // For py::asyncio

namespace screamer {

bool is_async_generator(const py::object& obj) {
    return py::hasattr(obj, "__aiter__") && py::hasattr(obj, "__anext__");
}

// Implementation of AnextAwaitable
AnextAwaitable::AnextAwaitable(py::object awaitable, ScreamerBase& processor)
    : awaitable_(std::move(awaitable)), processor_(processor) {}

// __await__ method
py::object AnextAwaitable::__await__() {
    // Define a Python coroutine function that awaits the original awaitable
    py::object asyncio = py::module::import("asyncio");


    // Corrected lambda capture list includes 'asyncio'
    py::object coroutine = py::cpp_function(
        [awaitable = this->awaitable_, &processor = this->processor_, asyncio]() -> py::object {
            // Use asyncio's Future to handle the awaitable
            py::object future = asyncio.attr("ensure_future")(awaitable);

            // Define a callback to process the result
            py::object process_result = py::cpp_function(
                [&processor](py::object result) -> py::object {
                    double value = result.cast<double>();
                    double processed_value = processor.process_scalar(value);
                    return py::float_(processed_value);
                }
            );

            // Attach the callback to the future
            future.attr("add_done_callback")(
                py::cpp_function([process_result](py::object fut) {
                    try {
                        py::object result = fut.attr("result")();
                        py::object processed = process_result(result);
                        fut.attr("set_result")(processed);
                    } catch (py::error_already_set& e) {
                        fut.attr("set_exception")(e.value());
                    }
                })
            );

            // Return the future as the awaitable
            return future;
        },
        py::name("anext_coroutine")
        // Removed 'py::is_method(this)' as per previous correction
    )();

    // Return the coroutine's __await__ method
    return coroutine.attr("__await__")();
}


// Implementation of LazyAsyncIterator

LazyAsyncIterator::LazyAsyncIterator(py::object async_iterable, ScreamerBase& processor)
    : async_iterator_(async_iterable.attr("__aiter__")()), processor_(processor) {}

LazyAsyncIterator& LazyAsyncIterator::__aiter__() {
    return *this;
}

py::object LazyAsyncIterator::__anext__() {
    // Get the awaitable from the underlying async iterator
    py::object awaitable = async_iterator_.attr("__anext__")();

    // Return an instance of AnextAwaitable
    return py::cast(AnextAwaitable(std::move(awaitable), processor_));
}

} // namespace screamer
