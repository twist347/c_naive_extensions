#include "nx/io/log.h"

#include <stdarg.h>

#include "nx/core/assert.h"

static nx_LogLevel g_level = NX_LOG_DEBUG;
static FILE *g_stream = nx_null; // null means stderr (resolved lazily)

static const nx_char *level_str(nx_LogLevel level);

void nx_log_set_level(nx_LogLevel level) {
    g_level = level;
}

nx_LogLevel nx_log_get_level(void) {
    return g_level;
}

void nx_log_set_stream(FILE *stream) {
    g_stream = stream;
}

FILE *nx_log_get_stream(void) {
    return g_stream ? g_stream : stderr;
}

void nx_log_(nx_LogLevel level, const nx_char *file, nx_i32 line, const nx_char *fmt, ...) {
    NX_ASSERT(fmt);

    if (level < g_level) {
        return;
    }

    FILE *out = g_stream ? g_stream : stderr;

    fprintf(out, "[%-5s] %s:%d: ", level_str(level), file, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);

    fputc('\n', out);
}

static const nx_char *level_str(nx_LogLevel level) {
    switch (level) {
        case NX_LOG_DEBUG: return "DEBUG";
        case NX_LOG_INFO: return "INFO";
        case NX_LOG_WARN: return "WARN";
        case NX_LOG_ERROR: return "ERROR";
        default: return "?";
    }
}
