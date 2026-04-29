#ifndef LOGGER_TYPES_H
#define LOGGER_TYPES_H
#include <stdint.h>
#include <stdbool.h>

enum { DYNORA_LOG_MESSAGE_MAX = 512 };

/* ===== LEVEL ===== */
enum DynoraLogLevel {
    DYNORA_LEVEL_DEBUG,
    DYNORA_LEVEL_INFO,
    DYNORA_LEVEL_WARNING,
    DYNORA_LEVEL_ERROR,
    DYNORA_LEVEL_FATAL,
};

/* ===== TYPES ===== */
typedef uint32_t DynoraLogCategory;

typedef struct DynoraLogEvent {
    uint64_t timestamp;   // tempo real (debug, profiling)
    uint64_t sequence;    // ordem dos logs
    const char* file;     // Arquivo onde ocorreu
    const char* function; // Função onde ocorreu
    uint32_t line;        // Linha do código
    uint32_t category;    // Categoria (bitmask)
    uint8_t level;        // Nível do log
    char message[DYNORA_LOG_MESSAGE_MAX];
    void* user_data; // Opcional para backends avançados
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
