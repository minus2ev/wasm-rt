# wasm-rt

A self-educational WebAssembly (Wasm) runtime built from scratch in C++. 

The primary goal of **wasm-rt** is a "back-to-basics" deep dive into the Wasm specification. It serves as a personal laboratory for sharpening C++ skills and deconstructing the mechanics of binary parsing, validation, and execution.

## 🛠 Project Philosophy
Unlike production-grade runtimes, `wasm-rt` is **not** intended for general use. It is a pedagogical tool designed to answer the question: *"How does a stack machine actually handle a binary format?"*

* **Focus:** Clarity and learning.
* **Spec Target:** WebAssembly 2.0.
* **Constraints:** Minimal external dependencies, staying close to the spec.

## 🏗 Current Architecture
The project is currently in the **Binary Phase**. I have implemented the infrastructure to ingest and interpret the Wasm format.

* **Language:** C++17
* **Build System:** CMake
* **Testing:** Unit tests powered by **Catch2**
* **Status:**
    * [x] Module Loading
    * [x] Section Parsing (Type, Function, Memory, Global, Export, Code)
    * [ ] Section Parsing (Import, Table, Start, Element, Data)
    * [ ] Custom Section handling
    * [x] Unit Test suite for the parser

## 🚀 Development Environment
This project is configured for development within a **VS Code DevContainer**. This ensures a consistent environment with the C++ toolchain, CMake, and Catch2 pre-configured.

To build:
1. Open in VS Code.
2. Reopen in Container.
3. Use the CMake extension to build.

## 🗺 Roadmap
The project is evolving through the standard lifecycle of a runtime:

1. **Phase 1: Decoding (MVP Complete)** – Reading bits and bytes into meaningful C++ structures.
2. **Phase 2: Execution (Upcoming)** – Implementing the runtime stack and instruction set.
3. **Phase 3: WASI Support (TBD)** – Minimal system interface for basic I/O.

## 📜 License
This project is open-source under the GPLv3 License.