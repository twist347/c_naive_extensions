#pragma once

#include "nx/core/type.h"

/* ========== byte-level pointer arithmetic ========== */

nx_byte *nx_byte_offset(void *base, nx_usize stride, nx_usize n);
const nx_byte *nx_byte_offset_c(const void *base, nx_usize stride, nx_usize n);
nx_isize nx_byte_diff(const void *a, const void *b);
nx_usize nx_ptr_distance(const void *a, const void *b, nx_usize stride);

/* ========== alignment ========== */

nx_usize nx_align_up(nx_usize val, nx_usize alignment);
nx_usize nx_align_down(nx_usize val, nx_usize alignment);
nx_bool nx_is_aligned(const void *ptr, nx_usize alignment);
void *nx_align_ptr_up(void *ptr, nx_usize alignment);

/* ========== range check ========== */

nx_bool nx_ptr_in_range(const void *ptr, const void *lo, const void *hi);

/* ========== memory ops ========== */

void nx_memswap(void *a, void *b, nx_usize n);
void nx_memzero(void *ptr, nx_usize n);
nx_bool nx_memeq(const void *a, const void *b, nx_usize n);

/* ========== exchange ========== */

void *nx_ptr_exchange(void **obj, void *new_val);
