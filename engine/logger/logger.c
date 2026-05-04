#include "logger/logger_internal.h"
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

enum { DYNORA_LOGGER_MAX_BACKENDS = 16 };

/* ===== BACKEND ===== */
typedef struct Backend {
    void (*write)(const DynoraLogEvent* event, void* user_data);
    void* user_data;
} Backend;

/* ===== LOGGER STATE ===== */
typedef struct DynoraLoggerState {
    uint64_t counter;
    enum DynoraLogLevel level;
    Backend backends[DYNORA_LOGGER_MAX_BACKENDS];
    uint32_t backend_count;
    DynoraLogCategory category_mask;
} DynoraLoggerState;

// isn't thread-safe
static DynoraLoggerState d_logger_state;

/* ===== TIME ===== */
#ifdef _WIN32
static inline uint64_t time_now_ns(void) {
    static LARGE_INTEGER freq = {0};

    if (freq.QuadPart == 0) {
        QueryPerformanceFrequency(&freq);
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    return (uint64_t)((now.QuadPart * 1000000000ULL) / freq.QuadPart);
}
#else
static inline uint64_t time_now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}
#endif

/* =====  CHECK ===== */
bool logger_should_emit(DynoraLogCategory category,
    enum DynoraLogLevel level) {
    return (category & d_logger_state.category_mask) &&
           (level >= d_logger_state.level);
}

/* ===== DISPATCH ===== */
void logger_dispatch(DynoraLogCategory category,
    enum DynoraLogLevel level,
    const char* file,
    uint32_t line,
    const char* function,
    const char* fmt,
    ...) {

    assert(level < DYNORA_LEVEL_COUNT);
    assert(fmt != NULL);

    if (level >= DYNORA_LEVEL_COUNT || !fmt)
        return;

    if (d_logger_state.backend_count == 0)
        return;

    DynoraLogEvent event;
    event.timestamp = time_now_ns();

    // NOTE: Not thread-safe (will become atomic in async version)
    event.sequence = d_logger_state.counter++;

    event.file = file;
    event.function = function;
    event.line = line;
    event.category = category;
    event.level = (uint8_t)level;

    va_list args;
    va_start(args, fmt);
    vsnprintf(event.message, DYNORA_LOG_MESSAGE_MAX, fmt, args);
    // message may be truncated
    event.message[DYNORA_LOG_MESSAGE_MAX - 1] = '\0';
    va_end(args);
    event.user_data = NULL;

    // WARNING:
    // Event memory is only valid during backend write call.
    // Backends must NOT store pointers to event fields.
    // If persistence is required, data must be copied.
    for (uint32_t i = 0; i < d_logger_state.backend_count; i++) {
        const Backend* backend = &d_logger_state.backends[i];
        if (backend->write)
            backend->write(&event, backend->user_data);
    }
}

/* ===== API ===== */
void logger_init(enum DynoraLogLevel level, DynoraLogCategory category) {
    d_logger_state.level = level;
    d_logger_state.backend_count = 0;
    d_logger_state.category_mask = category;
    d_logger_state.counter = 0;
}

void logger_set_level(enum DynoraLogLevel level) {
    d_logger_state.level = level;
}

void logger_set_category_mask(DynoraLogCategory category) {
    d_logger_state.category_mask = category;
}

/* ===== ADD BACKEND ===== */
void logger_add_backend(void (*write)(const DynoraLogEvent*, void*),
    void* user_data) {

    if (!write ||
        d_logger_state.backend_count >= DYNORA_LOGGER_MAX_BACKENDS)
        return;

    Backend backend = {.write = write, .user_data = user_data};
    d_logger_state.backends[d_logger_state.backend_count++] = backend;
}
