#ifndef LOGGER_INTERNAL_H
#define LOGGER_INTERNAL_H
#include "logger/logger_types.h"

void logger_dispatch(DynoraLogCategory category,
    DynoraLogLevel level,
    const char* file,
    uint32_t line,
    const char* function,
    const char* fmt,
    ...);

#endif /* LOGGER_INTERNAL_H */
