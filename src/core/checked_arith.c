#include "nx/core/checked_arith.h"

#include <stdint.h>

#include "nx/core/assert.h"

bool nx_size_add_overflow(size_t *out, size_t a, size_t b) {
    NX_ASSERT(out);

    if (a > SIZE_MAX - b) {
        return true;
    }
    *out = a + b;
    return false;
}

bool nx_size_mul_overflow(size_t *out, size_t a, size_t b) {
    NX_ASSERT(out);

    if (a == 0 || b == 0) {
        *out = 0;
        return false;
    }
    if (a > SIZE_MAX / b) {
        return true;
    }
    *out = a * b;
    return false;
}
