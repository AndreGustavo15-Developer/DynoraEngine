cmake_minimum_required(VERSION 3.10)
project(DynoraEngine C)

# ====  C Standard ====
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==== Paths ====
set(DYNORA_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(DYNORA_ENGINE_DIR ${DYNORA_ROOT}/engine)
set(DYNORA_BACKENDS_DIR ${DYNORA_ROOT}/engine/logger/backends)
set(DYNORA_INCLUDE_DIR ${DYNORA_ROOT}/include)

# ==== Common Compiler Settings ====
function(dynora_apply_common_settings target)

    # ==== Warnings ====
    target_compile_options(${target} PRIVATE

        # Base warnings
        -Wall
        -Wextra
        -Wpedantic

        # Safer C warnings
        -Wshadow
        -Wconversion
        -Wstrict-prototypes
    )

    # ==== Debug Flags ====
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:

            # Disable optimizations
            -O0

            # Debug symbols
            -g

            # Better stack traces/profiling
            -fno-omit-frame-pointer
        >
    )

    # ==== Release Flags ====
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Release>:

            # Maximum optimization
            -O3
        >
    )

    # ==== Release Definitions ====
    target_compile_definitions(${target} PRIVATE
        $<$<CONFIG:Release>:NDEBUG>
    )

endfunction()

# ==== Sources ====
# Core systems
set(DYNORA_ENGINE_SOURCES
    ${DYNORA_ENGINE_DIR}/logger/logger.c
)

# Backends
set(DYNORA_BACKEND_SOURCES
    ${DYNORA_BACKENDS_DIR}/logger_console.c
)

# ==== Library ====
add_library(DynoraEngine STATIC
    ${DYNORA_ENGINE_SOURCES}
    ${DYNORA_BACKEND_SOURCES}
)

# ==== Includes ====
target_include_directories(DynoraEngine
    PUBLIC
        ${DYNORA_INCLUDE_DIR}
    PRIVATE
        ${DYNORA_ENGINE_DIR}
        ${DYNORA_BACKENDS_DIR}
)

# ==== Apply Common Settings ====
dynora_apply_common_settings(DynoraEngine)

# ==== Examples ====
option(BUILD_EXAMPLES "Build examples" ON)

function(add_dynora_example name file)
    add_executable(${name} ${file})
    target_link_libraries(${name} PRIVATE DynoraEngine)
    dynora_apply_common_settings(${name})
endfunction()

if(BUILD_EXAMPLES)
    add_dynora_example(dynora_example examples/hello_logger.c)
##    add_dynora_example(dynora_test_performance test/test_performance.c)
endif()
