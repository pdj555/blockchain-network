# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

A C++ implementation of a decentralized blockchain network where multiple nodes each maintain their own blockchain. The project simulates transaction processing across a network of connected nodes, with each node building blocks containing a fixed number of transactions.

## Build and Test Commands

### Compilation
```bash
# Using CMake (recommended)
cmake -S . -B build
cmake --build build

# Using g++ directly (legacy)
g++ main.cpp blockNetwork.cpp blockChain.cpp block.cpp transaction.cpp -o p4
```

### Running the Program
```bash
# With CMake build
./build/p4 < input1.txt

# With direct compilation
./p4 < input1.txt

# On PowerShell
Get-Content input1.txt | ./p4
```

### Running Tests
```bash
cmake -S . -B build
cmake --build build
./build/tests
```

## Architecture

The codebase follows a hierarchical design with four core classes:

### Class Hierarchy
- **transaction** (transaction.h/cpp) - Atomic transfer between two IDs with amount and timestamp
- **block** (block.h/cpp) - Container for a fixed number of transactions with hash-based chaining
- **blockChain** (blockChain.h/cpp) - Ordered sequence of blocks for a single node using `std::list<block>`
- **blockNetwork** (blockNetwork.h/cpp) - Top-level network managing multiple blockchains and node connectivity

### Key Design Patterns

**Hash-Based Chain Integrity**: Each block computes a hash from its contents and stores the previous block's hash. The `blockChain::verifyChain()` method walks the chain to detect tampering. The `blockNetwork::verifyAllChains()` validates every chain in the network.

**Adjacency List Network Topology**: `blockNetwork` uses `vector<vector<int>> adjList` to represent node connections efficiently. Node IDs are tracked in `unordered_map<int, int> idMap` for O(1) balance lookups.

**Transaction Processing**: Transactions are inserted into a node's blockchain via `blockNetwork::insertTranToNode()`, which delegates to `blockChain::insertTran()`. When a block reaches capacity (`maxNumTransactions`), a new block is created and linked.

**Block Linking**: The `block` class contains a raw pointer `block* nextBlock` (note: this is a known memory management concern documented in improvement.md). Blocks are stored in `blockChain::bChain` as a `std::list<block>`.

## Known Issues and Improvement Areas

The .agent/instructions/ directory contains systematic improvement plans:

1. **Memory Safety**: The `block::nextBlock` raw pointer should be replaced with `std::unique_ptr` or eliminated in favor of the container's ownership model
2. **Typo in API**: `block::inseartTran` should be renamed to `block::insertTran`
3. **Input Validation**: `main.cpp` already validates node ranges and transaction amounts; edge cases are handled gracefully
4. **Hash Verification**: Implemented in `blockChain::verifyChain()` and `blockNetwork::verifyAllChains()`

## Code Standards

This project follows C++11 standard with these conventions:
- Use `#pragma once` for header guards
- Prefer `const` correctness for getters
- Use `std::unordered_map` for ID tracking instead of linear search
- Store objects by value in containers when possible
- Classes use setters/getters pattern consistently

## Input File Format

The program reads from stdin with this structure:
```
<numNodes>
<transactionsPerBlock>
<totalTransactions>
<numEdges>
<u> <v>  (repeated numEdges times)
<node> <txID> <fromID> <toID> <amount> <timestamp>  (repeated for transactions)
```

See input1.txt for a complete example.
