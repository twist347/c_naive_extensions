#include "nx/mem/ptr.h"

#include "nx/core/assert.h"

nx_byte *nx_byte_offset(void *base, nx_usize stride, nx_usize n) {
    NX_ASSERT(base);
    NX_ASSERT(stride > 0);

    return (nx_byte *) base + n * stride;
}

const nx_byte *nx_byte_offset_c(const void *base, nx_usize stride, nx_usize n) {
    NX_ASSERT(base);
    NX_ASSERT(stride > 0);

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

nx_usize nx_align_up(nx_usize val, nx_usize alignment) {
    NX_ASSERT(alignment > 0);
    NX_ASSERT((alignment & (alignment - 1)) == 0); // power of 2

    return (val + alignment - 1) & ~(alignment - 1);
}

nx_usize nx_align_down(nx_usize val, nx_usize alignment) {
    NX_ASSERT(alignment > 0);
    NX_ASSERT((alignment & (alignment - 1)) == 0); // power of 2

    return val & ~(alignment - 1);
}

nx_bool nx_is_aligned(const void *ptr, nx_usize alignment) {
    NX_ASSERT(ptr);
    NX_ASSERT(alignment > 0);
    NX_ASSERT((alignment & (alignment - 1)) == 0); // power of 2

    return ((nx_uptr)ptr & (alignment - 1)) == 0;
}

