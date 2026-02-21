#include "nx/numeric/sat.h"

#include "../../include/nx/numeric/limit.h"

#define NX_SAT_ADD_SIGNED(T, a, b, T_MAX, T_MIN)             \
    do {                                                     \
        if ((b) > 0 && (a) > (T_MAX) - (b)) return T_MAX;    \
        if ((b) < 0 && (a) < (T_MIN) - (b)) return T_MIN;    \
        return (a) + (b);                                    \
    } while (0)

#define NX_SAT_ADD_UNSIGNED(T, a, b, T_MAX)    \
    do {                                       \
        const T res = (a) + (b);               \
        return res < (a) ? T_MAX : res;        \
    } while (0)

#define NX_SAT_SUB_SIGNED(T, a, b, T_MAX, T_MIN)            \
    do {                                                    \
       if ((b) < 0 && (a) > (T_MAX) + (b)) return T_MAX;    \
       if ((b) > 0 && (a) < (T_MIN) + (b)) return T_MIN;    \
       return (a) - (b);                                    \
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

nx_i8 nx_sat_mul_i8(nx_i8 a, nx_i8 b) {
    NX_SAT_MUL_SIGNED(nx_i8, nx_i16, a, b, NX_I8_MAX, NX_I8_MIN);
}

nx_i16 nx_sat_mul_i16(nx_i16 a, nx_i16 b) {
    NX_SAT_MUL_SIGNED(nx_i16, nx_i32, a, b, NX_I16_MAX, NX_I16_MIN);
}

nx_i32 nx_sat_mul_i32(nx_i32 a, nx_i32 b) {
    NX_SAT_MUL_SIGNED(nx_i32, nx_i64, a, b, NX_I32_MAX, NX_I32_MIN);
}

nx_i64 nx_sat_mul_i64(nx_i64 a, nx_i64 b) {
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
            if (a == NX_I64_MIN || b == NX_I64_MIN) {
                return NX_I64_MAX;
            }
            if (-a > NX_I64_MAX / -b) {
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