const express = require('express');
const cors = require('cors');
const path = require('path');
require('dotenv').config();

const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// Serve static files
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// API endpoint to get contract configuration
app.get('/api/config', (req, res) => {
  res.json({
    contractAddress: process.env.CONTRACT_ADDRESS || '',
    networkId: 1337, // Hardhat local network
    rpcUrl: 'http://127.0.0.1:8545'
  });
});

// Health check endpoint
app.get('/api/health', (req, res) => {
  res.json({ status: 'ok', timestamp: new Date().toISOString() });
});

app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
  console.log(`Make sure Hardhat node is running: npx hardhat node`);
});
