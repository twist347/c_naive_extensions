#include "nx/mem/alloc_log.h"

#include <stdio.h>
#include <stdlib.h>

// TODO: nx print

static void *log_alloc(void *ctx, nx_usize size);
static void *log_calloc(void *ctx, nx_usize num, nx_usize size);
static void *log_realloc(void *ctx, void *ptr, nx_usize old_size, nx_usize new_size);
static void log_dealloc(void *ctx, void *ptr, nx_usize size);

/* ========== context ========== */

typedef struct {
    nx_al *wrapped;
    FILE *stream;
} nx_al_log_ctx;

nx_al *nx_al_log_new(nx_al *wrapped, FILE *stream) {
    NX_ASSERT(wrapped);

    nx_al_log_ctx *ctx = malloc(sizeof(nx_al_log_ctx));
    if (!ctx) {
        return nx_null;
    }

    ctx->wrapped = wrapped;
    ctx->stream = stream ? stream : stdout;

    nx_al *a = malloc(sizeof(nx_al));
    if (!a) {
        free(ctx);
        return nx_null;
    }

    a->ctx = ctx;
    a->alloc = log_alloc;
    a->calloc = log_calloc;
    a->realloc = log_realloc;
    a->dealloc = log_dealloc;

    fprintf(ctx->stream, "[NX] log allocator created (wrapping %p)\n", (void*)wrapped);
    fflush(ctx->stream);

    return a;
}

static void *log_alloc(void *ctx, nx_usize size) {
    NX_ASSERT(ctx);

    const nx_al_log_ctx *log_ctx = ctx;
    NX_ASSERT(log_ctx->stream);
    NX_ASSERT(log_ctx->wrapped->alloc);

    void *p = log_ctx->wrapped->alloc(log_ctx->wrapped->ctx, size);
    fprintf(
        log_ctx->stream,
        "[NX] alloc size = %zu -> %p\n", size, p
    );
    fflush(log_ctx->stream);
    return p;
}

static void *log_calloc(void *ctx, nx_usize num, nx_usize size) {
    NX_ASSERT(ctx);

    const nx_al_log_ctx *log_ctx = ctx;
    NX_ASSERT(log_ctx->stream);
    NX_ASSERT(log_ctx->wrapped->calloc);

    // TODO: check overflow
    void *p = log_ctx->wrapped->calloc(log_ctx->wrapped->ctx, num, size);
    fprintf(log_ctx->stream, "[NX] calloc num = %zu size = %zu total = %zu -> %p\n", num, size, num * size, p);
    fflush(log_ctx->stream);
    return p;
}

static void *log_realloc(void *ctx, void *ptr, nx_usize old_size, nx_usize new_size) {
    NX_ASSERT(ctx);

    const nx_al_log_ctx *log_ctx = ctx;
    NX_ASSERT(log_ctx->stream);
    NX_ASSERT(log_ctx->wrapped->realloc);

    void *p = log_ctx->wrapped->realloc(log_ctx->wrapped->ctx, ptr, old_size, new_size);
    fprintf(log_ctx->stream, "[NX] realloc %p old_size = %zu new_size = %zu -> %p\n", ptr, old_size, new_size, p);
    fflush(log_ctx->stream);
    return p;
}

static void log_dealloc(void *ctx, void *ptr, nx_usize size) {
    NX_ASSERT(ctx);

    const nx_al_log_ctx *log_ctx = ctx;
    NX_ASSERT(log_ctx->stream);
    NX_ASSERT(log_ctx->wrapped->dealloc);

    log_ctx->wrapped->dealloc(log_ctx->wrapped->ctx, ptr, size);
    fprintf(log_ctx->stream, "[NX] dealloc size = %zu -> %p\n", size, ptr);
    fflush(log_ctx->stream);
}
