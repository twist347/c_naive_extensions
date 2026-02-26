#include "nx/numeric/bit.h"

#include "nx/core/assert.h"

/* ========== bit set / clear / toggle / test ========== */

#define NX_BIT_SET_DEF(NAME, T, BITS)        \
    T nx_bit_set_##NAME(T val, nx_u32 pos) { \
        NX_ASSERT(pos < (BITS));             \
        return val | ((T) 1 << pos);         \
    }

#define NX_BIT_CLEAR_DEF(NAME, T, BITS)        \
    T nx_bit_clear_##NAME(T val, nx_u32 pos) { \
        NX_ASSERT(pos < (BITS));               \
        return val & ~((T)1 << pos);           \
    }

#define NX_BIT_TOGGLE_DEF(NAME, T, BITS)        \
    T nx_bit_toggle_##NAME(T val, nx_u32 pos) { \
        NX_ASSERT(pos < (BITS));                \
        return val ^ ((T)1 << pos);             \
    }

#define NX_BIT_TEST_DEF(NAME, T, BITS)              \
    nx_bool nx_bit_test_##NAME(T val, nx_u32 pos) { \
        NX_ASSERT(pos < (BITS));                    \
        return (val >> pos) & 1;                    \
    }

#define NX_BIT_OPS_DEF(NAME, T, BITS) \
    NX_BIT_SET_DEF(NAME, T, BITS)     \
    NX_BIT_CLEAR_DEF(NAME, T, BITS)   \
    NX_BIT_TOGGLE_DEF(NAME, T, BITS)  \
    NX_BIT_TEST_DEF(NAME, T, BITS)

NX_BIT_OPS_DEF(u8,  nx_u8,  8)
NX_BIT_OPS_DEF(u16, nx_u16, 16)
NX_BIT_OPS_DEF(u32, nx_u32, 32)
NX_BIT_OPS_DEF(u64, nx_u64, 64)

/* ========== rotate ========== */

/*
 * Standard rotate idiom. Compilers (GCC, Clang, MSVC) recognize
 * this pattern and emit single rol/ror instructions.
 */

#define NX_ROTL_DEF(NAME, T, BITS)                 \
    T nx_rotl_##NAME(T val, nx_u32 n) {            \
        NX_ASSERT(n < (BITS));                     \
        if (n == 0) return val;                    \
        return (val << n) | (val >> ((BITS) - n)); \
    }

#define NX_ROTR_DEF(NAME, T, BITS)                 \
    T nx_rotr_##NAME(T val, nx_u32 n) {            \
        NX_ASSERT(n < (BITS));                     \
        if (n == 0) return val;                    \
        return (val >> n) | (val << ((BITS) - n)); \
    }

NX_ROTL_DEF(u8,  nx_u8,  8)
NX_ROTL_DEF(u16, nx_u16, 16)
NX_ROTL_DEF(u32, nx_u32, 32)
NX_ROTL_DEF(u64, nx_u64, 64)

NX_ROTR_DEF(u8,  nx_u8,  8)
NX_ROTR_DEF(u16, nx_u16, 16)
NX_ROTR_DEF(u32, nx_u32, 32)
NX_ROTR_DEF(u64, nx_u64, 64)

/* ========== byte swap ========== */

nx_u16 nx_bswap_u16(nx_u16 val) {
    return (val >> 8) | (val << 8);
}

nx_u32 nx_bswap_u32(nx_u32 val) {
    return ((val >> 24) & 0x000000FF)
         | ((val >>  8) & 0x0000FF00)
         | ((val <<  8) & 0x00FF0000)
         | ((val << 24) & 0xFF000000);
}

nx_u64 nx_bswap_u64(nx_u64 val) {
    return ((val >> 56) & 0x00000000000000FF)
         | ((val >> 40) & 0x000000000000FF00)
         | ((val >> 24) & 0x0000000000FF0000)
         | ((val >>  8) & 0x00000000FF000000)
         | ((val <<  8) & 0x000000FF00000000)
         | ((val << 24) & 0x0000FF0000000000)
         | ((val << 40) & 0x00FF000000000000)
         | ((val << 56) & 0xFF00000000000000);
}

/* ========== bit mask ========== */

#define NX_BIT_MASK_DEF(NAME, T, BITS)          \
    T nx_bit_mask_##NAME(nx_u32 nbits) {        \
        NX_ASSERT(nbits <= (BITS));             \
        if (nbits == 0) return 0;               \
        if (nbits == (BITS)) return (T) ~(T) 0; \
        return ((T) 1 << nbits) - 1;            \
    }

NX_BIT_MASK_DEF(u8,  nx_u8,  8)
NX_BIT_MASK_DEF(u16, nx_u16, 16)
NX_BIT_MASK_DEF(u32, nx_u32, 32)
NX_BIT_MASK_DEF(u64, nx_u64, 64)

/* ========== bit extract / deposit ========== */

#define NX_BIT_EXTRACT_DEF(NAME, T, BITS)                    \
    T nx_bit_extract_##NAME(T val, nx_u32 pos, nx_u32 len) { \
        NX_ASSERT(pos < (BITS));                             \
        NX_ASSERT(len <= (BITS));                            \
        NX_ASSERT(pos + len <= (BITS));                      \
        if (len == 0) return 0;                              \
        return (val >> pos) & nx_bit_mask_##NAME(len);       \
    }

#define NX_BIT_DEPOSIT_DEF(NAME, T, BITS)                            \
    T nx_bit_deposit_##NAME(T val, nx_u32 pos, nx_u32 len, T bits) { \
        NX_ASSERT(pos < (BITS));                                     \
        NX_ASSERT(len <= (BITS));                                    \
        NX_ASSERT(pos + len <= (BITS));                              \
        if (len == 0) return val;                                    \
        const T mask = nx_bit_mask_##NAME(len);                      \
        return (val & ~(mask << pos)) | ((bits & mask) << pos);      \
    }

NX_BIT_EXTRACT_DEF(u8,  nx_u8,  8)
NX_BIT_EXTRACT_DEF(u16, nx_u16, 16)
NX_BIT_EXTRACT_DEF(u32, nx_u32, 32)
NX_BIT_EXTRACT_DEF(u64, nx_u64, 64)

NX_BIT_DEPOSIT_DEF(u8,  nx_u8,  8)
NX_BIT_DEPOSIT_DEF(u16, nx_u16, 16)
NX_BIT_DEPOSIT_DEF(u32, nx_u32, 32)
NX_BIT_DEPOSIT_DEF(u64, nx_u64, 64)

/* ========== bit reverse ========== */

nx_u8 nx_bit_reverse_u8(nx_u8 val) {
    val = ((val & 0xF0) >> 4) | ((val & 0x0F) << 4);
    val = ((val & 0xCC) >> 2) | ((val & 0x33) << 2);
    val = ((val & 0xAA) >> 1) | ((val & 0x55) << 1);
    return val;
}

nx_u16 nx_bit_reverse_u16(nx_u16 val) {
    val = ((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8);
    val = ((val & 0xF0F0) >> 4) | ((val & 0x0F0F) << 4);
    val = ((val & 0xCCCC) >> 2) | ((val & 0x3333) << 2);
    val = ((val & 0xAAAA) >> 1) | ((val & 0x5555) << 1);
    return val;
}

nx_u32 nx_bit_reverse_u32(nx_u32 val) {
    val = ((val & 0xFFFF0000) >> 16) | ((val & 0x0000FFFF) << 16);
    val = ((val & 0xFF00FF00) >>  8) | ((val & 0x00FF00FF) <<  8);
    val = ((val & 0xF0F0F0F0) >>  4) | ((val & 0x0F0F0F0F) <<  4);
    val = ((val & 0xCCCCCCCC) >>  2) | ((val & 0x33333333) <<  2);
    val = ((val & 0xAAAAAAAA) >>  1) | ((val & 0x55555555) <<  1);
    return val;
}

nx_u64 nx_bit_reverse_u64(nx_u64 val) {
    val = ((val & 0xFFFFFFFF00000000) >> 32) | ((val & 0x00000000FFFFFFFF) << 32);
    val = ((val & 0xFFFF0000FFFF0000) >> 16) | ((val & 0x0000FFFF0000FFFF) << 16);
    val = ((val & 0xFF00FF00FF00FF00) >>  8) | ((val & 0x00FF00FF00FF00FF) <<  8);
    val = ((val & 0xF0F0F0F0F0F0F0F0) >>  4) | ((val & 0x0F0F0F0F0F0F0F0F) <<  4);
    val = ((val & 0xCCCCCCCCCCCCCCCC) >>  2) | ((val & 0x3333333333333333) <<  2);
    val = ((val & 0xAAAAAAAAAAAAAAAA) >>  1) | ((val & 0x5555555555555555) <<  1);
    return val;
}