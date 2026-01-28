#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "nx/core/type.h"

typedef enum nx_status {
    NX_STATUS_OK = 0,
    NX_STATUS_OUT_OF_MEMORY,
    NX_STATUS_MUL_OVERFLOW,

    // TODO: unused
    NX_STATUS_INVALID_ARGUMENT,
    NX_STATUS_OUT_OF_BOUNDS,
} nx_status;

const nx_char *nx_status_to_str(nx_status status);

#ifdef __cplusplus
}
#endif
