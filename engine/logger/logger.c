#include "logger/logger_internal.h"
#include "logger/logger_result.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

enum { DYNORA_LOGGER_MAX_BACKENDS = 16 };

/* ==== BACKEND ==== */
typedef struct DynoraLoggerBackend {
    void (*write)(const DynoraLogEvent* event, void* user_data);
    void* user_data;
} DynoraLoggerBackend;

/* ==== LIFECYCLE === */
typedef enum DynoraLoggerLifecycle {
    DYNORA_LOGGER_STATE_UNINITIALIZED = 0,
    DYNORA_LOGGER_STATE_RUNNING,
} DynoraLoggerLifecycle;

/* ==== LOGGER STATE ==== */
// Current implementation is intentionally NOT thread-safe.
// Synchronization will be introduced in the async version.
typedef struct DynoraLoggerState {
    uint64_t sequence;
    DynoraLogCategoryMask enabled_categories;
    DynoraLoggerBackend backends[DYNORA_LOGGER_MAX_BACKENDS];
    DynoraLogLevel minimum_level;
    DynoraLoggerLifecycle lifecycle;
    uint32_t backend_count;

#ifndef NDEBUG
    bool is_dispatching;
#endif

} DynoraLoggerState;

static DynoraLoggerState d_logger_state;

/* ==== INTERNAL HELPERS ==== */
static inline bool logger_is_running(void) {
    return d_logger_state.lifecycle == DYNORA_LOGGER_STATE_RUNNING;
}

/* ==== TIME ==== */
#ifdef _WIN32

static inline uint64_t time_now_ns(void) {
    static LARGE_INTEGER freq = {0};

    if (freq.QuadPart == 0) {
        QueryPerformanceFrequency(&freq);
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    return ((uint64_t)now.QuadPart * 1000000000ULL) /
           (uint64_t)freq.QuadPart;
}

#else

static inline uint64_t time_now_ns(void) {
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ((uint64_t)ts.tv_sec * 1000000000ULL) + (uint64_t)ts.tv_nsec;
}

#endif

/* ==== FILTER ==== */
bool logger_should_emit(DynoraLogCategory category, DynoraLogLevel level) {

#ifndef NDEBUG
    assert(category < DYNORA_LOG_CATEGORY_COUNT);
    assert(level < DYNORA_LEVEL_COUNT);
#endif

    if (!logger_is_running()) {
        return false;
    }

    if (category >= DYNORA_LOG_CATEGORY_COUNT ||
        level >= DYNORA_LEVEL_COUNT) {
        return false;
    }

    if ((d_logger_state.enabled_categories &
            DYNORA_LOG_CATEGORY_BIT(category)) == 0) {
        return false;
    }

    return level >= d_logger_state.minimum_level;
}

/* ==== DISPATCH ==== */
void logger_dispatch(DynoraLogCategory category,
    DynoraLogLevel level,
    const char* file,
    uint32_t line,
    const char* function,
    const char* fmt,
    ...) {

#ifndef NDEBUG
    assert(category < DYNORA_LOG_CATEGORY_COUNT);
    assert(level < DYNORA_LEVEL_COUNT);
    assert(fmt != NULL);
    assert(!d_logger_state.is_dispatching);
#endif

    if (!logger_is_running()) {
        return;
    }

    if (category >= DYNORA_LOG_CATEGORY_COUNT ||
        level >= DYNORA_LEVEL_COUNT || fmt == NULL ||
        d_logger_state.backend_count == 0) {
        return;
    }

#ifndef NDEBUG
    d_logger_state.is_dispatching = true;
#endif

    DynoraLogEvent event;

    event.timestamp = time_now_ns();

    // NOTE:
    // Current implementation is NOT thread-safe.
    // Sequence will become atomic in async version.
    event.sequence = d_logger_state.sequence++;

    event.file = file;
    event.function = function;
    event.line = line;
    event.category = category;
    event.level = (uint8_t)level;

    va_list args;

    va_start(args, fmt);

    vsnprintf(event.message, DYNORA_LOG_MESSAGE_MAX, fmt, args);

    // Message may be truncated.
    event.message[DYNORA_LOG_MESSAGE_MAX - 1] = '\0';

    va_end(args);

    // WARNING:
    // Event memory is only valid during backend write call.
    // Backends must NOT store pointers to event fields.
    // If persistence is required, data must be copied.

    for (uint32_t i = 0; i < d_logger_state.backend_count; i++) {

        const DynoraLoggerBackend* backend = &d_logger_state.backends[i];

        if (backend->write) {
            backend->write(&event, backend->user_data);
        }
    }

#ifndef NDEBUG
    d_logger_state.is_dispatching = false;
#endif
}

/* ==== API ==== */
void logger_init(DynoraLogLevel minimum_level,
    DynoraLogCategoryMask enabled_categories) {

#ifndef NDEBUG
    assert(!d_logger_state.is_dispatching);
    assert(minimum_level < DYNORA_LEVEL_COUNT);
#endif

    if (minimum_level >= DYNORA_LEVEL_COUNT) {
        // Release fallback for invalid enum values.
        minimum_level = DYNORA_LEVEL_INFO;
    }

    memset(&d_logger_state, 0, sizeof(d_logger_state));

    d_logger_state.minimum_level = minimum_level;
    d_logger_state.enabled_categories = enabled_categories;
    d_logger_state.lifecycle = DYNORA_LOGGER_STATE_RUNNING;
}

void logger_set_level(DynoraLogLevel minimum_level) {

#ifndef NDEBUG
    assert(logger_is_running());
    assert(minimum_level < DYNORA_LEVEL_COUNT);
#endif

    if (!logger_is_running()) {
        return;
    }

    if (minimum_level >= DYNORA_LEVEL_COUNT) {
        return;
    }

    d_logger_state.minimum_level = minimum_level;
}

void logger_set_category_mask(DynoraLogCategoryMask enabled_categories) {

#ifndef NDEBUG
    assert(logger_is_running());
#endif

    if (!logger_is_running()) {
        return;
    }
    d_logger_state.enabled_categories = enabled_categories;
}

/* ==== BACKEND REGISTRATION ===== */
DynoraLoggerResult logger_add_backend(
    void (*write)(const DynoraLogEvent*, void*), void* user_data) {

#ifndef NDEBUG
    assert(!d_logger_state.is_dispatching);
#endif

    if (!logger_is_running()) {
        return DYNORA_LOGGER_ERROR_NOT_INITIALIZED;
    }

    if (!write) {
        return DYNORA_LOGGER_ERROR_NULL_BACKEND;
    }

    if (d_logger_state.backend_count >= DYNORA_LOGGER_MAX_BACKENDS) {
        return DYNORA_LOGGER_ERROR_BACKEND_LIMIT;
    }

    DynoraLoggerBackend backend = {
        .write = write,
        .user_data = user_data,
    };

    d_logger_state.backends[d_logger_state.backend_count++] = backend;

    return DYNORA_LOGGER_SUCCESS;
}
