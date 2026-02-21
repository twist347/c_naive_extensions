#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

#ifdef __cplusplus
extern "C" {
#endif

nx_bool nx_equal(nx_cspan a, nx_cspan b, nx_cmp cmp);
nx_bool nx_not_equal(nx_cspan a, nx_cspan b, nx_cmp cmp);

// lexicographic_compare, mismatch

#ifdef __cplusplus
}
#endif
