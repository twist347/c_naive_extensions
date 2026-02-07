#include "nx/data_structure/vec.h"

#include <stdlib.h>
#include <string.h>

#include "nx/core/limit.h"
#include "nx/mem/ptr.h"

struct nx_vec {
    void *data;
    nx_usize len;
    nx_usize cap;
    nx_usize esz;
};

// internals decls

static void vec_assert(const nx_vec *self);

static nx_status new_impl(nx_vec **out, nx_usize len, nx_usize cap, nx_usize esz);

static nx_status grow_if_needed(nx_vec *self);
static nx_status alloc_and_copy_data(void **out, const nx_vec *src);

static void set_fields(nx_vec *self, void *data, nx_usize len, nx_usize cap, nx_usize esz);

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

nx_vec_res nx_vec_new(nx_usize esz) {
    return nx_vec_new_p((nx_vec_params){
        .len = 0,
        .cap = 0,
        .tsz = esz,
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

void nx_vec_drop(nx_vec *self) {
    if (!self) {
        return;
    }

    free(self->data);
    free(self);
}

/* ========== copy/move semantic ========== */

nx_vec_res nx_vec_copy(const nx_vec *src) {
    vec_assert(src);

    nx_vec *dst = malloc(sizeof(nx_vec));
    if (!dst) {
        return NX_RES_NEW_ERR(nx_vec_res, NX_STATUS_OUT_OF_MEMORY);
    }

    set_fields(dst, nx_null, 0, 0, src->esz);

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

    set_fields(dst, data, src->len, src->cap, src->esz);
    return NX_RES_NEW_OK(nx_vec_res, dst);
}

// TODO: swap or exchange?
nx_vec *nx_vec_move(nx_vec **src) {
    NX_ASSERT(src);
    NX_ASSERT(*src);
    vec_assert(*src);

    nx_vec *tmp = *src;
    *src = nx_null;
    return tmp;
}

nx_status nx_vec_copy_assign(nx_vec *self, const nx_vec *src) {
    vec_assert(self);
    vec_assert(src);
    NX_ASSERT(self->esz == src->esz);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->cap == 0) {
        free(self->data);
        set_fields(self, nx_null, src->len, src->cap, self->esz);
        return NX_STATUS_OK;
    }

    void *data = nx_null;
    const nx_status st = alloc_and_copy_data(&data, src);
    if (st != NX_STATUS_OK) {
        return st;
    }

    free(self->data);
    set_fields(self, data, src->len, src->cap, self->esz);
    return NX_STATUS_OK;
}

// TODO: swap or exchange?
void nx_vec_move_assign(nx_vec *self, nx_vec *src) {
    vec_assert(self);
    vec_assert(src);
    NX_ASSERT(self->esz == src->esz);

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
    vec_assert(self);

    return self->len;
}

nx_bool nx_vec_empty(const nx_vec *self) {
    vec_assert(self);

    return self->len == 0;
}

nx_usize nx_vec_cap(const nx_vec *self) {
    vec_assert(self);

    return self->cap;
}

nx_usize nx_vec_tsz(const nx_vec *self) {
    vec_assert(self);

    return self->esz;
}

/* ========== access ========== */

void *nx_vec_get(nx_vec *self, nx_usize idx) {
    vec_assert(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset(self->data, self->esz, idx);
}

const void *nx_vec_get_c(const nx_vec *self, nx_usize idx) {
    vec_assert(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset_c(self->data, self->esz, idx);
}

void nx_vec_set(nx_vec *self, nx_usize idx, const void *val) {
    vec_assert(self);
    NX_ASSERT(idx < self->len);
    NX_ASSERT(val);

    memcpy(nx_byte_offset(self->data, self->esz, idx), val, self->esz);
}

void *nx_vec_data(nx_vec *self) {
    vec_assert(self);

    return self->data;
}

const void *nx_vec_data_c(const nx_vec *self) {
    vec_assert(self);

    return self->data;
}

/* ========== mods ========== */

nx_status nx_vec_push(nx_vec *self, const void *val) {
    vec_assert(self);
    NX_ASSERT(val);

    const nx_status st = grow_if_needed(self);
    if (st != NX_STATUS_OK) {
        return st;
    }

    ++self->len;
    nx_vec_set(self, self->len - 1, val);
    return NX_STATUS_OK;
}

void *nx_vec_pop(nx_vec *self) {
    vec_assert(self);
    NX_ASSERT(self->len > 0);

    void *val = nx_vec_get(self, self->len - 1);
    --self->len;
    return val;
}

void nx_vec_clear(nx_vec *self) {
    vec_assert(self);

    self->len = 0;
}

nx_status nx_vec_reserve(nx_vec *self, nx_usize new_cap) {
    vec_assert(self);

    if (new_cap <= self->cap) {
        return NX_STATUS_OK;
    }

    NX_ASSERT(new_cap <= NX_USIZE_MAX / self->esz);
    const nx_usize bytes = new_cap * self->esz;

    void *data = realloc(self->data, bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    self->data = data;
    self->cap = new_cap;

    return NX_STATUS_OK;
}

nx_status nx_vec_resize(nx_vec *self, nx_usize new_len) {
    vec_assert(self);

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
    const nx_usize add_bytes = (new_len - self->len) * self->esz;
    memset(nx_byte_offset(self->data, self->esz, self->len), 0, add_bytes);
    self->len = new_len;

    return NX_STATUS_OK;
}

nx_status nx_vec_shrink_to_fit(nx_vec *self) {
    vec_assert(self);

    if (self->len == self->cap) {
        return NX_STATUS_OK;
    }

    if (self->len == 0) {
        free(self->data);
        set_fields(self, nx_null, 0, 0, self->esz);
        return NX_STATUS_OK;
    }

    NX_ASSERT(self->len <= NX_USIZE_MAX / self->esz);
    const nx_usize bytes = self->len * self->esz;

    void *data = realloc(self->data, bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY; /* keep old buffer intact */
    }

    self->data = data;
    self->cap = self->len;

    return NX_STATUS_OK;
}

void nx_vec_swap(nx_vec *a, nx_vec *b) {
    vec_assert(a);
    vec_assert(b);
    NX_ASSERT(a->esz == b->esz);

    if (a == b) {
        return;
    }

    const nx_vec tmp = *a;
    *a = *b;
    *b = tmp;
}

nx_status nx_vec_insert(nx_vec *self, nx_usize idx, const void *val) {
    vec_assert(self);
    NX_ASSERT(idx <= self->len);
    NX_ASSERT(val);

    const nx_usize old_len = self->len;

    const nx_status st = grow_if_needed(self);
    if (st != NX_STATUS_OK) {
        return st;
    }

    if (idx < old_len) {
        shift_right(self, idx);
    }

    ++self->len;
    nx_vec_set(self, idx, val);

    return NX_STATUS_OK;
}

void nx_vec_erase(nx_vec *self, nx_usize idx) {
    vec_assert(self);
    NX_ASSERT(idx < self->len);

    shift_left(self, idx);
    --self->len;
}

/* ========== to span ========== */

nx_span nx_vec_to_span(nx_vec *self) {
    vec_assert(self);

    return nx_span_new(self->data, self->len, self->esz);
}

nx_cspan nx_vec_to_cspan(const nx_vec *self) {
    vec_assert(self);

    return nx_cspan_new(self->data, self->len, self->esz);
}

// internals defs

static void vec_assert(const nx_vec *self) {
    NX_ASSERT(self != nx_null);
    NX_ASSERT(self->esz > 0);
    NX_ASSERT(self->len <= self->cap);
    NX_ASSERT((self->cap == 0) == (self->data == nx_null));
    NX_ASSERT(self->cap <= NX_USIZE_MAX / self->esz);
}

static nx_status new_impl(nx_vec **out, nx_usize len, nx_usize cap, nx_usize esz) {
    NX_ASSERT(out);

    *out = nx_null;
    nx_vec *vec = malloc(sizeof(nx_vec));
    if (!vec) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(vec, nx_null, 0, 0, esz);

    if (cap == 0) {
        *out = vec;
        return NX_STATUS_OK;
    }

    NX_ASSERT(cap <= NX_USIZE_MAX / esz);
    const nx_usize cap_bytes = cap * esz;

    void *data = malloc(cap_bytes);
    if (!data) {
        free(vec);
        return NX_STATUS_OUT_OF_MEMORY;
    }

    NX_ASSERT(len <= NX_USIZE_MAX / esz);
    const nx_usize len_bytes = len * esz;
    memset(data, 0, len_bytes);

    set_fields(vec, data, len, cap, esz);
    *out = vec;
    return NX_STATUS_OK;
}

static nx_status grow_if_needed(nx_vec *self) {
    vec_assert(self);

    if (self->len < self->cap) {
        return NX_STATUS_OK;
    }

    nx_usize new_cap;
    if (self->cap == 0) {
        new_cap = 1;
    } else {
        NX_ASSERT(self->cap <= NX_USIZE_MAX / (nx_usize) 2);
        new_cap = self->cap * (nx_usize) 2;
    }

    return nx_vec_reserve(self, new_cap);
}

static nx_status alloc_and_copy_data(void **out, const nx_vec *src) {
    NX_ASSERT(out);
    *out = nx_null;

    if (src->cap == 0) {
        return NX_STATUS_OK;
    }

    NX_ASSERT(src->cap <= NX_USIZE_MAX / src->esz);
    const nx_usize cap_bytes = src->cap * src->esz;

    void *data = malloc(cap_bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    NX_ASSERT(src->len <= NX_USIZE_MAX / src->esz);
    const nx_usize len_bytes = src->len * src->esz;
    if (len_bytes > 0) {
        memcpy(data, src->data, len_bytes);
    }

    *out = data;
    return NX_STATUS_OK;
}

static void set_fields(nx_vec *self, void *data, nx_usize len, nx_usize cap, nx_usize esz) {
    self->data = data;
    self->len = len;
    self->cap = cap;
    self->esz = esz;
}

static void shift_left(nx_vec *self, nx_usize idx) {
    memmove(
        nx_byte_offset(self->data, self->esz, idx),
        nx_byte_offset(self->data, self->esz, idx + 1),
        (self->len - idx - 1) * self->esz
    );
}

static void shift_right(nx_vec *self, nx_usize idx) {
    memmove(
        nx_byte_offset(self->data, self->esz, idx + 1),
        nx_byte_offset(self->data, self->esz, idx),
        (self->len - idx) * self->esz
    );
}
