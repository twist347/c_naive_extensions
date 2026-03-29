#pragma once

#include "nx/core/type.h"

/* ========== lerp ========== */

/// linear interpolation: a + t * (b - a). t=0 returns a, t=1 returns b.
nx_f32 nx_lerp_f32(nx_f32 a, nx_f32 b, nx_f32 t);
nx_f32 nx_lerp_f64(nx_f64 a, nx_f64 b, nx_f64 t);

#define nx_lerp(a, b, t) _Generic((a), \
    nx_f32: nx_lerp_f32,               \
    nx_f64: nx_lerp_f64                \
)((a), (b), (t))

/* ========== inverse lerp ========== */

/// inverse lerp: returns t such that lerp(a, b, t) == val. a must != b.
nx_f32 nx_inv_lerp_f32(nx_f32 a, nx_f32 b, nx_f32 val);
nx_f64 nx_inv_lerp_f64(nx_f64 a, nx_f64 b, nx_f64 val);

#define nx_inv_lerp(a, b, val) _Generic((a), \
    nx_f32: nx_inv_lerp_f32,                 \
    nx_f64: nx_inv_lerp_f64                  \
)((a), (b), (val))

/* ========== approximate equality ========== */

/// true if |a - b| <= eps
nx_bool nx_approx_eq_f32(nx_f32 a, nx_f32 b, nx_f32 eps);
nx_bool nx_approx_eq_f64(nx_f64 a, nx_f64 b, nx_f64 eps);

#define nx_approx_eq(a, b, eps) _Generic((a), \
    nx_f32: nx_approx_eq_f32,                 \
    nx_f64: nx_approx_eq_f64                  \
)((a), (b), (eps))
