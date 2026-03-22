#include "nx/numeric/fp.h"

#include <math.h>

#include "nx/core/assert.h"

/* ========== lerp ========== */

nx_f32 nx_lerp_f32(nx_f32 a, nx_f32 b, nx_f32 t) {
    return a + t * (b - a);
}

nx_f32 nx_lerp_f64(nx_f64 a, nx_f64 b, nx_f64 t) {
    return a + t * (b - a);
}

/* ========== inverse lerp ========== */

nx_f32 nx_inv_lerp_f32(nx_f32 a, nx_f32 b, nx_f32 val) {
    NX_ASSERT(a != b);

    return (val - a) / (b - a);
}

nx_f64 nx_inv_lerp_f64(nx_f64 a, nx_f64 b, nx_f64 val) {
    NX_ASSERT(a != b);

    return (val - a) / (b - a);
}

/* ========== approximate equality ========== */

nx_bool nx_approx_eq_f32(nx_f32 a, nx_f32 b, nx_f32 eps) {
    NX_ASSERT(eps >= 0.0f);

    return fabsf(a - b) <= eps;
}

nx_bool nx_approx_eq_f64(nx_f64 a, nx_f64 b, nx_f64 eps) {
    NX_ASSERT(eps >= 0.0);

    return fabs(a - b) <= eps;
}
