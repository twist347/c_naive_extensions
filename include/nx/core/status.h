#pragma once

#include <stdio.h>

#include "nx/core/type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum nx_status {
    NX_STATUS_OK = 0,
    NX_STATUS_OUT_OF_MEMORY,

    NX_STATUS__COUNT
} nx_status;

const nx_char *nx_status_to_str(nx_status st);

void nx_status_fprintln(FILE *stream, nx_status st);

#ifdef __cplusplus
}
#endif
