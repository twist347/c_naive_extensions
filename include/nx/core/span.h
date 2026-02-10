#pragma once

#include "nx/core/type.h"
#include "nx/core/assert.h"
#include "nx/core/limit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nx_span {
    void *data;
    nx_usize len;
    nx_usize tsz;
} nx_span;

typedef struct nx_cspan {
    const void *data;
    nx_usize len;
    nx_usize tsz;
} nx_cspan;

/* ========== assert ========== */

#if NX_DEBUG
    void nx_span_any_assert_(const void *data, nx_usize len, nx_usize tsz);

#define NX_SPAN_ANY_ASSERT(s) \
    do { nx_span_any_assert_((s).data, (s).len, (s).tsz); } while (0)
#else
    #define NX_SPAN_ANY_ASSERT(s) ((void) 0)
#endif

/* ========== lifetime ========== */

nx_span nx_span_new(void *data, nx_usize len, nx_usize tsz);
nx_cspan nx_cspan_new(const void *data, nx_usize len, nx_usize tsz);
nx_cspan nx_cspan_from_span(nx_span s);

/* ========== access ========== */

void *nx_span_get(nx_span s, nx_usize idx);
const void *nx_span_get_c(nx_span s, nx_usize idx);
const void *nx_cspan_get_c(nx_cspan s, nx_usize idx);
void nx_span_set(nx_span s, nx_usize idx, const void *val);

/* ========== info ========== */

nx_bool nx_span_empty(nx_span s);
nx_bool nx_cspan_empty(nx_cspan s);

nx_usize nx_span_size_bytes(nx_span s);
nx_usize nx_cspan_size_bytes(nx_cspan s);

/* ========== subspan ========== */

nx_span nx_span_sub(nx_span s, nx_usize offset, nx_usize count);
nx_cspan nx_cspan_sub(nx_cspan s, nx_usize offset, nx_usize count);

nx_span nx_span_tail(nx_span s, nx_usize offset);
nx_cspan nx_cspan_tail(nx_cspan s, nx_usize offset);

/* ========== macros ========== */

#define NX_SPAN_FROM_PTR(ptr, len)    \
    nx_span_new((ptr), (len), sizeof((ptr)[0]))

#define NX_CSPAN_FROM_PTR(ptr, len)    \
    nx_cspan_new((ptr), (len), sizeof((ptr)[0]))

#define NX_SPAN_GET_AS(T, s, idx)        \
    (NX_ASSERT((s).tsz == sizeof(T)),    \
    (T *) nx_span_get((s), (idx)))

#define NX_SPAN_GET_AS_C(T, s, idx)          \
    (NX_ASSERT((s).tsz == sizeof(T)),        \
    (const T *) nx_span_get_c((s), (idx)))

#define NX_CSPAN_GET_AS_C(T, s, idx)     \
    (NX_ASSERT((s).tsz == sizeof(T)),    \
    (const T *) nx_cspan_get_c((s), (idx)))

#define NX_SPAN_SET(T, s, idx, expr)          \
    do {                                      \
        NX_ASSERT((s).tsz == sizeof(T));      \
        const T nx_tmp_ = (expr);             \
        nx_span_set((s), (idx), &nx_tmp_);    \
    } while (0)

#ifdef __cplusplus
}
#endif