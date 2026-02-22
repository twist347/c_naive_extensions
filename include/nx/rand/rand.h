#pragma once

#include "nx/core/type.h"
#include "nx/ds/span.h"

typedef struct nx_rng nx_rng;

nx_rng *nx_rng_new(nx_u64 seed);
nx_rng *nx_rng_drop(nx_rng *r);

nx_f32 nx_rng_f32(nx_rng *r);
nx_f64 nx_rng_f64(nx_rng *r);

// TODO: maybe move to algo/transform
// fisher–Yates
void nx_rng_shuffle_span(nx_rng *r, nx_span s);
