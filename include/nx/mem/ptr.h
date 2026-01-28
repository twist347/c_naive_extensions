#pragma once

#include "nx/core/type.h"

char *nx_byte_offset(void *base, nx_usize stride, nx_usize idx);

const char *nx_byte_offset_c(const void *base, nx_usize stride, nx_usize idx);

nx_isize nx_byte_diff(const void *a, const void *b);

void *nx_ptr_exchange(void **obj, void *new_val);
