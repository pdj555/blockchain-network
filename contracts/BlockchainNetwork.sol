// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

/**
 * @title BlockchainNetwork
 * @dev Implements a decentralized blockchain network with transactions and nodes
 */
contract BlockchainNetwork {
    
    struct Transaction {
        uint256 transactionId;
        uint256 fromId;
        uint256 toId;
        uint256 amount;
        uint256 timestamp;
        uint256 nodeNum;
    }
    
    struct Block {
        uint256 blockNumber;
        Transaction[] transactions;
        bytes32 previousHash;
        bytes32 blockHash;
        uint256 timestamp;
        uint256 nodeId;
    }
    
    struct Node {
        uint256 nodeId;
        uint256 blockCount;
        bool active;
    }
    
    // Storage
    mapping(uint256 => Node) public nodes;
    mapping(uint256 => mapping(uint256 => Block)) public nodeBlocks; // nodeId => blockNumber => Block
    mapping(uint256 => uint256[]) public nodeConnections; // nodeId => connected node IDs
    mapping(uint256 => uint256) public balances; // user ID => balance
    
    uint256 public nodeCount;
    uint256 public transactionsPerBlock;
    uint256 private transactionCounter;
    
    // Events
    event NodeCreated(uint256 indexed nodeId);
    event NodeConnected(uint256 indexed fromNode, uint256 indexed toNode);
    event TransactionAdded(uint256 indexed nodeId, uint256 indexed transactionId, uint256 from, uint256 to, uint256 amount);
    event BlockCreated(uint256 indexed nodeId, uint256 indexed blockNumber, bytes32 blockHash);
    
    constructor(uint256 _nodeCount, uint256 _transactionsPerBlock) {
        require(_nodeCount > 0, "Node count must be positive");
        require(_transactionsPerBlock > 0, "Transactions per block must be positive");
        
        nodeCount = _nodeCount;
        transactionsPerBlock = _transactionsPerBlock;
        
        // Initialize nodes
        for (uint256 i = 0; i < _nodeCount; i++) {
            nodes[i] = Node({
                nodeId: i,
                blockCount: 0,
                active: true
            });
            emit NodeCreated(i);
        }
    }
    
    /**
     * @dev Add a connection between two nodes
     */
    function addEdge(uint256 fromNode, uint256 toNode) public {
        require(fromNode < nodeCount && toNode < nodeCount, "Invalid node ID");
        require(nodes[fromNode].active && nodes[toNode].active, "Node not active");
        
        nodeConnections[fromNode].push(toNode);
        nodeConnections[toNode].push(fromNode); // Bidirectional connection
        
        emit NodeConnected(fromNode, toNode);
    }
    
    /**
     * @dev Add a transaction to a specific node
     */
    function addTransaction(
        uint256 nodeId,
        uint256 fromId,
        uint256 toId,
        uint256 amount
    ) public {
        require(nodeId < nodeCount, "Invalid node ID");
        require(nodes[nodeId].active, "Node not active");
        require(amount > 0, "Amount must be positive");
        require(balances[fromId] >= amount, "Insufficient balance");
        
        transactionCounter++;
        
        uint256 blockNum = nodes[nodeId].blockCount;
        Block storage currentBlock = nodeBlocks[nodeId][blockNum];
        
        // Create new block if needed
        if (currentBlock.transactions.length == 0 || currentBlock.transactions.length >= transactionsPerBlock) {
            if (currentBlock.transactions.length >= transactionsPerBlock) {
                _finalizeBlock(nodeId, blockNum);
                blockNum++;
                nodes[nodeId].blockCount = blockNum;
            }
            currentBlock = nodeBlocks[nodeId][blockNum];
            currentBlock.blockNumber = blockNum;
            currentBlock.nodeId = nodeId;
            currentBlock.timestamp = block.timestamp;
            
            if (blockNum > 0) {
                currentBlock.previousHash = nodeBlocks[nodeId][blockNum - 1].blockHash;
            }
        }
        
        // Add transaction
        Transaction memory newTrans = Transaction({
            transactionId: transactionCounter,
            fromId: fromId,
            toId: toId,
            amount: amount,
            timestamp: block.timestamp,
            nodeNum: nodeId
        });
        
        currentBlock.transactions.push(newTrans);
        
        // Update balances
        balances[fromId] -= amount;
        balances[toId] += amount;
        
        emit TransactionAdded(nodeId, transactionCounter, fromId, toId, amount);
    }
    
    /**
     * @dev Finalize a block by computing its hash
     */
    function _finalizeBlock(uint256 nodeId, uint256 blockNum) private {
        Block storage blk = nodeBlocks[nodeId][blockNum];
        
        bytes32 hash = keccak256(abi.encodePacked(
            blk.blockNumber,
            blk.previousHash,
            blk.timestamp,
            blk.nodeId,
            blk.transactions.length
        ));
        
        blk.blockHash = hash;
        emit BlockCreated(nodeId, blockNum, hash);
    }
    
    /**
     * @dev Set initial balance for a user
     */
    function setBalance(uint256 userId, uint256 amount) public {
        balances[userId] = amount;
    }
    
    /**
     * @dev Get balance of a user
     */
    function getBalance(uint256 userId) public view returns (uint256) {
        return balances[userId];
    }
    
    /**
     * @dev Get connections for a node
     */
    function getNodeConnections(uint256 nodeId) public view returns (uint256[] memory) {
        require(nodeId < nodeCount, "Invalid node ID");
        return nodeConnections[nodeId];
    }
    
    /**
     * @dev Get block details
     */
    function getBlock(uint256 nodeId, uint256 blockNum) public view returns (
        uint256 blockNumber,
        uint256 transactionCount,
        bytes32 previousHash,
        bytes32 blockHash,
        uint256 timestamp
    ) {
        require(nodeId < nodeCount, "Invalid node ID");
        Block storage blk = nodeBlocks[nodeId][blockNum];
        
        return (
            blk.blockNumber,
            blk.transactions.length,
            blk.previousHash,
            blk.blockHash,
            blk.timestamp
        );
    }
    
    /**
     * @dev Get transaction from a block
     */
    function getTransaction(uint256 nodeId, uint256 blockNum, uint256 txIndex) public view returns (
        uint256 transactionId,
        uint256 fromId,
        uint256 toId,
        uint256 amount,
        uint256 timestamp
    ) {
        require(nodeId < nodeCount, "Invalid node ID");
        Block storage blk = nodeBlocks[nodeId][blockNum];
        require(txIndex < blk.transactions.length, "Invalid transaction index");
        
        Transaction storage tx = blk.transactions[txIndex];
        return (tx.transactionId, tx.fromId, tx.toId, tx.amount, tx.timestamp);
    }
    
    /**
     * @dev Get node information
     */
    function getNode(uint256 nodeId) public view returns (
        uint256 id,
        uint256 blockCount,
        bool active
    ) {
        require(nodeId < nodeCount, "Invalid node ID");
        Node storage node = nodes[nodeId];
        return (node.nodeId, node.blockCount, node.active);
    }
}
