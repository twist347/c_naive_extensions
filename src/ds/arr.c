#include "nx/ds/arr.h"

#include <stdlib.h>
#include <string.h>

#include "nx/numeric/limit.h"
#include "nx/mem/alloc_libc.h"
#include "nx/mem/ptr.h"

struct nx_arr {
    void *data;
    nx_usize len;
    nx_usize tsz; // type size
    nx_al *al;    // must not be null
};

#if NX_DEBUG
    static void arr_assert_impl(const nx_arr *self) {
        NX_ASSERT(self != nx_null);
        NX_ASSERT(self->tsz > 0);
        NX_ASSERT(self->al != nx_null);
        NX_ASSERT(((self->len == 0) == (self->data == nx_null)));
    }
#define ARR_ASSERT(self)    \
    do { arr_assert_impl((self)); } while (0)
#else
    #define ARR_ASSERT(self)    ((void) 0)
#endif

// internals decls

static nx_status new_impl(nx_arr **out, nx_usize len, nx_usize tsz, nx_al *al);
static void set_fields(nx_arr *self, void *data, nx_usize len, nx_usize tsz, nx_al *al);
static nx_status alloc_and_copy_data(void **out, nx_al *alloc, const void *src_data, nx_usize len, nx_usize tsz);
static void free_data(nx_arr *self);

/* ========== lifetime ========== */

nx_arr_res nx_arr_new_p(nx_arr_params p) {
    NX_ASSERT(p.tsz > 0);
    NX_ASSERT(p.al);

    nx_arr *arr = nx_null;
    const nx_status st = new_impl(&arr, p.len, p.tsz, p.al);
    if (st != NX_STATUS_OK) {
        return NX_RES_NEW_ERR(nx_arr_res, st);
    }
    return NX_RES_NEW_OK(nx_arr_res, arr);
}

nx_arr_res nx_arr_new_len(nx_usize len, nx_usize tsz) {
    return nx_arr_new_p((nx_arr_params){
        .len = len,
        .tsz = tsz,
        .al = nx_al_libc_default_g(),
    });
}

nx_arr_res nx_arr_from_data(const void *data, nx_usize len, nx_usize tsz) {
    NX_ASSERT(data || len == 0);

    nx_arr_res res = nx_arr_new_len(len, tsz);
    if (!NX_RES_IS_OK(res)) {
        return res;
    }

    nx_arr *arr = NX_RES_UNWRAP(res);
    if (len > 0) {
        const nx_usize bytes = len * tsz;
        memcpy(arr->data, data, bytes);
    }

    return NX_RES_NEW_OK(nx_arr_res, arr);
}

void nx_arr_drop(nx_arr *self) {
    if (!self) {
        return;
    }

    free_data(self);
    free(self);
}

/* ========== copy/move semantic ========== */

nx_arr_res nx_arr_copy(const nx_arr *src) {
    ARR_ASSERT(src);

    return nx_arr_copy_a(src, src->al);
}

nx_arr_res nx_arr_copy_a(const nx_arr *src, nx_al *al) {
    ARR_ASSERT(src);
    NX_ASSERT(al);

    nx_arr *dst = malloc(sizeof(nx_arr));
    if (!dst) {
        return NX_RES_NEW_ERR(nx_arr_res, NX_STATUS_OUT_OF_MEMORY);
    }

    set_fields(dst, nx_null, 0, src->tsz, al);

    if (src->len == 0) {
        return NX_RES_NEW_OK(nx_arr_res, dst);
    }

    void *data = nx_null;
    const nx_status st = alloc_and_copy_data(&data, al, src->data, src->len, src->tsz);
    if (st != NX_STATUS_OK) {
        free(dst);
        return NX_RES_NEW_ERR(nx_arr_res, st);
    }

    set_fields(dst, data, src->len, src->tsz, al);
    return NX_RES_NEW_OK(nx_arr_res, dst);
}

// TODO: swap or exchange?
nx_arr *nx_arr_move(nx_arr **src) {
    NX_ASSERT(src);
    NX_ASSERT(*src);
    ARR_ASSERT(*src);

    nx_arr *tmp = *src;
    *src = nx_null;
    return tmp;
}

nx_status nx_arr_copy_assign(nx_arr *self, const nx_arr *src) {
    ARR_ASSERT(self);
    ARR_ASSERT(src);
    NX_ASSERT(self->tsz == src->tsz);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->len == 0) {
        free_data(self);
        set_fields(self, nx_null, 0, src->tsz, self->al);
        return NX_STATUS_OK;
    }

    // optimization
    if (self->len == src->len) {
        const nx_usize bytes = src->len * src->tsz;
        memcpy(self->data, src->data, bytes);
        return NX_STATUS_OK;
    }

    void *data = nx_null;
    const nx_status st = alloc_and_copy_data(&data, self->al, src->data, src->len, src->tsz);
    if (st != NX_STATUS_OK) {
        return st;
    }

    // free old data
    free_data(self);

    set_fields(self, data, src->len, self->tsz, self->al);
    return NX_STATUS_OK;
}

// TODO: swap or exchange?
void nx_arr_move_assign(nx_arr *self, nx_arr *src) {
    ARR_ASSERT(self);
    ARR_ASSERT(src);
    NX_ASSERT(self->tsz == src->tsz);
    NX_ASSERT(nx_al_eq(self->al, src->al));

    if (self == src) {
        return;
    }

    // free self's old data
    free_data(self);

    // take src's data
    self->data = src->data;
    self->len = src->len;
    // NOTE: self keeps its own allocator!

    // clear src
    src->data = nx_null;
    src->len = 0;
}

/* ========== info ========== */

nx_usize nx_arr_len(const nx_arr *self) {
    ARR_ASSERT(self);

    return self->len;
}

nx_bool nx_arr_empty(const nx_arr *self) {
    ARR_ASSERT(self);

    return self->len == 0;
}

nx_usize nx_arr_tsz(const nx_arr *self) {
    ARR_ASSERT(self);

    return self->tsz;
}

nx_al *nx_arr_al(const nx_arr *self) {
    ARR_ASSERT(self);

    return self->al;
}

/* ========== access ========== */

void *nx_arr_get(nx_arr *self, nx_usize idx) {
    ARR_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset(self->data, self->tsz, idx);
}

const void *nx_arr_get_c(const nx_arr *self, nx_usize idx) {
    ARR_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset_c(self->data, self->tsz, idx);
}

void *nx_arr_at(nx_arr *self, nx_usize idx) {
    ARR_ASSERT(self);

    return idx < self->len ? nx_byte_offset(self->data, self->tsz, idx) : nx_null;
}

const void *nx_arr_at_c(const nx_arr *self, nx_usize idx) {
    ARR_ASSERT(self);

    return idx < self->len ? nx_byte_offset_c(self->data, self->tsz, idx) : nx_null;
}

void nx_arr_set(nx_arr *self, nx_usize idx, const void *val) {
    ARR_ASSERT(self);
    NX_ASSERT(idx < self->len);
    NX_ASSERT(val);

    memcpy(nx_byte_offset(self->data, self->tsz, idx), val, self->tsz);
}

void *nx_arr_data(nx_arr *self) {
    ARR_ASSERT(self);

    return self->data;
}

const void *nx_arr_data_c(const nx_arr *self) {
    ARR_ASSERT(self);

    return self->data;
}

/* ========== mods ========== */

void nx_arr_swap(nx_arr *a, nx_arr *b) {
    ARR_ASSERT(a);
    ARR_ASSERT(b);
    NX_ASSERT(a->tsz == b->tsz);
    NX_ASSERT(nx_al_eq(a->al, b->al));

    if (a == b) {
        return;
    }

    // swap data and len, but NOT allocators
    void *tmp_data = a->data;
    const nx_usize tmp_len = a->len;

    a->data = b->data;
    a->len = b->len;

    b->data = tmp_data;
    b->len = tmp_len;
}

/* ========== to span ========== */

nx_span nx_arr_to_span(nx_arr *self) {
    ARR_ASSERT(self);

    return nx_span_new(self->data, self->len, self->tsz);
}

nx_cspan nx_arr_to_cspan(const nx_arr *self) {
    ARR_ASSERT(self);

    return nx_cspan_new(self->data, self->len, self->tsz);
}

// internals defs

static nx_status new_impl(nx_arr **out, nx_usize len, nx_usize tsz, nx_al *al) {
    NX_ASSERT(out);

    *out = nx_null;

    nx_arr *arr = malloc(sizeof(nx_arr));
    if (!arr) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(arr, nx_null, 0, tsz, al);

    if (len == 0) {
        *out = arr;
        return NX_STATUS_OK;
    }

    const nx_usize bytes = len * tsz;
    void *data = nx_al_calloc(al, 1, bytes);
    if (!data) {
        free(arr);
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(arr, data, len, tsz, al);
    *out = arr;

    return NX_STATUS_OK;
}

static void set_fields(nx_arr *self, void *data, nx_usize len, nx_usize tsz, nx_al *al) {
    self->data = data;
    self->len = len;
    self->tsz = tsz;
    self->al = al;
}

static nx_usize calc_bytes(nx_usize len, nx_usize tsz) {
    return len * tsz;
}

static nx_status alloc_and_copy_data(void **out, nx_al *alloc, const void *src_data, nx_usize len, nx_usize tsz) {
    NX_ASSERT(out);
    NX_ASSERT(alloc);
    NX_ASSERT(src_data);

    *out = nx_null;

    const nx_usize bytes = calc_bytes(len, tsz);

    void *data = nx_al_alloc(alloc, bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    memcpy(data, src_data, bytes);
    *out = data;
    return NX_STATUS_OK;
}

static void free_data(nx_arr *self) {
    if (self->data) {
        const nx_usize bytes = calc_bytes(self->len, self->tsz);
        nx_al_dealloc(self->al, self->data, bytes);
    }
}
