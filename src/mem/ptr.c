#include "nx/mem/ptr.h"

#include <string.h>

#include "nx/core/assert.h"

/* ========== byte-level pointer arithmetic ========== */

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

nx_usize nx_ptr_distance(const void *a, const void *b, nx_usize stride) {
    NX_ASSERT(a);
    NX_ASSERT(b);
    NX_ASSERT(stride > 0);
    NX_ASSERT((const nx_byte *) a >= (const nx_byte *) b);

    const nx_usize diff = (nx_usize) ((const nx_byte *) a - (const nx_byte *) b);
    NX_ASSERT(diff % stride == 0);

    return diff / stride;
}

/* ========== alignment ========== */

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

    return ((nx_uptr) ptr & (alignment - 1)) == 0;
}

void *nx_align_ptr_up(void *ptr, nx_usize alignment) {
    NX_ASSERT(ptr);
    NX_ASSERT(alignment > 0);
    NX_ASSERT((alignment & (alignment - 1)) == 0);

    const nx_uptr addr = (nx_uptr) ptr;
    const nx_uptr aligned = (addr + alignment - 1) & ~(alignment - 1);

    return (void *) aligned;
}

/* ========== range check ========== */

nx_bool nx_ptr_in_range(const void *ptr, const void *lo, const void *hi) {
    NX_ASSERT(ptr);
    NX_ASSERT(lo);
    NX_ASSERT(hi);
    NX_ASSERT((const nx_byte *) hi >= (const nx_byte *) lo);

    return (const nx_byte *) ptr >= (const nx_byte *) lo
           && (const nx_byte *) ptr < (const nx_byte *) hi;
}

/* ========== memory ops ========== */

void nx_memswap(void *a, void *b, nx_usize n) {
    NX_ASSERT(a);
    NX_ASSERT(b);

    if (a == b || n == 0) {
        return;
    }

    nx_byte *pa = a;
    nx_byte *pb = b;

    typedef nx_usize word;
    constexpr nx_usize wsz = sizeof(word);

    nx_usize i = 0;

    // swap word-at-a-time if both pointers are word-aligned
    if (((nx_uptr) pa | (nx_uptr) pb) % wsz == 0) {
        for (; i + wsz <= n; i += wsz) {
            word tmp;
            memcpy(&tmp, pa + i, wsz);
            memcpy(pa + i, pb + i, wsz);
            memcpy(pb + i, &tmp, wsz);
        }
    }

    for (; i < n; ++i) {
        const nx_byte tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
    }
}

void nx_memzero(void *ptr, nx_usize n) {
    NX_ASSERT(ptr || n == 0);

    if (n > 0) {
        memset(ptr, 0, n);
    }
}

nx_bool nx_memeq(const void *a, const void *b, nx_usize n) {
    NX_ASSERT(a || n == 0);
    NX_ASSERT(b || n == 0);

    if (n == 0) {
        return true;
    }

    return memcmp(a, b, n) == 0;
}

/* ========== exchange ========== */


void *nx_ptr_exchange(void **obj, void *new_val) {
    NX_ASSERT(obj);

    void *old = *obj;
    *obj = new_val;
    return old;
}
