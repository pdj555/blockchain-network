# Blockchain Network: Creating a Decentralized Network of Blockchains

## 🚀 Now Available as a Full dApp!

This project is now available as a **complete decentralized application (dApp)** with:
- ✅ Ethereum smart contracts (Solidity)
- ✅ Web3.js integration
- ✅ MetaMask wallet support
- ✅ Modern web interface
- ✅ Real blockchain interaction

**Quick Start:** See [QUICKSTART.md](QUICKSTART.md) to run the dApp in 5 minutes!

**Full Documentation:** See [README.dapp.md](README.dapp.md) for complete dApp documentation.

---

## Objective
The goal of this project is to create a network of BlockChains using the given class structures. This is an extension of [Simple-Blockchain-Implementation](https://github.com/pdj555/Simple-Blockchain-Implementation).

This project now includes **two implementations**:
1. **C++ Implementation** (original) - Command-line blockchain network simulator
2. **dApp Implementation** (new) - Full Ethereum-based decentralized application

## Description
In this project, we have an additional class called `blockNetwork` that includes four fields: `numNodes`, `allNodes`, `u`, and `v`. The input file provides a list of `u-v` values representing the connections between nodes.

## Input File Explanation
The input file is structured as follows:
- Number of Nodes: Specifies the total number of nodes in the network.
- Number of Transactions per Block: Indicates the number of transactions per block.
- Total Number of Transactions: Represents the overall number of transactions.
- Number of Edges: Specifies the number of edges connecting nodes.
- Edge List: Lists the edges connecting nodes `u` and `v`.
- Transactions: Provides the transactions in the format: Transaction ID, FromID, ToID, Amount, and Timestamp.

## Implementation Guidelines
1. Understand the provided class structures and relationships.
2. Parse the input file and extract relevant information.
3. Use the provided boilerplate code as a starting point.
4. Implement logic to track `fromValue` and `toValue` for each transaction.
5. Update the blockchain for each node based on the transactions.

## Running the Program
### Visual Studio (Windows)
1. Open or create a new project.
2. Go to Project > Project Properties.
3. Expand Configuration Properties and select Debugging.
4. In the Command Arguments field, type `<input_filename`.
5. Build and run the program.

### macOS/Linux/PowerShell
If you have g++ installed, compile and run the program using the terminal:

        g++ main.cpp blockNetwork.cpp blockChain.cpp block.cpp transaction.cpp -o p4
        ./p4 < input1.txt

For powershell:

        Get-Content input1.txt | ./p4

## Architecture Overview

The project is composed of four key classes:

- **transaction** – represents a single transfer between two IDs.
- **block** – stores a fixed number of transactions and links to the previous block using a hash.
- **blockChain** – maintains the sequence of blocks for one node and verifies integrity.
- **blockNetwork** – manages multiple blockchains and the adjacency relationships between nodes.

Blocks compute their hashes from their contents and the previous block hash. This allows `blockChain` to verify that a chain has not been altered, and `blockNetwork` can validate every chain across the network.

## Running the Tests

### C++ Tests

The repository includes a small test suite built with a minimal Google Test wrapper. To build and run the tests:

```bash
cmake -S . -B build
cmake --build build
./build/tests
```

The tests exercise transaction creation, block insertion, and network verification.

### Smart Contract Tests

To run the Hardhat tests for the blockchain network smart contract:

```bash
npm install
npx hardhat test
```

This will test the Solidity contract functionality including deployment, transactions, blocks, and events.

---

## Project Structure

```
blockchain-network/
├── contracts/              # Solidity smart contracts (dApp)
│   └── BlockchainNetwork.sol
├── scripts/                # Deployment scripts (dApp)
│   └── deploy.js
├── public/                 # Frontend files (dApp)
│   ├── index.html
│   ├── css/style.css
│   └── js/app.js
├── test/                   # Smart contract tests (dApp)
│   └── BlockchainNetwork.test.js
├── tests/                  # C++ tests
│   ├── test_blockchain.cpp
│   ├── test_network.cpp
│   └── test_transaction.cpp
├── *.cpp, *.h              # C++ implementation
├── server.js               # Express backend (dApp)
├── hardhat.config.js       # Hardhat configuration (dApp)
├── package.json            # Node.js dependencies (dApp)
├── QUICKSTART.md           # Quick start guide for dApp
└── README.dapp.md          # Full dApp documentation
```

## Documentation

- **[QUICKSTART.md](QUICKSTART.md)** - Get the dApp running in 5 minutes
- **[README.dapp.md](README.dapp.md)** - Complete dApp documentation
- **[improvement.md](improvement.md)** - Development roadmap and improvements

## Technologies Used

### C++ Implementation
- C++11
- CMake
- Google Test

### dApp Implementation
- Solidity 0.8.20
- Hardhat
- Web3.js
- Express.js
- MetaMask
- Ethereum
