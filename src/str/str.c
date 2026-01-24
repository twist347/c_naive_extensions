#include "nx/str/str.h"

#include <stddef.h>

struct nx_str {
    char *data;
    size_t len;
    size_t cap;
};
