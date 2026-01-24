#include "nx/core/span.h"

#include "nx/core/assert.h"

nx_span nx_make_span(void *data, size_t len, size_t elem_size) {
    return (nx_span){.data = data, .len = len, .elem_size = elem_size};
}

nx_cspan nx_make_cspan(const void *data, size_t len, size_t elem_size) {
    return (nx_cspan){.data = data, .len = len, .elem_size = elem_size};
}

void *nx_span_get(nx_span s, size_t idx) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return (char *) s.data + idx * s.elem_size;
}

const void *nx_cspan_get(nx_cspan s, size_t idx) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return (const char *) s.data + idx * s.elem_size;
}

