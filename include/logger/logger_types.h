#ifndef LOGGER_TYPES_H
#define LOGGER_TYPES_H
#include <stdint.h>
#include <stdbool.h>

enum { DYNORA_LOG_MESSAGE_MAX = 512 };

/* ===== LEVEL ===== */
enum DynoraLogLevel {
    DYNORA_LEVEL_DEBUG = 0,
    DYNORA_LEVEL_INFO,
    DYNORA_LEVEL_WARNING,
    DYNORA_LEVEL_ERROR,
    DYNORA_LEVEL_FATAL,
    DYNORA_LEVEL_COUNT // Sentinel value (not a valid level)
};
_Static_assert(DYNORA_LEVEL_COUNT <= 255, "LogLevel must fit in uint8_t");

/* ===== TYPES ===== */
typedef uint32_t DynoraLogCategory;

typedef struct DynoraLogEvent {
    uint64_t timestamp; // monotonic timestamp (ns)
    uint64_t sequence;
    const char* file;
    const char* function;
    void* user_data;
    char message[DYNORA_LOG_MESSAGE_MAX];
    uint32_t line;
    DynoraLogCategory category;
    uint8_t level;
} DynoraLogEvent;

// Move defines category to logger_h after adding new system backends
/* ===== CATEGORY ===== */
#define DYNORA_LOG_RENDER ((DynoraLogCategory)(1u << 0))
#define DYNORA_LOG_AUDIO ((DynoraLogCategory)(1u << 1))
#define DYNORA_LOG_PHYSICS ((DynoraLogCategory)(1u << 2))
#define DYNORA_LOG_ECS ((DynoraLogCategory)(1u << 3))
#define DYNORA_LOG_IO ((DynoraLogCategory)(1u << 4))
#define DYNORA_LOG_GENERAL ((DynoraLogCategory)(1u << 5))

// Use just on function logger_init()
#define DYNORA_LOG_NONE ((DynoraLogCategory)0)
#define DYNORA_LOG_ALL                                                    \
    (DYNORA_LOG_RENDER | DYNORA_LOG_AUDIO | DYNORA_LOG_PHYSICS |          \
        DYNORA_LOG_ECS | DYNORA_LOG_IO | DYNORA_LOG_GENERAL)

#endif /* LOGGER_TYPES_H */
