#pragma once

#include <stdbool.h>
#include <stddef.h>

// true if overflow

// TODO: why size?

bool nx_size_add_overflow(size_t *out, size_t a, size_t b);

bool nx_size_mul_overflow(size_t *out, size_t a, size_t b);
