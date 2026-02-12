#pragma once

#include "nx/core/type.h"

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// constants
// =============================================================================

constexpr nx_f64 NX_PI       = NX_F64_C(3.14159265358979323846);
constexpr nx_f64 NX_PI_2     = NX_F64_C(1.57079632679489661923);
constexpr nx_f64 NX_PI_4     = NX_F64_C(0.78539816339744830962);
constexpr nx_f64 NX_TAU      = NX_F64_C(6.28318530717958647692);
constexpr nx_f64 NX_E        = NX_F64_C(2.71828182845904523536);
constexpr nx_f64 NX_SQRT2    = NX_F64_C(1.41421356237309504880);
constexpr nx_f64 NX_SQRT3    = NX_F64_C(1.73205080756887729353);
constexpr nx_f64 NX_LN2      = NX_F64_C(0.69314718055994530942);
constexpr nx_f64 NX_LN10     = NX_F64_C(2.30258509299404568402);

constexpr nx_f32 NX_PI_F     = NX_F32_C(3.14159265358979323846);
constexpr nx_f32 NX_PI_2_F   = NX_F32_C(1.57079632679489661923);
constexpr nx_f32 NX_E_F      = NX_F32_C(2.71828182845904523536);

// =============================================================================
// min/max
// =============================================================================

#define NX_MIN(a, b) ((a) < (b) ? (a) : (b))
#define NX_MAX(a, b) ((a) > (b) ? (a) : (b))

nx_i8 mx_min_i8(nx_i8 a, nx_i8 b);
nx_i8 mx_max_i8(nx_i8 a, nx_i8 b);

nx_i16 mx_min_i16(nx_i16 a, nx_i16 b);
nx_i16 mx_max_i16(nx_i16 a, nx_i16 b);

nx_i32 mx_min_i32(nx_i32 a, nx_i32 b);
nx_i32 mx_max_i32(nx_i32 a, nx_i32 b);

nx_i64 mx_min_i64(nx_i64 a, nx_i64 b);
nx_i64 mx_max_i64(nx_i64 a, nx_i64 b);

nx_u8 mx_min_u8(nx_i8 a, nx_i8 b);
nx_u8 mx_max_u8(nx_i8 a, nx_i8 b);

nx_u16 mx_min_u16(nx_u16 a, nx_u16 b);
nx_u16 mx_max_u16(nx_u16 a, nx_u16 b);

nx_u32 mx_min_u32(nx_u32 a, nx_u32 b);
nx_u32 mx_max_u32(nx_u32 a, nx_u32 b);

nx_u64 mx_min_u64(nx_u64 a, nx_u64 b);
nx_u64 mx_max_u64(nx_u64 a, nx_u64 b);

nx_f32 mx_min_f32(nx_f32 a, nx_f32 b);
nx_f32 mx_max_f32(nx_f32 a, nx_f32 b);

nx_f64 mx_min_f64(nx_f64 a, nx_f64 b);
nx_f64 mx_max_f64(nx_f64 a, nx_f64 b);

// =============================================================================
// clamp
// =============================================================================

#define NX_CLAMP(x, min_val, max_val) \
    ((x) < (min_val) ? (min_val) : ((x) > (max_val) ? (max_val) : (x)))

#ifdef __cplusplus
}
#endif
