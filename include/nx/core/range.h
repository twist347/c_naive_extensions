#pragma once

// TODO: unused

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// [first, last)
typedef struct nx_range {
    ptrdiff_t first;
    ptrdiff_t last;
} nx_range;

#ifdef __cplusplus
}
#endif
