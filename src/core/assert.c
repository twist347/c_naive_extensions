#include "nx/core/assert.h"

#include <stdio.h>
#include <stdlib.h>

void nx_assert_fail(
    const char *type,
    const char *assertion,
    const char *file,
    int line,
    const char *func,
    const char *msg
) {
    fprintf(stderr, "[NX] %s: %s\n", type, assertion);
    fprintf(stderr, "     File: %s\n", file);
    fprintf(stderr, "     Line: %d\n", line);
    fprintf(stderr, "     Function: %s\n", func);

    if (msg && msg[0] != '\0') {
        fprintf(stderr, "     Message: %s\n", msg);
    }

    fflush(stderr);
    abort();
}
