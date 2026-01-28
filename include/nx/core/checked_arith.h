#pragma once

#include "nx/core/type.h"

// TODO: why size?

// true if overflow

nx_bool nx_size_add_overflow(nx_usize *out, nx_usize a, nx_usize b);

nx_bool nx_size_mul_overflow(nx_usize *out, nx_usize a, nx_usize b);
