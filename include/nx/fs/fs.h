#pragma once

#include <stdio.h>

#include "nx/core/type.h"
#include "nx/core/status.h"
#include "nx/core/result.h"
#include "nx/string/str.h"
#include "nx/mem/alloc.h"

/* Contract:
 * - Path must be a valid null-terminated C string.
 * - Read functions allocate memory; caller owns the result.
 * - Write functions create or truncate. Append functions create or append.
 * - All I/O errors are reported as NX_STATUS_IO_ERROR.
 * - Pure standard C (fopen/fread/fwrite/fseek/ftell). No platform-specific APIs.
 */

/* ========== info ========== */

/// returns true if path can be opened for reading.
nx_bool nx_fs_exists(const nx_char *path);

/// returns file size in bytes, or -1 on error.
nx_isize nx_fs_file_size(const nx_char *path);

/* ========== read ========== */

/// reads entire file into a new nx_Str. caller owns the result.
nx_StrRes nx_fs_read_to_str(const nx_char *path);
nx_StrRes nx_fs_read_to_str_a(const nx_char *path, nx_Al *al);

/// reads entire file into a byte buffer. caller must free *out via al.
/// on success, *out points to the data and *out_len is the byte count.
/// on error, *out is null and *out_len is 0.
nx_Status nx_fs_read_bytes(const nx_char *path, nx_byte **out, nx_usize *out_len, nx_Al *al);

/* ========== write ========== */

/// writes len bytes to file (creates or truncates).
nx_Status nx_fs_write(const nx_char *path, const void *data, nx_usize len);

/// writes null-terminated string to file (creates or truncates).
nx_Status nx_fs_write_str(const nx_char *path, const nx_char *str);

/// appends len bytes to file (creates if doesn't exist).
nx_Status nx_fs_append(const nx_char *path, const void *data, nx_usize len);
