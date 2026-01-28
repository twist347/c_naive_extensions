#pragma once

#include <stdint.h>

#include "nx/core/type.h"
#include "nx/core/assert.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nx_span {
    void *data;
    nx_usize len;
    nx_usize elem_size;
} nx_span;

typedef struct nx_cspan {
    const void *data;
    nx_usize len;
    nx_usize elem_size;
} nx_cspan;

#define NX_ANY_SPAN_ASSERT(s)                              \
    do {                                                   \
        NX_ASSERT((s).elem_size > 0);                      \
        NX_ASSERT((s).len == 0 || (s).data != NULL);       \
        NX_ASSERT((s).len <= SIZE_MAX / (s).elem_size);    \
    } while (0)

/* ---------- make ---------- */

nx_span nx_span_make(void *data, nx_usize len, nx_usize elem_size);
nx_cspan nx_cspan_make(const void *data, nx_usize len, nx_usize elem_size);
nx_cspan nx_cspan_make_from_span(nx_span s);

/* ---------- access ---------- */

void *nx_span_get(nx_span s, nx_usize idx);
const void *nx_span_get_c(nx_span s, nx_usize idx);
const void *nx_cspan_get_c(nx_cspan s, nx_usize idx);
void nx_span_set(nx_span s, nx_usize idx, const void *elem);

/* ---------- info ---------- */

bool nx_span_empty(nx_span s);
bool nx_cspan_empty(nx_cspan s);

nx_usize nx_span_size_bytes(nx_span s);
nx_usize nx_cspan_size_bytes(nx_cspan s);

/* ---------- subspan ---------- */

nx_span nx_span_sub(nx_span s, nx_usize offset, nx_usize count);
nx_cspan nx_cspan_sub(nx_cspan s, nx_usize offset, nx_usize count);

nx_span nx_span_tail(nx_span s, nx_usize offset);
nx_cspan nx_cspan_tail(nx_cspan s, nx_usize offset);

/* ---------- macros ---------- */

#define NX_SPAN_FROM_PTR(ptr, len) \
    nx_span_make((ptr), (len), sizeof((ptr)[0]))

#define NX_CSPAN_FROM_PTR(ptr, len) \
    nx_cspan_make((ptr), (len), sizeof((ptr)[0]))

#define NX_SPAN_GET_AS(T, s, idx)              \
    (NX_ASSERT((s).elem_size == sizeof(T)),    \
    (T *) nx_span_get((s), (idx)))

#define NX_SPAN_GET_AS_C(T, s, idx)            \
    (NX_ASSERT((s).elem_size == sizeof(T)),    \
    (const T *) nx_span_get_c((s), (idx)))

#define NX_CSPAN_GET_AS_C(T, s, idx)           \
    (NX_ASSERT((s).elem_size == sizeof(T)),    \
    (const T *) nx_cspan_get_c((s), (idx)))

#define NX_SPAN_SET(T, s, idx, expr)              \
    do {                                          \
        NX_ASSERT((s).elem_size == sizeof(T));    \
        const T nx_tmp_ = (expr);                 \
        nx_span_set((s), (idx), &nx_tmp_);        \
    } while (0)

#ifdef __cplusplus
}
#endif