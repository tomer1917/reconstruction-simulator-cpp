# Reconstruction Simulator (C++ Memory Management)

**Assignment 1 – Systems Programming: Reconstruction Simulation**

This repository contains a C++ implementation of a reconstruction simulator for "SPLand", focusing on memory management, efficient use of classes, and avoidance of leaks via the Rule of Five. It compiles and runs on UNIX lab machines, validated with Valgrind.

---

## 📂 Repository Structure

```
├── docs/
│   └── assignment1-spec.pdf        # Original assignment specification
├── configs/                       # Simulation configuration files
│   └── example_config.txt         # Sample config demonstrating syntax
├── include/                       # Header files
│   ├── Action.h
│   ├── Auxiliary.h
│   ├── Facility.h
│   ├── Plan.h
│   ├── SelectionPolicy.h
│   ├── Settlement.h
│   └── Simulation.h
├── src/                           # Source files
│   ├── Action.cpp
│   ├── Auxiliary.cpp
│   ├── Facility.cpp
│   ├── Plan.cpp
│   ├── SelectionPolicy.cpp
│   ├── Settlement.cpp
│   ├── Simulation.cpp
│   └── main.cpp
├── makefile                       # Builds executable into bin/
└── README.md                      # Project overview and instructions
```

---

## 📋 Requirements

- **Compiler:** g++ (GNU C++11) on CS LAB UNIX
- **Flags:** `-g -Wall -Weffc++ -std=c++11 -Iinclude`
- **Tools:** `make`, `valgrind`

---

## ⚙️ Building

From the project root, run:

```sh
make
```

This will compile all sources into the `bin/` directory and produce the `simulation` executable.

---

## ▶️ Usage

```sh
bin/simulation <path/to/config.txt>
```

After startup, interact via commands:

- `step N`              : Advance simulation by N steps
- `plan <name> <policy>`: Create a plan for settlement `<name>` using policy `nve|bal|eco|env`
- `settlement <name> <type>` : Add a new settlement
- `facility <args>`     : Define a facility type
- `planStatus <id>`     : Display status of plan `<id>`
- `changePolicy <id> <policy>`
- `log`                 : Print action history
- `backup` / `restore`  : Save and restore simulation snapshot
- `close`               : End simulation and print final scores

Refer to `docs/assignment1-spec.pdf` for full details.

---

## 💡 Design & Memory Management

- **Rule of Five** implemented in `Plan` and `Simulation` to ensure deep copies, proper cleanup, and safe moves:
  - Custom copy constructor, copy-assignment, move constructor, move-assignment, and destructor
- **RAII** for owned pointers:
  - `SelectionPolicy*` and `Settlement*` clones managed and deleted in destructors
- **Action Logging** via polymorphic `BaseAction` clones ensures clean ownership and no leaks
- **Validation** with Valgrind yields zero leaks:
  ```sh
  valgrind --leak-check=full --show-reachable=yes bin/simulation <config>
  ```

---

## 📝 License

This project is licensed under the [MIT License](LICENSE).

