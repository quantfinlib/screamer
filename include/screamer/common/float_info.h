#ifndef SCREAMER_FLOAT_INFO_H
#define SCREAMER_FLOAT_INFO_H

/*
we use -ffast_math, but...

-ffast-math
Sets the options -fno-math-errno, -funsafe-math-optimizations, -ffinite-math-only, -fno-rounding-math, 
-fno-signaling-nans, -fcx-limited-range and -fexcess-precision=fast.

This option causes the preprocessor macro __FAST_MATH__ to be defined.

This option is not turned on by any -O option besides -Ofast since it can result in incorrect output 
for programs that depend on an exact implementation of IEEE or ISO rules/specifications for math 
functions. It may, however, yield faster code for programs that do not require the guarantees of these 
specifications.

The code below provided NaN tests that DO work with -ffast_math
Code below is copied from Maxim Egorushkin on https://stackoverflow.com/a/57770634
Thanks Maxim!
*/

namespace screamer {

    static inline uint64_t load_ieee754_rep(double a) {
        uint64_t r;
        static_assert(sizeof r == sizeof a, "Unexpected sizes.");
        std::memcpy(&r, &a, sizeof a); // Generates movq instruction.
        return r;
    }

    static inline uint32_t load_ieee754_rep(float a) {
        uint32_t r;
        static_assert(sizeof r == sizeof a, "Unexpected sizes.");
        std::memcpy(&r, &a, sizeof a); // Generates movd instruction.
        return r;
    }

    constexpr uint64_t inf_double_shl1 = UINT64_C(0xffe0000000000000);
    constexpr uint32_t inf_float_shl1 = UINT32_C(0xff000000);

    // The shift left removes the sign bit. The exponent moves into the topmost bits,
    // so that plain unsigned comparison is enough.
    static inline bool isnan2(double a)    { return load_ieee754_rep(a) << 1  > inf_double_shl1; }
    static inline bool isinf2(double a)    { return load_ieee754_rep(a) << 1 == inf_double_shl1; }
    static inline bool isfinite2(double a) { return load_ieee754_rep(a) << 1  < inf_double_shl1; }
    static inline bool isnan2(float a)     { return load_ieee754_rep(a) << 1  > inf_float_shl1; }
    static inline bool isinf2(float a)     { return load_ieee754_rep(a) << 1 == inf_float_shl1; }
    static inline bool isfinite2(float a)  { return load_ieee754_rep(a) << 1  < inf_float_shl1; }

} // namespace

#endif