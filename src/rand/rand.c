#include "nx/rand/rand.h"

#include <stdlib.h>

#include "nx/core/limits.h"

/* ========== state ========== */

struct nx_Rng {
    nx_u64 s[4];
};

/* ========== SplitMix64 (for seed expansion) ========== */

static nx_u64 splitmix64(nx_u64 *state);

/* ========== xoshiro256** core ========== */

static nx_u64 rotl(nx_u64 x, nx_i32 k);

static nx_u64 next(nx_Rng *self);

/* ========== lifetime ========== */

nx_Rng *nx_rng_new(nx_u64 seed) {
    nx_Rng *r = malloc(sizeof(nx_Rng));
    if (!r) {
        return nx_null;
    }

    // expand single u64 seed into 256-bit state via SplitMix64
    nx_u64 sm = seed;
    r->s[0] = splitmix64(&sm);
    r->s[1] = splitmix64(&sm);
    r->s[2] = splitmix64(&sm);
    r->s[3] = splitmix64(&sm);

    return r;
}

void nx_rng_drop(nx_Rng *self) {
    free(self);
}

/* ========== raw generation ========== */

nx_u64 nx_rng_u64(nx_Rng *self) {
    NX_ASSERT(self);

    return next(self);
}

nx_u32 nx_rng_u32(nx_Rng *self) {
    NX_ASSERT(self);

    return (nx_u32) (next(self) >> 32);
}

/* ========== bounded integers ========== */

nx_u64 nx_rng_u64_max(nx_Rng *self, nx_u64 max) {
    NX_ASSERT(self);

    if (max == NX_U64_MAX) {
        return next(self);
    }

    // Lemire's nearly divisionless method
    const nx_u64 range = max + 1;
    const nx_u64 threshold = (-range) % range; // 2^64 mod range

    while (true) {
        const nx_u64 x = next(self);
        if (x >= threshold) {
            return x % range;
        }
    }
}

nx_u32 nx_rng_u32_max(nx_Rng *self, nx_u32 max) {
    NX_ASSERT(self);

    if (max == NX_U32_MAX) {
        return nx_rng_u32(self);
    }

    const nx_u32 range = max + 1;
    const nx_u32 threshold = (-range) % range;

    while (true) {
        const nx_u32 x = nx_rng_u32(self);
        if (x >= threshold) {
            return x % range;
        }
    }
}

nx_i32 nx_rng_i32_range(nx_Rng *self, nx_i32 lo, nx_i32 hi) {
    NX_ASSERT(self);
    NX_ASSERT(lo <= hi);

    const nx_u32 range = (nx_u32) hi - (nx_u32) lo;
    return lo + (nx_i32) nx_rng_u32_max(self, range);
}

nx_i64 nx_rng_i64_range(nx_Rng *self, nx_i64 lo, nx_i64 hi) {
    NX_ASSERT(self);
    NX_ASSERT(lo <= hi);

    const nx_u64 range = (nx_u64) hi - (nx_u64) lo;
    return lo + (nx_i64) nx_rng_u64_max(self, range);
}

/* ========== floating point ========== */

nx_f64 nx_rng_f64(nx_Rng *self) {
    NX_ASSERT(self);

    // top 53 bits → [0, 2^53) → divide by 2^53 → [0.0, 1.0)
    return (nx_f64) (next(self) >> 11) * NX_F64_C(0x1.0p-53);
}

nx_f32 nx_rng_f32(nx_Rng *self) {
    NX_ASSERT(self);

    // top 24 bits → [0, 2^24) → divide by 2^24 → [0.0, 1.0)
    return (nx_f32) (next(self) >> 40) * NX_F32_C(0x1.0p-24);
}

nx_f64 nx_rng_f64_range(nx_Rng *self, nx_f64 lo, nx_f64 hi) {
    NX_ASSERT(self);
    NX_ASSERT(lo <= hi);

    return lo + nx_rng_f64(self) * (hi - lo);
}

nx_f32 nx_rng_f32_range(nx_Rng *self, nx_f32 lo, nx_f32 hi) {
    NX_ASSERT(self);
    NX_ASSERT(lo <= hi);

    return lo + nx_rng_f32(self) * (hi - lo);
}

/* ========== bool ========== */

nx_bool nx_rng_bool(nx_Rng *self) {
    NX_ASSERT(self);
    return (next(self) >> 63) != 0;
}

/* ========== shuffle ========== */

void nx_rng_shuffle(nx_Rng *self, nx_Span s) {
    NX_ASSERT(self);
    NX_SPAN_ANY_ASSERT(s);

    if (s.len < 2) {
        return;
    }

    for (nx_usize i = s.len - 1; i > 0; --i) {
        const nx_usize j = nx_rng_u64_max(self, i);
        nx_span_swap(s, i, j);
    }
}

// private defs

static nx_u64 splitmix64(nx_u64 *state) {
    nx_u64 z = (*state += NX_U64_C(0x9e3779b97f4a7c15));
    z = (z ^ (z >> 30)) * NX_U64_C(0xbf58476d1ce4e5b9);
    z = (z ^ (z >> 27)) * NX_U64_C(0x94d049bb133111eb);
    return z ^ (z >> 31);
}

static nx_u64 rotl(nx_u64 x, nx_i32 k) {
    return (x << k) | (x >> (64 - k));
}

static nx_u64 next(nx_Rng *self) {
    const nx_u64 result = rotl(self->s[1] * 5, 7) * 9;

    const nx_u64 t = self->s[1] << 17;

    self->s[2] ^= self->s[0];
    self->s[3] ^= self->s[1];
    self->s[1] ^= self->s[2];
    self->s[0] ^= self->s[3];

    self->s[2] ^= t;

    self->s[3] = rotl(self->s[3], 45);

    return result;
}
