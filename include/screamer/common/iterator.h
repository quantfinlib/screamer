#ifndef SCREAMER_ITERATOR_H
#define SCREAMER_ITERATOR_H

#include <pybind11/pybind11.h>
#include "screamer/common/base.h"

namespace py = pybind11;

namespace screamer {

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
};

} // namespace
#endif