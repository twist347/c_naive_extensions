#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

nx_bool nx_equal(nx_cspan a, nx_cspan b, nx_cmp cmp);
nx_bool nx_not_equal(nx_cspan a, nx_cspan b, nx_cmp cmp);

// lexicographic_compare, mismatch
