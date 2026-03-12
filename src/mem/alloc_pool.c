#include "nx/mem/alloc_pool.h"

#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include "nx/core/assert.h"
#include "nx/core/util.h"
#include "nx/mem/ptr.h"

constexpr nx_usize POOL_ALIGNMENT = alignof(max_align_t);

typedef struct PoolNode PoolNode;

struct PoolNode {
    PoolNode *next;
};

typedef struct {
    nx_byte *data;
    PoolNode *free_head;
    nx_usize block_size;
    nx_usize block_count;
    nx_usize used;
} nx_AlPoolCtx;

static void *pool_alloc(void *ctx, nx_usize size);
static void *pool_calloc(void *ctx, nx_usize num, nx_usize size);
static void pool_dealloc(void *ctx, void *ptr, nx_usize size);

static void pool_build_free_list(nx_AlPoolCtx *ctx);
static nx_bool pool_owns(const nx_AlPoolCtx *ctx, const void *ptr);

nx_Al *nx_al_pool_new(nx_usize block_size, nx_usize block_count) {
    NX_ASSERT(block_size > 0);
    NX_ASSERT(block_count > 0);

    // each block must hold at least a free-list pointer
    if (block_size < sizeof(PoolNode)) {
        block_size = sizeof(PoolNode);
    }

    // align up
    block_size = nx_align_up(block_size, POOL_ALIGNMENT);

    // allocate context
    nx_AlPoolCtx *ctx = malloc(sizeof(nx_AlPoolCtx));
    if (!ctx) {
        return nx_null;
    }

    // TODO: check overflow
    // allocate backing buffer
    const nx_usize total_bytes = block_size * block_count;

    nx_byte *data = malloc(total_bytes);
    if (!data) {
        free(ctx);
        return nx_null;
    }

    ctx->data = data;
    ctx->block_size = block_size;
    ctx->block_count = block_count;
    ctx->used = 0;
    ctx->free_head = nx_null;

    pool_build_free_list(ctx);

    // allocate the nx_Al itself
    nx_Al *al = malloc(sizeof(nx_Al));
    if (!al) {
        free(data);
        free(ctx);
        return nx_null;
    }

    al->ctx = ctx;
    al->alloc = pool_alloc;
    al->calloc = pool_calloc;
    al->realloc = nx_null; // fallback in nx_al
    al->dealloc = pool_dealloc;

    return al;
}

void nx_al_pool_drop(nx_Al *al) {
    if (!al) {
        return;
    }

    nx_AlPoolCtx *ctx = al->ctx;
    if (ctx) {
        free(ctx->data);
        free(ctx);
    }
    free(al);
}

void nx_al_pool_reset(nx_Al *al) {
    NX_ASSERT(al);
    NX_ASSERT(al->ctx);

    nx_AlPoolCtx *ctx = al->ctx;
    ctx->used = 0;
    pool_build_free_list(ctx);
}

nx_AlPoolStats nx_al_pool_get_stats(const nx_Al *al) {
    NX_ASSERT(al);
    NX_ASSERT(al->ctx);

    const nx_AlPoolCtx *ctx = al->ctx;

    return (nx_AlPoolStats){
        .block_size = ctx->block_size,
        .block_count = ctx->block_count,
        .used = ctx->used,
        .free = ctx->block_count - ctx->used,
    };
}

static void *pool_alloc(void *ctx, nx_usize size) {
    NX_ASSERT(ctx);

    if (size == 0) {
        return nx_null;
    }

    nx_AlPoolCtx *pool = ctx;
    if (size > pool->block_size) {
        return nx_null;
    }

    PoolNode *node = pool->free_head;
    if (!node) {
        return nx_null;
    }

    pool->free_head = node->next;
    ++pool->used;

    return node;
}

static void *pool_calloc(void *ctx, nx_usize num, nx_usize size) {
    NX_ASSERT(ctx);

    // TODO: overflow check

    const nx_usize total = num * size;
    void *ptr = pool_alloc(ctx, total);
    if (ptr) {
        memset(ptr, 0, total);
    }
    return ptr;
}

static void pool_dealloc(void *ctx, void *ptr, nx_usize size) {
    NX_ASSERT(ctx);
    NX_UNUSED(size);

    if (!ptr) {
        return;
    }

    nx_AlPoolCtx *pool = ctx;

    NX_ASSERT(pool_owns(pool, ptr));
    NX_ASSERT(pool->used > 0);

    // push onto free list
    PoolNode *node = ptr;
    node->next = pool->free_head;
    pool->free_head = node;
    --pool->used;
}

static void pool_build_free_list(nx_AlPoolCtx *ctx) {
    ctx->free_head = nx_null;

    // build list in reverse so that first alloc returns the first block
    for (nx_usize i = ctx->block_count; i > 0; --i) {
        PoolNode *node = (PoolNode *) (ctx->data + (i - 1) * ctx->block_size);
        node->next = ctx->free_head;
        ctx->free_head = node;
    }
}

static nx_bool pool_owns(const nx_AlPoolCtx *ctx, const void *ptr) {
    const nx_byte *p = ptr;
    const nx_byte *begin = ctx->data;
    const nx_byte *end = ctx->data + ctx->block_size * ctx->block_count;

    if (p < begin || p >= end) {
        return false;
    }

    // must be aligned to block boundary
    const nx_usize offset = (nx_usize) (p - begin);
    return (offset % ctx->block_size) == 0;
}