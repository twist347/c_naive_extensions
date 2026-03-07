#include "nx/ds/arr.h"

#include <stdlib.h>
#include <string.h>

#include "../../include/nx/core/limits.h"
#include "nx/mem/alloc_libc.h"
#include "nx/mem/ptr.h"

struct nx_Arr {
    void *data;
    nx_usize len;
    nx_usize tsz; // type size
    nx_Al *al;    // must not be null
};

#if NX_DEBUG
    static void arr_assert_impl(const nx_Arr *self) {
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

static nx_Status new_impl(nx_Arr **out, nx_usize len, nx_usize tsz, nx_Al *al);
static void set_fields(nx_Arr *self, void *data, nx_usize len, nx_usize tsz, nx_Al *al);
static nx_Status alloc_and_copy_data(void **out, nx_Al *alloc, const void *src_data, nx_usize len, nx_usize tsz);

/* ========== lifetime ========== */

nx_ArrRes nx_arr_new_p(nx_ArrParams p) {
    NX_ASSERT(p.tsz > 0);
    NX_ASSERT(p.al);

    nx_Arr *arr = nx_null;
    const nx_Status st = new_impl(&arr, p.len, p.tsz, p.al);
    if (st != NX_STATUS_OK) {
        return NX_RES_NEW_ERR(nx_ArrRes, st);
    }
    return NX_RES_NEW_OK(nx_ArrRes, arr);
}

nx_ArrRes nx_arr_new_len(nx_usize len, nx_usize tsz) {
    return nx_arr_new_p((nx_ArrParams){
        .len = len,
        .tsz = tsz,
        .al = nx_al_libc_default_g(),
    });
}

nx_ArrRes nx_arr_from_data(const void *data, nx_usize len, nx_usize tsz) {
    NX_ASSERT(data || len == 0);

    nx_ArrRes res = nx_arr_new_len(len, tsz);
    if (!NX_RES_IS_OK(res)) {
        return res;
    }

    nx_Arr *arr = NX_RES_UNWRAP(res);
    if (len > 0) {
        const nx_usize bytes = len * tsz;
        memcpy(arr->data, data, bytes);
    }

    return NX_RES_NEW_OK(nx_ArrRes, arr);
}

void nx_arr_drop(nx_Arr *self) {
    if (!self) {
        return;
    }

    nx_al_dealloc(self->al, self->data, self->len * self->tsz);
    free(self);
}

/* ========== copy/move semantic ========== */

nx_ArrRes nx_arr_copy(const nx_Arr *src) {
    ARR_ASSERT(src);

    return nx_arr_copy_a(src, src->al);
}

nx_ArrRes nx_arr_copy_a(const nx_Arr *src, nx_Al *al) {
    ARR_ASSERT(src);
    NX_ASSERT(al);

    nx_Arr *dst = malloc(sizeof(nx_Arr));
    if (!dst) {
        return NX_RES_NEW_ERR(nx_ArrRes, NX_STATUS_OUT_OF_MEMORY);
    }

    set_fields(dst, nx_null, 0, src->tsz, al);

    if (src->len == 0) {
        return NX_RES_NEW_OK(nx_ArrRes, dst);
    }

    void *data = nx_null;
    const nx_Status st = alloc_and_copy_data(&data, al, src->data, src->len, src->tsz);
    if (st != NX_STATUS_OK) {
        free(dst);
        return NX_RES_NEW_ERR(nx_ArrRes, st);
    }

    set_fields(dst, data, src->len, src->tsz, al);
    return NX_RES_NEW_OK(nx_ArrRes, dst);
}

// TODO: swap or exchange?
nx_Arr *nx_arr_move(nx_Arr **src) {
    NX_ASSERT(src);
    NX_ASSERT(*src);
    ARR_ASSERT(*src);

    nx_Arr *tmp = *src;
    *src = nx_null;
    return tmp;
}

nx_Status nx_arr_copy_assign(nx_Arr *self, const nx_Arr *src) {
    ARR_ASSERT(self);
    ARR_ASSERT(src);
    NX_ASSERT(self->tsz == src->tsz);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->len == 0) {
        nx_al_dealloc(self->al, self->data, self->len * self->tsz);
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
    const nx_Status st = alloc_and_copy_data(&data, self->al, src->data, src->len, src->tsz);
    if (st != NX_STATUS_OK) {
        return st;
    }

    // free old data
    nx_al_dealloc(self->al, self->data, self->len * self->tsz);

    set_fields(self, data, src->len, self->tsz, self->al);
    return NX_STATUS_OK;
}

// TODO: swap or exchange?
void nx_arr_move_assign(nx_Arr *self, nx_Arr *src) {
    ARR_ASSERT(self);
    ARR_ASSERT(src);
    NX_ASSERT(self->tsz == src->tsz);
    NX_ASSERT(nx_al_eq(self->al, src->al));

    if (self == src) {
        return;
    }

    // free self's old data
    nx_al_dealloc(self->al, self->data, self->len * self->tsz);

    // take src's data
    self->data = src->data;
    self->len = src->len;
    // NOTE: self keeps its own allocator!

    // clear src
    src->data = nx_null;
    src->len = 0;
}

/* ========== info ========== */

nx_usize nx_arr_len(const nx_Arr *self) {
    ARR_ASSERT(self);

    return self->len;
}

nx_bool nx_arr_empty(const nx_Arr *self) {
    ARR_ASSERT(self);

    return self->len == 0;
}

nx_usize nx_arr_tsz(const nx_Arr *self) {
    ARR_ASSERT(self);

    return self->tsz;
}

nx_Al *nx_arr_al(const nx_Arr *self) {
    ARR_ASSERT(self);

    return self->al;
}

/* ========== access ========== */

void *nx_arr_get(nx_Arr *self, nx_usize idx) {
    ARR_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset(self->data, self->tsz, idx);
}

const void *nx_arr_get_c(const nx_Arr *self, nx_usize idx) {
    ARR_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return nx_byte_offset_c(self->data, self->tsz, idx);
}

void *nx_arr_at(nx_Arr *self, nx_usize idx) {
    ARR_ASSERT(self);

    return idx < self->len ? nx_byte_offset(self->data, self->tsz, idx) : nx_null;
}

const void *nx_arr_at_c(const nx_Arr *self, nx_usize idx) {
    ARR_ASSERT(self);

    return idx < self->len ? nx_byte_offset_c(self->data, self->tsz, idx) : nx_null;
}

void nx_arr_set(nx_Arr *self, nx_usize idx, const void *val) {
    ARR_ASSERT(self);
    NX_ASSERT(idx < self->len);
    NX_ASSERT(val);

    memcpy(nx_byte_offset(self->data, self->tsz, idx), val, self->tsz);
}

void *nx_arr_data(nx_Arr *self) {
    ARR_ASSERT(self);

    return self->data;
}

const void *nx_arr_data_c(const nx_Arr *self) {
    ARR_ASSERT(self);

    return self->data;
}

/* ========== mods ========== */

void nx_arr_swap(nx_Arr *a, nx_Arr *b) {
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

nx_Span nx_arr_to_span(nx_Arr *self) {
    ARR_ASSERT(self);

    return nx_span_new(self->data, self->len, self->tsz);
}

nx_CSpan nx_arr_to_cspan(const nx_Arr *self) {
    ARR_ASSERT(self);

    return nx_cspan_new(self->data, self->len, self->tsz);
}

// internals defs

static nx_Status new_impl(nx_Arr **out, nx_usize len, nx_usize tsz, nx_Al *al) {
    NX_ASSERT(out);

    *out = nx_null;

    nx_Arr *arr = malloc(sizeof(nx_Arr));
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

static void set_fields(nx_Arr *self, void *data, nx_usize len, nx_usize tsz, nx_Al *al) {
    self->data = data;
    self->len = len;
    self->tsz = tsz;
    self->al = al;
}

static nx_Status alloc_and_copy_data(void **out, nx_Al *alloc, const void *src_data, nx_usize len, nx_usize tsz) {
    NX_ASSERT(out);
    NX_ASSERT(alloc);
    NX_ASSERT(src_data);

    *out = nx_null;

    const nx_usize bytes = len * tsz;

    void *data = nx_al_alloc(alloc, bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    memcpy(data, src_data, bytes);
    *out = data;
    return NX_STATUS_OK;
}
