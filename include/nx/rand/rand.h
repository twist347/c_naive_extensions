#pragma once

#include "nx/core/type.h"
#include "nx/ds/span.h"

typedef struct nx_Rng nx_Rng;

/* ========== lifetime ========== */

nx_Rng *nx_rng_new(nx_u64 seed);
void nx_rng_drop(nx_Rng *self);

/* ========== raw generation ========== */

nx_u64 nx_rng_u64(nx_Rng *self);
nx_u32 nx_rng_u32(nx_Rng *self);

/* ========== bounded integers ========== */

/// uniform in [0, max] inclusive
nx_u64 nx_rng_u64_max(nx_Rng *self, nx_u64 max);
nx_u32 nx_rng_u32_max(nx_Rng *self, nx_u32 max);

/// uniform in [lo, hi] inclusive
nx_i32 nx_rng_i32_range(nx_Rng *self, nx_i32 lo, nx_i32 hi);
nx_i64 nx_rng_i64_range(nx_Rng *self, nx_i64 lo, nx_i64 hi);

/* ========== floating point ========== */

/// uniform in [0.0, 1.0)
nx_f32 nx_rng_f32(nx_Rng *self);
nx_f64 nx_rng_f64(nx_Rng *self);

/// uniform in [lo, hi)
nx_f32 nx_rng_f32_range(nx_Rng *self, nx_f32 lo, nx_f32 hi);
nx_f64 nx_rng_f64_range(nx_Rng *self, nx_f64 lo, nx_f64 hi);

/* ========== bool ========== */

/// true with probability 0.5
nx_bool nx_rng_bool(nx_Rng *self);

/* ========== shuffle ========== */

/// Fisher-Yates shuffle in-place
void nx_rng_shuffle(nx_Rng *self, nx_Span s);
