#include "nx/core/span.h"

#include <string.h>

#include "nx/mem/byte.h"

/* ---------- make ---------- */

nx_span nx_span_make(void *data, nx_usize len, nx_usize elem_size) {
    const nx_span s = {.data = data, .len = len, .elem_size = elem_size};

    NX_ANY_SPAN_ASSERT(s);

    return s;
}

nx_cspan nx_cspan_make(const void *data, nx_usize len, nx_usize elem_size) {
    const nx_cspan s = {.data = data, .len = len, .elem_size = elem_size};

    NX_ANY_SPAN_ASSERT(s);

    return s;
}

nx_cspan nx_cspan_make_from_span(nx_span s) {
    NX_ANY_SPAN_ASSERT(s);

    return nx_cspan_make(s.data, s.len, s.elem_size);
}

/* ---------- access ---------- */

void *nx_span_get(nx_span s, nx_usize idx) {
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return nx_byte_offset(s.data, s.elem_size, idx);
}

const void *nx_span_get_c(nx_span s, nx_usize idx) {
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return nx_byte_offset_c(s.data, s.elem_size, idx);
}

const void *nx_cspan_get_c(nx_cspan s, nx_usize idx) {
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return nx_byte_offset_c(s.data, s.elem_size, idx);
}

void nx_span_set(nx_span s, nx_usize idx, const void *elem) {
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(idx < s.len);
    NX_ASSERT(elem);

    memmove(nx_byte_offset(s.data, s.elem_size, idx), elem, s.elem_size);
}

/* ---------- info ---------- */

bool nx_span_empty(nx_span s) {
    NX_ANY_SPAN_ASSERT(s);

    return s.len == 0;
}
bool nx_cspan_empty(nx_cspan s) {
    NX_ANY_SPAN_ASSERT(s);

    return s.len == 0;
}

nx_usize nx_span_size_bytes(nx_span s) {
    NX_ANY_SPAN_ASSERT(s);

    return s.len * s.elem_size;
}

nx_usize nx_cspan_size_bytes(nx_cspan s) {
    NX_ANY_SPAN_ASSERT(s);

    return s.len * s.elem_size;
}

/* ---------- subspan ---------- */

nx_span nx_span_sub(nx_span s, nx_usize offset, nx_usize count) {
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(offset <= s.len);
    NX_ASSERT(count <= s.len - offset);

    void *p = s.data == nx_null ? nx_null : nx_byte_offset(s.data, s.elem_size, offset);

    return nx_span_make(p, count, s.elem_size);
}

nx_cspan nx_cspan_sub(nx_cspan s, nx_usize offset, nx_usize count) {
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(offset <= s.len);
    NX_ASSERT(count <= s.len - offset);

    const void *p = s.data == nx_null ? nx_null : nx_byte_offset_c(s.data, s.elem_size, offset);

    return nx_cspan_make(p, count, s.elem_size);
}

nx_span nx_span_tail(nx_span s, nx_usize offset) {
    return nx_span_sub(s, offset, s.len - offset);
}

nx_cspan nx_cspan_tail(nx_cspan s, nx_usize offset) {
    return nx_cspan_sub(s, offset, s.len - offset);
}
