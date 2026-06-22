#ifndef LOGGER_TYPES_H
#define LOGGER_TYPES_H
#include <stdint.h>
#include <stdbool.h>

enum { DYNORA_LOG_MESSAGE_MAX = 512, DYNORA_LOG_MAX_CATEGORIES = 64 };

/* ===== LEVEL ===== */
typedef enum DynoraLogLevel {
    DYNORA_LEVEL_TRACE = 0,
    DYNORA_LEVEL_DEBUG,
    DYNORA_LEVEL_INFO,
    DYNORA_LEVEL_WARNING,
    DYNORA_LEVEL_ERROR,
    DYNORA_LEVEL_FATAL,
    DYNORA_LEVEL_COUNT // Sentinel value (not a valid level)
} DynoraLogLevel;
_Static_assert(DYNORA_LEVEL_COUNT <= UINT8_MAX,
    "DynoraLogLevel must fit in uint8_t");

/* ===== CATEGORY ===== */
typedef enum DynoraLogCategory {
    DYNORA_LOG_CATEGORY_RENDER = 0,
    DYNORA_LOG_CATEGORY_AUDIO,
    DYNORA_LOG_CATEGORY_PHYSICS,
    DYNORA_LOG_CATEGORY_ECS,
    DYNORA_LOG_CATEGORY_IO,
    DYNORA_LOG_CATEGORY_GENERAL,

    DYNORA_LOG_CATEGORY_COUNT
} DynoraLogCategory;

_Static_assert(DYNORA_LOG_CATEGORY_COUNT <= DYNORA_LOG_MAX_CATEGORIES,
    "Too many log categories for uint64_t mask");

/* ===== CATEGORY MASK ===== */
typedef uint64_t DynoraLogCategoryMask;

#define DYNORA_LOG_CATEGORY_BIT(cat) ((DynoraLogCategoryMask)1ull << (cat))

#define DYNORA_LOG_CATEGORY_MASK_NONE ((DynoraLogCategoryMask)0ull)

#define DYNORA_LOG_CATEGORY_MASK_ALL                                      \
    ((DynoraLogCategoryMask)((1ull << DYNORA_LOG_CATEGORY_COUNT) - 1ull))

#define DYNORA_LOG_CATEGORY_RENDER_MASK                                   \
    DYNORA_LOG_CATEGORY_BIT(DYNORA_LOG_CATEGORY_RENDER)
#define DYNORA_LOG_CATEGORY_AUDIO_MASK                                    \
    DYNORA_LOG_CATEGORY_BIT(DYNORA_LOG_CATEGORY_AUDIO)
#define DYNORA_LOG_CATEGORY_PHYSICS_MASK                                  \
    DYNORA_LOG_CATEGORY_BIT(DYNORA_LOG_CATEGORY_PHYSICS)
#define DYNORA_LOG_CATEGORY_ECS_MASK                                      \
    DYNORA_LOG_CATEGORY_BIT(DYNORA_LOG_CATEGORY_ECS)
#define DYNORA_LOG_CATEGORY_IO_MASK                                       \
    DYNORA_LOG_CATEGORY_BIT(DYNORA_LOG_CATEGORY_IO)
#define DYNORA_LOG_CATEGORY_GENERAL_MASK                                  \
    DYNORA_LOG_CATEGORY_BIT(DYNORA_LOG_CATEGORY_GENERAL)

/* ===== EVENT ===== */
typedef struct DynoraLogEvent {
    uint64_t timestamp; // monotonic timestamp (ns)
    uint64_t sequence;
    const char* file;
    const char* function;
    char message[DYNORA_LOG_MESSAGE_MAX];
    uint32_t line;
    DynoraLogCategory category;
    uint8_t level;
} DynoraLogEvent;

#endif /* LOGGER_TYPES_H */
