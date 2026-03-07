#include "nx/ds/span.h"

#include <string.h>

#include "nx/mem/ptr.h"
#include "../../include/nx/core/limits.h"

/* ========== assert ========== */

#if NX_DEBUG
void nx_span_any_assert_(const void *data, nx_usize len, nx_usize tsz) {
    NX_ASSERT(tsz > 0);
    NX_ASSERT(len == 0 || data != nx_null);
}
#endif

/* ========== lifetime ========== */

nx_Span nx_span_new(void *data, nx_usize len, nx_usize tsz) {
    const nx_Span s = {.data = data, .len = len, .tsz = tsz};

    NX_SPAN_ANY_ASSERT(s);

    return s;
}

nx_CSpan nx_cspan_new(const void *data, nx_usize len, nx_usize tsz) {
    const nx_CSpan s = {.data = data, .len = len, .tsz = tsz};

    NX_SPAN_ANY_ASSERT(s);

    return s;
}

nx_CSpan nx_cspan_from_span(nx_Span s) {
    NX_SPAN_ANY_ASSERT(s);

    return nx_cspan_new(s.data, s.len, s.tsz);
}

/* ========== access ========== */

void *nx_span_get(nx_Span s, nx_usize idx) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return nx_byte_offset(s.data, s.tsz, idx);
}

const void *nx_span_get_c(nx_Span s, nx_usize idx) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return nx_byte_offset_c(s.data, s.tsz, idx);
}

const void *nx_cspan_get_c(nx_CSpan s, nx_usize idx) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(idx < s.len);

    return nx_byte_offset_c(s.data, s.tsz, idx);
}

void *nx_span_at(nx_Span s, nx_usize idx) {
    NX_SPAN_ANY_ASSERT(s);

    return idx < s.len ? nx_byte_offset(s.data, s.tsz, idx) : nx_null;
}

const void *nx_span_at_c(nx_Span s, nx_usize idx) {
    NX_SPAN_ANY_ASSERT(s);

    return idx < s.len ? nx_byte_offset_c(s.data, s.tsz, idx) : nx_null;
}

const void *nx_cspan_at_c(nx_CSpan s, nx_usize idx) {
    NX_SPAN_ANY_ASSERT(s);

    return idx < s.len ? nx_byte_offset_c(s.data, s.tsz, idx) : nx_null;
}

void nx_span_set(nx_Span s, nx_usize idx, const void *val) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(idx < s.len);
    NX_ASSERT(val);

    memcpy(nx_byte_offset(s.data, s.tsz, idx), val, s.tsz);
}

/* ========== info ========== */

nx_bool nx_span_empty(nx_Span s) {
    NX_SPAN_ANY_ASSERT(s);

    return s.len == 0;
}

nx_bool nx_cspan_empty(nx_CSpan s) {
    NX_SPAN_ANY_ASSERT(s);

    return s.len == 0;
}

nx_usize nx_span_size_bytes(nx_Span s) {
    NX_SPAN_ANY_ASSERT(s);

    return s.len * s.tsz;
}

nx_usize nx_cspan_size_bytes(nx_CSpan s) {
    NX_SPAN_ANY_ASSERT(s);

    return s.len * s.tsz;
}

/* ========== operations ========== */

void nx_span_copy(nx_Span dst, nx_CSpan src) {
    NX_SPAN_ANY_ASSERT(dst);
    NX_SPAN_ANY_ASSERT(src);
    NX_ASSERT(dst.len == src.len);
    NX_ASSERT(dst.tsz == src.tsz);

    if (dst.len == 0) {
        return;
    }

    const nx_usize bytes = dst.len * dst.tsz;
    memmove(dst.data, src.data, bytes);
}

/* ========== subspan ========== */

nx_Span nx_span_sub(nx_Span s, nx_usize offset, nx_usize count) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(offset <= s.len);
    NX_ASSERT(count <= s.len - offset);

    void *p = s.data == nx_null ? nx_null : nx_byte_offset(s.data, s.tsz, offset);

    return nx_span_new(p, count, s.tsz);
}

nx_CSpan nx_cspan_sub(nx_CSpan s, nx_usize offset, nx_usize count) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(offset <= s.len);
    NX_ASSERT(count <= s.len - offset);

    const void *p = s.data == nx_null ? nx_null : nx_byte_offset_c(s.data, s.tsz, offset);

    return nx_cspan_new(p, count, s.tsz);
}

nx_Span nx_span_tail(nx_Span s, nx_usize offset) {
    return nx_span_sub(s, offset, s.len - offset);
}

nx_CSpan nx_cspan_tail(nx_CSpan s, nx_usize offset) {
    return nx_cspan_sub(s, offset, s.len - offset);
}

/* ========== span printing ========== */

void nx_fprintln_cspan(FILE *stream, nx_CSpan s, nx_fprint_fn f) {
    NX_ASSERT(stream);
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(f);

    fputc('[', stream);
    for (nx_usize i = 0; i < s.len; ++i) {
        if (i > 0) {
            fputs(", ", stream);
        }
        f(stream, nx_cspan_get_c(s, i));
    }
    fputs("]\n", stream);
}

void nx_fprintln_span(FILE *stream, nx_Span s, nx_fprint_fn f) {
    nx_fprintln_cspan(stream, nx_cspan_from_span(s), f);
}

void nx_println_cspan(nx_CSpan s, nx_fprint_fn f) {
    nx_fprintln_cspan(stdout, s, f);
}

void nx_println_span(nx_Span s, nx_fprint_fn f) {
    nx_fprintln_span(stdout, s, f);
}
