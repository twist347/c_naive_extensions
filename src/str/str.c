#include "nx/str/str.h"

#include <stdlib.h>
#include <string.h>

#include "nx/core/assert.h"
#include "nx/core/limit.h"
#include "nx/core/panic.h"

struct nx_str {
    nx_char *data;
    nx_usize len; /* excludes trailing '\0' */
    nx_usize cap; /* allocated bytes INCLUDING trailing '\0' */
};

#define NX_STR_ASSERT(self_)                                             \
    do {                                                                 \
        NX_ASSERT((self_) != nx_null);                                   \
        NX_ASSERT((self_)->len <= (self_)->cap);                         \
        NX_ASSERT(((self_)->cap == 0) == ((self_)->data == nx_null));    \
        /* ensure cap+1 fits */                                          \
        NX_ASSERT((self_)->cap <= (nx_usize) (NX_USIZE_MAX - 1));        \
        if ((self_)->cap > 0) {                                          \
            NX_ASSERT((self_)->data[(self_)->len] == '\0');              \
        }                                                                \
    } while (0)

// internals decls

static nx_status new_impl(nx_str **out, nx_usize len, nx_usize cap);

static void set_fields(nx_str *self, nx_char *data, nx_usize len, nx_usize cap);

/* ---------- lifetime ---------- */

nx_str_res nx_str_new(void) {
    nx_str *s = nx_null;
    const nx_status st = new_impl(&s, 0, 0);
    if (st != NX_STATUS_OK) {
        return NX_RES_ERR(nx_str_res, st);
    }
    return NX_RES_OK(nx_str_res, s);
}

nx_str_res nx_str_new_len(nx_usize len) {
    nx_str *s = nx_null;
    const nx_status st = new_impl(&s, len, len);
    if (st != NX_STATUS_OK) {
        return NX_RES_ERR(nx_str_res, st);
    }
    return NX_RES_OK(nx_str_res, s);
}

nx_str_res nx_str_new_cap(nx_usize cap) {
    nx_str *s = nx_null;
    const nx_status st = new_impl(&s, 0, cap);
    if (st != NX_STATUS_OK) {
        return NX_RES_ERR(nx_str_res, st);
    }
    return NX_RES_OK(nx_str_res, s);
}

nx_str_res nx_str_from_cstr(const nx_char *cstr) {
    NX_ASSERT(cstr);

    const nx_usize n = strlen(cstr);

    nx_str *s = nx_null;
    const nx_status st = new_impl(&s, 0, n);
    if (st != NX_STATUS_OK) {
        return NX_RES_ERR(nx_str_res, st);
    }

    if (n != 0) {
        memcpy(s->data, cstr, n);
    }
    s->len = n;
    if (s->data) {
        s->data[s->len] = '\0';
    }
    NX_STR_ASSERT(s);

    return NX_RES_OK(nx_str_res, s);
}

void nx_str_drop(nx_str *self) {
    if (!self) {
        return;
    }

    free(self->data);
    free(self);
}

/* ---------- copy/move ---------- */

nx_str_res nx_str_copy(const nx_str *src) {
    NX_STR_ASSERT(src);

    nx_str *dst = nx_null;
    const nx_status st = new_impl(&dst, src->len, src->cap);
    if (st != NX_STATUS_OK) {
        return NX_RES_ERR(nx_str_res, st);
    }

    if (src->len != 0) {
        memcpy(dst->data, src->data, src->len);
    }
    if (dst->data) {
        dst->data[dst->len] = '\0';
    }
    NX_STR_ASSERT(dst);

    return NX_RES_OK(nx_str_res, dst);
}

nx_str *nx_str_move(nx_str **src) {
    NX_ASSERT(src);
    NX_ASSERT(*src);
    NX_STR_ASSERT(*src);

    nx_str *tmp = *src;
    *src = nx_null;
    return tmp;
}

nx_status nx_str_copy_assign(nx_str *self, const nx_str *src) {
    NX_STR_ASSERT(self);
    NX_STR_ASSERT(src);

    if (self == src) {
        return NX_STATUS_OK;
    }

    if (src->cap == 0) {
        free(self->data);
        set_fields(self, nx_null, 0, 0);
        return NX_STATUS_OK;
    }

    if (src->cap > NX_USIZE_MAX - 1) {
        return NX_STATUS_MUL_OVERFLOW;
    }
    const nx_usize bytes = src->cap + 1;

    nx_char *data = malloc(bytes);
    if (!data) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    if (src->len != 0) {
        memcpy(data, src->data, src->len);
    }
    data[src->len] = '\0';

    free(self->data);
    set_fields(self, data, src->len, src->cap);
    NX_STR_ASSERT(self);

    NX_UNIMPLEMENTED();
}

void nx_str_move_assign(nx_str *self, nx_str *src) {
    NX_STR_ASSERT(self);
    NX_STR_ASSERT(src);

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

/* ---------- info ---------- */

nx_usize nx_str_len(const nx_str *self) {
    NX_STR_ASSERT(self);

    return self->len;
}

nx_usize nx_str_cap(const nx_str *self) {
    NX_STR_ASSERT(self);

    return self->cap;
}

nx_bool nx_str_empty(const nx_str *self) {
    NX_STR_ASSERT(self);

    return self->len == 0;
}

/* ---------- access ---------- */

nx_char nx_str_get(const nx_str *self, nx_usize idx) {
    NX_ASSERT(self);
    NX_ASSERT(idx < self->len);

    return self->data[idx];
}

void nx_str_set(nx_str *self, nx_usize idx, nx_char ch) {
    NX_ASSERT(self);
    NX_ASSERT(idx < self->len);

    self->data[idx] = ch;
}

nx_char *nx_str_data(nx_str *self) {
    NX_ASSERT(self);

    return self->data;
}

const nx_char *nx_str_data_c(const nx_str *self) {
    NX_ASSERT(self);

    return self->data;
}

const nx_char *nx_str_cstr(const nx_str *self) {
    NX_ASSERT(self);

    return self->data ? self->data : "";
}

/* ---------- mods ---------- */

void nx_str_clear(nx_str *self) {
    NX_STR_ASSERT(self);

    self->len = 0;
    if (self->data) {
        self->data[0] = '\0';
    }
}

nx_status nx_str_reserve(nx_str *self, nx_usize new_cap) {
    NX_STR_ASSERT(self);

    if (new_cap <= self->cap) {
        return NX_STATUS_OK;
    }

    if (new_cap > NX_USIZE_MAX - 1) {
        return NX_STATUS_MUL_OVERFLOW; /* cap+1 overflow */
    }

    const size_t bytes = new_cap + 1;

    nx_char *data = nx_null;
    if (self->cap == 0) {
        data = (nx_char *) malloc(bytes);
        if (!data) {
            return NX_STATUS_OUT_OF_MEMORY;
        }
        data[0] = '\0';
    } else {
        data = (nx_char *) realloc(self->data, bytes);
        if (!data) {
            return NX_STATUS_OUT_OF_MEMORY; /* keep old buffer intact */
        }
    }

    self->data = data;
    self->cap = new_cap;

    /* keep invariant */
    self->data[self->len] = '\0';
    NX_STR_ASSERT(self);

    return NX_STATUS_OK;
}

nx_status nx_str_resize(nx_str *self, nx_usize new_len) {
    NX_STR_ASSERT(self);

    if (new_len <= self->len) {
        self->len = new_len;
        if (self->data) {
            self->data[self->len] = '\0';
        }
        NX_STR_ASSERT(self);
        return NX_STATUS_OK;
    }

    if (new_len > self->cap) {
        const nx_status st = nx_str_reserve(self, new_len);
        if (st != NX_STATUS_OK) {
            return st;
        }
    }

    /* new bytes are '\0' */
    memset(self->data + self->len, 0, new_len - self->len);
    self->len = new_len;
    self->data[self->len] = '\0';
    NX_STR_ASSERT(self);

    return NX_STATUS_OK;
}

nx_status nx_str_push(nx_str *self, nx_char ch) {
    NX_STR_ASSERT(self);

    if (self->len == self->cap) {
        nx_usize new_cap = 0;
        if (self->cap == 0) {
            new_cap = 1;
        } else {
            if (self->cap > NX_USIZE_MAX / 2) {
                return NX_STATUS_MUL_OVERFLOW;
            }
            new_cap = self->cap * 2;
        }

        const nx_status st = nx_str_reserve(self, new_cap);
        if (st != NX_STATUS_OK) {
            return st;
        }
    }

    self->data[self->len] = ch;
    ++self->len;
    self->data[self->len] = '\0';
    NX_STR_ASSERT(self);

    return NX_STATUS_OK;
}

nx_status nx_str_append_str_view(nx_str *self, nx_str_view sv) {
    NX_STR_ASSERT(self);
    NX_ASSERT(sv.len == 0 || sv.data != nx_null);

    if (sv.len == 0) {
        return NX_STATUS_OK;
    }

    if (sv.len > NX_USIZE_MAX - self->len) {
        return NX_STATUS_MUL_OVERFLOW; /* len + sv.len overflow */
    }

    const nx_usize new_len = self->len + sv.len;

    /* handle aliasing: if sv points into self->data, keep offset across realloc */
    nx_usize off = 0;
    nx_bool alias = false;
    if (self->data) {
        const nx_char *begin = self->data;
        const nx_char *end = self->data + self->cap + 1; /* include '\0' area */
        if (sv.data >= begin && sv.data < end) {
            alias = true;
            off = (nx_usize) (sv.data - begin);
        }
    }

    if (new_len > self->cap) {
        const nx_status st = nx_str_reserve(self, new_len);
        if (st != NX_STATUS_OK) {
            return st;
        }
    }

    const nx_char *src = alias ? (self->data + off) : sv.data;

    memmove(self->data + self->len, src, sv.len);
    self->len = new_len;
    self->data[self->len] = '\0';
    NX_STR_ASSERT(self);

    return NX_STATUS_OK;
}

nx_status nx_str_append_cstr(nx_str *self, const nx_char *cstr) {
    NX_STR_ASSERT(self);
    NX_ASSERT(cstr);

    return nx_str_append_str_view(self, nx_str_view_from_cstr(cstr));
}

/* ---------- view ---------- */

nx_str_view nx_str_as_view(const nx_str *self) {
    NX_STR_ASSERT(self);

    return self->len == 0
               ? nx_str_view_new(nx_null, 0)
               : nx_str_view_new(self->data, self->len);
}

// internals defs

static nx_status new_impl(nx_str **out, nx_usize len, nx_usize cap) {
    NX_ASSERT(out);
    NX_ASSERT(len <= cap);

    *out = nx_null;

    nx_str *s = malloc(sizeof(nx_str));
    if (!s) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(s, nx_null, 0, 0);

    if (cap == 0) {
        set_fields(s, nx_null, len, cap);
        *out = s;
        return NX_STATUS_OK;
    }

    /* bytes = cap + 1 */
    if (cap > NX_USIZE_MAX - 1) {
        free(s);
        return NX_STATUS_MUL_OVERFLOW;
    }

    nx_char *data = calloc(cap + 1, 1); /* +1 and zero-init */
    if (!data) {
        free(s);
        return NX_STATUS_OUT_OF_MEMORY;
    }

    set_fields(s, data, len, cap);
    NX_STR_ASSERT(s);

    *out = s;
    return NX_STATUS_OK;
}

static void set_fields(nx_str *self, nx_char *data, nx_usize len, nx_usize cap) {
    self->data = data;
    self->len = len;
    self->cap = cap;
}
