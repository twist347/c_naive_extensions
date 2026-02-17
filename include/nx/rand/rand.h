#pragma once

#include "nx/core/type.h"
#include "nx/core/span.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nx_rng nx_rng;

nx_rng *nx_rng_new(nx_u64 seed);
nx_rng *nx_rng_drop(nx_rng *r);

nx_f32 nx_rng_f32(nx_rng *r);
nx_f64 nx_rng_f64(nx_rng *r);

// Fisher–Yates
void nx_rng_shuffle_span(nx_rng *r, nx_span s);

#ifdef __cplusplus
}
#endif
