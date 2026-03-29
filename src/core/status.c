#include "nx/core/status.h"
#include "nx/core/util.h"

const nx_char *nx_status_to_str(nx_Status st) {
    switch (st) {
        case NX_STATUS_OK:
            return NX_STRINGIFY(NX_STATUS_OK);
        case NX_STATUS_OUT_OF_MEMORY:
            return NX_STRINGIFY(NX_STATUS_OUT_OF_MEMORY);
        case NX_STATUS_IO_ERROR:
            return NX_STRINGIFY(NX_STATUS_IO_ERROR);
        case NX_STATUS__COUNT:
            break;
    }
    return "UNKNOWN_NX_STATUS";
}
