#include "nx/core/cmp.h"

#include <math.h>
#include <stddef.h>
#include <string.h>

#include "nx/core/type.h"
#include "nx/core/assert.h"

#define NX_CMP_INT_DEF(NAME, TYPE)                           \
    int nx_cmp_##NAME(const void *lhs, const void *rhs) {    \
        NX_ASSERT(lhs);                                      \
        NX_ASSERT(rhs);                                      \
        const TYPE a = *(const TYPE *) lhs;                  \
        const TYPE b = *(const TYPE *) rhs;                  \
        return (a < b) ? -1 : (a > b) ? 1 : 0;               \
    }

#define NX_CMP_FP_DEF(NAME, TYPE)                            \
    int nx_cmp_##NAME(const void *lhs, const void *rhs) {    \
        NX_ASSERT(lhs);                                      \
        NX_ASSERT(rhs);                                      \
        const TYPE a = *(const TYPE *) lhs;                  \
        const TYPE b = *(const TYPE *) rhs;                  \
        const nx_bool a_nan = isnan(a);                      \
        const nx_bool b_nan = isnan(b);                      \
        if (a_nan || b_nan) {                                \
            if (a_nan && b_nan) {                            \
                return 0;                                    \
            }                                                \
            return a_nan ? 1 : -1;                           \
        }                                                    \
        return (a < b) ? -1 : (a > b) ? 1 : 0;               \
    }

NX_CMP_INT_DEF(i8,    nx_i8)
NX_CMP_INT_DEF(i16,   nx_i16)
NX_CMP_INT_DEF(i32,   nx_i32)
NX_CMP_INT_DEF(i64,   nx_i64)

NX_CMP_INT_DEF(u8,    nx_u8)
NX_CMP_INT_DEF(u16,   nx_u16)
NX_CMP_INT_DEF(u32,   nx_u32)
NX_CMP_INT_DEF(u64,   nx_u64)

NX_CMP_INT_DEF(isize, nx_isize)
NX_CMP_INT_DEF(usize, nx_usize)

NX_CMP_INT_DEF(bool,  nx_bool)
NX_CMP_INT_DEF(byte,  nx_byte)
NX_CMP_INT_DEF(char,  nx_char)

NX_CMP_FP_DEF(flt,    nx_flt)
NX_CMP_FP_DEF(dbl,    nx_dbl)

NX_CMP_FP_DEF(f32,    nx_f32)
NX_CMP_FP_DEF(f64,    nx_f64)

int nx_cmp_cstr(const void *lhs, const void *rhs) {
    NX_ASSERT(lhs);
    NX_ASSERT(rhs);

    const char *a = *(const char * const *) lhs;
    const char *b = *(const char * const *) rhs;

    /* Policy: NULL is less than non-NULL */
    if (!a || !b) {
        if (a == b) {
            return 0;
        }
        return a ? 1 : -1;
    }

    return strcmp(a, b);
}
