#include "nx/mem/ptr.h"

#include "nx/core/assert.h"

nx_byte *nx_byte_offset(void *base, nx_usize stride, nx_usize n) {
    NX_ASSERT(base);

    return (nx_byte *) base + n * stride;
}

const nx_byte *nx_byte_offset_c(const void *base, nx_usize stride, nx_usize n) {
    NX_ASSERT(base);

    return (const nx_byte *) base + n * stride;
}

nx_isize nx_byte_diff(const void *a, const void *b) {
    NX_ASSERT(a);
    NX_ASSERT(b);

    return (const nx_byte *) a - (const nx_byte *) b;
}

void *nx_ptr_exchange(void **obj, void *new_val) {
    NX_ASSERT(obj);

    void *old = *obj;
    *obj = new_val;
    return old;
}

