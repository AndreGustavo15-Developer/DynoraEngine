# 🍇 Grape Engine  
*A high-performance modular game engine written in Zig + C*

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)  
[![GitHub issues](https://img.shields.io/github/issues/AndreGustavo15-Developer/GrapeEngine.svg)](https://github.com/AndreGustavo15-Developer/GrapeEngine/issues)  
[![GitHub stars](https://img.shields.io/github/stars/AndreGustavo15-Developer/GrapeEngine.svg)](https://github.com/AndreGustavo15-Developer/GrapeEngine/stargazers)

---

## 📚 Table of Contents

- [Overview](#overview)  
- [Core Philosophy](#core-philosophy)  
- [Goals](#goals)  
- [Current State](#current-state)  
- [First Implemented System](#first-implemented-system)  
- [Repository Structure](#repository-structure-evolving)  
- [Build](#build-in-progress)  
- [Documentation](#documentation)  
- [Contributing](#contributing)  
- [Project Direction](#project-direction)  
- [Future Vision](#future-vision)  
- [License](#license)  
- [Author](#author)  


## 🌱 Overview

**Grape Engine** is a game engine in development, built around the concept of **organic modularity**.

Inspired by a *grape cluster*, each module — called a “berry” — is an independent unit that can be added, removed, or replaced without breaking the core system.

The goal is to create an engine that **grows and evolves like a living system**, enabling ideas to mature naturally into scalable and high-performance interactive experiences.

> **Status:** Pre-architecture phase
> The Technical Design Document (TDD) is actively evolving, and the final architecture is not yet fully defined.

---

## 🍇 Core Philosophy

### 🌿 Modular Architecture (Grape Model)

* Each system is an independent **module (berry)**
* The core acts as the **connector (stem)**
* Systems evolve independently without breaking the whole
* New features can be integrated organically

This philosophy is the **foundation of the engine** and guides every technical decision.

---

## 🎯 Goals

* Define a deeply modular and extensible architecture
* Achieve **extreme performance** in critical systems
* Enable pluggable systems (DLL/modules)
* Grow into a **high-quality, production-ready open-source engine**

---

## 🛠️ Current State

The project is currently focused on:

* Documentation and TDD development
* Core architectural decisions
* Modular system design
* Implementation of **early systems for validation**

Not yet implemented:

* ECS (Entity Component System)
* Rendering pipeline
* Editor
* Physics / Audio subsystems

These will be defined as the architecture matures.

---

## 🧪 First Implemented System

### Logger

The logging system is the first subsystem implemented to validate core design decisions.

It is built with focus on:

* ⚡ Performance (hot-path optimized)
* 🧱 Modular backend system
* 🔌 Decoupled architecture
* 🔄 Future async support (ring buffer planned)

Example:

```c
GRAPE_LOG_INFO(GRAPE_LOG_GENERAL, "System initialized");
```

This system acts as a **foundation experiment**, guiding architectural decisions for future modules.

---

## 📂 Repository Structure (evolving)

```
GrapeEngine/
│
├── engine/
│   ├── logger/
│   ├── ecs/
│   ├── renderer/
│
├── backends/
│   └── logger_console/
│
├── docs/
│   └── tdd/
│
├── examples/
│
├── tests/
│
└── (README.md, LICENSE, ...)
```

> This structure will evolve alongside the architecture.

---

## ⚙️ Build (in progress)

The build system is currently under development and will be based on:

* Zig Build System
* C integration for low-level systems

More details will be added as the architecture stabilizes.

---

## 📘 Documentation

Documentation is maintained in:

```
/docs
/docs/tdd
```

The TDD defines the conceptual and architectural direction of the engine and evolves continuously.

---

## 🤝 Contributing

At this stage, contributions are focused on **design and architecture**.

You can help by:

* Opening issues to discuss ideas
* Reviewing architectural decisions
* Suggesting improvements to systems (e.g. logger)
* Proposing modular designs

📄 See [CONTRIBUTING.md](CONTRIBUTING.md) for full guidelines, coding standards, and commit conventions.

---

## 🧭 Project Direction

Current focus:

* Define core architecture
* Validate systems through implementation
* Ensure performance-first design

Avoid:

* Large feature implementations without alignment
* Adding external dependencies prematurely

---

## 📄 License

MIT License — free for personal, educational, and commercial use.

---

## 👤 Author

Created and maintained by **André Gustavo**
https://github.com/AndreGustavo15-Developer

---

🍇 *Growing ideas into systems.*
