#include "nx/core/span.h"

#include <string.h>

#include "nx/mem/ptr.h"

/* ---------- assert ---------- */

void nx_span_any_assert(const void *data, nx_usize len, nx_usize esz) {
    NX_ASSERT(esz > 0);
    NX_ASSERT(len == 0 || data != nx_null);
    NX_ASSERT(len <= NX_USIZE_MAX / esz);
}

/* ---------- lifetime ---------- */

nx_span nx_span_new(void *data, nx_usize len, nx_usize esz) {
    const nx_span s = {.data = data, .len = len, .esz = esz};

    NX_SPAN_ANY_ASSERT(s);

    return s;
}

nx_cspan nx_cspan_new(const void *data, nx_usize len, nx_usize esz) {
    const nx_cspan s = {.data = data, .len = len, .esz = esz};

    NX_SPAN_ANY_ASSERT(s);

    return s;
}

nx_cspan nx_cspan_from_span(nx_span s) {
    NX_SPAN_ANY_ASSERT(s);

    return nx_cspan_new(s.data, s.len, s.esz);
}

/* ---------- access ---------- */

void *nx_span_get(nx_span s, nx_usize idx) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return nx_byte_offset(s.data, s.esz, idx);
}

const void *nx_span_get_c(nx_span s, nx_usize idx) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return nx_byte_offset_c(s.data, s.esz, idx);
}

const void *nx_cspan_get_c(nx_cspan s, nx_usize idx) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return nx_byte_offset_c(s.data, s.esz, idx);
}

void nx_span_set(nx_span s, nx_usize idx, const void *elem) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(idx < s.len);
    NX_ASSERT(elem);

    memmove(nx_byte_offset(s.data, s.esz, idx), elem, s.esz);
}

/* ---------- info ---------- */

bool nx_span_empty(nx_span s) {
    NX_SPAN_ANY_ASSERT(s);

    return s.len == 0;
}
bool nx_cspan_empty(nx_cspan s) {
    NX_SPAN_ANY_ASSERT(s);

    return s.len == 0;
}

nx_usize nx_span_size_bytes(nx_span s) {
    NX_SPAN_ANY_ASSERT(s);

    return s.len * s.esz;
}

nx_usize nx_cspan_size_bytes(nx_cspan s) {
    NX_SPAN_ANY_ASSERT(s);

    return s.len * s.esz;
}

/* ---------- subspan ---------- */

nx_span nx_span_sub(nx_span s, nx_usize offset, nx_usize count) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(offset <= s.len);
    NX_ASSERT(count <= s.len - offset);

    void *p = s.data == nx_null ? nx_null : nx_byte_offset(s.data, s.esz, offset);

    return nx_span_new(p, count, s.esz);
}

nx_cspan nx_cspan_sub(nx_cspan s, nx_usize offset, nx_usize count) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(offset <= s.len);
    NX_ASSERT(count <= s.len - offset);

    const void *p = s.data == nx_null ? nx_null : nx_byte_offset_c(s.data, s.esz, offset);

    return nx_cspan_new(p, count, s.esz);
}

nx_span nx_span_tail(nx_span s, nx_usize offset) {
    return nx_span_sub(s, offset, s.len - offset);
}

nx_cspan nx_cspan_tail(nx_cspan s, nx_usize offset) {
    return nx_cspan_sub(s, offset, s.len - offset);
}
