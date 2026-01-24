#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nx_span {
    void *data;
    size_t len;
    size_t elem_size;
} nx_span;

typedef struct nx_cspan {
    const void *data;
    size_t len;
    size_t elem_size;
} nx_cspan;

#define NX_SPAN_ASSERT(s)                               \
    do {                                                \
        NX_ASSERT((s).elem_size != 0);                  \
        NX_ASSERT((s).len == 0 || (s).data != NULL);    \
    } while (0)

nx_span nx_make_span(void *data, size_t len, size_t elem_size);
nx_cspan nx_make_cspan(const void *data, size_t len, size_t elem_size);

void *nx_span_get(nx_span s, size_t idx);
const void *nx_cspan_get(nx_cspan s, size_t idx);

#define NX_SPAN_FROM_PTR(ptr, len) \
    nx_make_span((ptr), (len), sizeof((ptr)[0]))

#define NX_CSPAN_FROM_PTR(ptr, len) \
    nx_make_cspan((ptr), (len), sizeof((ptr)[0]))

#ifdef __cplusplus
}
#endif