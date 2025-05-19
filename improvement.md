# Improvement Plan

This repository contains a single C++ program (`project4.cpp`) that simulates a network of blockchains. The code compiles and produces output based on an input file, but it is monolithic and lacks modular design, error checking, and documentation. Below are actionable tasks structured as short user stories.

## Story 1: Modularize the Code Base
- **Task 1.1**: Split `project4.cpp` into separate header and source files for each class (`transaction`, `block`, `blockChain`, `blockNetwork`).
- **Task 1.2**: Create a top-level `main.cpp` that only contains the entry point and high-level orchestration.
- **Task 1.3**: Add include guards and use `#pragma once` for clarity.

## Story 2: Manage Memory Safely
- **Task 2.1**: Replace manual `new` allocations in `main` (lines 636–658) with automatic storage or `std::unique_ptr` to avoid leaks【F:project4.cpp†L636-L659】.
- **Task 2.2**: Ensure each container stores objects directly or via smart pointers so that transactions and block networks are released automatically.

## Story 3: Introduce Build System and Tests
- **Task 3.1**: Provide a `CMakeLists.txt` that builds the project on macOS, Linux, and Windows.
- **Task 3.2**: Add unit tests using Google Test covering transaction creation, block insertion, and network display.
- **Task 3.3**: Set up a GitHub Actions workflow to compile and run the tests on each push.

## Story 4: Improve Blockchain Features
- **Task 4.1**: Add hash values to blocks and link each block with the previous block hash to validate chain integrity.
- **Task 4.2**: Provide a verification function that walks the chain and reports inconsistencies.

## Story 5: Enhance Data Structures
- **Task 5.1**: Replace parallel `u` and `v` vectors with a single adjacency list structure for readability and faster lookup.
- **Task 5.2**: Replace ID tracking vectors with an `unordered_map<int, int>` (ID -> value) for constant‑time queries.

## Story 6: Provide Robust Input Handling
- **Task 6.1**: Validate input ranges (node numbers, transaction amounts) and report problems via clear error messages instead of crashing.
- **Task 6.2**: When the input stream ends unexpectedly, exit with a descriptive message.

## Story 7: Document and Polish
- **Task 7.1**: Expand `README.md` with an architecture overview, explanation of classes, and instructions for running tests.
- **Task 7.2**: Add diagrams showing how blocks, blockchains, and the block network relate to each other.

Following these tasks will modernize the project, align it with Apple’s design emphasis on simplicity and reliability, and make future enhancements easier to implement.
