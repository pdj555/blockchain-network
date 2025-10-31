# Troubleshooting Guide

Common issues and solutions for the Blockchain Network dApp.

## Installation Issues

### Issue: `npm install` fails
**Symptoms:**
- Error messages during package installation
- Missing dependencies

**Solutions:**
1. **Check Node.js version:**
   ```bash
   node --version  # Should be v16 or higher
   ```
   If older, update Node.js from [nodejs.org](https://nodejs.org)

2. **Clear npm cache:**
   ```bash
   npm cache clean --force
   rm -rf node_modules package-lock.json
   npm install
   ```

3. **Use different registry:**
   ```bash
   npm install --registry https://registry.npmjs.org/
   ```

### Issue: Hardhat compile fails with "Compiler download failed"
**Symptoms:**
- Cannot download Solidity compiler
- Network timeout errors

**Solutions:**
1. **Check internet connection**

2. **Use pre-installed compiler:** (if available locally)
   Update `hardhat.config.js`:
   ```javascript
   solidity: {
     version: "0.8.20",
     settings: {
       optimizer: { enabled: true, runs: 200 }
     }
   }
   ```

3. **Manual compiler installation:**
   ```bash
   npx hardhat compile --force
   ```

## Deployment Issues

### Issue: "Contract not deployed" error
**Symptoms:**
- UI shows "Not deployed"
- Cannot interact with contract

**Solutions:**
1. **Verify Hardhat node is running:**
   ```bash
   # In separate terminal
   npx hardhat node
   ```
   Should show "Started HTTP and WebSocket JSON-RPC server"

2. **Deploy contract:**
   ```bash
   npx hardhat run scripts/deploy.js --network localhost
   ```

3. **Update .env file:**
   - Copy the contract address from deployment output
   - Paste into `.env`:
     ```
     CONTRACT_ADDRESS=0x5FbDB2315678afecb367f032d93F642f64180aa3
     ```

4. **Restart server:**
   ```bash
   npm start
   ```

### Issue: Deployment script fails
**Symptoms:**
- Error during `npx hardhat run scripts/deploy.js`

**Solutions:**
1. **Check Hardhat node is running** (see above)

2. **Verify network configuration:**
   In `hardhat.config.js`:
   ```javascript
   networks: {
     localhost: {
       url: "http://127.0.0.1:8545"
     }
   }
   ```

3. **Clean and recompile:**
   ```bash
   npx hardhat clean
   npx hardhat compile
   npx hardhat run scripts/deploy.js --network localhost
   ```

## MetaMask Issues

### Issue: Cannot connect to MetaMask
**Symptoms:**
- "Connect Wallet" button does nothing
- MetaMask doesn't prompt

**Solutions:**
1. **Install MetaMask:**
   - Download from [metamask.io](https://metamask.io)
   - Restart browser after installation

2. **Unlock MetaMask:**
   - Click MetaMask icon
   - Enter password

3. **Enable site permissions:**
   - MetaMask → Settings → Connected Sites
   - Add http://localhost:3000

4. **Check browser console:**
   - Press F12
   - Look for error messages
   - Common: "window.ethereum is undefined" means MetaMask not installed

### Issue: Wrong network in MetaMask
**Symptoms:**
- Transactions fail
- "Invalid network" errors

**Solutions:**
1. **Switch to Hardhat Local:**
   - Click network dropdown in MetaMask
   - Select "Hardhat Local"

2. **Add Hardhat Local network:**
   - MetaMask → Settings → Networks → Add Network
   - Fill in:
     - **Network Name:** Hardhat Local
     - **RPC URL:** http://127.0.0.1:8545
     - **Chain ID:** 1337
     - **Currency Symbol:** ETH

3. **Verify connection:**
   - Network indicator should show "Hardhat Local"
   - Account should show ~10000 ETH

### Issue: Account has 0 ETH
**Symptoms:**
- Cannot send transactions
- "Insufficient funds" errors

**Solutions:**
1. **Import test account:**
   - Copy private key from Hardhat node output
   - MetaMask → Import Account
   - Paste private key

2. **Use different test account:**
   - Hardhat creates 20 accounts
   - Try a different private key from the list

3. **Restart Hardhat node:**
   - Stop current node (Ctrl+C)
   - Start new node: `npx hardhat node`
   - Import new account with 10000 ETH

### Issue: Transaction stuck/pending
**Symptoms:**
- Transaction shows "Pending" for long time
- New transactions fail with nonce error

**Solutions:**
1. **Wait for confirmation:**
   - Local Hardhat network is usually instant
   - Check Event Log for confirmation

2. **Reset MetaMask account:**
   - MetaMask → Settings → Advanced
   - Click "Reset Account"
   - This clears transaction history and nonce

3. **Restart Hardhat node:**
   - Stop and restart `npx hardhat node`
   - Redeploy contract
   - Reset MetaMask account

## Runtime Issues

### Issue: Server won't start
**Symptoms:**
- `npm start` fails
- Port already in use

**Solutions:**
1. **Port 3000 in use:**
   ```bash
   # Find process using port 3000
   lsof -ti:3000
   
   # Kill the process
   kill -9 $(lsof -ti:3000)
   ```
   
   Or change port in `.env`:
   ```
   PORT=3001
   ```

2. **Missing dependencies:**
   ```bash
   npm install
   npm start
   ```

### Issue: Cannot access http://localhost:3000
**Symptoms:**
- Browser shows "Unable to connect"
- 404 Not Found

**Solutions:**
1. **Verify server is running:**
   - Check terminal for "Server running on http://localhost:3000"

2. **Check correct URL:**
   - Make sure it's `http://` not `https://`
   - Correct port (3000 or your PORT in .env)

3. **Try different browser:**
   - Some browsers cache aggressively
   - Try Chrome, Firefox, or Edge

4. **Clear browser cache:**
   - Hard refresh: Ctrl+Shift+R (Windows/Linux) or Cmd+Shift+R (Mac)

### Issue: UI shows old/cached data
**Symptoms:**
- Changes not reflected
- Old contract address shown

**Solutions:**
1. **Hard refresh browser:**
   - Ctrl+Shift+R (Windows/Linux)
   - Cmd+Shift+R (Mac)

2. **Clear browser cache:**
   - F12 → Application → Clear Storage → Clear site data

3. **Restart server:**
   ```bash
   # Stop server (Ctrl+C)
   npm start
   ```

## Transaction Issues

### Issue: Transaction fails with "Insufficient balance"
**Symptoms:**
- Transaction reverted
- Balance too low

**Solutions:**
1. **Set balance first:**
   - Go to "Set User Balance" section
   - Set adequate balance (e.g., 10000)

2. **Check current balance:**
   - Use "Check Balance" section
   - Verify balance is sufficient

3. **Check gas balance:**
   - MetaMask account needs ETH for gas
   - Use imported test account with 10000 ETH

### Issue: Transaction fails with "Invalid node ID"
**Symptoms:**
- Error when adding transaction
- Node doesn't exist

**Solutions:**
1. **Use valid node IDs:**
   - Default deployment creates 5 nodes (0-4)
   - Use node IDs from 0 to 4

2. **Check node exists:**
   - Use "Node Information" section
   - Get info for the node ID you're trying to use

### Issue: "Nonce too high" error
**Symptoms:**
- MetaMask shows nonce error
- Transactions won't send

**Solutions:**
1. **Reset MetaMask account:**
   - MetaMask → Settings → Advanced
   - "Reset Account" button

2. **Restart Hardhat node:**
   - This resets blockchain state
   - Redeploy contract
   - Reset MetaMask

### Issue: Gas estimation failed
**Symptoms:**
- Transaction fails before sending
- "Cannot estimate gas" error

**Solutions:**
1. **Transaction would revert:**
   - Fix the underlying issue (e.g., insufficient balance)
   - Try transaction again

2. **Manually set gas limit:**
   In MetaMask, click "Edit" on gas and set:
   - Gas Limit: 300000

3. **Check contract state:**
   - Verify node exists
   - Verify user balances
   - Ensure amounts are valid

## Smart Contract Issues

### Issue: Cannot read contract data
**Symptoms:**
- "Contract not initialized" errors
- Functions return no data

**Solutions:**
1. **Verify contract address:**
   - Check `.env` has correct CONTRACT_ADDRESS
   - Restart server after updating .env

2. **Verify deployment:**
   - Run `npx hardhat run scripts/deploy.js --network localhost`
   - Copy new address to .env

3. **Check browser console:**
   - F12 → Console
   - Look for Web3 errors

### Issue: Events not showing in Event Log
**Symptoms:**
- Actions complete but no events logged
- Event Log stays empty

**Solutions:**
1. **Check transaction succeeded:**
   - Look for confirmation in MetaMask
   - Check transaction hash on Hardhat node output

2. **Refresh page:**
   - Events are logged to UI
   - Refresh might restore logging

3. **Check browser console:**
   - F12 → Console
   - Events should log there even if UI fails

## Testing Issues

### Issue: Hardhat tests fail
**Symptoms:**
- `npx hardhat test` shows failures

**Solutions:**
1. **Clean and recompile:**
   ```bash
   npx hardhat clean
   npx hardhat compile
   npx hardhat test
   ```

2. **Check for syntax errors:**
   - Review test file: `test/BlockchainNetwork.test.js`
   - Check contract: `contracts/BlockchainNetwork.sol`

3. **Run specific test:**
   ```bash
   npx hardhat test --grep "Deployment"
   ```

## General Debugging Tips

### Enable verbose logging
```bash
# Run server with debugging
DEBUG=* npm start

# Run Hardhat with verbose output
npx hardhat run scripts/deploy.js --network localhost --verbose
```

### Check versions
```bash
node --version    # Should be v16+
npm --version     # Should be 8+
npx hardhat --version
```

### View Hardhat node logs
The terminal running `npx hardhat node` shows all blockchain activity:
- Transactions
- Contract deployments
- Gas usage
- Errors

### Browser Developer Tools
Press F12 and check:
- **Console:** JavaScript errors, Web3 logs
- **Network:** API calls, responses
- **Application:** Local storage, session data

### Reset Everything (Nuclear Option)
When nothing else works:

```bash
# 1. Stop all processes
# Press Ctrl+C in all terminals

# 2. Clean Node.js
rm -rf node_modules package-lock.json
npm install

# 3. Clean Hardhat
npx hardhat clean

# 4. Recompile
npx hardhat compile

# 5. Start fresh Hardhat node
npx hardhat node

# 6. Deploy (in new terminal)
npx hardhat run scripts/deploy.js --network localhost

# 7. Update .env with new contract address

# 8. Reset MetaMask
# MetaMask → Settings → Advanced → Reset Account

# 9. Start server
npm start

# 10. Hard refresh browser
# Ctrl+Shift+R
```

## Getting Help

If you're still stuck:

1. **Check documentation:**
   - README.md
   - README.dapp.md
   - QUICKSTART.md
   - ARCHITECTURE.md

2. **Review examples:**
   - EXAMPLES.md

3. **Check logs:**
   - Browser console (F12)
   - Server terminal
   - Hardhat node terminal

4. **Create minimal reproduction:**
   - Isolate the issue
   - Try with fresh deployment
   - Document exact steps to reproduce

5. **Ask for help:**
   - Include error messages
   - Describe what you tried
   - Share relevant logs
   - Mention your environment (OS, Node version, etc.)

## Prevention Tips

### Best Practices

1. **Keep terminals organized:**
   - Terminal 1: Hardhat node (always running)
   - Terminal 2: Server (always running)
   - Terminal 3: Commands (deployment, tests, etc.)

2. **Document changes:**
   - Keep track of contract addresses
   - Note which accounts you're using
   - Record important balances

3. **Test incrementally:**
   - Don't make many changes at once
   - Test after each change
   - Keep working state

4. **Regular restarts:**
   - Restart Hardhat node daily
   - Reset MetaMask when confused
   - Clear browser cache periodically

5. **Version control:**
   - Commit working states
   - Use git to track changes
   - Can revert if something breaks

## Common Error Messages Decoded

| Error Message | Meaning | Solution |
|---------------|---------|----------|
| `Insufficient balance` | User doesn't have enough funds | Set higher balance |
| `Invalid node ID` | Node doesn't exist | Use node ID 0-4 |
| `Amount must be positive` | Trying to send 0 or negative | Use positive amount |
| `Nonce too high` | Transaction order mismatch | Reset MetaMask account |
| `Cannot estimate gas` | Transaction would fail | Fix underlying issue |
| `Invalid address` | Wrong contract address | Check .env file |
| `Network error` | Cannot reach blockchain | Check Hardhat node running |
| `User denied transaction` | Rejected in MetaMask | Approve transaction |

## Still Having Issues?

If this guide doesn't solve your problem:
- Double-check you followed QUICKSTART.md exactly
- Try the "Reset Everything" procedure above
- Check for typos in configuration files
- Verify all prerequisites are installed correctly
- Make sure your system meets minimum requirements
