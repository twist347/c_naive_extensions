#include "nx/numeric/abs.h"

#include <math.h>

#include "nx/core/assert.h"
#include "nx/numeric/limit.h"

nx_i8 nx_abs_i8(nx_i8 x) {
    NX_ASSERT(x != NX_I8_MIN);

    return x < 0 ? -x : x;
}

nx_i16 nx_abs_i16(nx_i16 x) {
    NX_ASSERT(x != NX_I16_MIN);

    return x < 0 ? -x : x;
}

nx_i32 nx_abs_i32(nx_i32 x) {
    NX_ASSERT(x != NX_I32_MIN);

    return x < 0 ? -x : x;
}

nx_i64 nx_abs_i64(nx_i64 x) {
    NX_ASSERT(x != NX_I64_MIN);

    return x < 0 ? -x : x;
}

nx_f32 nx_abs_f32(nx_f32 x) {
    return fabsf(x);
}

nx_f64 nx_abs_f64(nx_f64 x) {
    return fabs(x);
}
