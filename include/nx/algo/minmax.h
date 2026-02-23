#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

typedef struct {
    nx_isize min;
    nx_isize max;
} nx_minmax;

nx_isize nx_min_element(nx_cspan s, nx_cmp_fn cmp);
nx_isize nx_max_element(nx_cspan s, nx_cmp_fn cmp);
nx_minmax nx_minmax_element(nx_cspan s, nx_cmp_fn cmp);
