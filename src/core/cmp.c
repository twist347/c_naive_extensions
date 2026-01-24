#include "nx/core/cmp.h"

#include <math.h>
#include <stddef.h>
#include <string.h>

#include "nx/core/assert.h"

#define NX_CMP_INT_DEF(NAME, TYPE)                           \
    int nx_cmp_##NAME(const void *lhs, const void *rhs) {    \
        NX_ASSERT(lhs);                                      \
        NX_ASSERT(rhs);                                      \
        const TYPE a = *(const TYPE *) lhs;                   \
        const TYPE b = *(const TYPE *) rhs;                   \
        return (a < b) ? -1 : (a > b) ? 1 : 0;                \
    }

#define NX_CMP_FP_DEF(NAME, TYPE)                            \
    int nx_cmp_##NAME(const void *lhs, const void *rhs) {    \
        NX_ASSERT(lhs);                                      \
        NX_ASSERT(rhs);                                      \
        const TYPE a = *(const TYPE *) lhs;                   \
        const TYPE b = *(const TYPE *) rhs;                   \
        const int a_nan = isnan(a);                           \
        const int b_nan = isnan(b);                           \
        if (a_nan || b_nan) {                                 \
            if (a_nan && b_nan) {                             \
                return 0;                                     \
            }                                                 \
            return a_nan ? 1 : -1;                            \
        }                                                     \
        return (a < b) ? -1 : (a > b) ? 1 : 0;                \
    }

NX_CMP_INT_DEF(c,   char)
NX_CMP_INT_DEF(sc,  signed char)
NX_CMP_INT_DEF(uc,  unsigned char)

NX_CMP_INT_DEF(s,   short)
NX_CMP_INT_DEF(us,  unsigned short)

NX_CMP_INT_DEF(i,   int)
NX_CMP_INT_DEF(ui,  unsigned int)

NX_CMP_INT_DEF(l,   long)
NX_CMP_INT_DEF(ul,  unsigned long)

NX_CMP_INT_DEF(ll,  long long)
NX_CMP_INT_DEF(ull, unsigned long long)

NX_CMP_FP_DEF(f,    float)
NX_CMP_FP_DEF(d,    double)
NX_CMP_FP_DEF(ld,   long double)

NX_CMP_INT_DEF(zu,  size_t)

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
