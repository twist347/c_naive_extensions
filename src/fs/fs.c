#include "nx/fs/fs.h"

#include <string.h>

#include "nx/core/assert.h"
#include "nx/mem/alloc_libc.h"

static nx_isize get_file_size(FILE *f);

nx_bool nx_fs_exists(const nx_char *path) {
    NX_ASSERT(path);

    FILE *f = fopen(path, "rb");
    if (!f) {
        return false;
    }
    fclose(f);
    return true;
}

nx_isize nx_fs_file_size(const nx_char *path) {
    NX_ASSERT(path);

    FILE *f = fopen(path, "rb");
    if (!f) {
        return -1;
    }

    const nx_isize size = get_file_size(f);
    fclose(f);
    return size;
}

nx_StrRes nx_fs_read_to_str(const nx_char *path) {
    return nx_fs_read_to_str_a(path, nx_al_libc_default_g());
}

nx_StrRes nx_fs_read_to_str_a(const nx_char *path, nx_Al *al) {
    NX_ASSERT(path);
    NX_ASSERT(al);

    FILE *f = fopen(path, "rb");
    if (!f) {
        return NX_RES_NEW_ERR(nx_StrRes, NX_STATUS_IO_ERROR);
    }

    const nx_isize size = get_file_size(f);
    if (size < 0) {
        fclose(f);
        return NX_RES_NEW_ERR(nx_StrRes, NX_STATUS_IO_ERROR);
    }

    const nx_usize len = (nx_usize) size;

    nx_StrRes res = nx_str_new_p((nx_StrParams){
        .len = len,
        .cap = len,
        .al = al,
    });

    if (!NX_RES_IS_OK(res)) {
        fclose(f);
        return res;
    }

    nx_Str *str = NX_RES_UNWRAP(res);

    if (len > 0) {
        const nx_usize read = fread(nx_str_data(str), 1, len, f);
        if (read != len) {
            fclose(f);
            nx_str_drop(str);
            return NX_RES_NEW_ERR(nx_StrRes, NX_STATUS_IO_ERROR);
        }
    }

    fclose(f);
    return NX_RES_NEW_OK(nx_StrRes, str);
}

nx_Status nx_fs_read_bytes(const nx_char *path, nx_byte **out, nx_usize *out_len, nx_Al *al) {
    NX_ASSERT(path);
    NX_ASSERT(out);
    NX_ASSERT(out_len);
    NX_ASSERT(al);

    *out = nx_null;
    *out_len = 0;

    FILE *f = fopen(path, "rb");
    if (!f) {
        return NX_STATUS_IO_ERROR;
    }

    const nx_isize size = get_file_size(f);
    if (size < 0) {
        fclose(f);
        return NX_STATUS_IO_ERROR;
    }

    const nx_usize len = (nx_usize) size;

    if (len == 0) {
        fclose(f);
        return NX_STATUS_OK;
    }

    nx_byte *buf = nx_al_alloc(al, len);
    if (!buf) {
        fclose(f);
        return NX_STATUS_OUT_OF_MEMORY;
    }

    const nx_usize read = fread(buf, 1, len, f);
    if (read != len) {
        fclose(f);
        nx_al_dealloc(al, buf, len);
        return NX_STATUS_IO_ERROR;
    }

    fclose(f);
    *out = buf;
    *out_len = len;
    return NX_STATUS_OK;
}

nx_Status nx_fs_write(const nx_char *path, const void *data, nx_usize len) {
    NX_ASSERT(path);
    NX_ASSERT(data || len == 0);

    FILE *f = fopen(path, "wb");
    if (!f) {
        return NX_STATUS_IO_ERROR;
    }

    if (len > 0) {
        const nx_usize written = fwrite(data, 1, len, f);
        if (written != len) {
            fclose(f);
            return NX_STATUS_IO_ERROR;
        }
    }

    fclose(f);
    return NX_STATUS_OK;
}

nx_Status nx_fs_write_str(const nx_char *path, const nx_char *str) {
    NX_ASSERT(path);
    NX_ASSERT(str);

    return nx_fs_write(path, str, strlen(str));
}

nx_Status nx_fs_append(const nx_char *path, const void *data, nx_usize len) {
    NX_ASSERT(path);
    NX_ASSERT(data || len == 0);

    FILE *f = fopen(path, "ab");
    if (!f) {
        return NX_STATUS_IO_ERROR;
    }

    if (len > 0) {
        const nx_usize written = fwrite(data, 1, len, f);
        if (written != len) {
            fclose(f);
            return NX_STATUS_IO_ERROR;
        }
    }

    fclose(f);
    return NX_STATUS_OK;
}

static nx_isize get_file_size(FILE *f) {
    if (fseek(f, 0, SEEK_END) != 0) {
        return -1;
    }

    const long size = ftell(f);
    if (size < 0) {
        return -1;
    }

    if (fseek(f, 0, SEEK_SET) != 0) {
        return -1;
    }

    return (nx_isize) size;
}
