#include "nx/mem/alloc_libc.h"

#include <stdlib.h>

#include "nx/core/util.h"

static void *malloc_wrapper(void *ctx, nx_usize size);
static void *calloc_wrapper(void *ctx, nx_usize num, nx_usize size);
static void *realloc_wrapper(void *ctx, void *ptr, nx_usize old_size, nx_usize new_size);
static void free_wrapper(void *ctx, void *ptr, nx_usize size);

nx_al nx_al_libc_new(void) {
    return (nx_al){
        .alloc = malloc_wrapper,
        .calloc = calloc_wrapper,
        .realloc = realloc_wrapper,
        .dealloc = free_wrapper
    };
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
