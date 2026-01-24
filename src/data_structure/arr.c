#include "nx/data_structure/arr.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct nx_arr {
    void *data;
    size_t len;
    size_t elem_size;
};

// internals decls

static void set_fields(nx_arr *self, void *data, size_t len, size_t elem_size);
static void reset_fields(nx_arr *self);
static char *ptr_at(nx_arr *self, size_t idx);
static const char *ptr_at_c(const nx_arr *self, size_t idx);
static nx_status alloc_and_copy_data(const nx_arr *src, void **out);
static nx_status checked_mul(size_t a, size_t b, size_t *out);

#define NX_ARR_IS_VALID(self_)                                        \
    do {                                                              \
        NX_ASSERT((self_));                                           \
        NX_ASSERT((self_)->elem_size > 0);                            \
        NX_ASSERT(((self_)->len == 0) == ((self_)->data == NULL));    \
    } while (0)

/* ---------- new/destroy ---------- */

nx_status nx_arr_create(nx_arr **out, size_t len, size_t elem_size) {
    NX_ASSERT(out);
    NX_ASSERT(elem_size > 0);

    *out = NULL;

    nx_arr *arr = malloc(sizeof(nx_arr));
    if (!arr) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(arr, NULL, 0, elem_size);

    if (len == 0) {
        *out = arr;
        return NX_STATUS_OK;
    }

    size_t bytes;
    const nx_status st = checked_mul(len, elem_size, &bytes);
    if (st != NX_STATUS_OK) {
        free(arr);
        return st;
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

void nx_arr_destroy(nx_arr *self) {
    if (!self) {
        return;
    }

    free(self->data);
    free(self);
}

/* ---------- copy/move semantic ---------- */

nx_status nx_arr_copy(nx_arr **out, const nx_arr *src) {
    NX_ASSERT(out);
    NX_ARR_IS_VALID(src);

    *out = NULL;

    nx_arr *arr = malloc(sizeof(nx_arr));
    if (!arr) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(arr, NULL, 0, src->elem_size);

    if (src->len == 0) {
        *out = arr;
        return NX_STATUS_OK;
    }

    void *data = NULL;
    const nx_status st = alloc_and_copy_data(src, &data);
    if (st != NX_STATUS_OK) {
        free(arr);
        return st;
    }

    set_fields(arr, data, src->len, src->elem_size);

    *out = arr;
    return NX_STATUS_OK;
}

nx_status nx_arr_move(nx_arr **out, nx_arr *src) {
    NX_ASSERT(out);
    NX_ARR_IS_VALID(src);

    *out = NULL;

    nx_arr *arr = malloc(sizeof(nx_arr));
    if (!arr) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    *arr = *src;
    reset_fields(src);

    *out = arr;
    return NX_STATUS_OK;
}

nx_status nx_arr_copy_assign(nx_arr *self, const nx_arr *src) {
    NX_ARR_IS_VALID(self);
    NX_ARR_IS_VALID(src);
    NX_ASSERT(self->elem_size == src->elem_size);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->len == 0) {
        free(self->data);
        set_fields(self, NULL, 0, src->elem_size);
        return NX_STATUS_OK;
    }

    void *data = NULL;
    const nx_status st = alloc_and_copy_data(src, &data);
    if (st != NX_STATUS_OK) {
        return st;
    }

    free(self->data);
    set_fields(self, data, src->len, self->elem_size);
    return NX_STATUS_OK;
}

nx_status nx_arr_move_assign(nx_arr *self, nx_arr *src) {
    NX_ARR_IS_VALID(self);
    NX_ARR_IS_VALID(src);
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

size_t nx_arr_len(const nx_arr *self) {
    NX_ARR_IS_VALID(self);

    return self->len;
}

bool nx_arr_empty(const nx_arr *self) {
    NX_ARR_IS_VALID(self);

    return self->len == 0;
}

size_t nx_arr_elem_size(const nx_arr *self) {
    NX_ARR_IS_VALID(self);

    return self->elem_size;
}

/* ---------- access ---------- */

void *nx_arr_get(nx_arr *self, size_t idx) {
    NX_ARR_IS_VALID(self);
    NX_ASSERT(idx < self->len);

    return ptr_at(self, idx);
}

const void *nx_arr_get_const(const nx_arr *self, size_t idx) {
    NX_ARR_IS_VALID(self);
    NX_ASSERT(idx < self->len);

    return ptr_at_c(self, idx);
}

void nx_arr_set(nx_arr *self, size_t idx, const void *elem) {
    NX_ARR_IS_VALID(self);
    NX_ASSERT(idx < self->len);
    NX_ASSERT(elem);

    memmove(ptr_at(self, idx), elem, self->elem_size);
}

void *nx_arr_data(nx_arr *self) {
    NX_ARR_IS_VALID(self);

    return self->data;
}

const void *nx_arr_data_const(const nx_arr *self) {
    NX_ARR_IS_VALID(self);

    return self->data;
}

/* ---------- mods ---------- */

void nx_arr_swap(nx_arr *a, nx_arr *b) {
    NX_ARR_IS_VALID(a);
    NX_ARR_IS_VALID(b);
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
    NX_ARR_IS_VALID(self);

    return nx_make_span(self->data, self->len, self->elem_size);
}

nx_cspan nx_arr_to_cspan(const nx_arr *self) {
    NX_ARR_IS_VALID(self);

    return nx_make_cspan(self->data, self->len, self->elem_size);
}

// internals defs

static void set_fields(nx_arr *self, void *data, size_t len, size_t elem_size) {
    self->data = data;
    self->len = len;
    self->elem_size = elem_size;
}

static void reset_fields(nx_arr *self) {
    set_fields(self, NULL, 0, self->elem_size);
}

static char *ptr_at(nx_arr *self, size_t idx) {
    return (char *) self->data + idx * self->elem_size;
}

static const char *ptr_at_c(const nx_arr *self, size_t idx) {
    return (const char *) self->data + idx * self->elem_size;
}

static nx_status alloc_and_copy_data(const nx_arr *src, void **out) {
    *out = NULL;

    size_t bytes = 0;
    const nx_status st = checked_mul(src->len, src->elem_size, &bytes);
    if (st != NX_STATUS_OK) {
        return st;
    }

    void *data = malloc(bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    memcpy(data, src->data, bytes);
    *out = data;
    return NX_STATUS_OK;
}

static nx_status checked_mul(size_t a, size_t b, size_t *out) {
    if (a == 0 || b == 0) {
        *out = 0;
        return NX_STATUS_OK;
    }
    if (a > SIZE_MAX / b) {
        return NX_STATUS_MUL_OVERFLOW;
    }
    *out = a * b;
    return NX_STATUS_OK;
}
