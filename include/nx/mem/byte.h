#pragma once

#include <stddef.h>

char *nx_byte_offset(void *base, size_t stride, size_t idx);

const char *nx_byte_offset_c(const void *base, size_t stride, size_t idx);

ptrdiff_t nx_byte_diff(const void *a, const void *b);
