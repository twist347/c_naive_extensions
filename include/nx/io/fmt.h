#pragma once

#include <inttypes.h>

/* ========== format strings ========== */

#define NX_FMT_I8         "%" PRId8
#define NX_FMT_I16        "%" PRId16
#define NX_FMT_I32        "%" PRId32
#define NX_FMT_I64        "%" PRId64

#define NX_FMT_U8         "%" PRIu8
#define NX_FMT_U16        "%" PRIu16
#define NX_FMT_U32        "%" PRIu32
#define NX_FMT_U64        "%" PRIu64

#define NX_FMT_USIZE      "%zu"
#define NX_FMT_ISIZE      "%td"

#define NX_FMT_CHAR       "%c"
#define NX_FMT_BYTE       "%hhu"

#define NX_FMT_F32        "%g"
#define NX_FMT_F64        "%g"

#define NX_FMT_STR        "%s"

#define NX_FMT_PTR        "%p"

#define NX_FMT_BOOL       "%s"
#define NX_BOOL_STR(b)    ((b) ? "true" : "false")

#define NX_FMT_HASH       NX_FMT_U64