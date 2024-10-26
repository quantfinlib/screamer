#ifndef SCREAMER_TWO_H
#define SCREAMER_TWO_H

#include <pybind11/pybind11.h>
#include <screamer/base.h>

namespace py = pybind11;

namespace screamer {

    class Two : public ScreamerBase {

    protected:
        double process_scalar(double value) override {
            return value * 2;
        }

    };

} // namespace screamer

#endif