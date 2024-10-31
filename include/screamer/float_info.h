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
