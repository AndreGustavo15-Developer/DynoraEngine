# Contributing to Dynora Engine

Thank you for contributing to **Dynora Engine**.

This document defines the standards and guidelines that keep the project consistent, performant, and aligned with its core philosophy:  
**modularity, performance, and evolving architecture**.

---

# Project Philosophy

Dynora is built around three fundamental principles:

* Performance — hot paths must remain minimal and predictable 
* Modularity — systems must be decoupled and replaceable  
* Evolving architecture — systems grow without breaking the core  

Before contributing, always ask:

> “Does this keep the system simple, fast, and decoupled?”

---

# How to Contribute

## 1. Fork & Branch

Create a descriptive branch:

```bash
git checkout -b feat/logger-ring-buffer
```

---

## 2. Keep Changes Small and Focused

Avoid large, mixed commits.

Good:

* One commit per feature
* One commit per fix

Avoid:

* Mixing refactor + feature + bugfix in one commit

---

## 3. Follow Commit Conventions

Required format:

```bash
type(scope): short description
```

---

# Commit Types

| Type       | Usage                   |
| ---------- | ----------------------- |
| `feat`     | New feature             |
| `fix`      | Bug fix                 |
| `perf`     | Performance improvement |
| `refactor` | Structural change       |
| `docs`     | Documentation           |
| `build`    | Build/config            |
| `test`     | Tests                   |

---

# Scopes

Use the affected system:

* `core`
* `logger`
* `ecs`
* `renderer`
* `physics`
* `audio`
* `io`
* `memory`
* `platform`
* `tools`
* `editor`

---

# Commit Rules

* Use imperative verbs (add, fix, remove, improve)
* Max 72 characters in the first line
* Do not use a period at the end
* Always include a scope
* Be clear and concise

---

# Commit Examples

```bash
fix(logger): avoid dangling pointer in log message

feat(logger): add ring buffer for async logging

perf(logger): remove time() call from hot path

refactor(logger): separate dispatch from backend execution
```

---

# Performance Guidelines

## Hot Path Rules (CRITICAL)

For performance-critical systems (logger, ECS, renderer):

 Avoid:

* dynamic allocation (`malloc/free`)
* heavy calls (`printf`, `time`, etc.)
* unnecessary branching
* any I/O operations

Prefer:

* contiguous memory (arrays, buffers)
* cache-friendly structures
* deterministic behavior

---

## Logging Rules

* Logs must be **self-contained**
* Never store pointers to temporary memory (stack/local)
* Events must be safe for async usage
* Logging must not block the hot path

---

## Modularity Rules

* Avoid direct dependencies between systems
* Use simple interfaces (function pointers, structs)
* Design systems to be replaceable
* Think plugin/DLL architecture from the start

---

# Code Guidelines

* Clear code > clever code
* Avoid unnecessary macros
* Prefer simple and predictable C
* Keep systems understandable and maintainable

---

# Documentation

If your change:

* modifies behavior
* changes APIs
* introduces a new system

Update the documentation accordingly

---

# Pull Requests

Before opening a PR:

* [ ] Code compiles
* [ ] Follows commit conventions
* [ ] Does not break existing APIs (or is documented)
* [ ] Does not introduce unnecessary cost in hot paths
* [ ] Aligns with Dynora’s philosophy

---

# Architecture Guidelines

* Data must be self-contained when crossing systems
* Avoid implicit global state
* Prefer composition over tight coupling
* Systems must be replaceable (plugin/DLL-friendly)

---

# Project Direction

Dynora does NOT aim for:

* excessive abstraction
* unnecessary complexity
* overengineering

Dynora focuses on:

* simplicity
* predictability
* real-world performance

---

# In Doubt?

When in doubt:

> Choose the simplest, most predictable, and most performant solution — always prioritize the hot path.

---

<p align="center">
  <strong>Assemble ideas. Build systems.</strong>
</p>

```
