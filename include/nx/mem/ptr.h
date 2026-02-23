#pragma once

#include "nx/core/type.h"

nx_byte *nx_byte_offset(void *base, nx_usize stride, nx_usize n);

const nx_byte *nx_byte_offset_c(const void *base, nx_usize stride, nx_usize n);

nx_isize nx_byte_diff(const void *a, const void *b);

void *nx_ptr_exchange(void **obj, void *new_val);

nx_usize nx_align_up(nx_usize val, nx_usize alignment);

nx_usize nx_align_down(nx_usize val, nx_usize alignment);

nx_bool nx_is_aligned(const void *ptr, nx_usize alignment);
