#pragma once

#include "nx/core/type.h"

nx_i8 nx_abs_i8(nx_i8 x);
nx_i16 nx_abs_i16(nx_i16 x);
nx_i32 nx_abs_i32(nx_i32 x);
nx_i64 nx_abs_i64(nx_i64 x);

nx_f32 nx_abs_f32(nx_f32 x);
nx_f64 nx_abs_f64(nx_f64 x);

#define nx_abs(x) _Generic((x), \
    nx_i8:  nx_abs_i8,          \
    nx_i16: nx_abs_i16,         \
    nx_i32: nx_abs_i32,         \
    nx_i64: nx_abs_i64,         \
    nx_f32: nx_abs_f32,         \
    nx_f64: nx_abs_f64          \
)(x)
