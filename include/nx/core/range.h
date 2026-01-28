#pragma once

// TODO: unused

#include "nx/core/type.h"

#ifdef __cplusplus
extern "C" {
#endif

// [first, last)
typedef struct nx_range {
    nx_isize first;
    nx_isize last;
} nx_range;

#ifdef __cplusplus
}
#endif
