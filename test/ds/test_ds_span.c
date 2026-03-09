#include "unity.h"

#include <stdio.h>

#include "nx/ds/span.h"
#include "nx/io/print.h"
#include "nx/core/type.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_A_B(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5};
    const nx_Span s = NX_SPAN_FROM_PTR(arr, 5);

    const nx_i32 x = 100;
    nx_span_set(s, 0, &x);

    TEST_ASSERT_EQUAL_INT32(100, arr[0]);

    nx_span_fprintln(s, stdout, nx_fprint_i32);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_A_B);
    return UNITY_END();
}
