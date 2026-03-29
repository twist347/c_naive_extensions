#pragma once

#include "nx/core/type.h"

/// 64-bit hash value. Uses FNV-1a for bytes, MurmurHash3 fmix64 for integers.
typedef nx_u64 nx_Hash;

/* ========== integer hashing ========== */

nx_Hash nx_hash_i8(nx_i8 x);
nx_Hash nx_hash_i16(nx_i16 x);
nx_Hash nx_hash_i32(nx_i32 x);
nx_Hash nx_hash_i64(nx_i64 x);

nx_Hash nx_hash_u8(nx_u8 x);
nx_Hash nx_hash_u16(nx_u16 x);
nx_Hash nx_hash_u32(nx_u32 x);
nx_Hash nx_hash_u64(nx_u64 x);

nx_Hash nx_hash_usize(nx_usize x);
nx_Hash nx_hash_isize(nx_isize x);

/* ========== float hashing ========== */

/// +0.0 and -0.0 hash equal. All NaNs hash equal.
nx_Hash nx_hash_f32(nx_f32 x);
nx_Hash nx_hash_f64(nx_f64 x);

/* ========== byte / string / pointer hashing ========== */

/// FNV-1a over raw bytes.
nx_Hash nx_hash_bytes(const void *data, nx_usize len);
nx_Hash nx_hash_cstr(const nx_char *s);

/// hashes the pointer address, not the pointee.
nx_Hash nx_hash_ptr(const void *ptr);

/* ========== combine ========== */

/// combines two hashes into one (order-dependent).
nx_Hash nx_hash_combine(nx_Hash h, nx_Hash x);

/* ========== callback ========== */

/// Type-erased hash function: hash the key pointed to by key.
typedef nx_Hash (*nx_hash_fn)(const void *key);

/* ========== nx_hash_fn-compatible callbacks (dereference key and hash) ========== */

nx_Hash nx_hash_fn_i8(const void *key);
nx_Hash nx_hash_fn_i16(const void *key);
nx_Hash nx_hash_fn_i32(const void *key);
nx_Hash nx_hash_fn_i64(const void *key);

nx_Hash nx_hash_fn_u8(const void *key);
nx_Hash nx_hash_fn_u16(const void *key);
nx_Hash nx_hash_fn_u32(const void *key);
nx_Hash nx_hash_fn_u64(const void *key);

nx_Hash nx_hash_fn_usize(const void *key);
nx_Hash nx_hash_fn_isize(const void *key);

nx_Hash nx_hash_fn_f32(const void *key);
nx_Hash nx_hash_fn_f64(const void *key);

/// key is `const char **` (pointer to string pointer)
nx_Hash nx_hash_fn_cstr(const void *key);

/* ========== generic ========== */

#define nx_hash(x) _Generic((x), \
    nx_i8:    nx_hash_i8,        \
    nx_i16:   nx_hash_i16,       \
    nx_i32:   nx_hash_i32,       \
    nx_i64:   nx_hash_i64,       \
    nx_u8:    nx_hash_u8,        \
    nx_u16:   nx_hash_u16,       \
    nx_u32:   nx_hash_u32,       \
    nx_u64:   nx_hash_u64,       \
    nx_f32:   nx_hash_f32,       \
    nx_f64:   nx_hash_f64        \
)(x)
