#include "unity.h"

#include <string.h>

#include "nx/io/log.h"

static char buf[1024];
static FILE *mem;

void setUp(void) {
    memset(buf, 0, sizeof(buf));
    mem = fmemopen(buf, sizeof(buf), "w");
    nx_log_set_stream(mem);
    nx_log_set_level(NX_LOG_DEBUG);
}

void tearDown(void) {
    if (mem) {
        fclose(mem);
        mem = nx_null;
    }
    nx_log_set_stream(nx_null);
    nx_log_set_level(NX_LOG_DEBUG);
}

/* ========== helpers ========== */

static void flush(void) {
    fflush(mem);
}

/* ========== basic output ========== */

static void test_log_debug_output(void) {
    nx_log_debug("hello %d", 42);
    flush();

    TEST_ASSERT_NOT_NULL(strstr(buf, "DEBUG"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "hello 42"));
}

static void test_log_info_output(void) {
    nx_log_info("count=%d", 10);
    flush();

    TEST_ASSERT_NOT_NULL(strstr(buf, "INFO"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "count=10"));
}

static void test_log_warn_output(void) {
    nx_log_warn("low memory");
    flush();

    TEST_ASSERT_NOT_NULL(strstr(buf, "WARN"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "low memory"));
}

static void test_log_error_output(void) {
    nx_log_error("failed: %s", "oom");
    flush();

    TEST_ASSERT_NOT_NULL(strstr(buf, "ERROR"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "failed: oom"));
}

/* ========== file and line ========== */

static void test_log_includes_file_and_line(void) {
    nx_log_info("marker");
    flush();

    // should contain this filename
    TEST_ASSERT_NOT_NULL(strstr(buf, "test_io_log.c"));
}

/* ========== level filtering ========== */

static void test_level_filter_suppresses_debug(void) {
    nx_log_set_level(NX_LOG_INFO);

    nx_log_debug("should not appear");
    flush();

    TEST_ASSERT_EQUAL_STRING("", buf);
}

static void test_level_filter_allows_same_level(void) {
    nx_log_set_level(NX_LOG_WARN);

    nx_log_warn("visible");
    flush();

    TEST_ASSERT_NOT_NULL(strstr(buf, "visible"));
}

static void test_level_filter_allows_higher(void) {
    nx_log_set_level(NX_LOG_WARN);

    nx_log_error("also visible");
    flush();

    TEST_ASSERT_NOT_NULL(strstr(buf, "also visible"));
}

static void test_level_filter_suppresses_lower(void) {
    nx_log_set_level(NX_LOG_ERROR);

    nx_log_debug("no");
    nx_log_info("no");
    nx_log_warn("no");
    flush();

    TEST_ASSERT_EQUAL_STRING("", buf);
}

static void test_level_off_suppresses_all(void) {
    nx_log_set_level(NX_LOG_OFF);

    nx_log_debug("no");
    nx_log_info("no");
    nx_log_warn("no");
    nx_log_error("no");
    flush();

    TEST_ASSERT_EQUAL_STRING("", buf);
}

/* ========== get/set ========== */

static void test_get_set_level(void) {
    nx_log_set_level(NX_LOG_WARN);
    TEST_ASSERT_EQUAL(NX_LOG_WARN, nx_log_get_level());
}

static void test_get_stream(void) {
    TEST_ASSERT_EQUAL_PTR(mem, nx_log_get_stream());
}

static void test_default_stream_is_stderr(void) {
    nx_log_set_stream(nx_null);
    TEST_ASSERT_EQUAL_PTR(stderr, nx_log_get_stream());
}

/* ========== multiple messages ========== */

static void test_multiple_messages(void) {
    nx_log_info("first");
    nx_log_info("second");
    flush();

    TEST_ASSERT_NOT_NULL(strstr(buf, "first"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "second"));
}

/* ========== no args ========== */

static void test_log_no_format_args(void) {
    nx_log_info("plain message");
    flush();

    TEST_ASSERT_NOT_NULL(strstr(buf, "plain message"));
}

int main(void) {
    UNITY_BEGIN();

    /* basic output */
    RUN_TEST(test_log_debug_output);
    RUN_TEST(test_log_info_output);
    RUN_TEST(test_log_warn_output);
    RUN_TEST(test_log_error_output);

    /* file and line */
    RUN_TEST(test_log_includes_file_and_line);

    /* level filtering */
    RUN_TEST(test_level_filter_suppresses_debug);
    RUN_TEST(test_level_filter_allows_same_level);
    RUN_TEST(test_level_filter_allows_higher);
    RUN_TEST(test_level_filter_suppresses_lower);
    RUN_TEST(test_level_off_suppresses_all);

    /* get/set */
    RUN_TEST(test_get_set_level);
    RUN_TEST(test_get_stream);
    RUN_TEST(test_default_stream_is_stderr);

    /* misc */
    RUN_TEST(test_multiple_messages);
    RUN_TEST(test_log_no_format_args);

    return UNITY_END();
}
