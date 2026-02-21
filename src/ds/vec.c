#include "nx/ds/vec.h"

#include <stdlib.h>
#include <string.h>

#include "../../include/nx/numeric/limit.h"
#include "nx/mem/ptr.h"

struct nx_vec {
    void *data;
    nx_usize len;
    nx_usize cap;
    nx_usize tsz;
};

#define ASSERT_ADD_OK(a, b, m)          \
    do {                                \
        NX_ASSERT((b) != 0);            \
        NX_ASSERT((a) <= (m) - (b));    \
    } while (0)

#define ASSERT_MUL_OK(a, b, m)          \
    do {                                \
        NX_ASSERT((b) != 0);            \
        NX_ASSERT((a) <= (m) / (b));    \
    } while (0)

#define ASSERT_INC_OK(a, m)    \
    do {                       \
        NX_ASSERT((a) < m);    \
    } while (0)

#if NX_DEBUG
static void vec_assert_impl(const nx_vec *self) {
    NX_ASSERT(self != nx_null);
    NX_ASSERT(self->tsz > 0);
    NX_ASSERT(self->len <= self->cap);
    NX_ASSERT((self->cap == 0) == (self->data == nx_null));
    ASSERT_MUL_OK(self->cap, self->tsz, NX_USIZE_MAX);
}

#define VEC_ASSERT(self)    \
    do { vec_assert_impl((self)); } while (0)
#else
    #define VEC_ASSERT(self)    ((void) 0)
#endif

// internals decls

static nx_status new_impl(nx_vec **out, nx_usize len, nx_usize cap, nx_usize tsz);

static nx_status ensure_cap(nx_vec *self, nx_usize needed_cap);
static nx_status alloc_and_copy_data(void **out, const nx_vec *src);

static void set_fields(nx_vec *self, void *data, nx_usize len, nx_usize cap, nx_usize tsz);

static void shift_left(nx_vec *self, nx_usize idx);
static void shift_right(nx_vec *self, nx_usize idx);

/* ========== lifetime ========== */

nx_vec_res nx_vec_new_p(nx_vec_params p) {
    NX_ASSERT(p.tsz != 0);
    NX_ASSERT(p.len <= p.cap);

    nx_vec *vec = nx_null;
    const nx_status st = new_impl(&vec, p.len, p.cap, p.tsz);
    if (st != NX_STATUS_OK) {
        return NX_RES_NEW_ERR(nx_vec_res, st);
    }
    return NX_RES_NEW_OK(nx_vec_res, vec);
}

nx_vec_res nx_vec_new(nx_usize tsz) {
    return nx_vec_new_p((nx_vec_params){
        .len = 0,
        .cap = 0,
        .tsz = tsz,
    });
}

nx_vec_res nx_vec_new_len(nx_usize len, nx_usize tsz) {
    return nx_vec_new_p((nx_vec_params){
        .len = len,
        .cap = len,
        .tsz = tsz,
    });
}

nx_vec_res nx_vec_new_cap(nx_usize cap, nx_usize tsz) {
    return nx_vec_new_p((nx_vec_params){
        .len = 0,
        .cap = cap,
        .tsz = tsz,
    });
}

nx_vec_res nx_vec_from_data(const void *data, nx_usize len, nx_usize tsz) {
    NX_ASSERT(data || len == 0);

    nx_vec_res res = nx_vec_new_len(len, tsz);
    if (!NX_RES_IS_OK(res)) {
        return res;
    }

    nx_vec *vec = NX_RES_UNWRAP(res);

    if (len > 0) {
        ASSERT_MUL_OK(len, tsz, NX_USIZE_MAX);
        const nx_usize bytes = len * tsz;
        memcpy(vec->data, data, bytes);
    }

    return NX_RES_NEW_OK(nx_vec_res, vec);
}

void nx_vec_drop(nx_vec *self) {
    if (!self) {
        return;
    }

    free(self->data);
    free(self);
}

/* ========== copy/move semantic ========== */

nx_vec_res nx_vec_copy(const nx_vec *src) {
    VEC_ASSERT(src);

    nx_vec *dst = malloc(sizeof(nx_vec));
    if (!dst) {
        return NX_RES_NEW_ERR(nx_vec_res, NX_STATUS_OUT_OF_MEMORY);
    }

    set_fields(dst, nx_null, 0, 0, src->tsz);

    if (src->cap == 0) {
        /* empty/unallocated */
        return NX_RES_NEW_OK(nx_vec_res, dst);
    }

    void *data = nx_null;
    const nx_status st = alloc_and_copy_data(&data, src);
    if (st != NX_STATUS_OK) {
        free(dst);
        return NX_RES_NEW_ERR(nx_vec_res, st);
    }

    set_fields(dst, data, src->len, src->cap, src->tsz);
    return NX_RES_NEW_OK(nx_vec_res, dst);
}

// TODO: swap or exchange?
nx_vec *nx_vec_move(nx_vec **src) {
    NX_ASSERT(src);
    NX_ASSERT(*src);
    VEC_ASSERT(*src);

    nx_vec *tmp = *src;
    *src = nx_null;
    return tmp;
}

nx_status nx_vec_copy_assign(nx_vec *self, const nx_vec *src) {
    VEC_ASSERT(self);
    VEC_ASSERT(src);
    NX_ASSERT(self->tsz == src->tsz);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->len == 0) {
        self->len = 0;
        return NX_STATUS_OK;
    }

    // Optimization: reuse existing buffer if it fits
    if (self->cap >= src->len) {
        ASSERT_MUL_OK(src->len, src->tsz, NX_USIZE_MAX);
        const nx_usize bytes = src->len * src->tsz;
        memcpy(self->data, src->data, bytes);
        self->len = src->len;
        return NX_STATUS_OK;
    }

    void *data = nx_null;
    const nx_status st = alloc_and_copy_data(&data, src);
    if (st != NX_STATUS_OK) {
        return st;
    }

    free(self->data);
    set_fields(self, data, src->len, src->cap, self->tsz);
    return NX_STATUS_OK;
}

// TODO: swap or exchange?
void nx_vec_move_assign(nx_vec *self, nx_vec *src) {
    VEC_ASSERT(self);
    VEC_ASSERT(src);
    NX_ASSERT(self->tsz == src->tsz);

    if (self == src) {
        return;
    }

    free(self->data);

    self->data = src->data;
    self->len = src->len;
    self->cap = src->cap;

    src->data = nx_null;
    src->len = 0;
    src->cap = 0;
}

/* ========== info ========== */

nx_usize nx_vec_len(const nx_vec *self) {
    VEC_ASSERT(self);

    return self->len;
}

nx_bool nx_vec_empty(const nx_vec *self) {
    VEC_ASSERT(self);

    return self->len == 0;
}

nx_usize nx_vec_cap(const nx_vec *self) {
    VEC_ASSERT(self);

    return self->cap;
}

nx_usize nx_vec_tsz(const nx_vec *self) {
    VEC_ASSERT(self);

    return self->tsz;
}

/* ========== access ========== */

void *nx_vec_get(nx_vec *self, nx_usize idx) {
    VEC_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset(self->data, self->tsz, idx);
}

const void *nx_vec_get_c(const nx_vec *self, nx_usize idx) {
    VEC_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset_c(self->data, self->tsz, idx);
}

void *nx_vec_at(nx_vec *self, nx_usize idx) {
    VEC_ASSERT(self);

    return idx < self->len ? nx_byte_offset(self->data, self->tsz, idx) : nx_null;
}

const void *nx_vec_at_c(const nx_vec *self, nx_usize idx) {
    VEC_ASSERT(self);

    return idx < self->len ? nx_byte_offset_c(self->data, self->tsz, idx) : nx_null;
}

void nx_vec_set(nx_vec *self, nx_usize idx, const void *val) {
    VEC_ASSERT(self);
    NX_ASSERT(idx < self->len);
    NX_ASSERT(val);

    memcpy(nx_byte_offset(self->data, self->tsz, idx), val, self->tsz);
}

void *nx_vec_data(nx_vec *self) {
    VEC_ASSERT(self);

    return self->data;
}

const void *nx_vec_data_c(const nx_vec *self) {
    VEC_ASSERT(self);

    return self->data;
}

/* ========== mods ========== */

nx_status nx_vec_push(nx_vec *self, const void *val) {
    VEC_ASSERT(self);
    NX_ASSERT(val);

    ASSERT_INC_OK(self->len, NX_USIZE_MAX);
    const nx_status st = ensure_cap(self, self->len + 1);
    if (st != NX_STATUS_OK) {
        return st;
    }

    ++self->len;
    nx_vec_set(self, self->len - 1, val);
    return NX_STATUS_OK;
}

void *nx_vec_pop(nx_vec *self) {
    VEC_ASSERT(self);
    NX_ASSERT(self->len > 0);

    void *val = nx_vec_get(self, self->len - 1);
    --self->len;
    return val;
}

void nx_vec_clear(nx_vec *self) {
    VEC_ASSERT(self);

    self->len = 0;
}

nx_status nx_vec_reserve(nx_vec *self, nx_usize new_cap) {
    VEC_ASSERT(self);

    if (new_cap <= self->cap) {
        return NX_STATUS_OK;
    }

    ASSERT_MUL_OK(new_cap, self->tsz, NX_USIZE_MAX);
    const nx_usize bytes = new_cap * self->tsz;

    void *data = realloc(self->data, bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    self->data = data;
    self->cap = new_cap;

    return NX_STATUS_OK;
}

nx_status nx_vec_resize(nx_vec *self, nx_usize new_len) {
    VEC_ASSERT(self);

    if (new_len <= self->len) {
        self->len = new_len;
        return NX_STATUS_OK;
    }

    if (new_len > self->cap) {
        const nx_status st = nx_vec_reserve(self, new_len);
        if (st != NX_STATUS_OK) {
            return st;
        }
    }

    /* zero-init new tail */
    ASSERT_MUL_OK(new_len - self->len, self->tsz, NX_USIZE_MAX);
    const nx_usize add_bytes = (new_len - self->len) * self->tsz;
    memset(nx_byte_offset(self->data, self->tsz, self->len), 0, add_bytes);
    self->len = new_len;

    return NX_STATUS_OK;
}

nx_status nx_vec_shrink_to_fit(nx_vec *self) {
    VEC_ASSERT(self);

    if (self->len == self->cap) {
        return NX_STATUS_OK;
    }

    if (self->len == 0) {
        free(self->data);
        set_fields(self, nx_null, 0, 0, self->tsz);
        return NX_STATUS_OK;
    }

    ASSERT_MUL_OK(self->len, self->tsz, NX_USIZE_MAX);
    const nx_usize bytes = self->len * self->tsz;

    void *data = realloc(self->data, bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY; /* keep old buffer intact */
    }

    self->data = data;
    self->cap = self->len;

    return NX_STATUS_OK;
}

void nx_vec_swap(nx_vec *a, nx_vec *b) {
    VEC_ASSERT(a);
    VEC_ASSERT(b);
    NX_ASSERT(a->tsz == b->tsz);

    if (a == b) {
        return;
    }

    const nx_vec tmp = *a;
    *a = *b;
    *b = tmp;
}

nx_status nx_vec_insert(nx_vec *self, nx_usize idx, const void *val) {
    VEC_ASSERT(self);
    NX_ASSERT(idx <= self->len);
    NX_ASSERT(val);

    const nx_usize old_len = self->len;

    const nx_status st = ensure_cap(self, self->len + 1);
    if (st != NX_STATUS_OK) {
        return st;
    }

    if (idx < old_len) {
        shift_right(self, idx);
    }

    ASSERT_INC_OK(self->len, NX_USIZE_MAX);
    ++self->len;
    nx_vec_set(self, idx, val);

    return NX_STATUS_OK;
}

void nx_vec_erase(nx_vec *self, nx_usize idx) {
    VEC_ASSERT(self);
    NX_ASSERT(idx < self->len);

    shift_left(self, idx);
    --self->len;
}

/* ========== to span ========== */

nx_span nx_vec_to_span(nx_vec *self) {
    VEC_ASSERT(self);

    return nx_span_new(self->data, self->len, self->tsz);
}

nx_cspan nx_vec_to_cspan(const nx_vec *self) {
    VEC_ASSERT(self);

    return nx_cspan_new(self->data, self->len, self->tsz);
}

// internals defs

static nx_status new_impl(nx_vec **out, nx_usize len, nx_usize cap, nx_usize tsz) {
    NX_ASSERT(out);

    *out = nx_null;
    nx_vec *vec = malloc(sizeof(nx_vec));
    if (!vec) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(vec, nx_null, 0, 0, tsz);

    if (cap == 0) {
        *out = vec;
        return NX_STATUS_OK;
    }

    ASSERT_MUL_OK(cap, tsz, NX_USIZE_MAX);
    const nx_usize cap_bytes = cap * tsz;

    void *data = malloc(cap_bytes);
    if (!data) {
        free(vec);
        return NX_STATUS_OUT_OF_MEMORY;
    }

    ASSERT_MUL_OK(len, tsz, NX_USIZE_MAX);
    const nx_usize len_bytes = len * tsz;
    memset(data, 0, len_bytes);

    set_fields(vec, data, len, cap, tsz);
    *out = vec;
    return NX_STATUS_OK;
}

static nx_status ensure_cap(nx_vec *self, nx_usize needed_cap) {
    VEC_ASSERT(self);

    if (needed_cap <= self->cap) {
        return NX_STATUS_OK;
    }

    ASSERT_MUL_OK(needed_cap, self->tsz, NX_USIZE_MAX);

    nx_usize new_cap = self->cap == 0 ? 1 : self->cap;

    while (new_cap < needed_cap) {
        // Check if we can double
        if (new_cap > NX_USIZE_MAX / 2) {
            // Can't double - use exact needed_cap
            new_cap = needed_cap;
            break;
        }
        new_cap *= 2;
    }

    return nx_vec_reserve(self, new_cap);
}

static nx_status alloc_and_copy_data(void **out, const nx_vec *src) {
    NX_ASSERT(out);
    *out = nx_null;

    if (src->cap == 0) {
        return NX_STATUS_OK;
    }

    ASSERT_MUL_OK(src->cap, src->tsz, NX_USIZE_MAX);
    const nx_usize cap_bytes = src->cap * src->tsz;

    void *data = malloc(cap_bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    if (src->len > 0) {
        ASSERT_MUL_OK(src->len, src->tsz, NX_USIZE_MAX);
        const nx_usize len_bytes = src->len * src->tsz;
        memcpy(data, src->data, len_bytes);
    }

    *out = data;
    return NX_STATUS_OK;
}

static void set_fields(nx_vec *self, void *data, nx_usize len, nx_usize cap, nx_usize tsz) {
    self->data = data;
    self->len = len;
    self->cap = cap;
    self->tsz = tsz;
}

static void shift_left(nx_vec *self, nx_usize idx) {
    memmove(
        nx_byte_offset(self->data, self->tsz, idx),
        nx_byte_offset(self->data, self->tsz, idx + 1),
        (self->len - idx - 1) * self->tsz
    );
}

static void shift_right(nx_vec *self, nx_usize idx) {
    memmove(
        nx_byte_offset(self->data, self->tsz, idx + 1),
        nx_byte_offset(self->data, self->tsz, idx),
        (self->len - idx) * self->tsz
    );
}
