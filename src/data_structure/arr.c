#include "nx/data_structure/arr.h"

#include <stdlib.h>
#include <string.h>

#include "nx/core/checked_arith.h"
#include "nx/core/limit.h"
#include "nx/mem/ptr.h"

struct nx_arr {
    void *data;
    nx_usize len;
    nx_usize elem_size;
};

#define NX_ARR_ASSERT(self_)                                             \
    do {                                                                 \
        NX_ASSERT((self_) != nx_null);                                   \
        NX_ASSERT((self_)->elem_size > 0);                               \
        NX_ASSERT(((self_)->len == 0) == ((self_)->data == nx_null));    \
        NX_ASSERT((self_)->len <= NX_USIZE_MAX / (self_)->elem_size);    \
    } while (0)

// internals decls

static nx_status new_impl(nx_arr **out, nx_usize len, nx_usize elem_size);

static void set_fields(nx_arr *self, void *data, nx_usize len, nx_usize elem_size);

static nx_status alloc_and_copy_data(void **out, const nx_arr *src);

/* ---------- new/drop ---------- */

nx_arr_res nx_arr_new(nx_usize len, nx_usize elem_size) {
    NX_ASSERT(elem_size > 0);

    nx_arr *arr = nx_null;
    const nx_status st = new_impl(&arr, len, elem_size);
    if (st != NX_STATUS_OK) {
        return NX_RES_ERR(nx_arr_res, st);
    }
    return NX_RES_OK(nx_arr_res, arr);
}

void nx_arr_drop(nx_arr *self) {
    if (!self) {
        return;
    }

    free(self->data);
    free(self);
}

/* ---------- copy/move semantic ---------- */

nx_arr_res nx_arr_copy(const nx_arr *src) {
    NX_ARR_ASSERT(src);

    nx_arr *dst = malloc(sizeof(nx_arr));
    if (!dst) {
        return NX_RES_ERR(nx_arr_res, NX_STATUS_OUT_OF_MEMORY);
    }

    set_fields(dst, nx_null, 0, src->elem_size);

    if (src->len == 0) {
        return NX_RES_OK(nx_arr_res, dst);
    }

    void *data = nx_null;
    const nx_status st = alloc_and_copy_data(&data, src);
    if (st != NX_STATUS_OK) {
        free(dst);
        return NX_RES_ERR(nx_arr_res, st);
    }

    set_fields(dst, data, src->len, src->elem_size);
    return NX_RES_OK(nx_arr_res, dst);
}

// TODO: swap or exchange?
nx_arr *nx_arr_move(nx_arr **src) {
    NX_ASSERT(src);
    NX_ASSERT(*src);
    NX_ARR_ASSERT(*src);

    nx_arr *tmp = *src;
    *src = nx_null;
    return tmp;
}

nx_status nx_arr_copy_assign(nx_arr *self, const nx_arr *src) {
    NX_ARR_ASSERT(self);
    NX_ARR_ASSERT(src);
    NX_ASSERT(self->elem_size == src->elem_size);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->len == 0) {
        free(self->data);
        set_fields(self, nx_null, 0, src->elem_size);
        return NX_STATUS_OK;
    }

    void *data = nx_null;
    const nx_status st = alloc_and_copy_data(&data, src);
    if (st != NX_STATUS_OK) {
        return st;
    }

    free(self->data);
    set_fields(self, data, src->len, self->elem_size);
    return NX_STATUS_OK;
}

// TODO: swap or exchange?
void nx_arr_move_assign(nx_arr *self, nx_arr *src) {
    NX_ARR_ASSERT(self);
    NX_ARR_ASSERT(src);
    NX_ASSERT(self->elem_size == src->elem_size);

    if (self == src) {
        return;
    }

    free(self->data);

    self->data = src->data;
    self->len = src->len;

    src->data = nx_null;
    src->len = 0;
}

/* ---------- info ---------- */

nx_usize nx_arr_len(const nx_arr *self) {
    NX_ARR_ASSERT(self);

    return self->len;
}

nx_bool nx_arr_empty(const nx_arr *self) {
    NX_ARR_ASSERT(self);

    return self->len == 0;
}

nx_usize nx_arr_elem_size(const nx_arr *self) {
    NX_ARR_ASSERT(self);

    return self->elem_size;
}

/* ---------- access ---------- */

void *nx_arr_get(nx_arr *self, nx_usize idx) {
    NX_ARR_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset(self->data, self->elem_size, idx);
}

const void *nx_arr_get_c(const nx_arr *self, nx_usize idx) {
    NX_ARR_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset_c(self->data, self->elem_size, idx);
}

void nx_arr_set(nx_arr *self, nx_usize idx, const void *elem) {
    NX_ARR_ASSERT(self);
    NX_ASSERT(idx < self->len);
    NX_ASSERT(elem);

    memmove(nx_byte_offset(self->data, self->elem_size, idx), elem, self->elem_size);
}

void *nx_arr_data(nx_arr *self) {
    NX_ARR_ASSERT(self);

    return self->data;
}

const void *nx_arr_data_c(const nx_arr *self) {
    NX_ARR_ASSERT(self);

    return self->data;
}

/* ---------- mods ---------- */

void nx_arr_swap(nx_arr *a, nx_arr *b) {
    NX_ARR_ASSERT(a);
    NX_ARR_ASSERT(b);
    NX_ASSERT(a->elem_size == b->elem_size);

    if (a == b) {
        return;
    }

    const nx_arr tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ---------- to span ---------- */

nx_span nx_arr_to_span(nx_arr *self) {
    NX_ARR_ASSERT(self);

    return nx_span_new(self->data, self->len, self->elem_size);
}

nx_cspan nx_arr_to_cspan(const nx_arr *self) {
    NX_ARR_ASSERT(self);

    return nx_cspan_new(self->data, self->len, self->elem_size);
}

// internals defs

static nx_status new_impl(nx_arr **out, nx_usize len, nx_usize elem_size) {
    *out = nx_null;

    nx_arr *arr = malloc(sizeof(nx_arr));
    if (!arr) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(arr, nx_null, 0, elem_size);

    if (len == 0) {
        *out = arr;
        return NX_STATUS_OK;
    }

    nx_usize bytes;
    if (nx_size_mul_overflow(&bytes, len, elem_size)) {
        free(arr);
        return NX_STATUS_MUL_OVERFLOW;
    }

    void *data = calloc(1, bytes);
    if (!data) {
        free(arr);
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(arr, data, len, elem_size);
    *out = arr;

    return NX_STATUS_OK;
}

static void set_fields(nx_arr *self, void *data, nx_usize len, nx_usize elem_size) {
    self->data = data;
    self->len = len;
    self->elem_size = elem_size;
}

static nx_status alloc_and_copy_data(void **out, const nx_arr *src) {
    NX_ASSERT(out);

    *out = nx_null;

    nx_usize bytes = 0;
    if (nx_size_mul_overflow(&bytes, src->len, src->elem_size)) {
        return NX_STATUS_MUL_OVERFLOW;
    }

    void *data = malloc(bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    memcpy(data, src->data, bytes);
    *out = data;
    return NX_STATUS_OK;
}
