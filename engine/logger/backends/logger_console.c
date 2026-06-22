/* ===== CONSOLE BACKEND ===== */

#include "logger/logger_backend.h"
#include <stddef.h>
#include <stdio.h>

static void console_write(const DynoraLogEvent* event, void* user_data) {
    (void)user_data;
    printf("[%llu][#%llu] %s:%u %s(): %s\n",
        (unsigned long long)event->timestamp,
        (unsigned long long)event->sequence,
        event->file,
        event->line,
        event->function,
        event->message);
}

void backend_console_init(void) {
    logger_add_backend(console_write, NULL);
}
