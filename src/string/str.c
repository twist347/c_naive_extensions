#include "nx/string/str.h"

#include <stdlib.h>
#include <string.h>

#include "nx/core/assert.h"
#include "nx/numeric/limit.h"
#include "nx/mem/alloc_libc.h"

struct nx_Str {
    nx_char *data;
    nx_usize len; // excludes trailing '\0'
    nx_usize cap; // usable capacity, excludes trailing '\0'
    nx_Al *al;    // must not be null
};

#if NX_DEBUG
    static void str_assert_impl(const nx_Str *self) {
        NX_ASSERT(self != nx_null);
        NX_ASSERT(self->al != nx_null);
        NX_ASSERT(self->len <= self->cap);
        NX_ASSERT((self->cap == 0) == (self->data == nx_null));
        if (self->data) {
            NX_ASSERT(self->data[self->len] == '\0');
        }
    }
#define STR_ASSERT(self) \
    do { str_assert_impl((self)); } while (0)
#else
    #define STR_ASSERT(self) ((void) 0)
#endif

// internals decls

static nx_Status new_impl(nx_Str **out, nx_usize len, nx_usize cap, nx_Al *al);
static nx_Status ensure_cap(nx_Str *self, nx_usize needed_cap);
static nx_Status alloc_and_copy_str(nx_char **out, nx_Al *al, const nx_char *src, nx_usize len, nx_usize cap);
static void set_fields(nx_Str *self, nx_char *data, nx_usize len, nx_usize cap, nx_Al *al);

/* ========== lifetime ========== */

nx_StrRes nx_str_new_p(nx_StrParams p) {
    NX_ASSERT(p.len <= p.cap);
    NX_ASSERT(p.al);

    nx_Str *s = nx_null;
    const nx_Status st = new_impl(&s, p.len, p.cap, p.al);
    if (st != NX_STATUS_OK) {
        return NX_RES_NEW_ERR(nx_StrRes, st);
    }
    return NX_RES_NEW_OK(nx_StrRes, s);
}

nx_StrRes nx_str_new(void) {
    return nx_str_new_p((nx_StrParams){
        .len = 0,
        .cap = 0,
        .al = nx_al_libc_default_g(),
    });
}

nx_StrRes nx_str_new_len(nx_usize len) {
    return nx_str_new_p((nx_StrParams){
        .len = len,
        .cap = len,
        .al = nx_al_libc_default_g(),
    });
}

nx_StrRes nx_str_new_cap(nx_usize cap) {
    return nx_str_new_p((nx_StrParams){
        .len = 0,
        .cap = cap,
        .al = nx_al_libc_default_g(),
    });
}

nx_StrRes nx_str_from_cstr(const nx_char *cstr) {
    NX_ASSERT(cstr);

    const nx_usize n = strlen(cstr);
    nx_Al *al = nx_al_libc_default_g();

    nx_Str *s = malloc(sizeof(nx_Str));
    if (!s) {
        return NX_RES_NEW_ERR(nx_StrRes, NX_STATUS_OUT_OF_MEMORY);
    }

    set_fields(s, nx_null, 0, 0, al);

    if (n == 0) {
        return NX_RES_NEW_OK(nx_StrRes, s);
    }

    nx_char *data = nx_null;
    const nx_Status st = alloc_and_copy_str(&data, al, cstr, n, n);
    if (st != NX_STATUS_OK) {
        free(s);
        return NX_RES_NEW_ERR(nx_StrRes, st);
    }
    set_fields(s, data, n, n, al);
    return NX_RES_NEW_OK(nx_StrRes, s);
}

void nx_str_drop(nx_Str *self) {
    if (!self) {
        return;
    }

    nx_al_dealloc(self->al, self->data, self->cap + 1);
    free(self);
}

/* ========== copy/move ========== */

nx_StrRes nx_str_copy(const nx_Str *src) {
    STR_ASSERT(src);

    return nx_str_copy_a(src, src->al);
}

nx_StrRes nx_str_copy_a(const nx_Str *src, nx_Al *al) {
    STR_ASSERT(src);
    NX_ASSERT(al);

    nx_Str *dst = malloc(sizeof(nx_Str));
    if (!dst) {
        return NX_RES_NEW_ERR(nx_StrRes, NX_STATUS_OUT_OF_MEMORY);
    }

    set_fields(dst, nx_null, 0, 0, al);

    if (src->cap == 0) {
        return NX_RES_NEW_OK(nx_StrRes, dst);
    }

    nx_char *data = nx_null;
    const nx_Status st = alloc_and_copy_str(&data, al, src->data, src->len, src->cap);
    if (st != NX_STATUS_OK) {
        free(dst);
        return NX_RES_NEW_ERR(nx_StrRes, st);
    }
    set_fields(dst, data, src->len, src->cap, al);
    return NX_RES_NEW_OK(nx_StrRes, dst);
}

nx_Str *nx_str_move(nx_Str **src) {
    NX_ASSERT(src);
    NX_ASSERT(*src);
    STR_ASSERT(*src);

    nx_Str *tmp = *src;
    *src = nx_null;
    return tmp;
}

nx_Status nx_str_copy_assign(nx_Str *self, const nx_Str *src) {
    STR_ASSERT(self);
    STR_ASSERT(src);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->len == 0) {
        self->len = 0;
        if (self->data) {
            self->data[0] = '\0';
        }
        return NX_STATUS_OK;
    }

    // reuse buffer if it fits
    if (self->cap >= src->len) {
        memcpy(self->data, src->data, src->len);
        self->len = src->len;
        self->data[self->len] = '\0';
        return NX_STATUS_OK;
    }

    nx_char *data = nx_null;
    const nx_Status st = alloc_and_copy_str(&data, self->al, src->data, src->len, src->cap);
    if (st != NX_STATUS_OK) {
        return st;
    }
    nx_al_dealloc(self->al, self->data, self->cap + 1);
    set_fields(self, data, src->len, src->cap, self->al);
    return NX_STATUS_OK;
}

void nx_str_move_assign(nx_Str *self, nx_Str *src) {
    STR_ASSERT(self);
    STR_ASSERT(src);
    NX_ASSERT(nx_al_eq(self->al, src->al));

    if (self == src) {
        return;
    }

    nx_al_dealloc(self->al, self->data, self->cap + 1);

    self->data = src->data;
    self->len = src->len;
    self->cap = src->cap;

    src->data = nx_null;
    src->len = 0;
    src->cap = 0;
}

/* ========== info ========== */

nx_usize nx_str_len(const nx_Str *self) {
    STR_ASSERT(self);
    return self->len;
}

nx_usize nx_str_cap(const nx_Str *self) {
    STR_ASSERT(self);
    return self->cap;
}

nx_bool nx_str_empty(const nx_Str *self) {
    STR_ASSERT(self);
    return self->len == 0;
}

nx_Al *nx_str_al(const nx_Str *self) {
    STR_ASSERT(self);
    return self->al;
}

/* ========== access ========== */

nx_char nx_str_get(const nx_Str *self, nx_usize idx) {
    STR_ASSERT(self);
    NX_ASSERT(idx < self->len);
    return self->data[idx];
}

nx_char nx_str_at(const nx_Str *self, nx_usize idx) {
    STR_ASSERT(self);
    return idx < self->len ? self->data[idx] : '\0';
}

void nx_str_set(nx_Str *self, nx_usize idx, nx_char ch) {
    STR_ASSERT(self);
    NX_ASSERT(idx < self->len);
    self->data[idx] = ch;
}

nx_char *nx_str_data(nx_Str *self) {
    STR_ASSERT(self);
    return self->data;
}

const nx_char *nx_str_data_c(const nx_Str *self) {
    STR_ASSERT(self);
    return self->data;
}

const nx_char *nx_str_cstr(const nx_Str *self) {
    STR_ASSERT(self);
    return self->data ? self->data : "";
}

/* ========== mods ========== */

void nx_str_clear(nx_Str *self) {
    STR_ASSERT(self);
    self->len = 0;
    if (self->data) {
        self->data[0] = '\0';
    }
}

nx_Status nx_str_reserve(nx_Str *self, nx_usize new_cap) {
    STR_ASSERT(self);

    if (new_cap <= self->cap) {
        return NX_STATUS_OK;
    }

    const nx_usize new_bytes = new_cap + 1;
    const nx_usize old_bytes = self->cap == 0 ? 0 : self->cap + 1;

    nx_char *data;
    if (self->data) {
        data = nx_al_realloc(self->al, self->data, old_bytes, new_bytes);
    } else {
        data = nx_al_alloc(self->al, new_bytes);
        if (data) {
            data[0] = '\0';
        }
    }

    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    self->data = data;
    self->cap = new_cap;
    return NX_STATUS_OK;
}

nx_Status nx_str_resize(nx_Str *self, nx_usize new_len) {
    STR_ASSERT(self);

    if (new_len <= self->len) {
        self->len = new_len;
        if (self->data) {
            self->data[self->len] = '\0';
        }
        return NX_STATUS_OK;
    }

    if (new_len > self->cap) {
        const nx_Status st = nx_str_reserve(self, new_len);
        if (st != NX_STATUS_OK) {
            return st;
        }
    }

    memset(self->data + self->len, 0, new_len - self->len);
    self->len = new_len;
    self->data[self->len] = '\0';
    return NX_STATUS_OK;
}

nx_Status nx_str_push(nx_Str *self, nx_char ch) {
    STR_ASSERT(self);

    const nx_Status st = ensure_cap(self, self->len + 1);
    if (st != NX_STATUS_OK) {
        return st;
    }

    self->data[self->len] = ch;
    ++self->len;
    self->data[self->len] = '\0';
    return NX_STATUS_OK;
}

nx_Status nx_str_append_str_view(nx_Str *self, nx_StrView sv) {
    STR_ASSERT(self);
    NX_ASSERT(sv.len == 0 || sv.data != nx_null);

    if (sv.len == 0) {
        return NX_STATUS_OK;
    }

    const nx_usize new_len = self->len + sv.len;

    // handle aliasing: sv may point into self->data
    nx_bool alias = false;
    nx_usize off = 0;
    if (self->data && sv.data >= self->data && sv.data < self->data + self->cap + 1) {
        alias = true;
        off = (nx_usize) (sv.data - self->data);
    }

    const nx_Status st = ensure_cap(self, new_len);
    if (st != NX_STATUS_OK) {
        return st;
    }

    const nx_char *src = alias ? (self->data + off) : sv.data;
    memmove(self->data + self->len, src, sv.len);
    self->len = new_len;
    self->data[self->len] = '\0';
    return NX_STATUS_OK;
}

nx_Status nx_str_append_cstr(nx_Str *self, const nx_char *cstr) {
    STR_ASSERT(self);
    NX_ASSERT(cstr);

    return nx_str_append_str_view(self, nx_str_view_from_cstr(cstr));
}

/* ========== view ========== */

nx_StrView nx_str_as_view(const nx_Str *self) {
    STR_ASSERT(self);

    return self->len == 0
               ? nx_str_view_new(nx_null, 0)
               : nx_str_view_new(self->data, self->len);
}

// internals defs

static nx_Status new_impl(nx_Str **out, nx_usize len, nx_usize cap, nx_Al *al) {
    NX_ASSERT(out);
    NX_ASSERT(len <= cap);

    *out = nx_null;

    nx_Str *s = malloc(sizeof(nx_Str));
    if (!s) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(s, nx_null, 0, 0, al);

    if (cap == 0) {
        *out = s;
        return NX_STATUS_OK;
    }

    nx_char *data = nx_al_calloc(al, cap + 1, 1);
    if (!data) {
        free(s);
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(s, data, len, cap, al);
    *out = s;
    return NX_STATUS_OK;
}

static nx_Status ensure_cap(nx_Str *self, nx_usize needed_cap) {
    STR_ASSERT(self);

    if (needed_cap <= self->cap) {
        return NX_STATUS_OK;
    }

    nx_usize new_cap = self->cap == 0 ? 4 : self->cap;

    while (new_cap < needed_cap) {
        if (new_cap > NX_USIZE_MAX / 2) {
            new_cap = needed_cap;
            break;
        }
        new_cap *= 2;
    }

    return nx_str_reserve(self, new_cap);
}

static nx_Status alloc_and_copy_str(nx_char **out, nx_Al *al, const nx_char *src, nx_usize len, nx_usize cap) {
    NX_ASSERT(out);
    NX_ASSERT(al);
    NX_ASSERT(len <= cap);

    *out = nx_null;

    nx_char *data = nx_al_alloc(al, cap + 1);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    if (len > 0) {
        NX_ASSERT(src);
        memcpy(data, src, len);
    }
    data[len] = '\0';

    *out = data;
    return NX_STATUS_OK;
}

static void set_fields(nx_Str *self, nx_char *data, nx_usize len, nx_usize cap, nx_Al *al) {
    self->data = data;
    self->len = len;
    self->cap = cap;
    self->al = al;
}

