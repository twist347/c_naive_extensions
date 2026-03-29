#pragma once

#include "nx/core/type.h"

/* ========== byte-level pointer arithmetic ========== */

/// returns (nx_byte *)base + stride * n
nx_byte *nx_byte_offset(void *base, nx_usize stride, nx_usize n);
const nx_byte *nx_byte_offset_c(const void *base, nx_usize stride, nx_usize n);

/// returns (nx_byte *)a - (nx_byte *)b
nx_isize nx_byte_diff(const void *a, const void *b);

/// returns (a - b) / stride. a must be >= b, difference must be aligned.
nx_usize nx_ptr_distance(const void *a, const void *b, nx_usize stride);

/* ========== alignment ========== */

/// round val up to next multiple of alignment. alignment must be power of 2.
nx_usize nx_align_up(nx_usize val, nx_usize alignment);
nx_usize nx_align_down(nx_usize val, nx_usize alignment);
nx_bool nx_is_aligned(const void *ptr, nx_usize alignment);
void *nx_align_ptr_up(void *ptr, nx_usize alignment);

/* ========== range check ========== */

/// returns true if ptr is in [lo, hi)
nx_bool nx_ptr_in_range(const void *ptr, const void *lo, const void *hi);

/* ========== memory ops ========== */

/// swap n bytes between a and b. must not overlap.
void nx_memswap(void *a, void *b, nx_usize n);
void nx_memzero(void *ptr, nx_usize n);
nx_bool nx_memeq(const void *a, const void *b, nx_usize n);

/* ========== exchange ========== */

/// atomically sets *obj = new_val, returns old value. NOT thread-safe.
void *nx_ptr_exchange(void **obj, void *new_val);
