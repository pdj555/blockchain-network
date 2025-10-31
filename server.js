const express = require('express');
const cors = require('cors');
const path = require('path');
require('dotenv').config();

const app = express();
const PORT = process.env.PORT || 3000;

// Simple in-memory rate limiting middleware
const rateLimitMap = new Map();
const RATE_LIMIT_WINDOW = 60000; // 1 minute
const MAX_REQUESTS = 100; // Max requests per window

const rateLimit = (req, res, next) => {
  const clientId = req.ip || 'unknown';
  const now = Date.now();
  
  if (!rateLimitMap.has(clientId)) {
    rateLimitMap.set(clientId, { count: 1, resetTime: now + RATE_LIMIT_WINDOW });
    return next();
  }
  
  const clientData = rateLimitMap.get(clientId);
  
  if (now > clientData.resetTime) {
    // Reset the window
    clientData.count = 1;
    clientData.resetTime = now + RATE_LIMIT_WINDOW;
    return next();
  }
  
  if (clientData.count >= MAX_REQUESTS) {
    return res.status(429).json({ 
      error: 'Too many requests',
      message: 'Please try again later'
    });
  }
  
  clientData.count++;
  next();
};

// Middleware
app.use(cors());
app.use(express.json());
app.use(rateLimit); // Apply rate limiting to all routes
app.use(express.static('public'));

// Serve static files
// Note: This route is protected by the rate limiting middleware applied globally above
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
