// Blockchain Network dApp - Main Application

let web3;
let contract;
let userAccount;
let contractAddress;
let contractABI;

// Contract ABI - simplified for the main functions
const BLOCKCHAIN_NETWORK_ABI = [
    {
        "inputs": [{"internalType": "uint256", "name": "_nodeCount", "type": "uint256"}, {"internalType": "uint256", "name": "_transactionsPerBlock", "type": "uint256"}],
        "stateMutability": "nonpayable",
        "type": "constructor"
    },
    {
        "anonymous": false,
        "inputs": [{"indexed": true, "internalType": "uint256", "name": "nodeId", "type": "uint256"}, {"indexed": true, "internalType": "uint256", "name": "blockNumber", "type": "uint256"}, {"indexed": false, "internalType": "bytes32", "name": "blockHash", "type": "bytes32"}],
        "name": "BlockCreated",
        "type": "event"
    },
    {
        "anonymous": false,
        "inputs": [{"indexed": true, "internalType": "uint256", "name": "fromNode", "type": "uint256"}, {"indexed": true, "internalType": "uint256", "name": "toNode", "type": "uint256"}],
        "name": "NodeConnected",
        "type": "event"
    },
    {
        "anonymous": false,
        "inputs": [{"indexed": true, "internalType": "uint256", "name": "nodeId", "type": "uint256"}],
        "name": "NodeCreated",
        "type": "event"
    },
    {
        "anonymous": false,
        "inputs": [{"indexed": true, "internalType": "uint256", "name": "nodeId", "type": "uint256"}, {"indexed": true, "internalType": "uint256", "name": "transactionId", "type": "uint256"}, {"indexed": false, "internalType": "uint256", "name": "from", "type": "uint256"}, {"indexed": false, "internalType": "uint256", "name": "to", "type": "uint256"}, {"indexed": false, "internalType": "uint256", "name": "amount", "type": "uint256"}],
        "name": "TransactionAdded",
        "type": "event"
    },
    {
        "inputs": [{"internalType": "uint256", "name": "fromNode", "type": "uint256"}, {"internalType": "uint256", "name": "toNode", "type": "uint256"}],
        "name": "addEdge",
        "outputs": [],
        "stateMutability": "nonpayable",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "uint256", "name": "nodeId", "type": "uint256"}, {"internalType": "uint256", "name": "fromId", "type": "uint256"}, {"internalType": "uint256", "name": "toId", "type": "uint256"}, {"internalType": "uint256", "name": "amount", "type": "uint256"}],
        "name": "addTransaction",
        "outputs": [],
        "stateMutability": "nonpayable",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "uint256", "name": "userId", "type": "uint256"}],
        "name": "getBalance",
        "outputs": [{"internalType": "uint256", "name": "", "type": "uint256"}],
        "stateMutability": "view",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "uint256", "name": "nodeId", "type": "uint256"}, {"internalType": "uint256", "name": "blockNum", "type": "uint256"}],
        "name": "getBlock",
        "outputs": [{"internalType": "uint256", "name": "blockNumber", "type": "uint256"}, {"internalType": "uint256", "name": "transactionCount", "type": "uint256"}, {"internalType": "bytes32", "name": "previousHash", "type": "bytes32"}, {"internalType": "bytes32", "name": "blockHash", "type": "bytes32"}, {"internalType": "uint256", "name": "timestamp", "type": "uint256"}],
        "stateMutability": "view",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "uint256", "name": "nodeId", "type": "uint256"}],
        "name": "getNode",
        "outputs": [{"internalType": "uint256", "name": "id", "type": "uint256"}, {"internalType": "uint256", "name": "blockCount", "type": "uint256"}, {"internalType": "bool", "name": "active", "type": "bool"}],
        "stateMutability": "view",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "uint256", "name": "nodeId", "type": "uint256"}],
        "name": "getNodeConnections",
        "outputs": [{"internalType": "uint256[]", "name": "", "type": "uint256[]"}],
        "stateMutability": "view",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "uint256", "name": "nodeId", "type": "uint256"}, {"internalType": "uint256", "name": "blockNum", "type": "uint256"}, {"internalType": "uint256", "name": "txIndex", "type": "uint256"}],
        "name": "getTransaction",
        "outputs": [{"internalType": "uint256", "name": "transactionId", "type": "uint256"}, {"internalType": "uint256", "name": "fromId", "type": "uint256"}, {"internalType": "uint256", "name": "toId", "type": "uint256"}, {"internalType": "uint256", "name": "amount", "type": "uint256"}, {"internalType": "uint256", "name": "timestamp", "type": "uint256"}],
        "stateMutability": "view",
        "type": "function"
    },
    {
        "inputs": [],
        "name": "nodeCount",
        "outputs": [{"internalType": "uint256", "name": "", "type": "uint256"}],
        "stateMutability": "view",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "uint256", "name": "userId", "type": "uint256"}, {"internalType": "uint256", "name": "amount", "type": "uint256"}],
        "name": "setBalance",
        "outputs": [],
        "stateMutability": "nonpayable",
        "type": "function"
    },
    {
        "inputs": [],
        "name": "transactionsPerBlock",
        "outputs": [{"internalType": "uint256", "name": "", "type": "uint256"}],
        "stateMutability": "view",
        "type": "function"
    }
];

// Initialize the dApp
async function init() {
    try {
        // Check if MetaMask is installed
        if (typeof window.ethereum === 'undefined') {
            logEvent('Please install MetaMask to use this dApp', 'error');
            return;
        }

        // Initialize Web3
        web3 = new Web3(window.ethereum);
        
        // Load contract configuration
        await loadContractConfig();
        
        // Set up event listeners
        setupEventListeners();
        
        logEvent('dApp initialized. Please connect your wallet.', 'success');
    } catch (error) {
        console.error('Initialization error:', error);
        logEvent('Error initializing dApp: ' + error.message, 'error');
    }
}

// Load contract configuration from server
async function loadContractConfig() {
    try {
        const response = await fetch('/api/config');
        const config = await response.json();
        contractAddress = config.contractAddress;
        
        if (contractAddress) {
            document.getElementById('contract-address').textContent = contractAddress;
            contract = new web3.eth.Contract(BLOCKCHAIN_NETWORK_ABI, contractAddress);
            await loadNetworkStatus();
        } else {
            document.getElementById('contract-address').textContent = 'Not deployed';
            logEvent('Contract not deployed. Please deploy the contract first.', 'warning');
        }
    } catch (error) {
        console.error('Error loading config:', error);
        logEvent('Error loading contract configuration', 'error');
    }
}

// Connect wallet
async function connectWallet() {
    try {
        const accounts = await window.ethereum.request({ method: 'eth_requestAccounts' });
        userAccount = accounts[0];
        
        // Update UI
        document.getElementById('connect-wallet').style.display = 'none';
        document.getElementById('account-info').classList.remove('hidden');
        document.getElementById('account-address').textContent = 
            userAccount.substring(0, 6) + '...' + userAccount.substring(38);
        
        // Get balance
        const balance = await web3.eth.getBalance(userAccount);
        const ethBalance = web3.utils.fromWei(balance, 'ether');
        document.getElementById('account-balance').textContent = 
            `${parseFloat(ethBalance).toFixed(4)} ETH`;
        
        logEvent('Wallet connected: ' + userAccount, 'success');
        
        // Listen for account changes
        window.ethereum.on('accountsChanged', handleAccountsChanged);
        
    } catch (error) {
        console.error('Error connecting wallet:', error);
        logEvent('Error connecting wallet: ' + error.message, 'error');
    }
}

// Handle account changes
function handleAccountsChanged(accounts) {
    if (accounts.length === 0) {
        logEvent('Please connect to MetaMask', 'warning');
        location.reload();
    } else if (accounts[0] !== userAccount) {
        userAccount = accounts[0];
        location.reload();
    }
}

// Load network status
async function loadNetworkStatus() {
    try {
        if (!contract) return;
        
        const nodeCount = await contract.methods.nodeCount().call();
        const txPerBlock = await contract.methods.transactionsPerBlock().call();
        
        document.getElementById('total-nodes').textContent = nodeCount.toString();
        document.getElementById('tx-per-block').textContent = txPerBlock.toString();
    } catch (error) {
        console.error('Error loading network status:', error);
    }
}

// Get node information
async function getNodeInfo() {
    try {
        const nodeId = document.getElementById('node-id').value;
        if (!nodeId && nodeId !== '0') {
            logEvent('Please enter a node ID', 'warning');
            return;
        }
        
        if (!contract) {
            logEvent('Contract not initialized', 'error');
            return;
        }
        
        const nodeInfo = await contract.methods.getNode(nodeId).call();
        const connections = await contract.methods.getNodeConnections(nodeId).call();
        
        document.getElementById('display-node-id').textContent = nodeInfo.id.toString();
        document.getElementById('display-block-count').textContent = nodeInfo.blockCount.toString();
        document.getElementById('display-active').textContent = nodeInfo.active ? 'Yes' : 'No';
        document.getElementById('display-connections').textContent = 
            connections.length > 0 ? connections.join(', ') : 'None';
        
        document.getElementById('node-info-display').classList.remove('hidden');
        logEvent(`Node ${nodeId} information loaded`, 'success');
    } catch (error) {
        console.error('Error getting node info:', error);
        logEvent('Error: ' + error.message, 'error');
    }
}

// Check balance
async function checkBalance() {
    try {
        const userId = document.getElementById('user-id').value;
        if (!userId && userId !== '0') {
            logEvent('Please enter a user ID', 'warning');
            return;
        }
        
        if (!contract) {
            logEvent('Contract not initialized', 'error');
            return;
        }
        
        const balance = await contract.methods.getBalance(userId).call();
        
        document.getElementById('display-balance').textContent = balance.toString();
        document.getElementById('balance-display').classList.remove('hidden');
        logEvent(`Balance for user ${userId}: ${balance}`, 'success');
    } catch (error) {
        console.error('Error checking balance:', error);
        logEvent('Error: ' + error.message, 'error');
    }
}

// Add transaction
async function addTransaction(event) {
    event.preventDefault();
    
    try {
        if (!userAccount) {
            logEvent('Please connect your wallet first', 'warning');
            return;
        }
        
        if (!contract) {
            logEvent('Contract not initialized', 'error');
            return;
        }
        
        const nodeId = document.getElementById('tx-node').value;
        const fromId = document.getElementById('tx-from').value;
        const toId = document.getElementById('tx-to').value;
        const amount = document.getElementById('tx-amount').value;
        
        logEvent('Sending transaction...', 'info');
        
        const tx = await contract.methods.addTransaction(nodeId, fromId, toId, amount)
            .send({ from: userAccount });
        
        logEvent(`Transaction added! Hash: ${tx.transactionHash}`, 'success');
        
        // Reset form
        event.target.reset();
        
        // Reload network status
        await loadNetworkStatus();
    } catch (error) {
        console.error('Error adding transaction:', error);
        logEvent('Error: ' + error.message, 'error');
    }
}

// Set balance
async function setBalance(event) {
    event.preventDefault();
    
    try {
        if (!userAccount) {
            logEvent('Please connect your wallet first', 'warning');
            return;
        }
        
        if (!contract) {
            logEvent('Contract not initialized', 'error');
            return;
        }
        
        const userId = document.getElementById('balance-user-id').value;
        const amount = document.getElementById('balance-amount').value;
        
        logEvent('Setting balance...', 'info');
        
        const tx = await contract.methods.setBalance(userId, amount)
            .send({ from: userAccount });
        
        logEvent(`Balance set for user ${userId}! Hash: ${tx.transactionHash}`, 'success');
        
        // Reset form
        event.target.reset();
    } catch (error) {
        console.error('Error setting balance:', error);
        logEvent('Error: ' + error.message, 'error');
    }
}

// Add connection
async function addConnection(event) {
    event.preventDefault();
    
    try {
        if (!userAccount) {
            logEvent('Please connect your wallet first', 'warning');
            return;
        }
        
        if (!contract) {
            logEvent('Contract not initialized', 'error');
            return;
        }
        
        const fromNode = document.getElementById('edge-from').value;
        const toNode = document.getElementById('edge-to').value;
        
        logEvent('Adding connection...', 'info');
        
        const tx = await contract.methods.addEdge(fromNode, toNode)
            .send({ from: userAccount });
        
        logEvent(`Connection added: ${fromNode} <-> ${toNode}! Hash: ${tx.transactionHash}`, 'success');
        
        // Reset form
        event.target.reset();
    } catch (error) {
        console.error('Error adding connection:', error);
        logEvent('Error: ' + error.message, 'error');
    }
}

// Explore block
async function exploreBlock() {
    try {
        const nodeId = document.getElementById('explore-node').value;
        const blockNum = document.getElementById('explore-block').value;
        
        if ((!nodeId && nodeId !== '0') || (!blockNum && blockNum !== '0')) {
            logEvent('Please enter both node ID and block number', 'warning');
            return;
        }
        
        if (!contract) {
            logEvent('Contract not initialized', 'error');
            return;
        }
        
        const block = await contract.methods.getBlock(nodeId, blockNum).call();
        
        document.getElementById('display-block-number').textContent = block.blockNumber.toString();
        document.getElementById('display-tx-count').textContent = block.transactionCount.toString();
        document.getElementById('display-prev-hash').textContent = block.previousHash;
        document.getElementById('display-block-hash').textContent = block.blockHash;
        document.getElementById('display-timestamp').textContent = 
            new Date(Number(block.timestamp) * 1000).toLocaleString();
        
        document.getElementById('block-display').classList.remove('hidden');
        logEvent(`Block ${blockNum} from node ${nodeId} loaded`, 'success');
    } catch (error) {
        console.error('Error exploring block:', error);
        logEvent('Error: ' + error.message, 'error');
    }
}

// Log event to the event log
function logEvent(message, type = 'info') {
    const eventLog = document.getElementById('event-log');
    const noEvents = eventLog.querySelector('.no-events');
    
    if (noEvents) {
        noEvents.remove();
    }
    
    const eventItem = document.createElement('div');
    eventItem.className = `event-item ${type}`;
    
    const eventTime = document.createElement('div');
    eventTime.className = 'event-time';
    eventTime.textContent = new Date().toLocaleTimeString();
    
    const eventMessage = document.createElement('div');
    eventMessage.className = 'event-message';
    eventMessage.textContent = message;
    
    eventItem.appendChild(eventTime);
    eventItem.appendChild(eventMessage);
    
    eventLog.insertBefore(eventItem, eventLog.firstChild);
    
    // Limit to 50 events
    while (eventLog.children.length > 50) {
        eventLog.removeChild(eventLog.lastChild);
    }
}

// Set up event listeners
function setupEventListeners() {
    document.getElementById('connect-wallet').addEventListener('click', connectWallet);
    document.getElementById('get-node-info').addEventListener('click', getNodeInfo);
    document.getElementById('check-balance').addEventListener('click', checkBalance);
    document.getElementById('transaction-form').addEventListener('submit', addTransaction);
    document.getElementById('balance-form').addEventListener('submit', setBalance);
    document.getElementById('connection-form').addEventListener('submit', addConnection);
    document.getElementById('explore-block-btn').addEventListener('click', exploreBlock);
}

// Initialize when page loads
window.addEventListener('load', init);
