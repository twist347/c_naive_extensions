#pragma once

#include "nx/io/print.h"

/* ========== debug ========== */

/// nx_dbg(x) => prints "x = <value>\n" with auto format
#define nx_dbg(x) _Generic((x),                                    \
    nx_i8:             nx_printf(#x " = " NX_FMT_I8    "\n", (x)), \
    nx_i16:            nx_printf(#x " = " NX_FMT_I16   "\n", (x)), \
    nx_i32:            nx_printf(#x " = " NX_FMT_I32   "\n", (x)), \
    nx_i64:            nx_printf(#x " = " NX_FMT_I64   "\n", (x)), \
    nx_u8:             nx_printf(#x " = " NX_FMT_U8    "\n", (x)), \
    nx_u16:            nx_printf(#x " = " NX_FMT_U16   "\n", (x)), \
    nx_u32:            nx_printf(#x " = " NX_FMT_U32   "\n", (x)), \
    nx_u64:            nx_printf(#x " = " NX_FMT_U64   "\n", (x)), \
    nx_f32:            nx_printf(#x " = " NX_FMT_F32   "\n", (x)), \
    nx_f64:            nx_printf(#x " = " NX_FMT_F64   "\n", (x)), \
    nx_char *:         nx_printf(#x " = " NX_FMT_STR   "\n", (x)), \
    const nx_char *:   nx_printf(#x " = " NX_FMT_STR   "\n", (x))  \
)

#define nx_fdbg(stream, x) _Generic((x),                                      \
    nx_i8:             nx_fprintf((stream), #x " = " NX_FMT_I8    "\n", (x)), \
    nx_i16:            nx_fprintf((stream), #x " = " NX_FMT_I16   "\n", (x)), \
    nx_i32:            nx_fprintf((stream), #x " = " NX_FMT_I32   "\n", (x)), \
    nx_i64:            nx_fprintf((stream), #x " = " NX_FMT_I64   "\n", (x)), \
    nx_u8:             nx_fprintf((stream), #x " = " NX_FMT_U8    "\n", (x)), \
    nx_u16:            nx_fprintf((stream), #x " = " NX_FMT_U16   "\n", (x)), \
    nx_u32:            nx_fprintf((stream), #x " = " NX_FMT_U32   "\n", (x)), \
    nx_u64:            nx_fprintf((stream), #x " = " NX_FMT_U64   "\n", (x)), \
    nx_f32:            nx_fprintf((stream), #x " = " NX_FMT_F32   "\n", (x)), \
    nx_f64:            nx_fprintf((stream), #x " = " NX_FMT_F64   "\n", (x)), \
    nx_char *:         nx_fprintf((stream), #x " = " NX_FMT_STR   "\n", (x)), \
    const nx_char *:   nx_fprintf((stream), #x " = " NX_FMT_STR   "\n", (x))  \
)
