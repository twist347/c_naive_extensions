#include "nx/core/status.h"

#include "nx/core/assert.h"

const nx_char *nx_status_to_str(nx_status st) {
    switch (st) {
        case NX_STATUS_OK:
            return "NX_STATUS_OK";
        case NX_STATUS_OUT_OF_MEMORY:
            return "NX_STATUS_OUT_OF_MEMORY";
        case NX_STATUS__COUNT:
            break;
    }
    return "UNKNOWN_NX_STATUS";
}

void nx_status_fprintln(FILE *stream, nx_status st) {
    NX_ASSERT(stream);

    fputs(nx_status_to_str(st), stream);
    fputc('\n', stream);
}
