#include "nx/core/hash.h"

#include <math.h>
#include <string.h>

#include "nx/core/assert.h"

// FNV-1a 64-bit
static constexpr nx_u64 NX_FNV_OFFSET_BASIS_64 = NX_U64_C(14695981039346656037);
static constexpr nx_u64 NX_FNV_PRIME_64 = NX_U64_C(1099511628211);

// MurmurHash3 fmix64 constants
static constexpr nx_u64 NX_FMIX64_C1 = NX_U64_C(0xff51afd7ed558ccd);
static constexpr nx_u64 NX_FMIX64_C2 = NX_U64_C(0xc4ceb9fe1a85ec53);

static constexpr nx_u64 NX_HASH_GOLDEN_64 = NX_U64_C(0x9e3779b97f4a7c15);

// internal decls

static nx_Hash hash_mix64(nx_u64 a);

/* ========== integer hashing ========== */

nx_Hash nx_hash_i8(nx_i8 x) {
    return hash_mix64((nx_u8) x);
}

nx_Hash nx_hash_i16(nx_i16 x) {
    return hash_mix64((nx_u16) x);
}

nx_Hash nx_hash_i32(nx_i32 x) {
    return hash_mix64((nx_u32) x);
}

nx_Hash nx_hash_i64(nx_i64 x) {
    return hash_mix64((nx_u64) x);
}

nx_Hash nx_hash_u8(nx_u8 x) {
    return hash_mix64(x);
}

nx_Hash nx_hash_u16(nx_u16 x) {
    return hash_mix64(x);
}

nx_Hash nx_hash_u32(nx_u32 x) {
    return hash_mix64(x);
}

nx_Hash nx_hash_u64(nx_u64 x) {
    return hash_mix64(x);
}

nx_Hash nx_hash_usize(nx_usize x) {
    return hash_mix64(x);
}

nx_Hash nx_hash_isize(nx_isize x) {
    return hash_mix64((nx_u64) x);
}

/* ========== float hashing ========== */

nx_Hash nx_hash_f32(nx_f32 x) {
    // normalize: +0 == -0, all NaN -> canonical

    // collapse -0 -> +0
    if (x == 0.0f) {
        x = 0.0f;
    }
    // canonical NaN bits
    if (isnan(x)) {
        x = NX_F32_C(0.0);
    }

    nx_u32 bits;
    memcpy(&bits, &x, sizeof(bits));
    return hash_mix64(bits);
}

nx_Hash nx_hash_f64(nx_f64 x) {
    if (x == 0.0) {
        x = 0.0;
    }
    if (isnan(x)) {
        x = NX_F64_C(0.0);
    }

    nx_u64 bits;
    memcpy(&bits, &x, sizeof(bits));
    return hash_mix64(bits);
}

/* ========== byte / string / pointer hashing ========== */

nx_Hash nx_hash_bytes(const void *data, nx_usize len) {
    if (len == 0) {
        return nx_hash_u64(NX_FNV_OFFSET_BASIS_64);
    }

    NX_ASSERT(data);

    const nx_u8 *p = data;
    nx_u64 h = NX_FNV_OFFSET_BASIS_64;

    for (nx_usize i = 0; i < len; ++i) {
        h ^= p[i];
        h *= NX_FNV_PRIME_64;
    }

    return nx_hash_u64(h);
}

nx_Hash nx_hash_cstr(const nx_char *s) {
    NX_ASSERT(s);

    const nx_u8 *p = (const nx_u8 *) s;
    nx_u64 h = NX_FNV_OFFSET_BASIS_64;

    while (*p) {
        h ^= *p++;
        h *= NX_FNV_PRIME_64;
    }
    return nx_hash_u64(h);
}

nx_Hash nx_hash_ptr(const void *ptr) {
    return hash_mix64((nx_uptr) ptr);
}

/* ========== combine ========== */

nx_Hash nx_hash_combine(nx_Hash h, nx_Hash x) {
    nx_Hash v = h;
    v ^= x + NX_HASH_GOLDEN_64 + (v << 6) + (v >> 2);
    return hash_mix64(v);
}

/* ========== nx_hash_fn-compatible callbacks ========== */

#define NX_HASHFN_DEF(NAME, TYPE)                   \
    nx_Hash nx_hash_fn_##NAME(const void *key) {     \
        NX_ASSERT(key);                             \
        return nx_hash_##NAME(*(const TYPE *) key); \
    }

NX_HASHFN_DEF(i8,    nx_i8)
NX_HASHFN_DEF(i16,   nx_i16)
NX_HASHFN_DEF(i32,   nx_i32)
NX_HASHFN_DEF(i64,   nx_i64)

NX_HASHFN_DEF(u8,    nx_u8)
NX_HASHFN_DEF(u16,   nx_u16)
NX_HASHFN_DEF(u32,   nx_u32)
NX_HASHFN_DEF(u64,   nx_u64)

NX_HASHFN_DEF(usize, nx_usize)
NX_HASHFN_DEF(isize, nx_isize)

NX_HASHFN_DEF(f32,   nx_f32)
NX_HASHFN_DEF(f64,   nx_f64)

nx_Hash nx_hash_fn_cstr(const void *key) {
    NX_ASSERT(key);
    return nx_hash_cstr(*(const nx_char *const *) key);
}

// internal defs

static nx_Hash hash_mix64(nx_u64 a) {
    a ^= a >> 33;
    a *= NX_FMIX64_C1;
    a ^= a >> 33;
    a *= NX_FMIX64_C2;
    a ^= a >> 33;
    return a;
}
