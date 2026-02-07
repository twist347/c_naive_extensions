#pragma once

#include "nx/core/type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef nx_u64 nx_hash;

nx_hash nx_hash_i8(nx_i8 x);
nx_hash nx_hash_i16(nx_i16 x);
nx_hash nx_hash_i32(nx_i32 x);
nx_hash nx_hash_i64(nx_i64 x);

nx_hash nx_hash_u8(nx_u8 x);
nx_hash nx_hash_u16(nx_u16 x);
nx_hash nx_hash_u32(nx_u32 x);
nx_hash nx_hash_u64(nx_u64 x);

nx_hash nx_hash_usize(nx_usize x);
nx_hash nx_hash_isize(nx_isize x);

nx_hash nx_hash_bytes(const void *data, nx_usize len);
nx_hash nx_hash_cstr(const nx_char *s);
nx_hash nx_hash_ptr(const void *ptr);

nx_hash nx_hash_combine(nx_hash h, nx_hash x);

#ifdef __cplusplus
}
#endif
