const hre = require("hardhat");

async function main() {
  console.log("Deploying BlockchainNetwork contract...");

  // Default parameters
  const nodeCount = 5;
  const transactionsPerBlock = 3;

  const BlockchainNetwork = await hre.ethers.getContractFactory("BlockchainNetwork");
  const blockchainNetwork = await BlockchainNetwork.deploy(nodeCount, transactionsPerBlock);

  await blockchainNetwork.waitForDeployment();

  const contractAddress = await blockchainNetwork.getAddress();
  console.log(`BlockchainNetwork deployed to: ${contractAddress}`);
  console.log(`- Node Count: ${nodeCount}`);
  console.log(`- Transactions Per Block: ${transactionsPerBlock}`);

  // Initialize some sample data
  console.log("\nInitializing sample network...");
  
  // Add edges (connections between nodes)
  const edges = [
    [0, 1], [0, 2], [1, 3], [2, 3], [3, 4]
  ];
  
  for (const [u, v] of edges) {
    const tx = await blockchainNetwork.addEdge(u, v);
    await tx.wait();
    console.log(`Connected node ${u} <-> ${v}`);
  }

  // Set initial balances
  const initialBalances = [
    [1, 1000],
    [2, 500],
    [3, 750],
    [4, 300],
    [5, 1200]
  ];

  for (const [userId, balance] of initialBalances) {
    const tx = await blockchainNetwork.setBalance(userId, balance);
    await tx.wait();
    console.log(`Set balance for user ${userId}: ${balance}`);
  }

  console.log("\nDeployment and initialization complete!");
  console.log("\nSave this contract address to your .env file:");
  console.log(`CONTRACT_ADDRESS=${contractAddress}`);

  return contractAddress;
}

main()
  .then(() => process.exit(0))
  .catch((error) => {
    console.error(error);
    process.exit(1);
  });
