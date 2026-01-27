#pragma once

// TODO: unused

#include <stddef.h>

typedef struct nx_allocator {
    void *ctx;

    void * (*alloc)(void *ctx, size_t size);

    void * (*realloc)(void *ctx, void *ptr, size_t size);

    void * (*calloc)(void *ctx, size_t nmemb, size_t size);

    void (*free)(void *ctx, void *ptr);
} nx_allocator;

static inline void *nx_alloc(nx_allocator a, size_t n) {
    return a.alloc ? a.alloc(a.ctx, n) : NULL;
}

static inline void *nx_calloc(nx_allocator a, size_t nmemb, size_t size) {
    return a.calloc ? a.calloc(a.ctx, nmemb, size) : NULL;
}

static inline void *nx_realloc(nx_allocator a, void *p, size_t n) {
    return a.realloc ? a.realloc(a.ctx, p, n) : NULL;
}

static inline void nx_free(nx_allocator a, void *p) {
    if (a.free) a.free(a.ctx, p);
}
