#include "nx/core/checked_arith.h"

#include <stdint.h>

#include "nx/core/assert.h"

nx_bool nx_size_add_overflow(nx_usize *out, nx_usize a, nx_usize b) {
    NX_ASSERT(out);

    if (a > SIZE_MAX - b) {
        return true;
    }
    *out = a + b;
    return false;
}

nx_bool nx_size_mul_overflow(nx_usize *out, nx_usize a, nx_usize b) {
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
