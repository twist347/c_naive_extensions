#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

#ifdef __cplusplus
extern "C" {
#endif

void nx_sort(nx_span s, nx_cmp cmp);
nx_bool nx_is_sorted(nx_cspan s, nx_cmp cmp);

#ifdef __cplusplus
}
#endif
