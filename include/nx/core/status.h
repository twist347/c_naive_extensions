#pragma once

#include "nx/core/type.h"

/// Recoverable error codes. NX_STATUS_OK (0) means success.
typedef enum {
    NX_STATUS_OK = 0,
    NX_STATUS_OUT_OF_MEMORY,
    NX_STATUS_IO_ERROR,

    NX_STATUS__COUNT
} nx_Status;

/// returns human-readable string for the status code.
const nx_char *nx_status_to_str(nx_Status st);
