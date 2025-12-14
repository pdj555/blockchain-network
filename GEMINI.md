# Blockchain Network

## Project Overview

This project is a C++ implementation of a decentralized blockchain network. It simulates a network of connected nodes, where each node maintains its own blockchain. The system processes transactions, groups them into blocks, and verifies the integrity of the chains using hash-based linking.

## Architecture

The project is structured around four core classes:

*   **`transaction`**: Represents a single value transfer between two IDs, including an amount and a timestamp.
*   **`block`**: A container for a fixed number of transactions. It includes a hash of its contents and the hash of the previous block, forming the chain.
*   **`blockChain`**: Manages the sequence of blocks for a single node. It handles block creation and chain verification.
*   **`blockNetwork`**: The high-level manager that controls multiple `blockChain` instances and the connectivity (edges) between nodes.

## Build and Run

### Prerequisites

*   C++ Compiler (supporting C++11 or later)
*   CMake (version 3.10 or later)

### Building with CMake (Recommended)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running the Application

The application reads input from standard input (stdin).

```bash
./build/p4 < input1.txt
```

### Legacy Build (g++)

You can also compile directly using `g++`:

```bash
g++ main.cpp blockNetwork.cpp blockChain.cpp block.cpp transaction.cpp -o p4
./p4 < input1.txt
```

## Testing

The project includes a Google Test-style test suite.

To run the tests using CMake:

```bash
cd build
ctest --output-on-failure
# OR directly run the test executable
./tests
```

## Development Conventions

*   **Standard**: C++11.
*   **File Structure**: Header files (`.h`) and source files (`.cpp`) are in the root directory. Tests are in `tests/`.
*   **Naming**: camelCase for methods and variables.
*   **Memory Management**: Currently uses some raw pointers (e.g., in `block` linking). Future improvements target `std::unique_ptr` for better safety.
*   **Input Format**: Strict input format expected via stdin (see `README.md` or `input1.txt` for details).

## Key Files

*   `main.cpp`: Entry point. Parses input and drives the simulation.
*   `blockNetwork.cpp/h`: Manages the network topology and delegates transaction insertion.
*   `blockChain.cpp/h`: Core logic for maintaining the integrity of a single chain.
*   `CMakeLists.txt`: Build configuration.
