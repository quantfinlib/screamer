#ifndef LAZY_ITERATOR_N_H
#define LAZY_ITERATOR_N_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace screamer {

// Forward declare ScreamerBaseN
template <typename Derived, size_t N>
class ScreamerBaseN;

class LazyIteratorN {
public:
    template <typename Derived, size_t N>
    LazyIteratorN(py::iterable iterable, ScreamerBaseN<Derived, N>& processor, std::function<double(const py::object&)> scalar_processor);

    LazyIteratorN& __iter__();
    py::object __next__();

private:
    py::iterator iterator_;
    std::function<double(const py::object&)> scalar_processor_;
};

} // namespace
#endif  // LAZY_ITERATOR_N_H
