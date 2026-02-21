#pragma once

#include "nx/core/type.h"

nx_i8 nx_sat_add_i8(nx_i8 a, nx_i8 b);
nx_i16 nx_sat_add_i16(nx_i16 a, nx_i16 b);
nx_i32 nx_sat_add_i32(nx_i32 a, nx_i32 b);
nx_i64 nx_sat_add_i64(nx_i64 a, nx_i64 b);

nx_u8 nx_sat_add_u8(nx_u8 a, nx_u8 b);
nx_u16 nx_sat_add_u16(nx_u16 a, nx_u16 b);
nx_u32 nx_sat_add_u32(nx_u32 a, nx_u32 b);
nx_u64 nx_sat_add_u64(nx_u64 a, nx_u64 b);

nx_i8 nx_sat_sub_i8(nx_i8 a, nx_i8 b);
nx_i16 nx_sat_sub_i16(nx_i16 a, nx_i16 b);
nx_i32 nx_sat_sub_i32(nx_i32 a, nx_i32 b);
nx_i64 nx_sat_sub_i64(nx_i64 a, nx_i64 b);

nx_u8 nx_sat_sub_u8(nx_u8 a, nx_u8 b);
nx_u16 nx_sat_sub_u16(nx_u16 a, nx_u16 b);
nx_u32 nx_sat_sub_u32(nx_u32 a, nx_u32 b);
nx_u64 nx_sat_sub_u64(nx_u64 a, nx_u64 b);

nx_i8 nx_sat_mul_i8(nx_i8 a, nx_i8 b);
nx_i16 nx_sat_mul_i16(nx_i16 a, nx_i16 b);
nx_i32 nx_sat_mul_i32(nx_i32 a, nx_i32 b);
nx_i64 nx_sat_mul_i64(nx_i64 a, nx_i64 b);

nx_u8 nx_sat_mul_u8(nx_u8 a, nx_u8 b);
nx_u16 nx_sat_mul_u16(nx_u16 a, nx_u16 b);
nx_u32 nx_sat_mul_u32(nx_u32 a, nx_u32 b);
nx_u64 nx_sat_mul_u64(nx_u64 a, nx_u64 b);

#ifndef __cplusplus

    #define nx_sat_add(a, b) _Generic((a),    \
        nx_i8:  nx_sat_add_i8,                \
        nx_i16: nx_sat_add_i16,               \
        nx_i32: nx_sat_add_i32,               \
        nx_i64: nx_sat_add_i64,               \
        nx_u8:  nx_sat_add_u8,                \
        nx_u16: nx_sat_add_u16,               \
        nx_u32: nx_sat_add_u32,               \
        nx_u64: nx_sat_add_u64                \
    )((a), (b))

    #define nx_sat_sub(a, b) _Generic((a),    \
        nx_i8:  nx_sat_sub_i8,                \
        nx_i16: nx_sat_sub_i16,               \
        nx_i32: nx_sat_sub_i32,               \
        nx_i64: nx_sat_sub_i64,               \
        nx_u8:  nx_sat_sub_u8,                \
        nx_u16: nx_sat_sub_u16,               \
        nx_u32: nx_sat_sub_u32,               \
        nx_u64: nx_sat_sub_u64                \
    )((a), (b))

    #define nx_sat_mul(a, b) _Generic((a),    \
        nx_i8:  nx_sat_mul_i8,                \
        nx_i16: nx_sat_mul_i16,               \
        nx_i32: nx_sat_mul_i32,               \
        nx_i64: nx_sat_mul_i64,               \
        nx_u8:  nx_sat_mul_u8,                \
        nx_u16: nx_sat_mul_u16,               \
        nx_u32: nx_sat_mul_u32,               \
        nx_u64: nx_sat_mul_u64                \
    )((a), (b))

#else
    // TODO: C++ overloads
#endif
