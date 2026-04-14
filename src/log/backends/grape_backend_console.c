/* ===== CONSOLE BACKEND ===== */

#include "log/grape_system_log_backend.h"
#include <stddef.h>   // NULL
#include <stdio.h>

/*
void dummy_write(const GrapeLogEvent* e, void* u) {
    (void)e;
    (void)u;
}
*/

void console_write(const GrapeLogEvent* event, void* user_data) {
    (void)user_data;
    printf("[%llu][#%llu] %s:%u %s(): %s\n",
        (unsigned long long)event->timestamp,
        (unsigned long long)event->sequence,
        event->file,
        event->line,
        event->function,
        event->message);
}

void grape_backend_console_init(void) {
    grape_log_add_backend(console_write, NULL);
}




/*
void grape_backend_console_init(void) {
    grape_log_add_backend(console_write, NULL);
}

*/