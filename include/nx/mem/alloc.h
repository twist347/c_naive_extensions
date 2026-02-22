#pragma once

#include "nx/core/type.h"

/**
 * Allocator interface
 *
 * Contract:
 * - Allocator is heap-allocated and reference-counted internally or managed by user
 * - User must ensure allocator outlives all containers using it
 */

typedef struct {
    void *ctx;
    void *(*alloc)(void *ctx, nx_usize size);
    void *(*calloc)(void *ctx, nx_usize num, nx_usize size);
    void *(*realloc)(void *ctx, void *ptr, nx_usize old_size, nx_usize new_size);
    void (*dealloc)(void *ctx, void *ptr, nx_usize size);
} nx_al;

/* ========== wrappers ========== */

void *nx_al_alloc(nx_al *al, nx_usize size);
void *nx_al_calloc(nx_al *al, nx_usize num, nx_usize size);
void *nx_al_realloc(nx_al *al, void *ptr, nx_usize old_size, nx_usize new_size);
void nx_al_dealloc(nx_al *al, void *ptr, nx_usize size);

/* ========== relation ========== */

nx_bool nx_al_eq(const nx_al *a, const nx_al *b);
nx_bool nx_al_neq(const nx_al *a, const nx_al *b);

/* ========== macros ========== */

#define NX_AL_ALLOC(T, al, count) \
    ((T*) nx_al_alloc((al), (count) * sizeof(T)))

#define NX_AL_CALLOC(T, al, count) \
    ((T*) nx_al_calloc((al), (count), sizeof(T)))

#define NX_AL_REALLOC(T, al, ptr, old_count, new_count) \
    ((T*) nx_al_realloc((al), (ptr), (old_count) * sizeof(T), (new_count) * sizeof(T)))

#define NX_AL_DEALLOC(T, al, ptr, count) \
    nx_al_dealloc((al), (ptr), (count) * sizeof(T))
