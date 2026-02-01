#pragma once

#include "nx/core/type.h"

#include <float.h>
#include <limits.h>

#define NX_I8_MIN     INT8_MIN
#define NX_I8_MAX     INT8_MAX
#define NX_I16_MIN    INT16_MIN
#define NX_I16_MAX    INT16_MAX
#define NX_I32_MIN    INT32_MIN
#define NX_I32_MAX    INT32_MAX
#define NX_I64_MIN    INT64_MIN
#define NX_I64_MAX    INT64_MAX

#define NX_U8_MAX     UINT8_MAX
#define NX_U16_MAX    UINT16_MAX
#define NX_U32_MAX    UINT32_MAX
#define NX_U64_MAX    UINT64_MAX

#define NX_USIZE_MAX    SIZE_MAX
#define NX_ISIZE_MIN    PTRDIFF_MIN
#define NX_ISIZE_MAX    PTRDIFF_MAX

#define NX_BYTE_MAX    UCHAR_MAX

#define NX_F32_MAX        FLT_MAX
#define NX_F32_MIN_POS    FLT_MIN
#define NX_F32_LOWEST     (-FLT_MAX)
#define NX_F32_EPS        FLT_EPSILON

#define NX_F64_MAX        DBL_MAX
#define NX_F64_MIN_POS    DBL_MIN
#define NX_F64_LOWEST     (-DBL_MAX)
#define NX_F64_EPS        DBL_EPSILON

#define NX_FLT_MAX        FLT_MAX
#define NX_FLT_MIN_POS    FLT_MIN
#define NX_FLT_LOWEST     (-FLT_MAX)

#define NX_DBL_MAX        DBL_MAX
#define NX_DBL_MIN_POS    DBL_MIN
#define NX_DBL_LOWEST     (-DBL_MAX)