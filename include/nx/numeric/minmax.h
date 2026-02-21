#pragma once

#include "nx/core/type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NX_MIN(a, b) ((a) < (b) ? (a) : (b))
#define NX_MAX(a, b) ((a) > (b) ? (a) : (b))

nx_i8 nx_min_i8(nx_i8 a, nx_i8 b);
nx_i8 nx_max_i8(nx_i8 a, nx_i8 b);

nx_i16 nx_min_i16(nx_i16 a, nx_i16 b);
nx_i16 nx_max_i16(nx_i16 a, nx_i16 b);

nx_i32 nx_min_i32(nx_i32 a, nx_i32 b);
nx_i32 nx_max_i32(nx_i32 a, nx_i32 b);

nx_i64 nx_min_i64(nx_i64 a, nx_i64 b);
nx_i64 nx_max_i64(nx_i64 a, nx_i64 b);

nx_u8 nx_min_u8(nx_u8 a, nx_u8 b);
nx_u8 nx_max_u8(nx_u8 a, nx_u8 b);

nx_u16 nx_min_u16(nx_u16 a, nx_u16 b);
nx_u16 nx_max_u16(nx_u16 a, nx_u16 b);

nx_u32 nx_min_u32(nx_u32 a, nx_u32 b);
nx_u32 nx_max_u32(nx_u32 a, nx_u32 b);

nx_u64 nx_min_u64(nx_u64 a, nx_u64 b);
nx_u64 nx_max_u64(nx_u64 a, nx_u64 b);

nx_f32 nx_min_f32(nx_f32 a, nx_f32 b);
nx_f32 nx_max_f32(nx_f32 a, nx_f32 b);

nx_f64 nx_min_f64(nx_f64 a, nx_f64 b);
nx_f64 nx_max_f64(nx_f64 a, nx_f64 b);

#ifndef __cplusplus

    #define nx_min(a, b) _Generic((a),   \
        nx_i8:  nx_min_i8,               \
        nx_i16: nx_min_i16,              \
        nx_i32: nx_min_i32,              \
        nx_i64: nx_min_i64,              \
        nx_u8:  nx_min_u8,               \
        nx_u16: nx_min_u16,              \
        nx_u32: nx_min_u32,              \
        nx_u64: nx_min_u64,              \
        nx_f32: nx_min_f32,              \
        nx_f64: nx_min_f64               \
    )((a), (b))

    #define nx_max(a, b) _Generic((a),   \
        nx_i8:  nx_max_i8,               \
        nx_i16: nx_max_i16,              \
        nx_i32: nx_max_i32,              \
        nx_i64: nx_max_i64,              \
        nx_u8:  nx_max_u8,               \
        nx_u16: nx_max_u16,              \
        nx_u32: nx_max_u32,              \
        nx_u64: nx_max_u64,              \
        nx_f32: nx_max_f32,              \
        nx_f64: nx_max_f64               \
    )((a), (b))

#else
    // TODO: C++ overloads
#endif

/* ========== clamp ========== */

#define NX_CLAMP(x, min_val, max_val) \
    ((x) < (min_val) ? (min_val) : ((x) > (max_val) ? (max_val) : (x)))

nx_i8 nx_clamp_i8(nx_i8 x, nx_i8 lo, nx_i8 hi);
nx_i16 nx_clamp_i16(nx_i16 x, nx_i16 lo, nx_i16 hi);
nx_i32 nx_clamp_i32(nx_i32 x, nx_i32 lo, nx_i32 hi);
nx_i64 nx_clamp_i64(nx_i64 x, nx_i64 lo, nx_i64 hi);

nx_u8 nx_clamp_u8(nx_u8 x, nx_u8 lo, nx_u8 hi);
nx_u16 nx_clamp_u16(nx_u16 x, nx_u16 lo, nx_u16 hi);
nx_u32 nx_clamp_u32(nx_u32 x, nx_u32 lo, nx_u32 hi);
nx_u64 nx_clamp_u64(nx_u64 x, nx_u64 lo, nx_u64 hi);

nx_f32 nx_clamp_f32(nx_f32 x, nx_f32 lo, nx_f32 hi);
nx_f64 nx_clamp_f64(nx_f64 x, nx_f64 lo, nx_f64 hi);

#ifndef __cplusplus

    #define nx_clamp(x, lo, hi) _Generic((x),    \
        nx_i8:  nx_clamp_i8,                     \
        nx_i16: nx_clamp_i16,                    \
        nx_i32: nx_clamp_i32,                    \
        nx_i64: nx_clamp_i64,                    \
        nx_u8:  nx_clamp_u8,                     \
        nx_u16: nx_clamp_u16,                    \
        nx_u32: nx_clamp_u32,                    \
        nx_u64: nx_clamp_u64,                    \
        nx_f32: nx_clamp_f32,                    \
        nx_f64: nx_clamp_f64                     \
    )((x), (lo), (hi))

#else
    // TODO: C++ overloads
#endif
