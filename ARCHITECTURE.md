# dApp Architecture and Flow

## System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        User's Browser                            │
│                                                                   │
│  ┌──────────────────┐              ┌──────────────────────┐     │
│  │   Frontend UI    │◄────────────►│     MetaMask         │     │
│  │  (HTML/CSS/JS)   │              │   Wallet Extension   │     │
│  │                  │              │                      │     │
│  │  - Web3.js       │              │  - Signs TX          │     │
│  │  - Event Log     │              │  - Manages Keys      │     │
│  │  - Forms         │              │  - Network Config    │     │
│  └────────┬─────────┘              └──────────┬───────────┘     │
│           │                                    │                 │
└───────────┼────────────────────────────────────┼─────────────────┘
            │                                    │
            │ HTTP                               │ JSON-RPC
            │                                    │
┌───────────▼────────────────────────────────────▼─────────────────┐
│                       Express Server                              │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  API Endpoints:                                          │   │
│  │  - GET /           → Serve frontend                      │   │
│  │  - GET /api/config → Contract address & network info     │   │
│  │  - GET /api/health → Health check                        │   │
│  └──────────────────────────────────────────────────────────┘   │
│                                                                   │
└───────────────────────────────────────────────────────────────────┘
                                    │
                                    │ JSON-RPC (via Web3)
                                    │
┌───────────────────────────────────▼───────────────────────────────┐
│                    Ethereum Network (Hardhat Local)                │
│                                                                    │
│  ┌──────────────────────────────────────────────────────────┐    │
│  │          BlockchainNetwork Smart Contract                │    │
│  │                                                          │    │
│  │  Storage:                      Functions:               │    │
│  │  - nodes[]                     - addTransaction()       │    │
│  │  - nodeBlocks[][]              - addEdge()              │    │
│  │  - nodeConnections[][]         - setBalance()           │    │
│  │  - balances[]                  - getNode()              │    │
│  │                                - getBlock()             │    │
│  │  Events:                       - getBalance()           │    │
│  │  - NodeCreated                                          │    │
│  │  - NodeConnected                                        │    │
│  │  - TransactionAdded                                     │    │
│  │  - BlockCreated                                         │    │
│  └──────────────────────────────────────────────────────────┘    │
│                                                                    │
└────────────────────────────────────────────────────────────────────┘
```

## Data Flow: Adding a Transaction

```
┌──────────┐
│  User    │
└────┬─────┘
     │ 1. Fill form & click "Send Transaction"
     │
     ▼
┌─────────────────┐
│  Frontend JS    │
│  (app.js)       │
└────┬────────────┘
     │ 2. Validate inputs
     │ 3. Call contract.methods.addTransaction()
     │
     ▼
┌─────────────────┐
│   MetaMask      │
└────┬────────────┘
     │ 4. User confirms & signs transaction
     │ 5. Send signed TX to blockchain
     │
     ▼
┌──────────────────────┐
│  Hardhat Network     │
│  (Local Blockchain)  │
└────┬─────────────────┘
     │ 6. Execute transaction
     │
     ▼
┌────────────────────────────┐
│  BlockchainNetwork.sol     │
│  addTransaction() function │
└────┬───────────────────────┘
     │ 7. Validate inputs
     │ 8. Check balance
     │ 9. Add to block
     │ 10. Update balances
     │ 11. Emit TransactionAdded event
     │
     ▼
┌────────────────────┐
│  Transaction Hash  │
│  & Receipt         │
└────┬───────────────┘
     │ 12. Return to frontend
     │
     ▼
┌─────────────────┐
│  Frontend JS    │
│  - Log event    │
│  - Update UI    │
└─────────────────┘
```

## Smart Contract Structure

```
┌───────────────────────────────────────────────────────────────┐
│                   BlockchainNetwork Contract                  │
├───────────────────────────────────────────────────────────────┤
│                                                               │
│  STRUCTS:                                                     │
│  ┌─────────────────────────────────────────────────┐         │
│  │ Transaction                                     │         │
│  │ - transactionId, fromId, toId, amount, ...     │         │
│  └─────────────────────────────────────────────────┘         │
│                                                               │
│  ┌─────────────────────────────────────────────────┐         │
│  │ Block                                           │         │
│  │ - blockNumber, transactions[], hashes, ...     │         │
│  └─────────────────────────────────────────────────┘         │
│                                                               │
│  ┌─────────────────────────────────────────────────┐         │
│  │ Node                                            │         │
│  │ - nodeId, blockCount, active                   │         │
│  └─────────────────────────────────────────────────┘         │
│                                                               │
│  STORAGE:                                                     │
│  ┌─────────────────────────────────────────────────┐         │
│  │ mapping(nodeId => Node)                         │         │
│  │ mapping(nodeId => blockNum => Block)           │         │
│  │ mapping(nodeId => connectedNodes[])            │         │
│  │ mapping(userId => balance)                     │         │
│  └─────────────────────────────────────────────────┘         │
│                                                               │
│  FUNCTIONS:                                                   │
│  ┌─────────────────────────────────────────────────┐         │
│  │ PUBLIC:                                         │         │
│  │ - addTransaction()  : Add TX to node           │         │
│  │ - addEdge()         : Connect nodes            │         │
│  │ - setBalance()      : Set user balance         │         │
│  │ - getBalance()      : Get user balance         │         │
│  │ - getNode()         : Get node info            │         │
│  │ - getBlock()        : Get block details        │         │
│  │ - getTransaction()  : Get TX details           │         │
│  │                                                 │         │
│  │ PRIVATE:                                        │         │
│  │ - _finalizeBlock()  : Compute block hash       │         │
│  └─────────────────────────────────────────────────┘         │
│                                                               │
└───────────────────────────────────────────────────────────────┘
```

## Frontend Components

```
┌────────────────────────────────────────────────────────────┐
│                      index.html                            │
├────────────────────────────────────────────────────────────┤
│                                                            │
│  Header                                                    │
│  ├─ Title                                                  │
│  └─ Wallet Connection Button/Info                         │
│                                                            │
│  Main Content (Cards)                                      │
│  ├─ Network Status                                         │
│  │  ├─ Total Nodes                                        │
│  │  ├─ Transactions Per Block                             │
│  │  └─ Contract Address                                   │
│  │                                                         │
│  ├─ Node Information                                       │
│  │  ├─ Input: Node ID                                     │
│  │  └─ Display: ID, Block Count, Active, Connections     │
│  │                                                         │
│  ├─ Balance Check                                          │
│  │  ├─ Input: User ID                                     │
│  │  └─ Display: Balance                                   │
│  │                                                         │
│  ├─ Add Transaction                                        │
│  │  ├─ Input: Node ID, From ID, To ID, Amount            │
│  │  └─ Button: Send Transaction                           │
│  │                                                         │
│  ├─ Set Balance                                            │
│  │  ├─ Input: User ID, Balance                            │
│  │  └─ Button: Set Balance                                │
│  │                                                         │
│  ├─ Add Connection                                         │
│  │  ├─ Input: From Node, To Node                          │
│  │  └─ Button: Add Connection                             │
│  │                                                         │
│  ├─ Block Explorer                                         │
│  │  ├─ Input: Node ID, Block Number                       │
│  │  └─ Display: Block details, hashes, timestamp          │
│  │                                                         │
│  └─ Event Log                                              │
│     └─ Real-time activity feed                             │
│                                                            │
│  Footer                                                    │
│                                                            │
└────────────────────────────────────────────────────────────┘

Styling: style.css (Dark theme with blockchain aesthetics)
Logic: app.js (Web3 integration, event handling)
```

## Deployment Process

```
1. Install Dependencies
   └─> npm install
       ├─ hardhat
       ├─ web3
       ├─ express
       └─ other packages

2. Compile Contracts
   └─> npx hardhat compile
       └─ Creates artifacts/
          └─ BlockchainNetwork.json

3. Start Local Blockchain
   └─> npx hardhat node
       ├─ Creates 20 test accounts
       ├─ Each with 10,000 ETH
       └─ Listens on localhost:8545

4. Deploy Contract
   └─> npx hardhat run scripts/deploy.js
       ├─ Deploy with parameters (nodeCount, txPerBlock)
       ├─ Initialize sample network
       ├─ Set initial balances
       └─ Return contract address

5. Configure Environment
   └─> Create .env file
       └─ Set CONTRACT_ADDRESS

6. Start Server
   └─> npm start
       └─ Express server on port 3000

7. Configure MetaMask
   ├─ Add Hardhat Local network
   └─ Import test account

8. Access dApp
   └─> Open http://localhost:3000
       └─ Connect wallet & interact
```

## Transaction Lifecycle

```
┌─────────────────────────────────────────────────────────────┐
│                    Transaction States                       │
└─────────────────────────────────────────────────────────────┘

1. INITIATED
   └─ User fills form and clicks "Send Transaction"

2. VALIDATION (Frontend)
   ├─ Check all fields are filled
   ├─ Check amounts are positive
   └─ Check node IDs are valid

3. METAMASK PROMPT
   ├─ Display transaction details
   ├─ Show gas estimate
   └─ Wait for user confirmation

4. SIGNED
   └─ User approves, MetaMask signs TX

5. SUBMITTED
   └─ Sent to Hardhat network mempool

6. MINING
   └─ Block miner includes TX in next block

7. VALIDATION (Smart Contract)
   ├─ Check node ID is valid
   ├─ Check node is active
   ├─ Check amount > 0
   ├─ Check sender has sufficient balance
   └─ If any check fails → REVERT

8. EXECUTION
   ├─ Get/create current block for node
   ├─ Add transaction to block
   ├─ Update sender balance (decrease)
   ├─ Update receiver balance (increase)
   └─ Emit TransactionAdded event

9. BLOCK FINALIZATION (if block is full)
   ├─ Compute block hash
   ├─ Link to previous block
   ├─ Increment node's block count
   └─ Emit BlockCreated event

10. CONFIRMED
    ├─ Transaction hash returned
    ├─ Receipt generated
    └─ Events emitted

11. UI UPDATE
    ├─ Log event to event log
    ├─ Update balances display
    └─ Clear form
```

## Key Interactions

### Web3 Connection Flow
```
Browser → window.ethereum → MetaMask → web3.eth → Hardhat Node → Smart Contract
```

### Read Operations (View Functions)
```
Frontend → web3.eth.Contract.methods.getBalance().call()
         → No gas cost, instant response
         → Returns data from blockchain state
```

### Write Operations (State Changes)
```
Frontend → web3.eth.Contract.methods.addTransaction().send()
         → Requires gas
         → MetaMask confirmation
         → Transaction mined
         → State updated
         → Events emitted
```

## Security Model

```
┌─────────────────────────────────────────────────────────┐
│                   Security Layers                       │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  1. Frontend Validation                                 │
│     ├─ Input sanitization                               │
│     ├─ Type checking                                    │
│     └─ Range validation                                 │
│                                                         │
│  2. MetaMask Security                                   │
│     ├─ Private key management                           │
│     ├─ Transaction signing                              │
│     └─ User confirmation                                │
│                                                         │
│  3. Smart Contract Validation                           │
│     ├─ require() checks                                 │
│     ├─ Balance verification                             │
│     ├─ Access control (node validity)                   │
│     └─ Amount validation                                │
│                                                         │
│  4. Blockchain Immutability                             │
│     ├─ Block hashing                                    │
│     ├─ Chain linking                                    │
│     └─ Consensus mechanism                              │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

## Event System

```
Smart Contract Events → Blockchain Logs → Web3 Subscription → Frontend Update

Events Emitted:
┌─────────────────────────────────────────────────────────┐
│ NodeCreated(nodeId)                                     │
│   └─ Fired when: Node initialized in constructor       │
│                                                         │
│ NodeConnected(fromNode, toNode)                         │
│   └─ Fired when: addEdge() creates connection          │
│                                                         │
│ TransactionAdded(nodeId, txId, from, to, amount)        │
│   └─ Fired when: Transaction added to block            │
│                                                         │
│ BlockCreated(nodeId, blockNumber, blockHash)            │
│   └─ Fired when: Block finalized with hash             │
└─────────────────────────────────────────────────────────┘

Frontend can listen to these events to update UI in real-time.
```

## Technology Stack Summary

```
┌─────────────────────┬──────────────────────────────────┐
│ Layer               │ Technology                       │
├─────────────────────┼──────────────────────────────────┤
│ Smart Contract      │ Solidity 0.8.20                  │
│ Development         │ Hardhat                          │
│ Blockchain          │ Ethereum (Hardhat local node)    │
│ Web3 Library        │ Web3.js v4                       │
│ Wallet              │ MetaMask                         │
│ Backend             │ Node.js + Express                │
│ Frontend            │ HTML5 + CSS3 + JavaScript        │
│ Testing             │ Hardhat + Chai                   │
│ Package Manager     │ npm                              │
└─────────────────────┴──────────────────────────────────┘
```

This architecture provides a complete, production-ready dApp with proper separation of concerns, security, and user experience.
