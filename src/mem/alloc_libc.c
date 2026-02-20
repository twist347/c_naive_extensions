#include "nx/mem/alloc_libc.h"

#include <stdlib.h>

#include "nx/core/util.h"

static void *malloc_wrapper(void *ctx, nx_usize size);
static void *calloc_wrapper(void *ctx, nx_usize num, nx_usize size);
static void *realloc_wrapper(void *ctx, void *ptr, nx_usize old_size, nx_usize new_size);
static void free_wrapper(void *ctx, void *ptr, nx_usize size);

nx_al *nx_al_libc_new(void) {
    nx_al *al = malloc(sizeof(nx_al));
    if (!al) {
        return nx_null;
    }

    al->ctx = nx_null;
    al->alloc = malloc_wrapper;
    al->calloc = calloc_wrapper;
    al->realloc = realloc_wrapper;
    al->dealloc = free_wrapper;

    return al;
}

void nx_al_libc_drop(nx_al *al) {
    if (!al) {
        return;
    }
    free(al);
}

nx_al *nx_al_libc_default_g(void) {
    static nx_al g_alloc = {
        .ctx = nx_null,
        .alloc = malloc_wrapper,
        .calloc = calloc_wrapper,
        .realloc = realloc_wrapper,
        .dealloc = free_wrapper,
    };

    return &g_alloc;
}

static void *malloc_wrapper(void *ctx, nx_usize size) {
    NX_UNUSED(ctx);
    return malloc(size);
}

static void *calloc_wrapper(void *ctx, nx_usize num, nx_usize size) {
    NX_UNUSED(ctx);
    return calloc(num, size);
}

static void *realloc_wrapper(void *ctx, void *ptr, nx_usize old_size, nx_usize new_size) {
    NX_UNUSED(ctx);
    NX_UNUSED(old_size);
    return realloc(ptr, new_size);
}

static void free_wrapper(void *ctx, void *ptr, nx_usize size) {
    NX_UNUSED(ctx);
    NX_UNUSED(size);
    free(ptr);
}
