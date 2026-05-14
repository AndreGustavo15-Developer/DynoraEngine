<!--
<p align="center"> <img src="./assets/banner2.png" alt="Dynora Engine Banner" width="100%" /> </p>
-->
<h1 align="center">DYNORA ENGINE</h1>

<p align="center"> <strong>Modular game engine focused on performance and scalability (Zig + C)</strong> </p> 

<p align="center"> <em>Grow modular worlds.</em> </p>

<p align="center"> 
  <a href="LICENSE"> 
    <img src="https://img.shields.io/badge/license-MIT-green.svg" /> 
  </a> 
  <a href="https://github.com/AndreGustavo15-Developer/DynoraEngine/issues"> 
    <img src="https://img.shields.io/github/issues/AndreGustavo15-Developer/DynoraEngine.svg" /> 
  </a> 
  <a href="https://github.com/AndreGustavo15-Developer/DynoraEngine/stargazers"> 
    <img src="https://img.shields.io/github/stars/AndreGustavo15-Developer/DynoraEngine.svg" /> 
  </a> 
</p>

---

## Overview

**Dynora Engine** is a modular game engine in development, **designed to grow, adapt, and scale as unified systems** — where independent modules work together while preserving performance and flexibility.

Every subsystem is built as an independent module, allowing the engine to scale without sacrificing maintainability.

Dynora’s mission is to provide a **high-performance, deeply modular architecture** where ideas can be assembled into scalable systems — empowering developers to build complex interactive worlds with precision and control.

---

## Core Philosophy

Dynora is built on the belief that a game engine should behave as an **adaptive architecture**:

- Systems are **modular and replaceable**
- The core coordinates independent subsystems
- Features evolve without compromising the whole

This philosophy drives every architectural decision.

### Principles

- **Modular Architecture** — loosely coupled and extensible systems  
- **Adaptive Pipelines** — execution paths adapt to context and hardware  
- **Hybrid ECS Design** — balancing flexibility and high performance  
- **Scalable Foundations** — systems designed to evolve with the project  

Dynora is not just built for features — it is built for **system growth**.

---

## Goals

Dynora aims to:

- Define a deeply modular and extensible architecture  
- Achieve high performance in critical systems  
- Support modular subsystems and runtime extensions
- Allow modules to be statically linked or dynamically loaded
- Support lightweight runtime-reloadable plugins
- Grow into a production-ready open-source engine  
- Give developers full architectural control  

---

## Current State

> **Architecture Validation Phase**

Dynora is currently validating its core runtime architecture and subsystem boundaries incrementally.

- Core runtime concepts are being defined through ADRs and versioned specifications
- The logger subsystem is the first architectural validation system
- Public APIs and module boundaries are being stabilized before large systems are introduced
- Zig is being introduced progressively while C remains the primary ABI and interface layer

This stage focuses on validating architecture correctness before expanding engine features.

---

## Why Dynora Exists

Many engines become difficult to evolve over time due to tightly coupled systems and unclear boundaries.

Dynora is an attempt to explore a different approach:

- Systems should be **independent but cooperative**
- Architecture should **adapt**, not resist change
- Performance should be considered **from the start**
- Growth should not require rewriting the engine

This project is as much about **architecture exploration** as it is about building an engine.

---

## What Exists Today

### Logger (first subsystem)

The logging system is the first implemented subsystem and acts as an architectural validation layer for Dynora.

It currently validates:

- Module boundaries
- Backend abstraction
- Performance considerations in hot paths
- Decoupled interfaces
- Runtime boundaries
- Module lifecycle concepts
- Concurrency model experiments
- Backend execution models
- Stable public C interfaces

Example:

```c
DYNORA_LOG_INFO(DYNORA_LOG_GENERAL, "System initialized");
```
Planned architectural evolution:

- Asynchronous logging
- Multiple backends
- Filtering and formatting layers

---

## Architecture Direction

Dynora is being designed around a few core principles:

### Modular Systems

Each subsystem should be independently replaceable and testable.

### Clear Boundaries

Modules communicate through well-defined interfaces.

### Performance Awareness

Critical paths are considered early in design, not as an afterthought.

### Incremental Validation

Systems are implemented early in minimal form to validate architectural decisions.

### Stable Boundaries

Public interfaces should remain stable and explicit.

Dynora prioritizes clear contracts between systems over hidden coupling or implicit dependencies.

---

## Architecture Model

Dynora is structured around three primary layers.

### Core

The core is intentionally minimal and responsible for:

- lifecycle management
- runtime orchestration
- platform abstraction
- service coordination
- module loading
- memory and threading interfaces

The core should remain small, stable, and independent from higher-level systems.

---

### Modules

Modules are large engine subsystems such as:

- renderer
- ECS
- physics
- audio
- asset pipeline

Modules are designed to:

- evolve independently
- expose stable interfaces
- remain loosely coupled
- be replaceable when possible

Modules may be:

- statically linked
- dynamically loaded
- compiled together with the engine runtime

---

### Plugins

Plugins are lightweight runtime extensions intended for:

- editor tooling
- debugging utilities
- scripting integrations
- experimental systems
- custom workflows

Unlike core modules, plugins are expected to support runtime reloadability whenever possible.

---

### Language Strategy

Dynora follows a layered language model:

- C defines stable interfaces and ABI boundaries
- Zig progressively implements runtime systems and internal subsystems

This separation allows architecture stability while enabling modern systems programming features internally.

---

### Planned Systems

These systems are planned but **not yet fully implemented**:

- ECS (Entity Component System)
- Rendering pipeline
- Editor
- Physics
- Audio
- Asset pipeline

All systems will follow the same modular design philosophy.

---

## Repository Structure

```bash
DynoraEngine/
│
├── engine/
│   ├── core/
│   ├── logger/
│   ├── ecs/
│   └── renderer/
│
├── backends/
│   └── logger_console/
│
├── plugins/
│
├── docs/
│   ├── roadmap.md
│   ├── core_rules.md
│   ├── adr/
│   ├── specs/
│   └── design/
│
├── examples/
├── tests/
│
└── README.md
```

The structure will continue evolving alongside the architecture.

---

## Build

> ⚠️ Temporary build system

Dynora currently uses **CMake** as a temporary build system while the architecture is being defined.

The long-term plan is to migrate to the **Zig build system**, but this will only happen after the core architecture stabilizes.

### Requirements

- C compiler (GCC, Clang, or MSVC)
- CMake 3.x

### Build Steps

```bash
git clone https://github.com/AndreGustavo15-Developer/DynoraEngine
cd DynoraEngine

mkdir build
cd build

cmake ..
cmake --build .
```
### Output

The current build produces early engine modules and test binaries used for architectural validation.

---

## Documentation

Dynora documentation is organized by responsibility.

### Core Rules

Global architectural constraints and engine-wide principles.

```bash
/docs/core_rules.md
```

### ADRs (Architecture Decision Records)

Formal architectural decisions and system contracts.

```bash
/docs/adr
```

### Specifications

Versioned subsystem contracts and behavioral definitions.

```bash
/docs/specs
```

### Design Notes

Exploratory documents and evolving architectural ideas.

```bash
/docs/design
```

---

## Contributing

Dynora is in an early stage, so contributions are **especially valuable right now** — particularly in architecture and system design.

## Ways to Contribute
- Review and discuss architecture decisions
- Suggest improvements to the logger system
- Propose module interfaces and boundaries
- Identify risks in modular design
- Improve documentation and clarity
- Add tests or small validation implementations

## Before You Start

For anything beyond small changes:

> Open an issue first to discuss your idea.

This helps keep the architecture consistent and avoids duplicated work.

## Good First Contributions
- Improve README or docs
- Suggest API improvements for the logger
- Add small test cases
- Review TDD sections and raise questions

### Current Needs

Dynora is currently looking for contributors interested in:

- Low-level systems design
- Engine architecture
- Performance-oriented C/Zig development
- API design and modular interfaces

See **[CONTRIBUTING.md](CONTRIBUTING.md)** for contribution guidelines.

---

## Project Direction
### Current Focus
- Validating runtime architecture incrementally
- Stabilizing subsystem contracts
- Defining module and plugin boundaries
- Building a minimal and scalable core runtime
- Using the logger subsystem as an architectural validation system

### Intentionally Avoiding (for now)
- Large feature expansion
- Premature optimization without validation
- External dependencies that constrain architecture

---

## Roadmap (Conceptual)
- Define core architecture
- Stabilize logger module
- Implement initial ECS prototype
- Define rendering abstraction
- Expand documentation and examples

---

## License

MIT License — free for personal, educational, and commercial use.

---

## Author

Created and maintained by **[André Gustavo](https://github.com/AndreGustavo15-Developer)**.

---

<p align="center"> <strong>Assemble ideas. Build systems.</strong> </p>
