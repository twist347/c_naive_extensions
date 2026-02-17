#include "nx/mem/alloc.h"

// wrappers

void *nx_al_alloc(nx_al a, nx_usize size) {
    NX_ASSERT(a.alloc);

    return a.alloc(a.ctx, size);
}

void *nx_al_calloc(nx_al a, nx_usize num, nx_usize size) {
    NX_ASSERT(a.calloc);

    return a.calloc(a.ctx, num, size);
}

void *nx_al_realloc(nx_al a, void *ptr, nx_usize old_size, nx_usize new_size) {
    NX_ASSERT(a.realloc);

    return a.realloc(a.ctx, ptr, old_size, new_size);
}

void nx_al_dealloc(nx_al a, void *ptr, nx_usize size) {
    NX_ASSERT(a.dealloc);

    a.dealloc(a.ctx, ptr, size);
}

// relation

nx_bool nx_al_eq(nx_al a, nx_al b) {
    return a.ctx == b.ctx &&
           a.alloc == b.alloc &&
           a.calloc == b.calloc &&
           a.realloc == b.realloc &&
           a.dealloc == b.dealloc;
}

nx_bool nx_al_neq(nx_al a, nx_al b) {
    return !nx_al_eq(a, b);
}
