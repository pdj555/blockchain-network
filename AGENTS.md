# Repository Guidelines

## Project Structure & Module Organization

- Source (C++): `main.cpp` plus the core modules `block*.*` and `transaction.*` in the repo root.
- Library/entry points (CMake): `blockchain` (library), `p4` (main executable), `tests` (test executable).
- Tests: `tests/test_*.cpp` (unit tests) with a lightweight GoogleTest-style harness in `gtest/`.
- CI: `.github/workflows/cmake.yml` configures CMake, builds, then runs `ctest` on Linux/macOS/Windows.
- Sample data: `input1.txt` (stdin) and `output1.txt` (expected output snapshot).

## Build, Test, and Development Commands

Prereqs: CMake ≥ 3.10 and a C++11 compiler.

- Configure: `cmake -S . -B build`
- Build: `cmake --build build`
- Run locally: `./build/p4 < input1.txt`
- Run tests: `ctest --test-dir build --output-on-failure` (or `./build/tests`)

`build/` is ignored by git; don’t commit generated artifacts.

## Coding Style & Naming Conventions

- Keep changes consistent with existing style (4-space indentation; follow the file’s brace/spacing patterns).
- Prefer C++11 standard-library types, `const` correctness, and passing by `const&` to avoid copies.
- Naming follows existing patterns: `blockChain`/`blockNetwork` classes, `insertTranToNode` methods, `blockNetwork.cpp` file naming.
- Headers use `#pragma once`; avoid introducing new `using namespace std;` in headers.

## Testing Guidelines

- Place tests in `tests/` and name files `test_<area>.cpp`.
- Use `gtest/gtest.h` macros (`TEST`, `EXPECT_EQ`, `EXPECT_TRUE`, `EXPECT_NE`).
- Tests should be deterministic; only validate `std::cout` output when behavior requires it.

## Commit & Pull Request Guidelines

- Commit subjects are short, imperative, and capitalized (e.g., `Add CMake build system and unit tests`).
- PRs should include: a brief rationale, how you validated (`ctest ...`), and any fixture updates (`input1.txt`/`output1.txt`) if output changes.
