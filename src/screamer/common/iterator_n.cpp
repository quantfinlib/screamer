#include "screamer/common/iterator_n.h"

namespace screamer {

template <typename Derived, size_t N>
LazyIteratorN::LazyIteratorN(py::iterable iterable, ScreamerBaseN<Derived, N>& processor, std::function<double(const py::object&)> scalar_processor)
    : iterator_(py::iter(iterable)), scalar_processor_(std::move(scalar_processor)) {}

LazyIteratorN& LazyIteratorN::__iter__() { return *this; }

py::object LazyIteratorN::__next__() {
    try {
        py::object item = iterator_.attr("__next__")();
        double result = scalar_processor_(item);
        return py::float_(result);
    } catch (py::error_already_set& e) {
        if (e.matches(PyExc_StopIteration)) {
            throw py::stop_iteration();
        } else {
            throw;
        }
    }
}

} // namespace