#pragma once

#include <stdio.h>
#include <stdlib.h>

#define NX_C_ARR_LEN(arr)    (sizeof((arr)) / sizeof((arr)[0]))

#define NX_UNUSED(val)    ((void) (val))
