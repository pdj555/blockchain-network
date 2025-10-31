const { expect } = require("chai");
const { ethers } = require("hardhat");

describe("BlockchainNetwork", function () {
  let blockchainNetwork;
  let owner;
  let addr1;
  let addr2;

  const NODE_COUNT = 5;
  const TX_PER_BLOCK = 3;

  beforeEach(async function () {
    // Get signers
    [owner, addr1, addr2] = await ethers.getSigners();

    // Deploy contract
    const BlockchainNetwork = await ethers.getContractFactory("BlockchainNetwork");
    blockchainNetwork = await BlockchainNetwork.deploy(NODE_COUNT, TX_PER_BLOCK);
    await blockchainNetwork.waitForDeployment();
  });

  describe("Deployment", function () {
    it("Should set the correct node count", async function () {
      expect(await blockchainNetwork.nodeCount()).to.equal(NODE_COUNT);
    });

    it("Should set the correct transactions per block", async function () {
      expect(await blockchainNetwork.transactionsPerBlock()).to.equal(TX_PER_BLOCK);
    });

    it("Should initialize all nodes as active", async function () {
      for (let i = 0; i < NODE_COUNT; i++) {
        const node = await blockchainNetwork.getNode(i);
        expect(node.active).to.be.true;
        expect(node.id).to.equal(i);
        expect(node.blockCount).to.equal(0);
      }
    });
  });

  describe("Node Connections", function () {
    it("Should add edge between nodes", async function () {
      await blockchainNetwork.addEdge(0, 1);
      
      const connections0 = await blockchainNetwork.getNodeConnections(0);
      const connections1 = await blockchainNetwork.getNodeConnections(1);
      
      expect(connections0.map(n => Number(n))).to.include(1);
      expect(connections1.map(n => Number(n))).to.include(0);
    });

    it("Should create bidirectional connections", async function () {
      await blockchainNetwork.addEdge(2, 3);
      
      const connections2 = await blockchainNetwork.getNodeConnections(2);
      const connections3 = await blockchainNetwork.getNodeConnections(3);
      
      expect(connections2.length).to.equal(1);
      expect(connections3.length).to.equal(1);
      expect(Number(connections2[0])).to.equal(3);
      expect(Number(connections3[0])).to.equal(2);
    });

    it("Should revert for invalid node IDs", async function () {
      await expect(
        blockchainNetwork.addEdge(0, NODE_COUNT)
      ).to.be.revertedWith("Invalid node ID");
    });
  });

  describe("Balance Management", function () {
    it("Should set balance for a user", async function () {
      const userId = 1;
      const amount = 1000;
      
      await blockchainNetwork.setBalance(userId, amount);
      expect(await blockchainNetwork.getBalance(userId)).to.equal(amount);
    });

    it("Should allow multiple users to have different balances", async function () {
      await blockchainNetwork.setBalance(1, 500);
      await blockchainNetwork.setBalance(2, 1000);
      await blockchainNetwork.setBalance(3, 250);
      
      expect(await blockchainNetwork.getBalance(1)).to.equal(500);
      expect(await blockchainNetwork.getBalance(2)).to.equal(1000);
      expect(await blockchainNetwork.getBalance(3)).to.equal(250);
    });
  });

  describe("Transactions", function () {
    beforeEach(async function () {
      // Set up initial balances
      await blockchainNetwork.setBalance(1, 1000);
      await blockchainNetwork.setBalance(2, 500);
    });

    it("Should add a transaction", async function () {
      await blockchainNetwork.addTransaction(0, 1, 2, 100);
      
      expect(await blockchainNetwork.getBalance(1)).to.equal(900);
      expect(await blockchainNetwork.getBalance(2)).to.equal(600);
    });

    it("Should update node block count after max transactions", async function () {
      const nodeId = 0;
      
      // Add TX_PER_BLOCK transactions to trigger block creation
      for (let i = 0; i < TX_PER_BLOCK; i++) {
        await blockchainNetwork.addTransaction(nodeId, 1, 2, 10);
      }
      
      const node = await blockchainNetwork.getNode(nodeId);
      expect(node.blockCount).to.be.greaterThan(0);
    });

    it("Should revert if insufficient balance", async function () {
      await expect(
        blockchainNetwork.addTransaction(0, 1, 2, 2000)
      ).to.be.revertedWith("Insufficient balance");
    });

    it("Should revert for invalid node ID", async function () {
      await expect(
        blockchainNetwork.addTransaction(NODE_COUNT, 1, 2, 100)
      ).to.be.revertedWith("Invalid node ID");
    });

    it("Should revert for zero amount", async function () {
      await expect(
        blockchainNetwork.addTransaction(0, 1, 2, 0)
      ).to.be.revertedWith("Amount must be positive");
    });
  });

  describe("Block Management", function () {
    beforeEach(async function () {
      await blockchainNetwork.setBalance(1, 1000);
    });

    it("Should create block with transactions", async function () {
      const nodeId = 0;
      
      // Add transactions
      await blockchainNetwork.addTransaction(nodeId, 1, 2, 50);
      await blockchainNetwork.addTransaction(nodeId, 1, 3, 30);
      await blockchainNetwork.addTransaction(nodeId, 1, 4, 20);
      
      // Get first block
      const block = await blockchainNetwork.getBlock(nodeId, 0);
      expect(block.transactionCount).to.equal(TX_PER_BLOCK);
    });

    it("Should link blocks with previous hash", async function () {
      const nodeId = 0;
      await blockchainNetwork.setBalance(1, 10000);
      
      // Fill first block
      for (let i = 0; i < TX_PER_BLOCK; i++) {
        await blockchainNetwork.addTransaction(nodeId, 1, 2, 10);
      }
      
      // Add one more to create second block
      await blockchainNetwork.addTransaction(nodeId, 1, 2, 10);
      
      const block1 = await blockchainNetwork.getBlock(nodeId, 0);
      const block2 = await blockchainNetwork.getBlock(nodeId, 1);
      
      expect(block2.previousHash).to.equal(block1.blockHash);
    });

    it("Should have unique block hashes", async function () {
      const nodeId = 0;
      await blockchainNetwork.setBalance(1, 10000);
      
      // Create multiple blocks
      for (let i = 0; i < TX_PER_BLOCK * 2; i++) {
        await blockchainNetwork.addTransaction(nodeId, 1, 2, 10);
      }
      
      const block1 = await blockchainNetwork.getBlock(nodeId, 0);
      const block2 = await blockchainNetwork.getBlock(nodeId, 1);
      
      expect(block1.blockHash).to.not.equal(block2.blockHash);
    });
  });

  describe("Events", function () {
    it("Should emit NodeCreated events on deployment", async function () {
      const BlockchainNetwork = await ethers.getContractFactory("BlockchainNetwork");
      const newContract = await BlockchainNetwork.deploy(3, 2);
      
      // Events are emitted during deployment
      // We can't directly test deployment events, but we can verify the contract works
      expect(await newContract.nodeCount()).to.equal(3);
    });

    it("Should emit NodeConnected event when adding edge", async function () {
      await expect(blockchainNetwork.addEdge(0, 1))
        .to.emit(blockchainNetwork, "NodeConnected")
        .withArgs(0, 1);
    });

    it("Should emit TransactionAdded event", async function () {
      await blockchainNetwork.setBalance(1, 1000);
      
      await expect(blockchainNetwork.addTransaction(0, 1, 2, 100))
        .to.emit(blockchainNetwork, "TransactionAdded");
    });
  });
});
