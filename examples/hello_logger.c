#include "logger/logger.h"
#include "logger/logger_types.h"

/* ===== BACKEND PROTOTYPES ===== */
void backend_console_init(void);

/* ===== EXAMPLE SYSTEMS ===== */
static void renderer_init(void);
static void audio_init(void);
static void physics_step(void);
static void ecs_spawn_player(void);
static void save_game(const char* path);

/* ===== MAIN ===== */
int main(void) {
    /*
        Initialize logger:
        - minimum level: DEBUG
        - all categories enabled
    */
    logger_init(DYNORA_LEVEL_TRACE, DYNORA_LOG_CATEGORY_MASK_ALL);

    /*
        Register console backend.
        Additional backends could also be registered:
        - file backend
        - remote telemetry backend
        - editor backend
        - in-memory capture backend
    */
    backend_console_init();

    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_GENERAL,
        "Dynora Engine starting...");

    /* ===== SYSTEM INITIALIZATION ===== */
    renderer_init();
    audio_init();

    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_GENERAL,
        "All engine systems initialized successfully");

    DYNORA_LOG_TRACE(DYNORA_LOG_CATEGORY_IO,
        "Frame 452: Key 'Space' state: Pressed");

    /* ===== GAMEPLAY SIMULATION ===== */
    for (uint32_t frame = 0; frame < 3; frame++) {

        DYNORA_LOG_DEBUG(DYNORA_LOG_CATEGORY_GENERAL,
            "Frame %u begin",
            frame);

        physics_step();

        DYNORA_LOG_DEBUG(DYNORA_LOG_CATEGORY_RENDER,
            "Submitting render commands for frame %u",
            frame);

        DYNORA_LOG_DEBUG(DYNORA_LOG_CATEGORY_AUDIO,
            "Updating audio listener for frame %u",
            frame);

        ecs_spawn_player();

        DYNORA_LOG_DEBUG(DYNORA_LOG_CATEGORY_ECS,
            "Entity registry size: %u",
            128 + frame);

        DYNORA_LOG_DEBUG(DYNORA_LOG_CATEGORY_GENERAL,
            "Frame %u end",
            frame);
    }

    /* ===== IO TEST ===== */
    save_game("savegame_01.dat");

    /* ===== FILTER TEST ===== */
    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_GENERAL,
        "Disabling AUDIO category at runtime");

    logger_set_category_mask(
        DYNORA_LOG_CATEGORY_MASK_ALL & ~DYNORA_LOG_CATEGORY_AUDIO_MASK);

    /*
        This message should NOT appear because
        AUDIO category is disabled.
    */
    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_AUDIO,
        "This audio log should be filtered");

    /*
        This one still appears.
    */
    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_RENDER,
        "Render category still enabled");

    /* ===== LEVEL FILTER TEST ===== */
    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_GENERAL,
        "Changing minimum level to WARNING");

    logger_set_level(DYNORA_LEVEL_WARNING);

    /*
        This should NOT appear.
    */
    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_GENERAL,
        "This INFO message should be filtered");

    DYNORA_LOG_TRACE(DYNORA_LOG_CATEGORY_IO,
        "Frame 490: Drawing 142 vertices for Mesh_Sword_01");

    /*
        This SHOULD appear.
    */
    DYNORA_LOG_WARNING(DYNORA_LOG_CATEGORY_GENERAL, "Low memory warning");

    DYNORA_LOG_ERROR(DYNORA_LOG_CATEGORY_IO,
        "Failed to open asset package: %s",
        "environment.pkg");

    DYNORA_LOG_FATAL(DYNORA_LOG_CATEGORY_GENERAL,
        "Fatal error: engine shutdown requested");

    return 0;
}

/* ===== MOCK SYSTEM IMPLEMENTATIONS ===== */

static void renderer_init(void) {
    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_RENDER, "Renderer initialized");

    DYNORA_LOG_DEBUG(DYNORA_LOG_CATEGORY_RENDER,
        "GPU vendor: %s",
        "NVIDIA");

    DYNORA_LOG_DEBUG(DYNORA_LOG_CATEGORY_RENDER,
        "Shader cache loaded successfully");
}

static void audio_init(void) {
    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_AUDIO, "Audio system initialized");

    DYNORA_LOG_WARNING(DYNORA_LOG_CATEGORY_AUDIO,
        "Audio device latency is above recommended threshold");
}

static void physics_step(void) {
    DYNORA_LOG_DEBUG(DYNORA_LOG_CATEGORY_PHYSICS,
        "Physics world step executed");
}

static void ecs_spawn_player(void) {
    static uint32_t next_entity = 1;

    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_ECS,
        "Spawned player entity id=%u",
        next_entity++);
}

static void save_game(const char* path) {
    DYNORA_LOG_INFO(DYNORA_LOG_CATEGORY_IO, "Saving game to '%s'", path);

    DYNORA_LOG_ERROR(DYNORA_LOG_CATEGORY_IO,
        "Failed to write save file '%s'",
        path);
}