#include "nx/data_structure/vec.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "nx/core/checked_arith.h"
#include "nx/mem/byte.h"

struct nx_vec {
    void *data;
    size_t len;
    size_t cap;
    size_t elem_size;
};

#define NX_VEC_ASSERT(self_)                                          \
    do {                                                              \
        NX_ASSERT((self_) != NULL);                                   \
        NX_ASSERT((self_)->elem_size > 0);                            \
        NX_ASSERT((self_)->len <= (self_)->cap);                      \
        NX_ASSERT(((self_)->cap == 0) == ((self_)->data == NULL));    \
        NX_ASSERT((self_)->len <= SIZE_MAX / (self_)->elem_size);     \
        NX_ASSERT((self_)->cap <= SIZE_MAX / (self_)->elem_size);     \
    } while (0)

// internals decls

static nx_status create(nx_vec **out, size_t len, size_t cap, size_t elem_size);

static nx_status grow_if_needed(nx_vec *self);
static nx_status alloc_and_copy_data(void **out, const nx_vec *src);

static void set_fields(nx_vec *self, void *data, size_t len, size_t cap, size_t elem_size);
static void reset_fields(nx_vec *self);

static void shift_left(nx_vec *self, size_t idx);
static void shift_right(nx_vec *self, size_t idx);

/* ---------- create/destroy ---------- */

nx_status nx_vec_make(nx_vec **out, size_t len, size_t elem_size) {
    NX_ASSERT(out);
    NX_ASSERT(elem_size > 0);

    return create(out, len, len, elem_size);
}

nx_status nx_vec_make_cap(nx_vec **out, size_t cap, size_t elem_size) {
    NX_ASSERT(out);
    NX_ASSERT(elem_size > 0);

    return create(out, 0, cap, elem_size);
}

void nx_vec_drop(nx_vec *self) {
    if (!self) {
        return;
    }

    free(self->data);
    free(self);
}

/* ---------- copy/move semantic ---------- */

nx_status nx_vec_copy(nx_vec **out, const nx_vec *src) {
    NX_ASSERT(out);
    NX_VEC_ASSERT(src);

    *out = NULL;

    nx_vec *dst = malloc(sizeof(nx_vec));
    if (!dst) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(dst, NULL, 0, 0, src->elem_size);

    if (src->cap == 0) {
        set_fields(dst, NULL, src->len, src->cap, src->elem_size);
        *out = dst;
        return NX_STATUS_OK;
    }

    void *data = NULL;
    const nx_status st = alloc_and_copy_data(&data, src);
    if (st != NX_STATUS_OK) {
        free(dst);
        return st;
    }

    set_fields(dst, data, src->len, src->cap, src->elem_size);
    *out = dst;
    return NX_STATUS_OK;
}

nx_status nx_vec_move(nx_vec **out, nx_vec *src) {
    NX_ASSERT(out);
    NX_VEC_ASSERT(src);

    *out = NULL;

    nx_vec *dst = malloc(sizeof(nx_vec));
    if (!dst) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    *dst = *src;
    reset_fields(src);

    *out = dst;
    return NX_STATUS_OK;
}

nx_status nx_vec_copy_assign(nx_vec *self, const nx_vec *src) {
    NX_VEC_ASSERT(self);
    NX_VEC_ASSERT(src);
    NX_ASSERT(self->elem_size == src->elem_size);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->cap == 0) {
        free(self->data);
        set_fields(self, NULL, src->len, src->cap, self->elem_size);
        return NX_STATUS_OK;
    }

    void *data = NULL;
    const nx_status st = alloc_and_copy_data(&data, src);
    if (st != NX_STATUS_OK) {
        return st;
    }

    free(self->data);
    set_fields(self, data, src->len, src->cap, self->elem_size);
    return NX_STATUS_OK;
}

nx_status nx_vec_move_assign(nx_vec *self, nx_vec *src) {
    NX_VEC_ASSERT(self);
    NX_VEC_ASSERT(src);
    NX_ASSERT(self->elem_size == src->elem_size);

    if (self == src) {
        return NX_STATUS_OK;
    }

    free(self->data);
    *self = *src;
    reset_fields(src);
    return NX_STATUS_OK;
}

/* ---------- info ---------- */

size_t nx_vec_len(const nx_vec *self) {
    NX_VEC_ASSERT(self);

    return self->len;
}

bool nx_vec_empty(const nx_vec *self) {
    NX_VEC_ASSERT(self);

    return self->len == 0;
}

size_t nx_vec_cap(const nx_vec *self) {
    NX_VEC_ASSERT(self);

    return self->cap;
}

size_t nx_vec_elem_size(const nx_vec *self) {
    NX_VEC_ASSERT(self);

    return self->elem_size;
}

/* ---------- access ---------- */

void *nx_vec_get(nx_vec *self, size_t idx) {
    NX_VEC_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset(self->data, self->elem_size, idx);
}

const void *nx_vec_get_const(const nx_vec *self, size_t idx) {
    NX_VEC_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset_c(self->data, self->elem_size, idx);
}

void nx_vec_set(nx_vec *self, size_t idx, const void *elem) {
    NX_VEC_ASSERT(self);
    NX_ASSERT(idx < self->len);
    NX_ASSERT(elem);

    memcpy(nx_byte_offset(self->data, self->elem_size, idx), elem, self->elem_size);
}

void *nx_vec_data(nx_vec *self) {
    NX_VEC_ASSERT(self);

    return self->data;
}

const void *nx_vec_data_const(const nx_vec *self) {
    NX_VEC_ASSERT(self);

    return self->data;
}

/* ---------- mods ---------- */

nx_status nx_vec_push(nx_vec *self, const void *elem) {
    NX_VEC_ASSERT(self);
    NX_ASSERT(elem);

    const nx_status st = grow_if_needed(self);
    if (st != NX_STATUS_OK) {
        return st;
    }

    ++self->len;
    nx_vec_set(self, self->len - 1, elem);
    return NX_STATUS_OK;
}

void *nx_vec_pop(nx_vec *self) {
    NX_VEC_ASSERT(self);
    NX_ASSERT(self->len > 0);

    void *elem = nx_vec_get(self, self->len - 1);
    --self->len;
    return elem;
}

void nx_vec_clear(nx_vec *self) {
    NX_VEC_ASSERT(self);

    self->len = 0;
}

nx_status nx_vec_reserve(nx_vec *self, size_t new_cap) {
    NX_VEC_ASSERT(self);

    if (new_cap <= self->cap) {
        return NX_STATUS_OK;
    }

    size_t bytes = 0;
    if (nx_size_mul_overflow(&bytes, new_cap, self->elem_size)) {
        return NX_STATUS_MUL_OVERFLOW;
    }

    void *data = realloc(self->data, bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    self->data = data;
    self->cap = new_cap;

    return NX_STATUS_OK;
}

nx_status nx_vec_resize(nx_vec *self, size_t new_len) {
    NX_VEC_ASSERT(self);

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
    const size_t add = new_len - self->len;
    memset(nx_byte_offset(self->data, self->elem_size, self->len), 0, add * self->elem_size);
    self->len = new_len;

    return NX_STATUS_OK;
}

nx_status nx_vec_shrink_to_fit(nx_vec *self) {
    NX_VEC_ASSERT(self);

    if (self->len == self->cap) {
        return NX_STATUS_OK;
    }

    if (self->len == 0) {
        free(self->data);
        set_fields(self, NULL, 0, 0, self->elem_size);
        return NX_STATUS_OK;
    }

    size_t bytes = 0;
    if (nx_size_mul_overflow(&bytes, self->len, self->elem_size)) {
        return NX_STATUS_MUL_OVERFLOW;
    }

    void *data = realloc(self->data, bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY; /* keep old buffer intact */
    }

    self->data = data;
    self->cap = self->len;

    return NX_STATUS_OK;
}

void nx_vec_swap(nx_vec *a, nx_vec *b) {
    NX_VEC_ASSERT(a);
    NX_VEC_ASSERT(b);
    NX_ASSERT(a->elem_size == b->elem_size);

    if (a == b) {
        return;
    }

    const nx_vec tmp = *a;
    *a = *b;
    *b = tmp;
}

nx_status nx_vec_insert(nx_vec *self, size_t idx, const void *elem) {
    NX_VEC_ASSERT(self);
    NX_ASSERT(idx <= self->len);
    NX_ASSERT(elem);

    const size_t old_len = self->len;

    const nx_status st = grow_if_needed(self);
    if (st != NX_STATUS_OK) {
        return st;
    }

    if (idx < old_len) {
        shift_right(self, idx);
    }

    ++self->len;
    nx_vec_set(self, idx, elem);

    return NX_STATUS_OK;
}

void nx_vec_erase(nx_vec *self, size_t idx) {
    NX_VEC_ASSERT(self);
    NX_ASSERT(idx < self->len);

    shift_left(self, idx);
    --self->len;
}

/* ---------- to span ---------- */

nx_span nx_vec_to_span(nx_vec *self) {
    NX_VEC_ASSERT(self);

    return nx_span_make(self->data, self->len, self->elem_size);
}

nx_cspan nx_vec_to_cspan(const nx_vec *self) {
    NX_VEC_ASSERT(self);

    return nx_cspan_make(self->data, self->len, self->elem_size);
}

// internals defs

static nx_status create(nx_vec **out, size_t len, size_t cap, size_t elem_size) {
    NX_ASSERT(out);
    NX_ASSERT(elem_size > 0);
    NX_ASSERT(len <= cap);

    *out = NULL;
    nx_vec *vec = malloc(sizeof(nx_vec));
    if (!vec) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(vec, NULL, 0, 0, elem_size);

    if (cap == 0) {
        *out = vec;
        return NX_STATUS_OK;
    }

    size_t bytes = 0;
    if (nx_size_mul_overflow(&bytes, cap, elem_size)) {
        free(vec);
        return NX_STATUS_MUL_OVERFLOW;
    }

    void *data = calloc(1, bytes);
    if (!data) {
        free(vec);
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(vec, data, len, cap, elem_size);
    *out = vec;
    return NX_STATUS_OK;
}

static nx_status grow_if_needed(nx_vec *self) {
    NX_VEC_ASSERT(self);

    if (self->len < self->cap) {
        return NX_STATUS_OK;
    }

    size_t new_cap = 0;
    if (self->cap == 0) {
        new_cap = 1;
    } else {
        if (self->cap > SIZE_MAX / 2) {
            return NX_STATUS_MUL_OVERFLOW;
        }
        new_cap = self->cap * 2;
    }

    return nx_vec_reserve(self, new_cap);
}

static nx_status alloc_and_copy_data(void **out, const nx_vec *src) {
    NX_ASSERT(out);
    *out = NULL;

    if (src->cap == 0) {
        return NX_STATUS_OK;
    }

    size_t cap_bytes = 0;
    if (nx_size_mul_overflow(&cap_bytes, src->cap, src->elem_size)) {
        return NX_STATUS_MUL_OVERFLOW;
    }

    size_t len_bytes = 0;
    if (nx_size_mul_overflow(&len_bytes, src->len, src->elem_size)) {
        return NX_STATUS_MUL_OVERFLOW;
    }

    void *data = malloc(cap_bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    if (len_bytes > 0) {
        memcpy(data, src->data, len_bytes);
    }

    *out = data;
    return NX_STATUS_OK;
}

static void set_fields(nx_vec *self, void *data, size_t len, size_t cap, size_t elem_size) {
    self->data = data;
    self->len = len;
    self->cap = cap;
    self->elem_size = elem_size;
}

static void reset_fields(nx_vec *self) {
    set_fields(self, NULL, 0, 0, self->elem_size);
}

static void shift_left(nx_vec *self, size_t idx) {
    memmove(
        nx_byte_offset(self->data, self->elem_size, idx),
        nx_byte_offset(self->data, self->elem_size, idx + 1),
        (self->len - idx - 1) * self->elem_size
    );
}

static void shift_right(nx_vec *self, size_t idx) {
    memmove(
        nx_byte_offset(self->data, self->elem_size, idx + 1),
        nx_byte_offset(self->data, self->elem_size, idx),
        (self->len - idx) * self->elem_size
    );
}
