#ifndef LOGGER_H
#define LOGGER_H
#include "logger/logger_types.h"

/* ===== API ===== */
void logger_init(DynoraLogLevel minimum_level,
    DynoraLogCategoryMask enabled_categories);
void logger_set_level(DynoraLogLevel minimum_level);
void logger_set_category_mask(DynoraLogCategoryMask enabled_categories);
bool logger_should_emit(DynoraLogCategory category, DynoraLogLevel level);
    
// IMPORTANT:
// logger_dispatch MUST NOT be called directly by user code.
// Use DYNORA_LOG macros instead.
void logger_dispatch(DynoraLogCategory category,
    DynoraLogLevel level,
    const char* file,
    uint32_t line,
    const char* function,
    const char* fmt,
    ...);

/* ===== MACROS ===== */
#define DYNORA_LOG(cat, level, fmt, ...)                                  \
    do {                                                                  \
        if (logger_should_emit((cat), (level)))                           \
            logger_dispatch((cat),                                        \
                (level),                                                  \
                __FILE__,                                                 \
                __LINE__,                                                 \
                __func__,                                                 \
                fmt __VA_OPT__(, ) __VA_ARGS__);                          \
    } while (0)

#define DYNORA_LOG_TRACE(cat, fmt, ...)                                   \
    DYNORA_LOG(cat, DYNORA_LEVEL_TRACE, fmt __VA_OPT__(, ) __VA_ARGS__)

#define DYNORA_LOG_DEBUG(cat, fmt, ...)                                   \
    DYNORA_LOG(cat, DYNORA_LEVEL_DEBUG, fmt __VA_OPT__(, ) __VA_ARGS__)

#define DYNORA_LOG_INFO(cat, fmt, ...)                                    \
    DYNORA_LOG(cat, DYNORA_LEVEL_INFO, fmt __VA_OPT__(, ) __VA_ARGS__)

#define DYNORA_LOG_WARNING(cat, fmt, ...)                                 \
    DYNORA_LOG(cat, DYNORA_LEVEL_WARNING, fmt __VA_OPT__(, ) __VA_ARGS__)

#define DYNORA_LOG_ERROR(cat, fmt, ...)                                   \
    DYNORA_LOG(cat, DYNORA_LEVEL_ERROR, fmt __VA_OPT__(, ) __VA_ARGS__)

#define DYNORA_LOG_FATAL(cat, fmt, ...)                                   \
    DYNORA_LOG(cat, DYNORA_LEVEL_FATAL, fmt __VA_OPT__(, ) __VA_ARGS__)

#endif /* LOGGER_H */
