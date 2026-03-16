#include "nx/io/dbg.h"

#include "nx/core/assert.h"

#define NX_DBG_DEF(SUFFIX, TYPE, FMT)                                                               \
    void nx_dbg_##SUFFIX##_(FILE *stream, const char *file, int line, const char *expr, TYPE val) { \
        NX_ASSERT(stream);                                                                          \
        fprintf(stream, "[%s:%d] %s = " FMT "\n", file, line, expr, val);                           \
    }

NX_DBG_DEF(i8,   nx_i8,   NX_FMT_I8)
NX_DBG_DEF(i16,  nx_i16,  NX_FMT_I16)
NX_DBG_DEF(i32,  nx_i32,  NX_FMT_I32)
NX_DBG_DEF(i64,  nx_i64,  NX_FMT_I64)

NX_DBG_DEF(u8,   nx_u8,   NX_FMT_U8)
NX_DBG_DEF(u16,  nx_u16,  NX_FMT_U16)
NX_DBG_DEF(u32,  nx_u32,  NX_FMT_U32)
NX_DBG_DEF(u64,  nx_u64,  NX_FMT_U64)

NX_DBG_DEF(f32,  nx_f32,  NX_FMT_F32)
NX_DBG_DEF(f64,  nx_f64,  NX_FMT_F64)

void nx_dbg_bool_(FILE *stream, const char *file, int line, const char *expr, nx_bool val) {
    NX_ASSERT(stream);
    fprintf(stream, "[%s:%d] %s = %s\n", file, line, expr, val ? "true" : "false");
}

void nx_dbg_cstr_(FILE *stream, const char *file, int line, const char *expr, const nx_char *val) {
    NX_ASSERT(stream);
    fprintf(stream, "[%s:%d] %s = %s\n", file, line, expr, val ? val : "(null)");
}