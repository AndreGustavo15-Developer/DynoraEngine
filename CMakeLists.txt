cmake_minimum_required(VERSION 3.10)
project(Grape_Engine C)

# ==== Padrão C ====
set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==== Pastas ====
set(GRAPE_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GRAPE_ENGINE_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GRAPE_ENGINE_INCLUDE_INTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ==== Fonte principal ====
set(GRAPE_ENGINE_CORE
    ${GRAPE_ENGINE_SRC_DIR}/log/grape_system_log.c
)

# ==== Backends ====
set(GRAPE_BACKENDS
    ${GRAPE_ENGINE_SRC_DIR}/log/backends/grape_backend_console.c
)

# ==== Biblioteca ====
add_library(Grape_Engine STATIC
    ${GRAPE_ENGINE_CORE}
    ${GRAPE_BACKENDS}
)

target_include_directories(Grape_Engine
    PUBLIC
        ${GRAPE_ENGINE_INCLUDE_DIR}
    PRIVATE
        ${GRAPE_ENGINE_SRC_DIR}
)

# ==== Exemplos ====
option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_EXAMPLES)
    add_executable(grape_example examples/test.c)
    target_link_libraries(grape_example PRIVATE Grape_Engine)
endif()