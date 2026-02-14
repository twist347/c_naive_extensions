#include "nx/algo/algo_numeric.h"

#include "nx/core/limit.h"

// =============================================================================
// min/max
// =============================================================================

nx_i8 mx_min_i8(nx_i8 a, nx_i8 b) {
    return NX_MIN(a, b);
}

nx_i8 mx_max_i8(nx_i8 a, nx_i8 b) {
    return NX_MAX(a, b);
}

nx_i16 mx_min_i16(nx_i16 a, nx_i16 b) {
    return NX_MIN(a, b);
}

nx_i16 mx_max_i16(nx_i16 a, nx_i16 b) {
    return NX_MAX(a, b);
}

nx_i32 mx_min_i32(nx_i32 a, nx_i32 b) {
    return NX_MIN(a, b);
}

nx_i32 mx_max_i32(nx_i32 a, nx_i32 b) {
    return NX_MAX(a, b);
}

nx_i64 mx_min_i64(nx_i64 a, nx_i64 b) {
    return NX_MIN(a, b);
}

nx_i64 mx_max_i64(nx_i64 a, nx_i64 b) {
    return NX_MAX(a, b);
}

nx_u8 mx_min_u8(nx_i8 a, nx_i8 b) {
    return NX_MIN(a, b);
}

nx_u8 mx_max_u8(nx_i8 a, nx_i8 b) {
    return NX_MAX(a, b);
}

nx_u16 mx_min_u16(nx_u16 a, nx_u16 b) {
    return NX_MIN(a, b);
}

nx_u16 mx_max_u16(nx_u16 a, nx_u16 b) {
    return NX_MAX(a, b);
}

nx_u32 mx_min_u32(nx_u32 a, nx_u32 b) {
    return NX_MIN(a, b);
}

nx_u32 mx_max_u32(nx_u32 a, nx_u32 b) {
    return NX_MAX(a, b);
}

nx_u64 mx_min_u64(nx_u64 a, nx_u64 b) {
    return NX_MIN(a, b);
}

nx_u64 mx_max_u64(nx_u64 a, nx_u64 b) {
    return NX_MAX(a, b);
}

nx_f32 mx_min_f32(nx_f32 a, nx_f32 b) {
    return NX_MIN(a, b);
}

nx_f32 mx_max_f32(nx_f32 a, nx_f32 b) {
    return NX_MAX(a, b);
}

nx_f64 mx_min_f64(nx_f64 a, nx_f64 b) {
    return NX_MIN(a, b);
}

nx_f64 mx_max_f64(nx_f64 a, nx_f64 b) {
    return NX_MAX(a, b);
}

// =============================================================================
// clamp
// =============================================================================

nx_i8 nx_clamp_i8(nx_i8 x, nx_i8 min_val, nx_i8 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}

nx_i16 nx_clamp_i16(nx_i16 x, nx_i16 min_val, nx_i16 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}

nx_i32 nx_clamp_i32(nx_i32 x, nx_i32 min_val, nx_i32 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}

nx_i64 nx_clamp_i64(nx_i64 x, nx_i64 min_val, nx_i64 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}

nx_u8 nx_clamp_u8(nx_u8 x, nx_u8 min_val, nx_u8 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}

nx_u16 nx_clamp_u16(nx_u16 x, nx_u16 min_val, nx_u16 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}

nx_u32 nx_clamp_u32(nx_u32 x, nx_u32 min_val, nx_u32 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}

nx_u64 nx_clamp_u64(nx_u64 x, nx_u64 min_val, nx_u64 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}

nx_f32 nx_clamp_f32(nx_f32 x, nx_f32 min_val, nx_f32 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}

nx_f64 nx_clamp_f64(nx_f64 x, nx_f64 min_val, nx_f64 max_val) {
    return NX_CLAMP(x, min_val, max_val);
}


// =============================================================================
// Saturating arithmetic
// =============================================================================

#define NX_SAT_ADD_SIGNED(T, a, b, T_MAX, T_MIN)            \
    do {                                                    \
        const T res = (a) + (b);                            \
        if ((a) > 0 && (b) > 0 && res < 0) return T_MAX;    \
        if ((a) < 0 && (b) < 0 && res > 0) return T_MIN;    \
        return res;                                         \
    } while (0)

#define NX_SAT_ADD_UNSIGNED(T, a, b, T_MAX)    \
    do {                                       \
        const T res = (a) + (b);               \
        return res < (a) ? T_MAX : res;        \
    } while (0)

#define NX_SAT_SUB_SIGNED(T, a, b, T_MAX, T_MIN)            \
    do {                                                    \
        const T res = (a) - (b);                            \
        if ((a) > 0 && (b) < 0 && res < 0) return T_MAX;    \
        if ((a) < 0 && (b) > 0 && res > 0) return T_MIN;    \
        return res; \
    } while (0)

#define NX_SAT_SUB_UNSIGNED(T, a, b)     \
    return (a) > (b) ? (a) - (b) : 0

#define NX_SAT_MUL_SIGNED(T, T_WIDE, a, b, T_MAX, T_MIN)    \
    do {                                                    \
        const T_WIDE res = (T_WIDE)(a) * (T_WIDE)(b);       \
        if (res > T_MAX) return T_MAX;                      \
        if (res < T_MIN) return T_MIN;                      \
        return (T)res;                                      \
    } while (0)

#define NX_SAT_MUL_UNSIGNED(T, T_WIDE, a, b, T_MAX)      \
    do {                                                 \
        const T_WIDE res = (T_WIDE)(a) * (T_WIDE)(b);    \
        return res > T_MAX ? T_MAX : (T)res;             \
    } while (0)

nx_i8 nx_sat_add_i8(nx_i8 a, nx_i8 b) {
    NX_SAT_ADD_SIGNED(nx_i8, a, b, NX_I8_MAX, NX_I8_MIN);
}

nx_i16 nx_sat_add_i16(nx_i16 a, nx_i16 b) {
    NX_SAT_ADD_SIGNED(nx_i16, a, b, NX_I16_MAX, NX_I16_MIN);
}

nx_i32 nx_sat_add_i32(nx_i32 a, nx_i32 b) {
    NX_SAT_ADD_SIGNED(nx_i32, a, b, NX_I32_MAX, NX_I32_MIN);
}

nx_i64 nx_sat_add_i64(nx_i64 a, nx_i64 b) {
    NX_SAT_ADD_SIGNED(nx_i64, a, b, NX_I64_MAX, NX_I64_MIN);
}

nx_u8 nx_sat_add_u8(nx_u8 a, nx_u8 b) {
    NX_SAT_ADD_UNSIGNED(nx_u8, a, b, NX_U8_MAX);
}

nx_u16 nx_sat_add_u16(nx_u16 a, nx_u16 b) {
    NX_SAT_ADD_UNSIGNED(nx_u16, a, b, NX_U16_MAX);
}

nx_u32 nx_sat_add_u32(nx_u32 a, nx_u32 b) {
    NX_SAT_ADD_UNSIGNED(nx_u32, a, b, NX_U32_MAX);
}

nx_u64 nx_sat_add_u64(nx_u64 a, nx_u64 b) {
    NX_SAT_ADD_UNSIGNED(nx_u64, a, b, NX_U64_MAX);
}

nx_i8 nx_sat_sub_i8(nx_i8 a, nx_i8 b) {
    NX_SAT_SUB_SIGNED(nx_i8, a, b, NX_I8_MAX, NX_I8_MIN);
}

nx_i16 nx_sat_sub_i16(nx_i16 a, nx_i16 b) {
    NX_SAT_SUB_SIGNED(nx_i16, a, b, NX_I16_MAX, NX_I16_MIN);
}

nx_i32 nx_sat_sub_i32(nx_i32 a, nx_i32 b) {
    NX_SAT_SUB_SIGNED(nx_i32, a, b, NX_I32_MAX, NX_I32_MIN);
}

nx_i64 nx_sat_sub_i64(nx_i64 a, nx_i64 b) {
    NX_SAT_SUB_SIGNED(nx_i64, a, b, NX_I64_MAX, NX_I64_MIN);
}

nx_u8 nx_sat_sub_u8(nx_u8 a, nx_u8 b) {
    NX_SAT_SUB_UNSIGNED(nx_u8, a, b);
}

nx_u16 nx_sat_sub_u16(nx_u16 a, nx_u16 b) {
    NX_SAT_SUB_UNSIGNED(nx_u16, a, b);
}

nx_u32 nx_sat_sub_u32(nx_u32 a, nx_u32 b) {
    NX_SAT_SUB_UNSIGNED(nx_u32, a, b);
}

nx_u64 nx_sat_sub_u64(nx_u64 a, nx_u64 b) {
    NX_SAT_SUB_UNSIGNED(nx_u64, a, b);
}

nx_u8 nx_sat_mul_i8(nx_i8 a, nx_i8 b) {
    NX_SAT_MUL_SIGNED(nx_i8, nx_i16, a, b, NX_I8_MAX, NX_I8_MIN);
}

nx_u16 nx_sat_mul_i16(nx_i16 a, nx_i16 b) {
    NX_SAT_MUL_SIGNED(nx_i16, nx_i32, a, b, NX_I16_MAX, NX_I16_MIN);
}

nx_u32 nx_sat_mul_i32(nx_i32 a, nx_i32 b) {
    NX_SAT_MUL_SIGNED(nx_i32, nx_i64, a, b, NX_I32_MAX, NX_I32_MIN);
}

nx_u64 nx_sat_mul_i64(nx_i64 a, nx_i64 b) {
    if (a == 0 || b == 0) {
        return 0;
    }

    if (a > 0) {
        if (b > 0) {
            if (a > NX_I64_MAX / b) {
                return NX_I64_MAX;
            }
        } else {
            if (b < NX_I64_MIN / a) {
                return NX_I64_MIN;
            }
        }
    } else {
        if (b > 0) {
            if (a < NX_I64_MIN / b) {
                return NX_I64_MIN;
            }
        } else {
            if (a != 0 && b < NX_I64_MAX / -a) {
                return NX_I64_MAX;
            }
        }
    }

    return a * b;
}

nx_u8 nx_sat_mul_u8(nx_u8 a, nx_u8 b) {
    NX_SAT_MUL_UNSIGNED(nx_u8, nx_u16, a, b, NX_U8_MAX);
}

nx_u16 nx_sat_mul_u16(nx_u16 a, nx_u16 b) {
    NX_SAT_MUL_UNSIGNED(nx_u16, nx_u32, a, b, NX_U16_MAX);
}

nx_u32 nx_sat_mul_u32(nx_u32 a, nx_u32 b) {
    NX_SAT_MUL_UNSIGNED(nx_u32, nx_u64, a, b, NX_U32_MAX);
}

nx_u64 nx_sat_mul_u64(nx_u64 a, nx_u64 b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    if (a > NX_U64_MAX / b) {
        return NX_U64_MAX;
    }
    return a * b;
}

// =============================================================================
// Checked arithmetic (returns false on overflow)
// =============================================================================

#define NX_CKD_ADD_SIGNED(a, b, res)       \
    do {                                      \
        *(res) = (a) + (b);                   \
        if ((b) > 0) return *(res) >= (a);    \
        else return *(res) <= (a);            \
    } while (0)

#define NX_CKD_ADD_UNSIGNED(a, b, res)    \
    do {                                     \
        *(res) = (a) + (b);                  \
        return *(res) >= (a);                \
    } while (0)

#define NX_CKD_SUB_SIGNED(a, b, res)       \
    do {                                      \
        *(res) = (a) - (b);                   \
        if ((b) < 0) return *(res) >= (a);    \
        else return *(res) <= (a);            \
    } while (0)

#define NX_CKD_SUB_UNSIGNED(a, b, res)    \
    do {                                     \
        *(res) = (a) - (b);                  \
        return (a) >= (b);                   \
    } while (0)

#define NX_CKD_MUL_SIGNED(T, T_WIDE, a, b, res, T_MAX, T_MIN)    \
    do {                                                         \
        const T_WIDE wide_res = (T_WIDE)(a) * (T_WIDE)(b);       \
        *(res) = (T)wide_res;                                    \
        return wide_res >= T_MIN && wide_res <= T_MAX;           \
    } while (0)

#define NX_CKD_MUL_UNSIGNED(T, T_WIDE, a, b, res, T_MAX)      \
    do {                                                      \
        const T_WIDE wide_res = (T_WIDE)(a) * (T_WIDE)(b);    \
        *(res) = (T)wide_res;                                 \
        return wide_res <= T_MAX;                             \
    } while (0)


nx_bool nx_ckd_add_i8(nx_i8 a, nx_i8 b, nx_i8 *res) {
    NX_CKD_ADD_SIGNED(a, b, res);
}

nx_bool nx_ckd_add_i16(nx_i16 a, nx_i16 b, nx_i16 *res) {
    NX_CKD_ADD_SIGNED(a, b, res);
}

nx_bool nx_ckd_add_i32(nx_i32 a, nx_i32 b, nx_i32 *res) {
    NX_CKD_ADD_SIGNED(a, b, res);
}

nx_bool nx_ckd_add_i64(nx_i64 a, nx_i64 b, nx_i64 *res) {
    NX_CKD_ADD_SIGNED(a, b, res);
}

nx_bool nx_ckd_add_u8(nx_u8 a, nx_u8 b, nx_u8 *res) {
    NX_CKD_ADD_UNSIGNED(a, b, res);
}

nx_bool nx_ckd_add_u16(nx_u16 a, nx_u16 b, nx_u16 *res) {
    NX_CKD_ADD_UNSIGNED(a, b, res);
}

nx_bool nx_ckd_add_u32(nx_u32 a, nx_u32 b, nx_u32 *res) {
    NX_CKD_ADD_UNSIGNED(a, b, res);
}

nx_bool nx_ckd_add_u64(nx_u64 a, nx_u64 b, nx_u64 *res) {
    NX_CKD_ADD_UNSIGNED(a, b, res);
}

nx_bool nx_ckd_sub_i8(nx_i8 a, nx_i8 b, nx_i8 *res) {
    NX_CKD_SUB_SIGNED(a, b, res);
}

nx_bool nx_ckd_sub_i16(nx_i16 a, nx_i16 b, nx_i16 *res) {
    NX_CKD_SUB_SIGNED(a, b, res);
}

nx_bool nx_ckd_sub_i32(nx_i32 a, nx_i32 b, nx_i32 *res) {
    NX_CKD_SUB_SIGNED(a, b, res);
}

nx_bool nx_ckd_sub_i64(nx_i64 a, nx_i64 b, nx_i64 *res) {
    NX_CKD_SUB_SIGNED(a, b, res);
}

nx_bool nx_ckd_sub_u8(nx_u8 a, nx_u8 b, nx_u8 *res) {
    NX_CKD_SUB_UNSIGNED(a, b, res);
}

nx_bool nx_ckd_sub_u16(nx_u16 a, nx_u16 b, nx_u16 *res) {
    NX_CKD_SUB_UNSIGNED(a, b, res);
}

nx_bool nx_ckd_sub_u32(nx_u32 a, nx_u32 b, nx_u32 *res) {
    NX_CKD_SUB_UNSIGNED(a, b, res);
}

nx_bool nx_ckd_sub_u64(nx_u64 a, nx_u64 b, nx_u64 *res) {
    NX_CKD_SUB_UNSIGNED(a, b, res);
}

nx_bool nx_ckd_mul_i8(nx_i8 a, nx_i8 b, nx_i8 *res) {
    NX_CKD_MUL_SIGNED(nx_i8, nx_i16, a, b, res, NX_I8_MAX, NX_I8_MIN);
}

nx_bool nx_ckd_mul_i16(nx_i16 a, nx_i16 b, nx_i16 *res) {
    NX_CKD_MUL_SIGNED(nx_i16, nx_i32, a, b, res, NX_I16_MAX, NX_I16_MIN);
}

nx_bool nx_ckd_mul_i32(nx_i32 a, nx_i32 b, nx_i32 *res) {
    NX_CKD_MUL_SIGNED(nx_i32, nx_i64, a, b, res, NX_I32_MAX, NX_I32_MIN);
}

nx_bool nx_ckd_mul_i64(nx_i64 a, nx_i64 b, nx_i64 *res) {
    if (a == 0 || b == 0) {
        *res = 0;
        return true;
    }

    if (a == -1) {
        *res = -b;
        return b != NX_I64_MIN; // -1 * INT64_MIN = overflow
    }
    if (b == -1) {
        *res = -a;
        return a != NX_I64_MIN;
    }

    if (a > 0) {
        if (b > 0) {
            if (a > NX_I64_MAX / b) {
                *res = a * b;
                return false;
            }
        } else {
            if (b < NX_I64_MIN / a) {
                *res = a * b;
                return false;
            }
        }
    } else {
        if (b > 0) {
            if (a < NX_I64_MIN / b) {
                *res = a * b;
                return false;
            }
        } else {
            if (b < NX_I64_MAX / a) {
                *res = a * b;
                return false;
            }
        }
    }

    *res = a * b;
    return true;
}

nx_bool nx_ckd_mul_u8(nx_u8 a, nx_u8 b, nx_u8 *res) {
    NX_CKD_MUL_UNSIGNED(nx_u8, nx_u16, a, b, res, NX_U8_MAX);
}

nx_bool nx_ckd_mul_u16(nx_u16 a, nx_u16 b, nx_u16 *res) {
    NX_CKD_MUL_UNSIGNED(nx_u16, nx_u32, a, b, res, NX_U16_MAX);
}

nx_bool nx_ckd_mul_u32(nx_u32 a, nx_u32 b, nx_u32 *res) {
    NX_CKD_MUL_UNSIGNED(nx_u32, nx_u64, a, b, res, NX_U32_MAX);
}

nx_bool nx_ckd_mul_u64(nx_u64 a, nx_u64 b, nx_u64 *res) {
    if (a == 0 || b == 0) {
        *res = 0;
        return true;
    }

    if (a > NX_U64_MAX / b) {
        *res = a * b;
        return false;
    }

    *res = a * b;
    return true;
}
