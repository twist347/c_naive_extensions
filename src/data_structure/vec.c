#include "nx/data_structure/vec.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

struct nx_vec {
    void *data;
    size_t len;
    size_t cap;
    size_t elem_size;
};

// internals decls

static nx_vec *create(size_t len, size_t cap, size_t elem_size);
static bool grow_if_needed(nx_vec *self);
static void set_fields(nx_vec *self, void *data, size_t len, size_t cap, size_t elem_size);
static void reset_fields(nx_vec *self);
static char *ptr_at(nx_vec *self, size_t idx);
static const char *ptr_at_c(const nx_vec *self, size_t idx);
static void shift_left(nx_vec *self, size_t idx);
static void shift_right(nx_vec *self, size_t idx);
static void *alloc_and_copy_data(const nx_vec *src);

/* ---------- create/destroy ---------- */

nx_vec *nx_vec_create(size_t len, size_t elem_size) {
    return create(len, len, elem_size);
}

nx_vec *nx_vec_create_cap(size_t cap, size_t elem_size) {
    return create(0, cap, elem_size);
}

void nx_vec_destroy(nx_vec *self) {
    if (!self) {
        return;
    }

    free(self->data);
    free(self);
}

/* ---------- copy/move semantic ---------- */

nx_vec *nx_vec_copy(const nx_vec *src) {
    assert(src);

    nx_vec *dst = malloc(sizeof(nx_vec));
    if (!dst) {
        return NULL;
    }

    set_fields(dst, NULL, src->len, src->cap, src->elem_size);
    if (src->cap == 0) {
        return dst;
    }

    void *data = alloc_and_copy_data(src);
    if (!data) {
        free(dst);
        return NULL;
    }

    dst->data = data;

    return dst;
}

nx_vec *nx_vec_move(nx_vec *src) {
    assert(src);

    nx_vec *dst = malloc(sizeof(nx_vec));
    if (!dst) {
        return NULL;
    }

    *dst = *src;

    reset_fields(src);

    return dst;
}

bool nx_vec_copy_assign(nx_vec *self, const nx_vec *src) {
    assert(self);
    assert(src);

    if (self == src) {
        return true;
    }

    void *data = alloc_and_copy_data(src);
    if (!data) {
        return false;
    }

    free(self->data);
    set_fields(self, data, src->len, src->cap, src->elem_size);

    return true;
}

void nx_vec_move_assign(nx_vec *self, nx_vec *src) {
    assert(self);
    assert(src);

    if (self == src) {
        return;
    }

    free(self->data);

    *self = *src;

    reset_fields(src);
}

/* ---------- info ---------- */

size_t nx_vec_len(const nx_vec *self) {
    assert(self);

    return self->len;
}

bool nx_vec_empty(const nx_vec *self) {
    assert(self);

    return self->len == 0;
}

size_t nx_vec_cap(const nx_vec *self) {
    assert(self);

    return self->cap;
}

size_t nx_vec_elem_size(const nx_vec *self) {
    assert(self);

    return self->elem_size;
}

/* ---------- access ---------- */

void *nx_vec_get(nx_vec *self, size_t idx) {
    assert(self);
    assert(idx < self->len);

    return ptr_at(self, idx);
}

const void *nx_vec_get_const(const nx_vec *self, size_t idx) {
    assert(self);
    assert(idx < self->len);

    return ptr_at_c(self, idx);
}

void nx_vec_set(nx_vec *self, size_t idx, const void *elem) {
    assert(self);
    assert(idx < self->len);
    assert(elem);

    memcpy(ptr_at(self, idx), elem, self->elem_size);
}

void *nx_vec_data(nx_vec *self) {
    assert(self);

    return self->data;
}
const void *nx_vec_data_const(const nx_vec *self) {
    assert(self);

    return self->data;
}

/* ---------- mods ---------- */

bool nx_vec_push(nx_vec *self, const void *elem) {
    assert(self);
    assert(elem);

    if (!grow_if_needed(self)) {
        return false;
    }

    ++self->len;
    nx_vec_set(self, self->len - 1, elem);
    return true;
}

void *nx_vec_pop(nx_vec *self) {
    assert(self);
    assert(self->len > 0);

    void *elem = nx_vec_get(self, self->len - 1);
    --self->len;
    return elem;
}

void nx_vec_clear(nx_vec *self) {
    assert(self);

    self->len = 0;
}

bool nx_vec_reserve(nx_vec *self, size_t new_cap) {
    assert(self);

    if (new_cap <= self->cap) {
        return true;
    }

    void *data = realloc(self->data, new_cap * self->elem_size);
    if (!data) {
        return false;
    }

    self->cap = new_cap;
    self->data = data;

    return true;
}

bool nx_vec_resize(nx_vec *self, size_t new_len) {
    assert(self);

    if (new_len <= self->len) {
        self->len = new_len;
        return true;
    }

    if (new_len > self->cap && !nx_vec_reserve(self, new_len)) {
        return false;
    }

    memset(ptr_at(self, self->len), 0, (new_len - self->len) * self->elem_size);
    self->len = new_len;

    return true;
}

bool nx_vec_shrink_to_fit(nx_vec *self) {
    assert(self);

    if (self->len == self->cap) {
        return true;
    }

    if (self->len == 0) {
        free(self->data);
        self->cap = 0;
        self->data = NULL;
        return true;
    }

    void *data = realloc(self->data, self->len * self->elem_size);
    if (!data) {
        return false;
    }

    self->data = data;
    self->cap = self->len;

    return true;
}

void nx_vec_swap(nx_vec *a, nx_vec *b) {
    assert(a);
    assert(b);

    const nx_vec tmp = *a;
    *a = *b;
    *b = tmp;
}

bool nx_vec_insert(nx_vec *self, size_t idx, const void *elem) {
    assert(self);
    assert(idx <= self->elem_size);
    assert(elem);

    if (!grow_if_needed(self)) {
        return false;
    }

    shift_right(self, idx);
    nx_vec_set(self, idx, elem);
    ++self->len;

    return true;
}

void nx_vec_erase(nx_vec *self, size_t idx) {
    assert(self);
    assert(idx < self->len);

    shift_left(self, idx);
    --self->len;
}

/* ---------- to span ---------- */

nx_span nx_vec_to_span(nx_vec *self) {
    assert(self);

    return nx_make_span(self->data, self->len, self->elem_size);
}

nx_cspan nx_vec_to_cspan(const nx_vec *self) {
    assert(self);

    return nx_make_cspan(self->data, self->len, self->elem_size);
}

// internals defs

static nx_vec *create(size_t len, size_t cap, size_t elem_size) {
    if (elem_size == 0) {
        return NULL;
    }
    if (len > cap) {
        return NULL;
    }

    nx_vec *vec = malloc(sizeof(*vec));
    if (!vec) {
        return NULL;
    }

    set_fields(vec, NULL, len, cap, elem_size);

    if (cap == 0) {
        return vec;
    }

    void *data = calloc(vec->cap, vec->elem_size);
    if (!data) {
        free(vec);
        return NULL;
    }
    vec->data = data;
    return vec;
}

static bool grow_if_needed(nx_vec *self) {
    assert(self);

    if (self->len < self->cap) {
        return true;
    }

    const size_t new_cap = self->cap == 0 ? 1 : self->cap * 2;
    return nx_vec_reserve(self, new_cap);
}

static void set_fields(nx_vec *self, void *data, size_t len, size_t cap, size_t elem_size) {
    assert(self);

    self->data = data;
    self->len = len;
    self->cap = cap;
    self->elem_size = elem_size;
}

static void reset_fields(nx_vec *self) {
    assert(self);

    set_fields(self, NULL, 0, 0, self->elem_size);
}

static char *ptr_at(nx_vec *self, size_t idx) {
    assert(self);

    return (char *) self->data + idx * self->elem_size;
}

static const char *ptr_at_c(const nx_vec *self, size_t idx) {
    assert(self);

    return (const char *) self->data + idx * self->elem_size;
}

static void shift_left(nx_vec *self, size_t idx) {
    assert(self);

    const size_t es = self->elem_size;
    memmove(ptr_at(self, idx), ptr_at(self, idx + 1), (self->len - idx - 1) * es);
}

static void shift_right(nx_vec *self, size_t idx) {
    assert(self);

    const size_t es = self->elem_size;
    memmove(ptr_at(self, idx + 1), ptr_at(self, idx), (self->len - idx) * es);
}

static void *alloc_and_copy_data(const nx_vec *src) {
    if (src->cap == 0) {
        return NULL;
    }

    void *data = malloc(src->cap * src->elem_size);
    if (!data) {
        return NULL;
    }

    memcpy(data, src->data, src->len * src->elem_size);
    return data;
}