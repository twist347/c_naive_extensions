#include "nx/mem/alloc.h"

#include <string.h>

#include "nx/core/assert.h"

/* ========== wrappers ========== */

void *nx_al_alloc(nx_al *al, nx_usize size) {
    NX_ASSERT(al);
    NX_ASSERT(al->alloc);

    return al->alloc(al->ctx, size);
}

void *nx_al_calloc(nx_al *al, nx_usize num, nx_usize size) {
    NX_ASSERT(al);

    // fallback
    if (!al->calloc) {
        // TODO: overflow check
        const nx_usize total = num * size;
        void *ptr = nx_al_alloc(al, total);
        if (!ptr) {
            return nx_null;
        }
        memset(ptr, 0, total);
    }

    return al->calloc(al->ctx, num, size);
}

void *nx_al_realloc(nx_al *al, void *ptr, nx_usize old_size, nx_usize new_size) {
    NX_ASSERT(al);

    // fallback
    if (!al->realloc) {
        if (new_size == 0) {
            nx_al_dealloc(al, ptr, old_size);
            return nx_null;
        }
        void *new_ptr = nx_al_alloc(al, new_size);
        if (!new_ptr) {
            return nx_null;
        }

        if (ptr && old_size > 0) {
            const nx_usize copy_size = old_size < new_size ? old_size : new_size;
            memcpy(new_ptr, ptr, copy_size);
            nx_al_dealloc(al, ptr, old_size);
        }
        return new_ptr;
    }

    return al->realloc(al->ctx, ptr, old_size, new_size);
}

void nx_al_dealloc(nx_al *al, void *ptr, nx_usize size) {
    NX_ASSERT(al);
    NX_ASSERT(al->dealloc);

    al->dealloc(al->ctx, ptr, size);
}

/* ========== relation ========== */

nx_bool nx_al_eq(const nx_al *a, const nx_al *b) {
    NX_ASSERT(a);
    NX_ASSERT(b);

    return a->ctx == b->ctx &&
           a->alloc == b->alloc &&
           a->calloc == b->calloc &&
           a->realloc == b->realloc &&
           a->dealloc == b->dealloc;
}

nx_bool nx_al_neq(const nx_al *a, const nx_al *b) {
    return !nx_al_eq(a, b);
}
