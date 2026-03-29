#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

typedef struct {
    nx_isize min;
    nx_isize max;
} nx_MinMax;

/// returns index of minimum element, or -1 if span is empty.
nx_isize nx_min_element(nx_CSpan s, nx_cmp_fn cmp);
nx_isize nx_max_element(nx_CSpan s, nx_cmp_fn cmp);

/// finds both min and max indices in a single pass.
nx_MinMax nx_minmax_element(nx_CSpan s, nx_cmp_fn cmp);
