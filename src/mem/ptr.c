#include "nx/mem/ptr.h"

#include "nx/core/assert.h"

char *nx_byte_offset(void *base, nx_usize stride, nx_usize idx) {
    NX_ASSERT(base);

    return (char *) base + idx * stride;
}

const char *nx_byte_offset_c(const void *base, nx_usize stride, nx_usize idx) {
    NX_ASSERT(base);

    return (const char *) base + idx * stride;
}

nx_isize nx_byte_diff(const void *a, const void *b) {
    NX_ASSERT(a);
    NX_ASSERT(b);

    return (const char *) a - (const char *) b;
}

void *nx_ptr_exchange(void **obj, void *new_val) {
    NX_ASSERT(obj);

    void *old = *obj;
    *obj = new_val;
    return old;
}

