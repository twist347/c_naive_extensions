#pragma once

#include "nx/core/type.h"

#ifdef __cplusplus
extern "C" {
#endif

// allocator interface

typedef struct {
    void *ctx;
    void *(*alloc)(void *ctx, nx_usize size);
    void *(*calloc)(void *ctx, nx_usize num, nx_usize size);
    void *(*realloc)(void *ctx, void *ptr, nx_usize old_size, nx_usize new_size);
    void (*dealloc)(void *ctx, void *ptr, nx_usize size);
} nx_al;

// wrappers

void *nx_al_alloc(nx_al a, nx_usize size);
void *nx_al_calloc(nx_al a, nx_usize num, nx_usize size);
void *nx_al_realloc(nx_al a, void *ptr, nx_usize old_size, nx_usize new_size);
void nx_al_dealloc(nx_al a, void *ptr, nx_usize size);

// relation

nx_bool nx_al_eq(nx_al a, nx_al b);
nx_bool nx_al_neq(nx_al a, nx_al b);

#ifdef __cplusplus
}
#endif
