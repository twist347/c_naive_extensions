#pragma once

#define NX_C_ARR_LEN(arr)    (sizeof((arr)) / sizeof((arr)[0]))

#define NX_UNUSED(val)    ((void) (val))

#define NX_REQUIRE_LVALUE(x)    ((void) sizeof(&(x)))
