#include "nx/numeric/sign.h"

#include <math.h>

#define SIGN_SIGNED(T, x) \
    (T)((x > 0) - (x < 0))

#define SIGN_UNSIGNED(T, x) \
    (T)(x > 0)

nx_i8 nx_sign_i8(nx_i8 x) {
    return SIGN_SIGNED(nx_i8, x);
}

nx_i16 nx_sign_i16(nx_i16 x) {
    return SIGN_SIGNED(nx_i16, x);
}

nx_i32 nx_sign_i32(nx_i32 x) {
    return SIGN_SIGNED(nx_i32, x);
}

nx_i64 nx_sign_i64(nx_i64 x) {
    return SIGN_SIGNED(nx_i64, x);
}

nx_u8 nx_sign_u8(nx_u8 x) {
    return SIGN_UNSIGNED(nx_u8, x);
}

nx_u16 nx_sign_u16(nx_u16 x) {
    return SIGN_UNSIGNED(nx_u16, x);
}

nx_u32 nx_sign_u32(nx_u32 x) {
    return SIGN_UNSIGNED(nx_u32, x);
}

nx_u64 nx_sign_u64(nx_u64 x) {
    return SIGN_UNSIGNED(nx_u64, x);
}

nx_f32 nx_sign_f32(nx_f32 x) {
    if (isnan(x)) return x;
    if (x > 0.0f) return 1.0f;
    if (x < 0.0f) return -1.0f;
    return x;
}

nx_f64 nx_sign_f64(nx_f64 x) {
    if (isnan(x)) return x;
    if (x > 0.0) return 1.0;
    if (x < 0.0) return -1.0;
    return x;
}
