#include "nx/data_structure/arr.h"

#include "nx/core/print.h"
#include "nx/core/span.h"
#include "nx/core/panic.h"

#include "nx/mem/ptr.h"

#include <stdio.h>
#include <stdlib.h>

static nx_arr *arr_new_or_die(nx_usize len, nx_usize elem_size);

static void demo_lifetime(void);
static void demo_copy_move(void);
static void demo_info(void);
static void demo_access(void);
static void demo_mods(void);
static void demo_to_span(void);

int main(void) {
    demo_lifetime();
    demo_copy_move();
    demo_info();
    demo_access();
    demo_mods();
    demo_to_span();

    return EXIT_SUCCESS;
}

static nx_arr *arr_new_or_die(nx_usize len, nx_usize elem_size) {
    nx_arr_res res = nx_arr_new(len, elem_size);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprint(stderr, NX_RES_GET_ERR(res));
        exit(EXIT_FAILURE);
    }
    return NX_RES_GET_VAL(res);
}

static void demo_lifetime(void) {
    /*
    nx_arr_res nx_arr_new(nx_usize len, nx_usize elem_size);
    void nx_arr_drop(nx_arr *self);
    */

    const nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 5);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprint(stderr, NX_RES_GET_ERR(res));
        exit(EXIT_FAILURE);
    }
    nx_arr *arr = NX_RES_GET_VAL(res);

    nx_arr_drop(arr);
}

static void demo_copy_move(void) {
    // TODO: not completed
    /*
    nx_arr_res nx_arr_copy(const nx_arr *src);
    nx_arr *nx_arr_move(nx_arr **src);
    nx_status nx_arr_copy_assign(nx_arr *self, const nx_arr *src);
    void nx_arr_move_assign(nx_arr *self, nx_arr *src);
    */

    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    nx_arr_res res1 = nx_arr_copy(arr);
    if (!NX_RES_IS_OK(res1)) {
        nx_status_fprint(stderr, NX_RES_GET_ERR(res1));
        exit(EXIT_FAILURE);
    }
    nx_arr *arr1 = NX_RES_GET_VAL(res1);

    nx_arr *arr2 = nx_arr_move(&arr);


    nx_arr_drop(arr1);
    nx_arr_drop(arr2);
    nx_arr_drop(arr);
}

static void demo_info(void) {
    /*
    nx_usize nx_arr_len(const nx_arr *self);
    nx_bool nx_arr_empty(const nx_arr *self);
    nx_usize nx_arr_elem_size(const nx_arr *self);
    */

    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    const nx_usize len = nx_arr_len(arr);
    NX_VERIFY(len == 5);

    const nx_bool is_empty = nx_arr_empty(arr);
    NX_VERIFY(is_empty == false);

    const nx_usize elem_size = nx_arr_elem_size(arr);
    NX_VERIFY(elem_size == sizeof(nx_i32));

    nx_arr_drop(arr);
}

static void demo_access(void) {
    /*
    void *nx_arr_get(nx_arr *self, nx_usize idx);
    const void *nx_arr_get_c(const nx_arr *self, nx_usize idx);
    void nx_arr_set(nx_arr *self, nx_usize idx, const void *elem);
    void *nx_arr_data(nx_arr *self);
    const void *nx_arr_data_c(const nx_arr *self);
    */

    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr, i, (nx_i32) i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        const nx_i32 val = (nx_i32) i;
        const nx_i32 get_val = *NX_ARR_GET_AS(nx_i32, arr, i);
        const nx_i32 get_val_c = *NX_ARR_GET_AS_C(nx_i32, arr, i);

        NX_VERIFY(get_val == val);
        NX_VERIFY(get_val_c == val);
    }

    const nx_usize elem_size = nx_arr_elem_size(arr);

    void *data = nx_arr_data(arr);
    const void *data_c = nx_arr_data_c(arr);

    *(nx_i32 *) nx_byte_offset(data, elem_size, 0) = 42;

    NX_VERIFY(*(nx_i32 *) nx_arr_get(arr, 0) == 42);

    for (nx_usize i = 1; i < nx_arr_len(arr); ++i) {
        NX_VERIFY(*(nx_i32 *) nx_byte_offset(data, elem_size, i) == *NX_ARR_GET_AS(nx_i32, arr, i));
        NX_VERIFY(*(nx_i32 *) nx_byte_offset_c(data_c, elem_size, i) == *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void demo_mods(void) {
    /*
    void nx_arr_swap(nx_arr *a, nx_arr *b);
    */

    const nx_usize len1 = 5, len2 = 3;

    nx_arr *arr1 = arr_new_or_die(len1, sizeof(nx_i32));
    nx_arr *arr2 = arr_new_or_die(len2, sizeof(nx_i32));

    for (nx_usize i = 0; i < len1; ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr1, i, (nx_i32) (i * i));
    }

    for (nx_usize i = 0; i < len2; ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr2, i, (nx_i32) (i * i * i));
    }

    nx_arr_swap(arr1, arr2);

    NX_VERIFY(nx_arr_len(arr1) == len2);
    NX_VERIFY(nx_arr_len(arr2) == len1);

    for (nx_usize i = 0; i < nx_arr_len(arr1); ++i) {
        NX_VERIFY(*NX_ARR_GET_AS_C(nx_i32, arr1, i) == (nx_i32) (i * i * i));
    }

    for (nx_usize i = 0; i < nx_arr_len(arr2); ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr2, i, (nx_i32) (i * i * i));
    }

    nx_arr_drop(arr1);
    nx_arr_drop(arr2);
}

static void demo_to_span(void) {
    // TODO: not completed

    /*
    nx_span nx_arr_to_span(nx_arr *self);
    nx_cspan nx_arr_to_cspan(const nx_arr *self);
    */
}
