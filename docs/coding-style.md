# Dynora Coding Style

This document defines the coding standards and naming conventions for **Dynora Engine**.

The goal is to ensure:
- consistency
- clarity
- performance
- maintainability

Dynora prioritizes **simple, predictable, and efficient code**.

---

# Core Principles

- Clear code > clever code  
- Performance is a feature  
- Avoid unnecessary abstractions  
- Prefer explicit over implicit behavior  
- Systems must remain decoupled  

---

# Naming & Prefix Conventions

Dynora uses a **layered naming system** to balance clarity, modularity, and global safety.

---

## Macros & Global Constants

- Use `UPPER_CASE`
- Always prefix with `DYNORA_`

```c
#define DYNORA_LOG_INFO(...)
#define DYNORA_ASSERT(...)
#define DYNORA_MAX_ENTITIES 1024
```

- Prevents global naming conflicts
- Clearly identifies engine-level symbols

---

## Public API Functions

* Use `snake_case`
* Prefix by module name

```c
logger_init()
renderer_create()
memory_allocate()
```

- Keeps API modular and readable
- Avoids unnecessary global prefixes

---

## Internal Functions

* Use `snake_case`
* Mark as `static`
* Optional `_internal` suffix for clarity

```c
static void logger_flush_internal();
static void renderer_submit_batch();
```

- Limits symbol visibility
- Keeps internal logic isolated

---

## Types (Structs, Enums, Typedefs)

* Use `PascalCase`
* Avoid global prefixes unless necessary

```c
typedef struct Logger Logger;
typedef struct Renderer Renderer;
```

---

## Global Variables

* Use `d_` prefix
* Keep scope minimal (`static` whenever possible)

```c
static Logger d_logger;
static Renderer d_renderer;
```

- Clearly identifies global state
- Avoids accidental misuse

---

## Local Variables

* Use `snake_case`
* No prefixes

```c
int frame_count;
float delta_time;
```

---

## Constants (Alternative to macros)

```c
enum {
    DYNORA_MAX_ENTITIES = 1024
};
```

---

# Code Organization

## File Structure

* One system per folder
* Public API in `.h`
* Implementation in `.c`

```c
// logger.h
void logger_init(Logger* logger);

// logger.c
void logger_init(Logger* logger) {
    ...
}
```

---

## API Design

* Keep APIs minimal and explicit
* Avoid hidden state
* Pass context via structs

```c
void renderer_draw(Renderer* renderer);
```

---

# Performance Guidelines

## Hot Path Rules (CRITICAL)

For performance-critical systems:

Avoid:

* dynamic allocation (`malloc/free`)
* heavy calls (`printf`, `time`, etc.)
* unnecessary branching
* any I/O operations

Prefer:

* contiguous memory (arrays, buffers)
* cache-friendly layouts
* deterministic execution

---

## Memory

* Avoid allocations in hot paths
* Prefer arenas, pools, or linear allocators
* Make ownership explicit

```c
void* memory_alloc(MemoryArena* arena, size_t size);
```

---

# Modularity

* Systems must be independent
* Avoid direct coupling between modules
* Use interfaces (function pointers, structs)

```c
typedef struct LoggerBackend {
    void (*write)(const char* message);
} LoggerBackend;
```

---

# Logging

* Logs must be self-contained
* Never store pointers to temporary memory
* Must be safe for async usage
* Must not block execution

---

# Code Style

## Formatting

* Use consistent indentation
* Keep line length reasonable (~100 chars)
* Always use braces

```c
if (condition) {
    do_something();
}
```

---

## Functions

* Keep functions small and focused
* One responsibility per function

---

## Macros

* Avoid complex macros
* Prefer inline functions when possible

---

# Documentation

* Document public APIs
* Use comments only when necessary
* Code should be self-explanatory

```c
// Initializes the logger system
void logger_init(Logger* logger);
```

---

# Avoid

* Overengineering
* Deep abstraction layers
* Hidden side effects
* Implicit global state

---

# Final Rule

When in doubt:

> Choose the simplest, most predictable, and most performant solution.

---

<p align="center">
  <strong>Assemble ideas. Build systems.</strong>
</p>
```
