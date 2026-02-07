#include "nx/data_structure/arr.h"

#include <stdlib.h>
#include <string.h>

#include "nx/core/limit.h"
#include "nx/mem/ptr.h"

struct nx_arr {
    void *data;
    nx_usize len;
    nx_usize tsz; // type size
};

// internals decls

static void arr_assert(const nx_arr *self);

static nx_status new_impl(nx_arr **out, nx_usize len, nx_usize tsz);

static void set_fields(nx_arr *self, void *data, nx_usize len, nx_usize tsz);

static nx_status alloc_and_copy_data(void **out, const nx_arr *src);

/* ========== lifetime ========== */

nx_arr_res nx_arr_new_p(nx_arr_params p) {
    NX_ASSERT(p.tsz != 0);

    nx_arr *arr = nx_null;
    const nx_status st = new_impl(&arr, p.len, p.tsz);
    if (st != NX_STATUS_OK) {
        return NX_RES_NEW_ERR(nx_arr_res, st);
    }
    return NX_RES_NEW_OK(nx_arr_res, arr);
}

nx_arr_res nx_arr_new_len(nx_usize len, nx_usize tsz) {
    return nx_arr_new_p((nx_arr_params){
        .len = len,
        .tsz = tsz,
    });
}

void nx_arr_drop(nx_arr *self) {
    if (!self) {
        return;
    }

    free(self->data);
    free(self);
}

/* ========== copy/move semantic ========== */

nx_arr_res nx_arr_copy(const nx_arr *src) {
    arr_assert(src);

    nx_arr *dst = malloc(sizeof(nx_arr));
    if (!dst) {
        return NX_RES_NEW_ERR(nx_arr_res, NX_STATUS_OUT_OF_MEMORY);
    }

    set_fields(dst, nx_null, 0, src->tsz);

    if (src->len == 0) {
        return NX_RES_NEW_OK(nx_arr_res, dst);
    }

    void *data = nx_null;
    const nx_status st = alloc_and_copy_data(&data, src);
    if (st != NX_STATUS_OK) {
        free(dst);
        return NX_RES_NEW_ERR(nx_arr_res, st);
    }

    set_fields(dst, data, src->len, src->tsz);
    return NX_RES_NEW_OK(nx_arr_res, dst);
}

// TODO: swap or exchange?
nx_arr *nx_arr_move(nx_arr **src) {
    NX_ASSERT(src);
    NX_ASSERT(*src);
    arr_assert(*src);

    nx_arr *tmp = *src;
    *src = nx_null;
    return tmp;
}

nx_status nx_arr_copy_assign(nx_arr *self, const nx_arr *src) {
    arr_assert(self);
    arr_assert(src);
    NX_ASSERT(self->tsz == src->tsz);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->len == 0) {
        free(self->data);
        set_fields(self, nx_null, 0, src->tsz);
        return NX_STATUS_OK;
    }

    void *data = nx_null;
    const nx_status st = alloc_and_copy_data(&data, src);
    if (st != NX_STATUS_OK) {
        return st;
    }

    free(self->data);
    set_fields(self, data, src->len, self->tsz);
    return NX_STATUS_OK;
}

// TODO: swap or exchange?
void nx_arr_move_assign(nx_arr *self, nx_arr *src) {
    arr_assert(self);
    arr_assert(src);
    NX_ASSERT(self->tsz == src->tsz);

    if (self == src) {
        return;
    }

    free(self->data);

    self->data = src->data;
    self->len = src->len;

    src->data = nx_null;
    src->len = 0;
}

/* ========== info ========== */

nx_usize nx_arr_len(const nx_arr *self) {
    arr_assert(self);

    return self->len;
}

nx_bool nx_arr_empty(const nx_arr *self) {
    arr_assert(self);

    return self->len == 0;
}

nx_usize nx_arr_tsz(const nx_arr *self) {
    arr_assert(self);

    return self->tsz;
}

/* ========== access ========== */

void *nx_arr_get(nx_arr *self, nx_usize idx) {
    arr_assert(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset(self->data, self->tsz, idx);
}

const void *nx_arr_get_c(const nx_arr *self, nx_usize idx) {
    arr_assert(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset_c(self->data, self->tsz, idx);
}

void nx_arr_set(nx_arr *self, nx_usize idx, const void *val) {
    arr_assert(self);
    NX_ASSERT(idx < self->len);
    NX_ASSERT(val);

    memmove(nx_byte_offset(self->data, self->tsz, idx), val, self->tsz);
}

void *nx_arr_data(nx_arr *self) {
    arr_assert(self);

    return self->data;
}

const void *nx_arr_data_c(const nx_arr *self) {
    arr_assert(self);

    return self->data;
}

/* ========== mods ========== */

void nx_arr_swap(nx_arr *a, nx_arr *b) {
    arr_assert(a);
    arr_assert(b);
    NX_ASSERT(a->tsz == b->tsz);

    if (a == b) {
        return;
    }

    // TODO: use nx_swap
    const nx_arr tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ========== to span ========== */

nx_span nx_arr_to_span(nx_arr *self) {
    arr_assert(self);

    return nx_span_new(self->data, self->len, self->tsz);
}

nx_cspan nx_arr_to_cspan(const nx_arr *self) {
    arr_assert(self);

    return nx_cspan_new(self->data, self->len, self->tsz);
}

// internals defs

static void arr_assert(const nx_arr *self) {
    NX_ASSERT(self != nx_null);
    NX_ASSERT(self->tsz > 0);
    NX_ASSERT(((self->len == 0) == (self->data == nx_null)));
    NX_ASSERT(self->len <= NX_USIZE_MAX / self->tsz);
}

static nx_status new_impl(nx_arr **out, nx_usize len, nx_usize tsz) {
    *out = nx_null;

    nx_arr *arr = malloc(sizeof(nx_arr));
    if (!arr) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(arr, nx_null, 0, tsz);

    if (len == 0) {
        *out = arr;
        return NX_STATUS_OK;
    }

    NX_ASSERT(len <= NX_USIZE_MAX / tsz);
    const nx_usize bytes = len * tsz;

    void *data = calloc(1, bytes);
    if (!data) {
        free(arr);
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(arr, data, len, tsz);
    *out = arr;

    return NX_STATUS_OK;
}

static void set_fields(nx_arr *self, void *data, nx_usize len, nx_usize tsz) {
    self->data = data;
    self->len = len;
    self->tsz = tsz;
}

static nx_status alloc_and_copy_data(void **out, const nx_arr *src) {
    NX_ASSERT(out);

    *out = nx_null;

    NX_ASSERT(src->len <= NX_USIZE_MAX / src->tsz);
    const nx_usize bytes = src->len * src->tsz;

    void *data = malloc(bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    memcpy(data, src->data, bytes);
    *out = data;
    return NX_STATUS_OK;
}
