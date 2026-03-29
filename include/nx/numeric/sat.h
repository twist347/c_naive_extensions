#pragma once

#include "nx/core/type.h"

/// saturating arithmetic: clamps to T_MIN/T_MAX on overflow instead of wrapping.
nx_i8 nx_sat_add_i8(nx_i8 a, nx_i8 b);
nx_i16 nx_sat_add_i16(nx_i16 a, nx_i16 b);
nx_i32 nx_sat_add_i32(nx_i32 a, nx_i32 b);
nx_i64 nx_sat_add_i64(nx_i64 a, nx_i64 b);

nx_u8 nx_sat_add_u8(nx_u8 a, nx_u8 b);
nx_u16 nx_sat_add_u16(nx_u16 a, nx_u16 b);
nx_u32 nx_sat_add_u32(nx_u32 a, nx_u32 b);
nx_u64 nx_sat_add_u64(nx_u64 a, nx_u64 b);

nx_usize nx_sat_add_usize(nx_usize a, nx_usize b);
nx_isize nx_sat_add_isize(nx_isize a, nx_isize b);

nx_i8 nx_sat_sub_i8(nx_i8 a, nx_i8 b);
nx_i16 nx_sat_sub_i16(nx_i16 a, nx_i16 b);
nx_i32 nx_sat_sub_i32(nx_i32 a, nx_i32 b);
nx_i64 nx_sat_sub_i64(nx_i64 a, nx_i64 b);

nx_u8 nx_sat_sub_u8(nx_u8 a, nx_u8 b);
nx_u16 nx_sat_sub_u16(nx_u16 a, nx_u16 b);
nx_u32 nx_sat_sub_u32(nx_u32 a, nx_u32 b);
nx_u64 nx_sat_sub_u64(nx_u64 a, nx_u64 b);

nx_usize nx_sat_sub_usize(nx_usize a, nx_usize b);
nx_isize nx_sat_sub_isize(nx_isize a, nx_isize b);

nx_i8 nx_sat_mul_i8(nx_i8 a, nx_i8 b);
nx_i16 nx_sat_mul_i16(nx_i16 a, nx_i16 b);
nx_i32 nx_sat_mul_i32(nx_i32 a, nx_i32 b);
nx_i64 nx_sat_mul_i64(nx_i64 a, nx_i64 b);

nx_u8 nx_sat_mul_u8(nx_u8 a, nx_u8 b);
nx_u16 nx_sat_mul_u16(nx_u16 a, nx_u16 b);
nx_u32 nx_sat_mul_u32(nx_u32 a, nx_u32 b);
nx_u64 nx_sat_mul_u64(nx_u64 a, nx_u64 b);

nx_usize nx_sat_mul_usize(nx_usize a, nx_usize b);
nx_isize nx_sat_mul_isize(nx_isize a, nx_isize b);

#define nx_sat_add(a, b) _Generic((a), \
    nx_i8:  nx_sat_add_i8,             \
    nx_i16: nx_sat_add_i16,            \
    nx_i32: nx_sat_add_i32,            \
    nx_i64: nx_sat_add_i64,            \
    nx_u8:  nx_sat_add_u8,             \
    nx_u16: nx_sat_add_u16,            \
    nx_u32: nx_sat_add_u32,            \
    nx_u64: nx_sat_add_u64             \
)((a), (b))

#define nx_sat_sub(a, b) _Generic((a), \
    nx_i8:  nx_sat_sub_i8,             \
    nx_i16: nx_sat_sub_i16,            \
    nx_i32: nx_sat_sub_i32,            \
    nx_i64: nx_sat_sub_i64,            \
    nx_u8:  nx_sat_sub_u8,             \
    nx_u16: nx_sat_sub_u16,            \
    nx_u32: nx_sat_sub_u32,            \
    nx_u64: nx_sat_sub_u64             \
)((a), (b))

#define nx_sat_mul(a, b) _Generic((a), \
    nx_i8:  nx_sat_mul_i8,             \
    nx_i16: nx_sat_mul_i16,            \
    nx_i32: nx_sat_mul_i32,            \
    nx_i64: nx_sat_mul_i64,            \
    nx_u8:  nx_sat_mul_u8,             \
    nx_u16: nx_sat_mul_u16,            \
    nx_u32: nx_sat_mul_u32,            \
    nx_u64: nx_sat_mul_u64             \
)((a), (b))
