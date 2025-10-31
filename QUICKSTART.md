# Quick Start Guide - Blockchain Network dApp

## 🚀 Getting Started in 5 Minutes

This guide will help you quickly set up and run the blockchain network dApp.

### Prerequisites Checklist
- [ ] Node.js installed (v16+): `node --version`
- [ ] MetaMask browser extension installed
- [ ] Git installed
- [ ] Terminal/Command prompt

### Step-by-Step Setup

#### 1. Install Dependencies (2 minutes)

```bash
cd blockchain-network
npm install
```

This installs:
- Hardhat (Ethereum development environment)
- Express (Web server)
- Web3.js (Blockchain interaction)
- Other required packages

#### 2. Compile Smart Contracts (1 minute)

```bash
npx hardhat compile
```

This compiles the Solidity smart contract `BlockchainNetwork.sol` into deployable bytecode.

**Expected output:**
```
Compiled 1 Solidity file successfully
```

#### 3. Start Local Blockchain (Keep Running)

Open a **new terminal** window and run:

```bash
npx hardhat node
```

**Expected output:**
```
Started HTTP and WebSocket JSON-RPC server at http://127.0.0.1:8545/

Accounts
========
Account #0: 0xf39Fd6e51aad88F6F4ce6aB8827279cffFb92266 (10000 ETH)
Private Key: 0xac0974bec39a17e36ba4a6b4d238ff944bacb478cbed5efcae784d7bf4f2ff80
...
```

⚠️ **Keep this terminal running** - This is your local blockchain!

#### 4. Deploy Smart Contract (1 minute)

Open a **third terminal** and run:

```bash
npx hardhat run scripts/deploy.js --network localhost
```

**Expected output:**
```
Deploying BlockchainNetwork contract...
BlockchainNetwork deployed to: 0x5FbDB2315678afecb367f032d93F642f64180aa3
Connected node 0 <-> 1
...
Save this contract address to your .env file:
CONTRACT_ADDRESS=0x5FbDB2315678afecb367f032d93F642f64180aa3
```

**⚠️ IMPORTANT:** Copy the contract address!

#### 5. Configure Environment

Create a `.env` file:

```bash
cp .env.example .env
```

Edit `.env` and paste your contract address:

```env
CONTRACT_ADDRESS=0x5FbDB2315678afecb367f032d93F642f64180aa3
PORT=3000
```

#### 6. Start Web Server

```bash
npm start
```

**Expected output:**
```
Server running on http://localhost:3000
Make sure Hardhat node is running: npx hardhat node
```

#### 7. Configure MetaMask

1. **Add Local Network:**
   - Open MetaMask
   - Click network dropdown → "Add Network" → "Add a network manually"
   - Fill in:
     - **Network Name:** `Hardhat Local`
     - **RPC URL:** `http://127.0.0.1:8545`
     - **Chain ID:** `1337`
     - **Currency Symbol:** `ETH`
   - Click "Save"

2. **Import Test Account:**
   - In MetaMask: Click account icon → "Import Account"
   - Paste a private key from the Hardhat node output (e.g., the first one)
   - Click "Import"

#### 8. Open the dApp

Navigate to: **http://localhost:3000**

Click **"Connect Wallet"** and approve in MetaMask.

🎉 **You're all set!**

---

## 🎯 Quick Test Workflow

### Test 1: Check Network Status
- The homepage shows total nodes (5) and transactions per block (3)

### Test 2: View Node Information
1. Enter Node ID: `0`
2. Click "Get Node Info"
3. See node details and connections

### Test 3: Check User Balance
1. Enter User ID: `1`
2. Click "Check Balance"
3. See balance: `1000` units

### Test 4: Send a Transaction
1. Fill in form:
   - Node ID: `0`
   - From User ID: `1`
   - To User ID: `2`
   - Amount: `100`
2. Click "Send Transaction"
3. Confirm in MetaMask
4. Wait for confirmation
5. Check balances again (User 1: 900, User 2: 600)

### Test 5: Explore a Block
1. Enter Node ID: `0`, Block Number: `0`
2. Click "Explore Block"
3. View block details and hashes

---

## 📝 Terminal Windows Summary

You should have **3 terminals running**:

1. **Terminal 1 (Hardhat Node):**
   ```bash
   npx hardhat node
   # Keep running - this is your blockchain
   ```

2. **Terminal 2 (Web Server):**
   ```bash
   npm start
   # Keep running - this serves your dApp
   ```

3. **Terminal 3 (Commands):**
   ```bash
   # Use this for deployment, testing, etc.
   ```

---

## 🔧 Common Issues & Solutions

### Issue: "Contract not initialized"
**Solution:** 
- Verify CONTRACT_ADDRESS is set in `.env`
- Restart the web server after updating `.env`

### Issue: "Transaction failed"
**Solution:**
- Ensure Hardhat node is running
- Check MetaMask is on "Hardhat Local" network
- Verify user has sufficient balance
- Reset MetaMask account if nonce errors occur

### Issue: "Cannot connect to MetaMask"
**Solution:**
- Ensure MetaMask extension is installed and unlocked
- Refresh the page
- Check browser console for errors

### Issue: Port 3000 already in use
**Solution:**
- Edit `.env` and change PORT to `3001`
- Or stop the process using port 3000: `lsof -ti:3000 | xargs kill`

---

## 🧪 Testing the Smart Contract

Run automated tests:

```bash
npx hardhat test
```

This runs the test suite to verify contract functionality.

---

## 🛑 Stopping the dApp

1. **Stop Web Server:** Press `Ctrl+C` in Terminal 2
2. **Stop Hardhat Node:** Press `Ctrl+C` in Terminal 1

---

## 🔄 Restarting Later

When you come back:

1. Start Hardhat node: `npx hardhat node`
2. Deploy contract: `npx hardhat run scripts/deploy.js --network localhost`
3. Update `.env` with new contract address
4. Start server: `npm start`
5. Open http://localhost:3000

---

## 📚 Next Steps

- Read [README.dapp.md](README.dapp.md) for detailed documentation
- Explore the smart contract code in `contracts/BlockchainNetwork.sol`
- Customize the frontend in `public/`
- Add new features to the contract

---

## 💡 Tips

- **Keep Hardhat node running** while using the dApp
- **Each restart** of Hardhat node requires redeployment
- **Test accounts** have 10,000 ETH each (fake test ETH)
- **MetaMask transactions** can be sped up by increasing gas limit

---

## 🎓 Learning Resources

- [Hardhat Documentation](https://hardhat.org/getting-started/)
- [Solidity Documentation](https://docs.soliditylang.org/)
- [Web3.js Documentation](https://web3js.readthedocs.io/)
- [MetaMask Developer Docs](https://docs.metamask.io/)

---

## ✨ Features to Try

1. **Add Node Connections:** Create network topology
2. **Set User Balances:** Initialize user funds
3. **Send Multiple Transactions:** Watch blocks auto-create
4. **Explore Block Hashes:** See blockchain integrity
5. **Monitor Event Log:** Track all activities in real-time

---

**Happy Building! 🚀**
