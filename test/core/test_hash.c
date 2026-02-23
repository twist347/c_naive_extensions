#include "unity.h"

#include <stdio.h>

#include "nx/core/hash.h"
#include "nx/core/print.h"

// TODO: impl

void setUp(void) {}
void tearDown(void) {}

static void test_A_B(void) {
    for (nx_i32 i = -10; i < 10; ++i) {
        const nx_hash h = nx_hash_i32(i);
        nx_fprintf(stdout, NX_FMT_HASH "\n", h);
    }

    const nx_hash hs = nx_hash_cstr("hello");
    nx_fprintf(stdout, NX_FMT_HASH "\n", hs);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_A_B);
    return UNITY_END();
}
