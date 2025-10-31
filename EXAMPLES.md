# Example Usage Scenarios

This document provides practical examples of using the blockchain network dApp.

## Scenario 1: Setting Up a Small Network

### Objective
Create a 3-node network with initial balances and connections.

### Steps

1. **Start the dApp** (see QUICKSTART.md)

2. **Connect your MetaMask wallet**
   - Click "Connect Wallet"
   - Approve in MetaMask

3. **Verify Network Configuration**
   - Check that "Total Nodes" shows 5
   - Note the contract address

4. **Set Initial Balances**
   ```
   User ID: 1, Balance: 1000
   User ID: 2, Balance: 500
   User ID: 3, Balance: 750
   ```
   For each:
   - Go to "Set User Balance" section
   - Enter User ID and Balance
   - Click "Set Balance"
   - Confirm in MetaMask

5. **Add Node Connections**
   ```
   Node 0 ↔ Node 1
   Node 1 ↔ Node 2
   Node 0 ↔ Node 2
   ```
   For each connection:
   - Go to "Add Node Connection" section
   - Enter From Node and To Node
   - Click "Add Connection"
   - Confirm in MetaMask

6. **Verify Connections**
   - Go to "Node Information"
   - Enter Node ID: 0
   - Click "Get Node Info"
   - See Connections: 1, 2

## Scenario 2: Processing Transactions

### Objective
Send multiple transactions and observe block creation.

### Prerequisites
- User 1 has balance of 1000
- User 2 has balance of 500

### Steps

1. **Send First Transaction**
   - Node ID: 0
   - From User ID: 1
   - To User ID: 2
   - Amount: 100
   - Result: User 1: 900, User 2: 600

2. **Send Second Transaction**
   - Node ID: 0
   - From User ID: 2
   - To User ID: 3
   - Amount: 50
   - Result: User 2: 550, User 3: 50

3. **Send Third Transaction**
   - Node ID: 0
   - From User ID: 1
   - To User ID: 3
   - Amount: 200
   - Result: User 1: 700, User 3: 250

4. **Check Node Status**
   - Go to "Node Information"
   - Enter Node ID: 0
   - Block Count should show 1 (first block created with 3 transactions)

5. **Explore the Block**
   - Go to "Block Explorer"
   - Node ID: 0
   - Block Number: 0
   - View:
     - Transaction Count: 3
     - Block Hash
     - Previous Hash (0x0 for first block)
     - Timestamp

## Scenario 3: Multi-Node Transaction Network

### Objective
Distribute transactions across multiple nodes and verify each node's blockchain.

### Steps

1. **Setup Initial State**
   ```
   User 1: 2000
   User 2: 2000
   User 3: 2000
   ```

2. **Transactions on Node 0**
   ```
   TX1: User 1 → User 2, Amount: 100
   TX2: User 2 → User 3, Amount: 50
   TX3: User 3 → User 1, Amount: 75
   ```

3. **Transactions on Node 1**
   ```
   TX4: User 1 → User 3, Amount: 200
   TX5: User 2 → User 1, Amount: 150
   TX6: User 3 → User 2, Amount: 100
   ```

4. **Transactions on Node 2**
   ```
   TX7: User 1 → User 2, Amount: 250
   TX8: User 2 → User 3, Amount: 175
   TX9: User 3 → User 1, Amount: 125
   ```

5. **Verify Each Node**
   - Node 0: Should have 1 block (3 transactions)
   - Node 1: Should have 1 block (3 transactions)
   - Node 2: Should have 1 block (3 transactions)

6. **Check Final Balances**
   ```
   User 1: 2000 - 100 + 75 - 200 + 150 - 250 + 125 = 1800
   User 2: 2000 + 100 - 50 + 200 - 150 + 250 - 175 = 2175
   User 3: 2000 + 50 - 75 + 200 - 100 + 175 - 125 = 2025
   ```

## Scenario 4: Block Chain Verification

### Objective
Create multiple blocks and verify the chain integrity through hashes.

### Steps

1. **Set High Balance**
   - User 1: 10,000

2. **Create Multiple Blocks on Node 0**
   Send 9 transactions (3 per block):
   ```
   Block 0:
   - TX1: User 1 → User 2, Amount: 100
   - TX2: User 1 → User 3, Amount: 100
   - TX3: User 1 → User 4, Amount: 100
   
   Block 1:
   - TX4: User 1 → User 2, Amount: 100
   - TX5: User 1 → User 3, Amount: 100
   - TX6: User 1 → User 4, Amount: 100
   
   Block 2:
   - TX7: User 1 → User 2, Amount: 100
   - TX8: User 1 → User 3, Amount: 100
   - TX9: User 1 → User 4, Amount: 100
   ```

3. **Explore Block Chain**
   - Explore Block 0 on Node 0
     - Note Block Hash (e.g., 0x1a2b3c...)
     - Previous Hash: 0x0
   
   - Explore Block 1 on Node 0
     - Note Block Hash (e.g., 0x4d5e6f...)
     - Previous Hash: Should match Block 0's hash
   
   - Explore Block 2 on Node 0
     - Note Block Hash (e.g., 0x7g8h9i...)
     - Previous Hash: Should match Block 1's hash

4. **Verify Chain Integrity**
   - Each block's Previous Hash should match the previous block's Block Hash
   - This creates an immutable chain

## Scenario 5: Error Handling

### Objective
Test various error conditions and see how the dApp handles them.

### Test Cases

1. **Insufficient Balance**
   - User 1 balance: 100
   - Try: User 1 → User 2, Amount: 200
   - Expected: Transaction reverted with "Insufficient balance"

2. **Invalid Node ID**
   - Try: Add transaction to Node 10 (doesn't exist)
   - Expected: Transaction reverted with "Invalid node ID"

3. **Zero Amount**
   - Try: User 1 → User 2, Amount: 0
   - Expected: Transaction reverted with "Amount must be positive"

4. **Invalid Connection**
   - Try: Add edge between Node 0 and Node 10
   - Expected: Transaction reverted with "Invalid node ID"

## Scenario 6: Event Monitoring

### Objective
Monitor blockchain events in real-time through the Event Log.

### Steps

1. **Open Event Log**
   - Scroll to "Event Log" section
   - Should show "No events yet"

2. **Perform Actions and Watch Events**
   - Set a balance → See event logged
   - Add a connection → See event logged
   - Send a transaction → See event logged
   - Each event shows timestamp and message

3. **Event Types**
   - Success events (green border): Successful operations
   - Error events (red border): Failed operations
   - Info events (blue border): Status updates

## Scenario 7: Network Exploration

### Objective
Explore the entire network topology and state.

### Steps

1. **Map All Nodes**
   For each node (0-4):
   - Get Node Info
   - Record: Block Count, Connections
   
   Example output:
   ```
   Node 0: 2 blocks, Connected to: [1, 2]
   Node 1: 1 block,  Connected to: [0, 3]
   Node 2: 0 blocks, Connected to: [0, 3]
   Node 3: 3 blocks, Connected to: [1, 2, 4]
   Node 4: 1 block,  Connected to: [3]
   ```

2. **Map All Balances**
   For each user ID (1-10):
   - Check Balance
   - Record current balance
   
   Example output:
   ```
   User 1: 800
   User 2: 1200
   User 3: 500
   User 4: 300
   User 5: 1500
   ```

3. **Explore All Blocks**
   For each node with blocks:
   - Explore each block number
   - Record transaction count and hashes

## Scenario 8: Performance Testing

### Objective
Test the system's ability to handle many transactions.

### Steps

1. **Setup**
   - User 1: 100,000

2. **Rapid Transaction Submission**
   - Send 20+ transactions quickly
   - Observe block creation pattern
   - Each 3 transactions creates a new block

3. **Monitor**
   - Watch Event Log for confirmations
   - Check MetaMask for pending/confirmed transactions
   - Verify final balances are correct

4. **Expected Results**
   - All transactions eventually confirm
   - Blocks are created in sequence
   - No transactions are lost
   - Chain integrity is maintained

## Tips for Testing

1. **Reset Between Scenarios**
   - Restart Hardhat node
   - Redeploy contract
   - Update .env with new address
   - Restart server
   - Reset MetaMask account (Settings → Advanced → Reset Account)

2. **Monitor Gas Usage**
   - Each transaction requires gas
   - Check MetaMask for gas estimates
   - Test accounts have 10,000 ETH (more than enough)

3. **Use Browser Developer Tools**
   - Open Console (F12) to see detailed logs
   - Check Network tab for API calls
   - View any error messages

4. **Keep Track of State**
   - Write down balances before/after
   - Track which nodes have which blocks
   - Document the network topology

## Common Workflows

### Daily Development Workflow
```
1. npx hardhat node (Terminal 1)
2. npx hardhat run scripts/deploy.js --network localhost (Terminal 2)
3. Update .env with contract address
4. npm start (Terminal 2)
5. Open http://localhost:3000
6. Connect MetaMask
7. Test features
```

### Testing Workflow
```
1. Make changes to contract
2. npx hardhat compile
3. npx hardhat test
4. If tests pass:
   - Deploy to local network
   - Test in UI
5. If tests fail:
   - Fix issues
   - Repeat
```

### Production Deployment Workflow (Future)
```
1. npx hardhat test (ensure all tests pass)
2. Update hardhat.config.js with target network
3. Add private key to .env
4. npx hardhat run scripts/deploy.js --network <network>
5. Update .env with production contract address
6. Deploy frontend to hosting service
7. Update environment variables
```

## Troubleshooting Examples

### Problem: Transaction Fails
**Steps to Debug:**
1. Check Event Log for error message
2. Verify MetaMask is on correct network
3. Check account has ETH for gas
4. Verify input values are valid
5. Check browser console for errors

### Problem: Can't See Updated Balance
**Solution:**
1. Wait for transaction confirmation (check Event Log)
2. Refresh the page
3. Re-query the balance

### Problem: MetaMask Shows Wrong Network
**Solution:**
1. Click network dropdown in MetaMask
2. Select "Hardhat Local"
3. Refresh the page

## Advanced Usage

### Programmatic Interaction
You can also interact with the contract using the browser console:

```javascript
// Get contract instance
const contractAddress = document.getElementById('contract-address').textContent;
const contract = new web3.eth.Contract(BLOCKCHAIN_NETWORK_ABI, contractAddress);

// Read operations (no gas)
const nodeCount = await contract.methods.nodeCount().call();
console.log('Total nodes:', nodeCount);

const balance = await contract.methods.getBalance(1).call();
console.log('User 1 balance:', balance);

// Write operations (requires gas and account)
const accounts = await web3.eth.getAccounts();
await contract.methods.setBalance(10, 5000).send({ from: accounts[0] });
```

This allows for automated testing and custom interactions beyond the UI.
