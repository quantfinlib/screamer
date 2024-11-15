#ifndef SCREAMER_ASYNC_GENERATOR_H
#define SCREAMER_ASYNC_GENERATOR_H

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace screamer {

// Forward declaration of ScreamerBase
class ScreamerBase;

// Function to check if an object is an async generator
bool is_async_generator(const py::object& obj);


/*
class Awaiter {
public:
    Awaiter(py::object future, ScreamerBase& processor);
    Awaiter& __iter__();
    py::object __next__();
private:
    py::object future_;
    ScreamerBase& processor_;
    int state_;
};
*/

// Class representing the awaitable returned by __anext__
class AnextAwaitable {
public:
    AnextAwaitable(py::object awaitable, ScreamerBase& processor);
    py::object __await__();

private:
    py::object awaitable_;
    ScreamerBase& processor_;
};

// Class representing the async iterator
class LazyAsyncIterator {
public:
    LazyAsyncIterator(py::object async_iterable, ScreamerBase& processor);

    // __aiter__ method
    LazyAsyncIterator& __aiter__();

    // __anext__ method
    py::object __anext__();

private:
    py::object async_iterator_;
    ScreamerBase& processor_;
};

} // namespace screamer

#endif // SCREAMER_ASYNC_GENERATOR_H
