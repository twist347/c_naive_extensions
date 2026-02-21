#include "nx/core/hash.h"

#include "nx/core/assert.h"

/* FNV-1a 64-bit */
static constexpr nx_u64 NX_FNV_OFFSET_BASIS_64 = 14695981039346656037ULL;
static constexpr nx_u64 NX_FNV_PRIME_64 = 1099511628211ULL;

/* MurmurHash3 fmix64 constants */
static constexpr nx_u64 NX_FMIX64_C1 = 0xff51afd7ed558ccdULL;
static constexpr nx_u64 NX_FMIX64_C2 = 0xc4ceb9fe1a85ec53ULL;

static constexpr nx_u64 NX_HASH_GOLDEN_64 = 0x9e3779b97f4a7c15ULL;

static nx_hash hash_mix64(nx_u64 a);

nx_hash nx_hash_i8(nx_i8 x) {
    return hash_mix64((nx_u8) x);
}

nx_hash nx_hash_i16(nx_i16 x) {
    return hash_mix64((nx_u16) x);
}

nx_hash nx_hash_i32(nx_i32 x) {
    return hash_mix64((nx_u32) x);
}

nx_hash nx_hash_i64(nx_i64 x) {
    return hash_mix64((nx_u64) x);
}

nx_hash nx_hash_u8(nx_u8 x) {
    return hash_mix64(x);
}

nx_hash nx_hash_u16(nx_u16 x) {
    return hash_mix64(x);
}

nx_hash nx_hash_u32(nx_u32 x) {
    return hash_mix64(x);
}

nx_hash nx_hash_u64(nx_u64 x) {
    return hash_mix64(x);
}

nx_hash nx_hash_usize(nx_usize x) {
    return hash_mix64(x);
}

nx_hash nx_hash_isize(nx_isize x) {
    return hash_mix64((nx_u64) x);
}

nx_hash nx_hash_bytes(const void *data, nx_usize len) {
    NX_ASSERT(data);
    NX_ASSERT(len > 0);

    const nx_u8 *p = data;
    nx_u64 h = NX_FNV_OFFSET_BASIS_64;

    for (nx_usize i = 0; i < len; ++i) {
        h ^= p[i];
        h *= NX_FNV_PRIME_64;
    }

    return nx_hash_u64(h);
}

nx_hash nx_hash_cstr(const nx_char *s) {
    const nx_u8 *p = (const nx_u8 *) s;
    nx_u64 h = NX_FNV_OFFSET_BASIS_64;

    while (*p) {
        h ^= *p++;
        h *= NX_FNV_PRIME_64;
    }
    return nx_hash_u64(h);
}

nx_hash nx_hash_ptr(const void *ptr) {
    return hash_mix64((nx_uptr) ptr);
}

nx_hash nx_hash_combine(nx_hash h, nx_hash x) {
    nx_hash v = h;
    v ^= x + NX_HASH_GOLDEN_64 + (v << 6) + (v >> 2);
    return hash_mix64(v);
}

static nx_hash hash_mix64(nx_u64 a) {
    a ^= a >> 33;
    a *= NX_FMIX64_C1;
    a ^= a >> 33;
    a *= NX_FMIX64_C2;
    a ^= a >> 33;
    return a;
}
