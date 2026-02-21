#include "nx/numeric/minmax.h"

#include "nx/core/assert.h"

nx_i8 nx_min_i8(nx_i8 a, nx_i8 b) {
    return NX_MIN(a, b);
}

nx_i8 nx_max_i8(nx_i8 a, nx_i8 b) {
    return NX_MAX(a, b);
}

nx_i16 nx_min_i16(nx_i16 a, nx_i16 b) {
    return NX_MIN(a, b);
}

nx_i16 nx_max_i16(nx_i16 a, nx_i16 b) {
    return NX_MAX(a, b);
}

nx_i32 nx_min_i32(nx_i32 a, nx_i32 b) {
    return NX_MIN(a, b);
}

nx_i32 nx_max_i32(nx_i32 a, nx_i32 b) {
    return NX_MAX(a, b);
}

nx_i64 nx_min_i64(nx_i64 a, nx_i64 b) {
    return NX_MIN(a, b);
}

nx_i64 nx_max_i64(nx_i64 a, nx_i64 b) {
    return NX_MAX(a, b);
}

nx_u8 nx_min_u8(nx_u8 a, nx_u8 b) {
    return NX_MIN(a, b);
}

nx_u8 nx_max_u8(nx_u8 a, nx_u8 b) {
    return NX_MAX(a, b);
}

nx_u16 nx_min_u16(nx_u16 a, nx_u16 b) {
    return NX_MIN(a, b);
}

nx_u16 nx_max_u16(nx_u16 a, nx_u16 b) {
    return NX_MAX(a, b);
}

nx_u32 nx_min_u32(nx_u32 a, nx_u32 b) {
    return NX_MIN(a, b);
}

nx_u32 nx_max_u32(nx_u32 a, nx_u32 b) {
    return NX_MAX(a, b);
}

nx_u64 nx_min_u64(nx_u64 a, nx_u64 b) {
    return NX_MIN(a, b);
}

nx_u64 nx_max_u64(nx_u64 a, nx_u64 b) {
    return NX_MAX(a, b);
}

nx_f32 nx_min_f32(nx_f32 a, nx_f32 b) {
    return NX_MIN(a, b);
}

nx_f32 nx_max_f32(nx_f32 a, nx_f32 b) {
    return NX_MAX(a, b);
}

nx_f64 nx_min_f64(nx_f64 a, nx_f64 b) {
    return NX_MIN(a, b);
}

nx_f64 nx_max_f64(nx_f64 a, nx_f64 b) {
    return NX_MAX(a, b);
}

/* ========== clamp ========== */

nx_i8 nx_clamp_i8(nx_i8 x, nx_i8 lo, nx_i8 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}

nx_i16 nx_clamp_i16(nx_i16 x, nx_i16 lo, nx_i16 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}

nx_i32 nx_clamp_i32(nx_i32 x, nx_i32 lo, nx_i32 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}

nx_i64 nx_clamp_i64(nx_i64 x, nx_i64 lo, nx_i64 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}

nx_u8 nx_clamp_u8(nx_u8 x, nx_u8 lo, nx_u8 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}

nx_u16 nx_clamp_u16(nx_u16 x, nx_u16 lo, nx_u16 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}

nx_u32 nx_clamp_u32(nx_u32 x, nx_u32 lo, nx_u32 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}

nx_u64 nx_clamp_u64(nx_u64 x, nx_u64 lo, nx_u64 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}

nx_f32 nx_clamp_f32(nx_f32 x, nx_f32 lo, nx_f32 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}

nx_f64 nx_clamp_f64(nx_f64 x, nx_f64 lo, nx_f64 hi) {
    NX_ASSERT(lo <= hi);

    return NX_CLAMP(x, lo, hi);
}
