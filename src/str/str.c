#include "nx/str/str.h"

#include "nx/core/type.h"

struct nx_str {
    char *data;
    nx_usize len;
    nx_usize cap;
};
