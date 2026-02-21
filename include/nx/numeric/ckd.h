#pragma once

#include "nx/core/type.h"

#ifdef __cplusplus
extern "C" {
#endif

nx_bool nx_ckd_add_i8(nx_i8 *res, nx_i8 a, nx_i8 b);
nx_bool nx_ckd_add_i16(nx_i16 *res, nx_i16 a, nx_i16 b);
nx_bool nx_ckd_add_i32(nx_i32 *res, nx_i32 a, nx_i32 b);
nx_bool nx_ckd_add_i64(nx_i64 *res, nx_i64 a, nx_i64 b);

nx_bool nx_ckd_add_u8(nx_u8 *res, nx_u8 a, nx_u8 b);
nx_bool nx_ckd_add_u16(nx_u16 *res, nx_u16 a, nx_u16 b);
nx_bool nx_ckd_add_u32(nx_u32 *res, nx_u32 a, nx_u32 b);
nx_bool nx_ckd_add_u64(nx_u64 *res, nx_u64 a, nx_u64 b);

nx_bool nx_ckd_sub_i8(nx_i8 *res, nx_i8 a, nx_i8 b);
nx_bool nx_ckd_sub_i16(nx_i16 *res, nx_i16 a, nx_i16 b);
nx_bool nx_ckd_sub_i32(nx_i32 *res, nx_i32 a, nx_i32 b);
nx_bool nx_ckd_sub_i64(nx_i64 *res, nx_i64 a, nx_i64 b);

nx_bool nx_ckd_sub_u8(nx_u8 *res, nx_u8 a, nx_u8 b);
nx_bool nx_ckd_sub_u16(nx_u16 *res, nx_u16 a, nx_u16 b);
nx_bool nx_ckd_sub_u32(nx_u32 *res, nx_u32 a, nx_u32 b);
nx_bool nx_ckd_sub_u64(nx_u64 *res, nx_u64 a, nx_u64 b);

nx_bool nx_ckd_mul_i8(nx_i8 *res, nx_i8 a, nx_i8 b);
nx_bool nx_ckd_mul_i16(nx_i16 *res, nx_i16 a, nx_i16 b);
nx_bool nx_ckd_mul_i32(nx_i32 *res, nx_i32 a, nx_i32 b);
nx_bool nx_ckd_mul_i64(nx_i64 *res, nx_i64 a, nx_i64 b);

nx_bool nx_ckd_mul_u8(nx_u8 *res, nx_u8 a, nx_u8 b);
nx_bool nx_ckd_mul_u16(nx_u16 *res, nx_u16 a, nx_u16 b);
nx_bool nx_ckd_mul_u32(nx_u32 *res, nx_u32 a, nx_u32 b);
nx_bool nx_ckd_mul_u64(nx_u64 *res, nx_u64 a, nx_u64 b);

#ifndef __cplusplus

    #define nx_ckd_add(res, a, b) _Generic((a), \
        nx_i8:  nx_ckd_add_i8,                  \
        nx_i16: nx_ckd_add_i16,                 \
        nx_i32: nx_ckd_add_i32,                 \
        nx_i64: nx_ckd_add_i64,                 \
        nx_u8:  nx_ckd_add_u8,                  \
        nx_u16: nx_ckd_add_u16,                 \
        nx_u32: nx_ckd_add_u32,                 \
        nx_u64: nx_ckd_add_u64                  \
    )((res), (a), (b))

    #define nx_ckd_sub(res, a, b) _Generic((a), \
        nx_i8:  nx_ckd_sub_i8,                  \
        nx_i16: nx_ckd_sub_i16,                 \
        nx_i32: nx_ckd_sub_i32,                 \
        nx_i64: nx_ckd_sub_i64,                 \
        nx_u8:  nx_ckd_sub_u8,                  \
        nx_u16: nx_ckd_sub_u16,                 \
        nx_u32: nx_ckd_sub_u32,                 \
        nx_u64: nx_ckd_sub_u64                  \
    )((res), (a), (b))

    #define nx_ckd_mul(res, a, b) _Generic((a), \
        nx_i8:  nx_ckd_mul_i8,                  \
        nx_i16: nx_ckd_mul_i16,                 \
        nx_i32: nx_ckd_mul_i32,                 \
        nx_i64: nx_ckd_mul_i64,                 \
        nx_u8:  nx_ckd_mul_u8,                  \
        nx_u16: nx_ckd_mul_u16,                 \
        nx_u32: nx_ckd_mul_u32,                 \
        nx_u64: nx_ckd_mul_u64                  \
    )((res), (a), (b))

#else /* __cplusplus */
    // TODO: C++ overloads

#endif


#ifdef __cplusplus
}
#endif
