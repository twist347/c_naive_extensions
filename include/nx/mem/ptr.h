#pragma once

#include "nx/core/type.h"

nx_byte *nx_byte_offset(void *base, nx_usize stride, nx_usize n);

const nx_byte *nx_byte_offset_c(const void *base, nx_usize stride, nx_usize n);

nx_isize nx_byte_diff(const void *a, const void *b);

void *nx_ptr_exchange(void **obj, void *new_val);
