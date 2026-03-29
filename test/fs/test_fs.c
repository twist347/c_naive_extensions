#include "unity.h"

#include <stdio.h>
#include <string.h>

#include "nx/fs/fs.h"
#include "nx/mem/alloc_libc.h"

static const nx_char *TMP_FILE = "/tmp/nx_test_fs.tmp";

void setUp(void) {
    remove(TMP_FILE);
}

void tearDown(void) {
    remove(TMP_FILE);
}

/* ========== exists ========== */

static void test_exists_nonexistent(void) {
    TEST_ASSERT_FALSE(nx_fs_exists("/tmp/nx_test_fs_nonexistent_12345.tmp"));
}

static void test_exists_after_write(void) {
    nx_Status st = nx_fs_write_str(TMP_FILE, "hello");
    TEST_ASSERT_EQUAL(NX_STATUS_OK, st);
    TEST_ASSERT_TRUE(nx_fs_exists(TMP_FILE));
}

/* ========== file_size ========== */

static void test_file_size_nonexistent(void) {
    TEST_ASSERT_EQUAL(-1, nx_fs_file_size("/tmp/nx_test_fs_nonexistent_12345.tmp"));
}

static void test_file_size_empty(void) {
    nx_fs_write(TMP_FILE, "", 0);
    TEST_ASSERT_EQUAL(0, nx_fs_file_size(TMP_FILE));
}

static void test_file_size_with_content(void) {
    const nx_char *text = "hello world";
    nx_fs_write_str(TMP_FILE, text);
    TEST_ASSERT_EQUAL((nx_isize) strlen(text), nx_fs_file_size(TMP_FILE));
}

/* ========== write + read_to_str ========== */

static void test_write_and_read(void) {
    const nx_char *text = "hello, nx/fs!";
    nx_Status st = nx_fs_write_str(TMP_FILE, text);
    TEST_ASSERT_EQUAL(NX_STATUS_OK, st);

    nx_StrRes res = nx_fs_read_to_str(TMP_FILE);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_Str *str = NX_RES_UNWRAP(res);
    TEST_ASSERT_EQUAL(strlen(text), nx_str_len(str));
    TEST_ASSERT_EQUAL_STRING(text, nx_str_cstr(str));

    nx_str_drop(str);
}

static void test_write_binary(void) {
    const nx_byte data[] = {0x00, 0x01, 0xFF, 0x7F, 0x80};
    nx_Status st = nx_fs_write(TMP_FILE, data, sizeof(data));
    TEST_ASSERT_EQUAL(NX_STATUS_OK, st);

    TEST_ASSERT_EQUAL((nx_isize) sizeof(data), nx_fs_file_size(TMP_FILE));
}

static void test_write_empty(void) {
    nx_Status st = nx_fs_write(TMP_FILE, nx_null, 0);
    TEST_ASSERT_EQUAL(NX_STATUS_OK, st);
    TEST_ASSERT_EQUAL(0, nx_fs_file_size(TMP_FILE));
}

static void test_write_truncates(void) {
    nx_fs_write_str(TMP_FILE, "long content here");
    nx_fs_write_str(TMP_FILE, "short");

    nx_StrRes res = nx_fs_read_to_str(TMP_FILE);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_Str *str = NX_RES_UNWRAP(res);
    TEST_ASSERT_EQUAL_STRING("short", nx_str_cstr(str));

    nx_str_drop(str);
}

static void test_read_empty_file(void) {
    nx_fs_write(TMP_FILE, nx_null, 0);

    nx_StrRes res = nx_fs_read_to_str(TMP_FILE);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_Str *str = NX_RES_UNWRAP(res);
    TEST_ASSERT_EQUAL(0, nx_str_len(str));
    TEST_ASSERT_EQUAL_STRING("", nx_str_cstr(str));

    nx_str_drop(str);
}

static void test_read_nonexistent(void) {
    nx_StrRes res = nx_fs_read_to_str("/tmp/nx_test_fs_nonexistent_12345.tmp");
    TEST_ASSERT_TRUE(NX_RES_IS_ERR(res));
    TEST_ASSERT_EQUAL(NX_STATUS_IO_ERROR, NX_RES_ERR(res));
}

/* ========== append ========== */

static void test_append_creates_file(void) {
    nx_Status st = nx_fs_append(TMP_FILE, "hello", 5);
    TEST_ASSERT_EQUAL(NX_STATUS_OK, st);

    nx_StrRes res = nx_fs_read_to_str(TMP_FILE);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_Str *str = NX_RES_UNWRAP(res);
    TEST_ASSERT_EQUAL_STRING("hello", nx_str_cstr(str));

    nx_str_drop(str);
}

static void test_append_to_existing(void) {
    nx_fs_write_str(TMP_FILE, "hello");
    nx_fs_append(TMP_FILE, " world", 6);

    nx_StrRes res = nx_fs_read_to_str(TMP_FILE);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_Str *str = NX_RES_UNWRAP(res);
    TEST_ASSERT_EQUAL_STRING("hello world", nx_str_cstr(str));

    nx_str_drop(str);
}

static void test_append_multiple(void) {
    for (int i = 0; i < 3; ++i) {
        nx_fs_append(TMP_FILE, "ab", 2);
    }

    nx_StrRes res = nx_fs_read_to_str(TMP_FILE);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_Str *str = NX_RES_UNWRAP(res);
    TEST_ASSERT_EQUAL_STRING("ababab", nx_str_cstr(str));
    TEST_ASSERT_EQUAL(6, nx_str_len(str));

    nx_str_drop(str);
}

/* ========== read_bytes ========== */

static void test_read_bytes_basic(void) {
    const nx_byte data[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x42};
    nx_fs_write(TMP_FILE, data, sizeof(data));

    nx_byte *out = nx_null;
    nx_usize out_len = 0;
    nx_Status st = nx_fs_read_bytes(TMP_FILE, &out, &out_len, nx_al_libc_default_g());

    TEST_ASSERT_EQUAL(NX_STATUS_OK, st);
    TEST_ASSERT_EQUAL(sizeof(data), out_len);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_EQUAL_MEMORY(data, out, sizeof(data));

    nx_al_dealloc(nx_al_libc_default_g(), out, out_len);
}

static void test_read_bytes_empty(void) {
    nx_fs_write(TMP_FILE, nx_null, 0);

    nx_byte *out = nx_null;
    nx_usize out_len = 0;
    nx_Status st = nx_fs_read_bytes(TMP_FILE, &out, &out_len, nx_al_libc_default_g());

    TEST_ASSERT_EQUAL(NX_STATUS_OK, st);
    TEST_ASSERT_EQUAL(0, out_len);
    TEST_ASSERT_NULL(out);
}

static void test_read_bytes_nonexistent(void) {
    nx_byte *out = nx_null;
    nx_usize out_len = 0;
    nx_Status st = nx_fs_read_bytes("/tmp/nx_test_fs_nonexistent_12345.tmp", &out, &out_len, nx_al_libc_default_g());

    TEST_ASSERT_EQUAL(NX_STATUS_IO_ERROR, st);
    TEST_ASSERT_NULL(out);
    TEST_ASSERT_EQUAL(0, out_len);
}

static void test_read_bytes_contains_null(void) {
    const nx_byte data[] = {0x01, 0x00, 0x02, 0x00, 0x03};
    nx_fs_write(TMP_FILE, data, sizeof(data));

    nx_byte *out = nx_null;
    nx_usize out_len = 0;
    nx_Status st = nx_fs_read_bytes(TMP_FILE, &out, &out_len, nx_al_libc_default_g());

    TEST_ASSERT_EQUAL(NX_STATUS_OK, st);
    TEST_ASSERT_EQUAL(sizeof(data), out_len);
    TEST_ASSERT_EQUAL_MEMORY(data, out, sizeof(data));

    nx_al_dealloc(nx_al_libc_default_g(), out, out_len);
}

/* ========== status_to_str ========== */

static void test_io_error_str(void) {
    const nx_char *s = nx_status_to_str(NX_STATUS_IO_ERROR);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(strstr(s, "IO_ERROR"));
}

int main(void) {
    UNITY_BEGIN();

    /* exists */
    RUN_TEST(test_exists_nonexistent);
    RUN_TEST(test_exists_after_write);

    /* file_size */
    RUN_TEST(test_file_size_nonexistent);
    RUN_TEST(test_file_size_empty);
    RUN_TEST(test_file_size_with_content);

    /* write + read */
    RUN_TEST(test_write_and_read);
    RUN_TEST(test_write_binary);
    RUN_TEST(test_write_empty);
    RUN_TEST(test_write_truncates);
    RUN_TEST(test_read_empty_file);
    RUN_TEST(test_read_nonexistent);

    /* append */
    RUN_TEST(test_append_creates_file);
    RUN_TEST(test_append_to_existing);
    RUN_TEST(test_append_multiple);

    /* read_bytes */
    RUN_TEST(test_read_bytes_basic);
    RUN_TEST(test_read_bytes_empty);
    RUN_TEST(test_read_bytes_nonexistent);
    RUN_TEST(test_read_bytes_contains_null);

    /* status */
    RUN_TEST(test_io_error_str);

    return UNITY_END();
}
