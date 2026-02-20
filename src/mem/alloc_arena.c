#include "nx/mem/alloc_arena.h"

#include <string.h>
#include <stdlib.h>

#include "nx/core/type.h"
#include "nx/core/util.h"
#include "nx/mem/alloc.h"

constexpr nx_usize DEFAULT_ALIGNMENT = 8;

static nx_usize align_up(nx_usize size, nx_usize alignment);

static void *arena_alloc(void *ctx, nx_usize size);
static void *arena_calloc(void *ctx, nx_usize num, nx_usize size);
static void arena_dealloc(void *ctx, void *ptr, nx_usize size);

typedef struct {
    void *data;
    nx_usize cap;
    nx_usize offset;
} nx_al_arena_ctx;

nx_al *nx_al_arena_new(nx_usize cap) {
    NX_ASSERT(cap != 0);

    // allocate context
    nx_al_arena_ctx *ctx = malloc(sizeof(nx_al_arena_ctx));
    if (!ctx) {
        return nx_null;
    }

    // allocate data
    void *data = malloc(cap);
    if (!data) {
        free(ctx);
        return nx_null;
    }

    ctx->data = data;
    ctx->cap = cap;
    ctx->offset = 0;

    // allocate allocator
    nx_al *al = malloc(sizeof(nx_al));
    if (!al) {
        free(data);
        free(ctx);
        return nx_null;
    }

    al->ctx = ctx;
    al->alloc = arena_alloc;
    al->calloc = arena_calloc;
    al->realloc = nx_null; // fallback
    al->dealloc = arena_dealloc;

    return al;
}

void nx_al_arena_reset(nx_al *al) {
    NX_ASSERT(al);
    NX_ASSERT(al->ctx);

    nx_al_arena_ctx *ctx = al->ctx;
    ctx->offset = 0;
}

nx_al_arena_stats nx_al_arena_get_stats(const nx_al *al) {
    NX_ASSERT(al);
    NX_ASSERT(al->ctx);

    const nx_al_arena_ctx *ctx = al->ctx;

    return (nx_al_arena_stats){
        .cap = ctx->cap,
        .used = ctx->offset,
        .available = ctx->cap - ctx->offset,
    };
}

static nx_usize align_up(nx_usize size, nx_usize alignment) {
    NX_ASSERT(alignment > 0 && (alignment & (alignment - 1)) == 0);
    return (size + alignment - 1) & ~(alignment - 1);
}

static void *arena_alloc(void *ctx, nx_usize size) {
    NX_ASSERT(ctx);

    nx_al_arena_ctx *arena = ctx;

    if (size == 0) {
        return nx_null;
    }

    const nx_usize aligned_size = align_up(size, DEFAULT_ALIGNMENT);
    if (arena->offset + aligned_size > arena->cap) {
        return nx_null;
    }

    void *ptr = (nx_char *) arena->data + arena->offset;
    arena->offset += aligned_size;

    return ptr;
}

static void *arena_calloc(void *ctx, nx_usize num, nx_usize size) {
    // TODO: check overflow

    const nx_usize total = num * size;
    void *ptr = arena_alloc(ctx, total);

    if (ptr) {
        memset(ptr, 0, total);
    }

    return ptr;
}

static void arena_dealloc(void *ctx, void *ptr, nx_usize size) {
    // arena doesn't free individual allocations

    NX_UNUSED(ctx);
    NX_UNUSED(ptr);
    NX_UNUSED(size);
}
