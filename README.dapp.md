# Blockchain Network dApp

A full-stack decentralized application (dApp) that implements a blockchain network with smart contracts, Web3 integration, and a user-friendly web interface.

## Overview

This project transforms a C++ blockchain network simulation into a fully functional Ethereum-based dApp. It includes:

- **Smart Contracts**: Solidity contracts deployed on Ethereum/Hardhat network
- **Web3 Integration**: JavaScript frontend using Web3.js for blockchain interaction
- **MetaMask Support**: Wallet integration for transaction signing
- **Express Backend**: API server for serving the frontend and configuration
- **Interactive UI**: Modern, responsive web interface for managing the blockchain network

## Architecture

The dApp consists of three main components:

### 1. Smart Contracts (Solidity)
- `BlockchainNetwork.sol`: Main contract managing nodes, blocks, and transactions
- Features:
  - Dynamic node creation and management
  - Transaction processing with automatic block creation
  - Bidirectional node connections (edges)
  - Balance management for users
  - Block hash computation and verification

### 2. Backend (Node.js/Express)
- `server.js`: API server providing contract configuration and static file serving
- Endpoints:
  - `GET /`: Serves the main dApp interface
  - `GET /api/config`: Returns contract address and network configuration
  - `GET /api/health`: Health check endpoint

### 3. Frontend (HTML/CSS/JavaScript)
- `public/index.html`: Main user interface
- `public/css/style.css`: Modern, responsive styling
- `public/js/app.js`: Web3 integration and dApp logic

## Features

### Network Management
- View total nodes and network configuration
- Get detailed information about specific nodes
- View node connections (edges)
- Add new connections between nodes

### Transaction Management
- Send transactions between users on specific nodes
- Automatic block creation when transaction limit is reached
- Real-time balance tracking
- Set initial balances for users

### Block Explorer
- Explore blocks on any node
- View block details (hash, previous hash, timestamp)
- See transaction count per block
- Verify block integrity

### Wallet Integration
- MetaMask connection
- Account balance display
- Transaction signing and confirmation
- Account change detection

## Installation

### Prerequisites
- Node.js (v16 or higher)
- MetaMask browser extension
- Git

### Setup Steps

1. **Install Dependencies**
```bash
npm install
```

2. **Compile Smart Contracts**
```bash
npx hardhat compile
```

3. **Start Local Blockchain**
```bash
npx hardhat node
```
Keep this terminal running.

4. **Deploy Contracts** (in a new terminal)
```bash
npx hardhat run scripts/deploy.js --network localhost
```

5. **Update Configuration**
Copy the deployed contract address and create a `.env` file:
```bash
cp .env.example .env
```
Edit `.env` and set:
```
CONTRACT_ADDRESS=<your_deployed_contract_address>
PORT=3000
```

6. **Start the Server**
```bash
npm start
```

7. **Access the dApp**
Open your browser and navigate to:
```
http://localhost:3000
```

## MetaMask Configuration

1. Install MetaMask browser extension
2. Add Hardhat local network:
   - Network Name: `Hardhat Local`
   - RPC URL: `http://127.0.0.1:8545`
   - Chain ID: `1337`
   - Currency Symbol: `ETH`

3. Import a test account:
   - Copy a private key from the Hardhat node output
   - In MetaMask: Import Account → Paste private key

## Usage Guide

### Connect Wallet
1. Click "Connect Wallet" button
2. Approve MetaMask connection
3. Your account address and balance will be displayed

### Check Node Information
1. Enter a node ID (0 to n-1)
2. Click "Get Node Info"
3. View node details including block count and connections

### Send a Transaction
1. Fill in the transaction form:
   - Node ID: Target node for the transaction
   - From User ID: Sender
   - To User ID: Receiver
   - Amount: Amount to transfer
2. Click "Send Transaction"
3. Confirm in MetaMask
4. Wait for transaction confirmation

### Set User Balance
1. Enter user ID and desired balance
2. Click "Set Balance"
3. Confirm in MetaMask

### Add Node Connection
1. Enter source and target node IDs
2. Click "Add Connection"
3. Confirm in MetaMask

### Explore Blocks
1. Enter node ID and block number
2. Click "Explore Block"
3. View block details and hash information

## Smart Contract API

### Key Functions

**Constructor**
```solidity
constructor(uint256 _nodeCount, uint256 _transactionsPerBlock)
```

**Add Transaction**
```solidity
function addTransaction(uint256 nodeId, uint256 fromId, uint256 toId, uint256 amount)
```

**Add Edge (Node Connection)**
```solidity
function addEdge(uint256 fromNode, uint256 toNode)
```

**Get Balance**
```solidity
function getBalance(uint256 userId) returns (uint256)
```

**Get Node Information**
```solidity
function getNode(uint256 nodeId) returns (uint256 id, uint256 blockCount, bool active)
```

**Get Block Details**
```solidity
function getBlock(uint256 nodeId, uint256 blockNum) returns (uint256 blockNumber, uint256 transactionCount, bytes32 previousHash, bytes32 blockHash, uint256 timestamp)
```

## Events

The contract emits the following events:

- `NodeCreated(uint256 indexed nodeId)`
- `NodeConnected(uint256 indexed fromNode, uint256 indexed toNode)`
- `TransactionAdded(uint256 indexed nodeId, uint256 indexed transactionId, uint256 from, uint256 to, uint256 amount)`
- `BlockCreated(uint256 indexed nodeId, uint256 indexed blockNumber, bytes32 blockHash)`

## Development

### Run Tests
```bash
npx hardhat test
```

### Compile Contracts
```bash
npx hardhat compile
```

### Clean Build Artifacts
```bash
npx hardhat clean
```

### Development Mode (with auto-restart)
```bash
npm run dev
```

## Project Structure

```
blockchain-network/
├── contracts/              # Solidity smart contracts
│   └── BlockchainNetwork.sol
├── scripts/                # Deployment scripts
│   └── deploy.js
├── public/                 # Frontend files
│   ├── index.html
│   ├── css/
│   │   └── style.css
│   └── js/
│       └── app.js
├── hardhat.config.js       # Hardhat configuration
├── server.js               # Express backend server
├── package.json            # Node.js dependencies
├── .env.example            # Environment variables template
└── README.dapp.md          # This file
```

## C++ Original Implementation

The original C++ blockchain network implementation is still available:

```bash
# Build C++ version
cmake -S . -B build
cmake --build build

# Run C++ version
./build/p4 < input1.txt

# Run C++ tests
./build/tests
```

## Troubleshooting

### MetaMask Issues
- Ensure you're connected to the correct network (Hardhat Local)
- Reset account in MetaMask if nonce errors occur
- Clear browser cache if connection fails

### Contract Not Found
- Verify contract is deployed: check console output from deployment
- Ensure CONTRACT_ADDRESS is set correctly in .env
- Restart the server after updating .env

### Transaction Failures
- Check account has sufficient ETH for gas
- Verify user has sufficient balance for transfer
- Ensure node IDs are valid (0 to nodeCount-1)

## Security Considerations

⚠️ **Warning**: This is a demonstration dApp for educational purposes.

- Never share private keys
- Don't use real ETH on mainnet without proper security audit
- The contract has not been professionally audited
- Test thoroughly on local/testnet before any production use

## License

MIT License - See LICENSE file for details

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## Support

For issues and questions:
- Check existing GitHub issues
- Create a new issue with detailed description
- Include error messages and environment details

## Acknowledgments

Built upon the Simple Blockchain Implementation project, extended with:
- Ethereum/Solidity smart contracts
- Web3.js integration
- Modern frontend design
- Full dApp functionality
