#include "nx/core/status.h"

#include "nx/core/assert.h"

const nx_char *nx_status_to_str(nx_status status) {
    switch (status) {
        case NX_STATUS_OK:
            return "NX_STATUS_OK";
        case NX_STATUS_OUT_OF_MEMORY:
            return "NX_STATUS_OUT_OF_MEMORY";
        case NX_STATUS_MUL_OVERFLOW:
            return "NX_STATUS_MUL_OVERFLOW";
        case NX_STATUS_INVALID_ARGUMENT:
            return "NX_STATUS_INVALID_ARGUMENT";
        case NX_STATUS_OUT_OF_BOUNDS:
            return "NX_STATUS_OUT_OF_BOUNDS";
        default:
            return "UNKNOWN_NX_STATUS";
    }
}

void nx_status_fprintln(FILE *stream, nx_status status) {
    NX_ASSERT(stream);

    fputs(nx_status_to_str(status), stream);
    fputc('\n', stream);
}