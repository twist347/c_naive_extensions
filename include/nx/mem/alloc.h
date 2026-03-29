#pragma once

#include "nx/core/type.h"

// TODO: aligned alloc (for the future)

/**
 * Allocator interface.
 *
 * `nx_Al` is a value type: { ctx, function pointers }.
 *
 * Contract:
 * - The allocator (ctx + callbacks) must outlive all allocations made through it.
 * - `alloc` and `dealloc` must be provided; `calloc`/`realloc` are optional.
 * - Wrappers do not perform runtime checks in release builds.
 */

typedef struct {
    void *ctx;
    void *(*alloc)(void *ctx, nx_usize size);
    void *(*calloc)(void *ctx, nx_usize num, nx_usize size);
    void *(*realloc)(void *ctx, void *ptr, nx_usize old_size, nx_usize new_size);
    void (*dealloc)(void *ctx, void *ptr, nx_usize size);
} nx_Al;

/* ========== wrappers ========== */

/// returns null on failure. size == 0 returns null.
void *nx_al_alloc(nx_Al *al, nx_usize size);

/// zero-initialized allocation. falls back to alloc + memset if calloc is null.
void *nx_al_calloc(nx_Al *al, nx_usize num, nx_usize size);

/// falls back to alloc + memcpy + dealloc if realloc is null.
void *nx_al_realloc(nx_Al *al, void *ptr, nx_usize old_size, nx_usize new_size);

/// no-op if ptr is null.
void nx_al_dealloc(nx_Al *al, void *ptr, nx_usize size);

/* ========== relation ========== */

/// two allocators are equal if they share the same ctx and function pointers.
nx_bool nx_al_eq(const nx_Al *a, const nx_Al *b);
nx_bool nx_al_neq(const nx_Al *a, const nx_Al *b);

/* ========== macros ========== */

#define NX_AL_ALLOC(T, al, count) \
    ((T*) nx_al_alloc((al), (count) * sizeof(T)))

#define NX_AL_CALLOC(T, al, count) \
    ((T*) nx_al_calloc((al), (count), sizeof(T)))

#define NX_AL_REALLOC(T, al, ptr, old_count, new_count) \
    ((T*) nx_al_realloc((al), (ptr), (old_count) * sizeof(T), (new_count) * sizeof(T)))

#define NX_AL_DEALLOC(T, al, ptr, count) \
    nx_al_dealloc((al), (ptr), (count) * sizeof(T))
