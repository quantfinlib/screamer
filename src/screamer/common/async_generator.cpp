#include "screamer/common/async_generator.h"
#include "screamer/common/base.h" // Include full definition of ScreamerBase
#include <pybind11/functional.h>   // For py::cpp_function
//#include <pybind11/async.h>        // For py::asyncio
#include <pybind11/eval.h>

namespace screamer {

bool is_async_generator(const py::object& obj) {
    return py::hasattr(obj, "__aiter__") && py::hasattr(obj, "__anext__");
}

/*

Awaiter::Awaiter(py::object future, ScreamerBase& processor)
        : future_(future), processor_(processor), state_(0) {}

// __iter__ method
Awaiter& Awaiter::__iter__() {
    return *this;
}

// __next__ method
py::object Awaiter::__next__() {
    if (state_ == 0) {
        ++state_;
        // Yield control to Python to await the future
        PyErr_SetObject(PyExc_StopIteration, future_.ptr());
        throw py::error_already_set();
    } else if (state_ == 1) {
        ++state_;
        // Check if the future is done
        if (!future_.attr("done")().cast<bool>()) {
            // If the future is not done, yield control again
            PyErr_SetObject(PyExc_StopIteration, future_.ptr());
            throw py::error_already_set();
        }
        // Future is done; get the result
        py::object result = future_.attr("result")();

        // Process the result
        double value = result.cast<double>();
        double processed_value = processor_.process_scalar(value);

        // Return the processed value
        py::object processed_result = py::float_(processed_value);

        // Raise StopIteration with the processed result
        PyErr_SetObject(PyExc_StopIteration, processed_result.ptr());
        throw py::error_already_set();
    } else {
        // Iteration is complete; raise StopIteration without a value
        throw py::stop_iteration();
    }
}
*/

// Implementation of AnextAwaitable
AnextAwaitable::AnextAwaitable(py::object awaitable, ScreamerBase& processor)
    : awaitable_(std::move(awaitable)), processor_(processor) {}

py::object AnextAwaitable::__await__() {
    /*
    py::object asyncio = py::module::import("asyncio");
    py::object future = asyncio.attr("ensure_future")(awaitable_);
    return py::cast(Awaiter(future, processor_));
    */
        // Define the coroutine function in Python code
        const char* code = R"(
async def process_awaitable(awaitable, processor):
    result = await awaitable
    processed_result = processor.process_scalar(result)
    return processed_result
)";

    // Execute the code
    py::dict globals = py::globals();
    py::dict locals;
    locals["processor"] = py::cast(&processor_, py::return_value_policy::reference);

    py::exec(code, globals, locals);

    // Retrieve the coroutine function
    py::object process_awaitable = locals["process_awaitable"];

    // Create the coroutine object by calling the function
    py::object coro = process_awaitable(awaitable_, locals["processor"]);

    // Return the coroutine's __await__ method
    return coro.attr("__await__")();   
}


// Implementation of LazyAsyncIterator

LazyAsyncIterator::LazyAsyncIterator(py::object async_iterable, ScreamerBase& processor)
    : async_iterator_(async_iterable.attr("__aiter__")()), processor_(processor) {}

LazyAsyncIterator& LazyAsyncIterator::__aiter__() {
    return *this;
}

py::object LazyAsyncIterator::__anext__() {
    // Get the next item from the async iterator
    py::object awaitable = async_iterator_.attr("__anext__")();
    // Return an AnextAwaitable that will process the item
    return py::cast(AnextAwaitable(std::move(awaitable), processor_));    
}

} // namespace screamer
