#pragma once

// TODO: unused

#include "nx/core/type.h"

typedef struct nx_allocator {
    void *ctx;

    void * (*alloc)(void *ctx, nx_usize size);

    void * (*realloc)(void *ctx, void *ptr, nx_usize size);

    void * (*calloc)(void *ctx, size_t nmemb, nx_usize size);

    void (*free)(void *ctx, void *ptr);
} nx_allocator;

static inline void *nx_alloc(nx_allocator a, size_t n) {
    return a.alloc ? a.alloc(a.ctx, n) : NULL;
}

static inline void *nx_calloc(nx_allocator a, nx_usize nmemb, nx_usize size) {
    return a.calloc ? a.calloc(a.ctx, nmemb, size) : NULL;
}

static inline void *nx_realloc(nx_allocator a, void *p, nx_usize n) {
    return a.realloc ? a.realloc(a.ctx, p, n) : NULL;
}

static inline void nx_free(nx_allocator a, void *p) {
    if (a.free) a.free(a.ctx, p);
}
