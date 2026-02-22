#pragma once

#include "nx/core/type.h"

/// returns -1 if x < 0, 0 if x == 0, 1 if x > 0.

nx_i8 nx_sign_i8(nx_i8 x);
nx_i16 nx_sign_i16(nx_i16 x);
nx_i32 nx_sign_i32(nx_i32 x);
nx_i64 nx_sign_i64(nx_i64 x);

/// returns 0 if x == 0, 1 if x > 0.

nx_u8 nx_sign_u8(nx_u8 x);
nx_u16 nx_sign_u16(nx_u16 x);
nx_u32 nx_sign_u32(nx_u32 x);
nx_u64 nx_sign_u64(nx_u64 x);

/// returns -1.0 if x < 0, 0.0 if x == 0, 1.0 if x > 0.
/// NaN returns NaN.

nx_f32 nx_sign_f32(nx_f32 x);
nx_f64 nx_sign_f64(nx_f64 x);

#define nx_sign(x) _Generic((x), \
    nx_i8:  nx_sign_i8,          \
    nx_i16: nx_sign_i16,         \
    nx_i32: nx_sign_i32,         \
    nx_i64: nx_sign_i64,         \
    nx_u8:  nx_sign_u8,          \
    nx_u16: nx_sign_u16,         \
    nx_u32: nx_sign_u32,         \
    nx_u64: nx_sign_u64,         \
    nx_f32: nx_sign_f32,         \
    nx_f64: nx_sign_f64          \
)(x)
