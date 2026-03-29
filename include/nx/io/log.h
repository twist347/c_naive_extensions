#pragma once

#include <stdio.h>

#include "nx/core/type.h"

/* Log levels, ordered by severity. */
typedef enum {
    NX_LOG_DEBUG = 0,
    NX_LOG_INFO = 1,
    NX_LOG_WARN = 2,
    NX_LOG_ERROR = 3,
    NX_LOG_OFF = 4,
} nx_LogLevel;

/// Set the minimum log level. Messages below this level are suppressed.
/// Default: NX_LOG_DEBUG (everything shown).
void nx_log_set_level(nx_LogLevel level);

/// Get the current minimum log level.
nx_LogLevel nx_log_get_level(void);

/// Set the output stream. Default: stderr.
void nx_log_set_stream(FILE *stream);

/// Get the current output stream.
FILE *nx_log_get_stream(void);

/// Core log function. Prefer the macros below.
void nx_log_(nx_LogLevel level, const nx_char *file, nx_i32 line, const nx_char *fmt, ...);

/* ========== compile-time filtering ========== */

/// Define NX_LOG_MIN_LEVEL before including this header to strip
/// messages below that level at compile time. Stripped calls produce no code.
#ifndef NX_LOG_MIN_LEVEL
    #define NX_LOG_MIN_LEVEL NX_LOG_DEBUG
#endif

/* ========== macros ========== */

#if NX_LOG_MIN_LEVEL <= NX_LOG_DEBUG
    #define nx_log_debug(...)  nx_log_(NX_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define nx_log_debug(...)  ((void) 0)
#endif

#if NX_LOG_MIN_LEVEL <= NX_LOG_INFO
    #define nx_log_info(...)   nx_log_(NX_LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#else
    #define nx_log_info(...)   ((void) 0)
#endif

#if NX_LOG_MIN_LEVEL <= NX_LOG_WARN
    #define nx_log_warn(...)   nx_log_(NX_LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#else
    #define nx_log_warn(...)   ((void) 0)
#endif

#if NX_LOG_MIN_LEVEL <= NX_LOG_ERROR
    #define nx_log_error(...)  nx_log_(NX_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define nx_log_error(...)  ((void) 0)
#endif
