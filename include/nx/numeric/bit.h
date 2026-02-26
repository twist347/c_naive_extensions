#pragma once

#include <stdbit.h>

#include "nx/core/type.h"

/* ========== count ========== */

#define nx_count_ones(x)   stdc_count_ones(x)
#define nx_count_zeros(x)  stdc_count_zeros(x)

/* ========== leading / trailing zeros and ones ========== */

#define nx_leading_zeros(x)  stdc_leading_zeros(x)
#define nx_leading_ones(x)   stdc_leading_ones(x)
#define nx_trailing_zeros(x) stdc_trailing_zeros(x)
#define nx_trailing_ones(x)  stdc_trailing_ones(x)

/* ========== bit width / ceil / floor ========== */

/// minimum bits needed to represent x (0 => 0)
#define nx_bit_width(x)      stdc_bit_width(x)

/// smallest power of 2 >= x
#define nx_bit_ceil(x)       stdc_bit_ceil(x)

/// largest power of 2 <= x (0 => 0)
#define nx_bit_floor(x)      stdc_bit_floor(x)

/* ========== power of 2 test ========== */

#define nx_has_single_bit(x) stdc_has_single_bit(x)
#define nx_is_power_of_2(x)  stdc_has_single_bit(x)

/* ========== bit set / clear / toggle / test ========== */

nx_u8  nx_bit_set_u8(nx_u8 val, nx_u32 pos);
nx_u16 nx_bit_set_u16(nx_u16 val, nx_u32 pos);
nx_u32 nx_bit_set_u32(nx_u32 val, nx_u32 pos);
nx_u64 nx_bit_set_u64(nx_u64 val, nx_u32 pos);

nx_u8  nx_bit_clear_u8(nx_u8 val, nx_u32 pos);
nx_u16 nx_bit_clear_u16(nx_u16 val, nx_u32 pos);
nx_u32 nx_bit_clear_u32(nx_u32 val, nx_u32 pos);
nx_u64 nx_bit_clear_u64(nx_u64 val, nx_u32 pos);

nx_u8  nx_bit_toggle_u8(nx_u8 val, nx_u32 pos);
nx_u16 nx_bit_toggle_u16(nx_u16 val, nx_u32 pos);
nx_u32 nx_bit_toggle_u32(nx_u32 val, nx_u32 pos);
nx_u64 nx_bit_toggle_u64(nx_u64 val, nx_u32 pos);

nx_bool nx_bit_test_u8(nx_u8 val, nx_u32 pos);
nx_bool nx_bit_test_u16(nx_u16 val, nx_u32 pos);
nx_bool nx_bit_test_u32(nx_u32 val, nx_u32 pos);
nx_bool nx_bit_test_u64(nx_u64 val, nx_u32 pos);

#define nx_bit_set(val, pos) _Generic((val), \
    nx_u8:  nx_bit_set_u8,                   \
    nx_u16: nx_bit_set_u16,                  \
    nx_u32: nx_bit_set_u32,                  \
    nx_u64: nx_bit_set_u64                   \
)((val), (pos))

#define nx_bit_clear(val, pos) _Generic((val), \
    nx_u8:  nx_bit_clear_u8,                   \
    nx_u16: nx_bit_clear_u16,                  \
    nx_u32: nx_bit_clear_u32,                  \
    nx_u64: nx_bit_clear_u64                   \
)((val), (pos))

#define nx_bit_toggle(val, pos) _Generic((val), \
    nx_u8:  nx_bit_toggle_u8,                   \
    nx_u16: nx_bit_toggle_u16,                  \
    nx_u32: nx_bit_toggle_u32,                  \
    nx_u64: nx_bit_toggle_u64                   \
)((val), (pos))

#define nx_bit_test(val, pos) _Generic((val), \
    nx_u8:  nx_bit_test_u8,                   \
    nx_u16: nx_bit_test_u16,                  \
    nx_u32: nx_bit_test_u32,                  \
    nx_u64: nx_bit_test_u64                   \
)((val), (pos))

/* ========== rotate ========== */

nx_u8  nx_rotl_u8(nx_u8 val, nx_u32 n);
nx_u16 nx_rotl_u16(nx_u16 val, nx_u32 n);
nx_u32 nx_rotl_u32(nx_u32 val, nx_u32 n);
nx_u64 nx_rotl_u64(nx_u64 val, nx_u32 n);

nx_u8  nx_rotr_u8(nx_u8 val, nx_u32 n);
nx_u16 nx_rotr_u16(nx_u16 val, nx_u32 n);
nx_u32 nx_rotr_u32(nx_u32 val, nx_u32 n);
nx_u64 nx_rotr_u64(nx_u64 val, nx_u32 n);

#define nx_rotl(val, n) _Generic((val), \
    nx_u8:  nx_rotl_u8,                 \
    nx_u16: nx_rotl_u16,                \
    nx_u32: nx_rotl_u32,                \
    nx_u64: nx_rotl_u64                 \
)((val), (n))

#define nx_rotr(val, n) _Generic((val), \
    nx_u8:  nx_rotr_u8,                 \
    nx_u16: nx_rotr_u16,                \
    nx_u32: nx_rotr_u32,                \
    nx_u64: nx_rotr_u64                 \
)((val), (n))

/* ========== byte swap ========== */

nx_u16 nx_bswap_u16(nx_u16 val);
nx_u32 nx_bswap_u32(nx_u32 val);
nx_u64 nx_bswap_u64(nx_u64 val);

#define nx_bswap(val) _Generic((val), \
    nx_u16: nx_bswap_u16,             \
    nx_u32: nx_bswap_u32,             \
    nx_u64: nx_bswap_u64              \
)((val))

/* ========== bit mask ========== */

/// returns mask with nbits lowest bits set. nx_bit_mask_u32(3) => 0b111
nx_u8  nx_bit_mask_u8(nx_u32 nbits);
nx_u16 nx_bit_mask_u16(nx_u32 nbits);
nx_u32 nx_bit_mask_u32(nx_u32 nbits);
nx_u64 nx_bit_mask_u64(nx_u32 nbits);

/* ========== bit extract / deposit ========== */

/// extract len bits starting at pos. nx_bit_extract_u32(0xFF, 4, 4) => 0xF
nx_u8  nx_bit_extract_u8(nx_u8 val, nx_u32 pos, nx_u32 len);
nx_u16 nx_bit_extract_u16(nx_u16 val, nx_u32 pos, nx_u32 len);
nx_u32 nx_bit_extract_u32(nx_u32 val, nx_u32 pos, nx_u32 len);
nx_u64 nx_bit_extract_u64(nx_u64 val, nx_u32 pos, nx_u32 len);

/// insert len low bits of bits into val starting at pos
nx_u8  nx_bit_deposit_u8(nx_u8 val, nx_u32 pos, nx_u32 len, nx_u8 bits);
nx_u16 nx_bit_deposit_u16(nx_u16 val, nx_u32 pos, nx_u32 len, nx_u16 bits);
nx_u32 nx_bit_deposit_u32(nx_u32 val, nx_u32 pos, nx_u32 len, nx_u32 bits);
nx_u64 nx_bit_deposit_u64(nx_u64 val, nx_u32 pos, nx_u32 len, nx_u64 bits);

/* ========== bit reverse ========== */

nx_u8  nx_bit_reverse_u8(nx_u8 val);
nx_u16 nx_bit_reverse_u16(nx_u16 val);
nx_u32 nx_bit_reverse_u32(nx_u32 val);
nx_u64 nx_bit_reverse_u64(nx_u64 val);

#define nx_bit_reverse(val) _Generic((val), \
    nx_u8:  nx_bit_reverse_u8,              \
    nx_u16: nx_bit_reverse_u16,             \
    nx_u32: nx_bit_reverse_u32,             \
    nx_u64: nx_bit_reverse_u64              \
)((val))
