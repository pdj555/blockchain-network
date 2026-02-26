#include "blockChain.h"
#include "sha256.h"
#include <iostream>
#include <utility>

blockChain::blockChain()
    : currentNumBlocks(0),
      bChain(),
      nodeNum(0),
      logStream(&std::cout),
      maxTransactionsPerBlock(10),
      balances(),
      transactionIds(),
      rejectedTransactions(0) {}

blockChain::blockChain(int tPerB)
    : currentNumBlocks(0),
      bChain(),
      nodeNum(0),
      logStream(&std::cout),
      maxTransactionsPerBlock(tPerB > 0 ? tPerB : 1),
      balances(),
      transactionIds(),
      rejectedTransactions(0) {
    bChain.push_front(block(1, maxTransactionsPerBlock));
    currentNumBlocks = 1;
    bChain.front().setPrevHash("0");
    bChain.front().computeHash();
}

bool blockChain::canAcceptTran(const transaction &t) const {
    const int transactionId = t.getTranID();
    if (transactionIds.find(transactionId) != transactionIds.end()) {
        return false;
    }

    if (transactionId < 0 || t.getFromID() < 0 || t.getToID() < 0) {
        return false;
    }

    const int fromID = t.getFromID();
    const int amount = t.getTranAmount();
    if (amount <= 0) {
        return false;
    }

    int fromBalance = 100;
    const auto fromIt = balances.find(fromID);
    if (fromIt != balances.end()) {
        fromBalance = fromIt->second;
    }

    return amount <= fromBalance;
}

bool blockChain::insertTran(const transaction &t) {
    if (bChain.empty()) {
        bChain.push_front(block(1, maxTransactionsPerBlock));
        currentNumBlocks = 1;
        bChain.front().setPrevHash("0");
        bChain.front().computeHash();
        balances.clear();
        transactionIds.clear();
        rejectedTransactions = 0;
    }

    transaction annotated = t;
    annotated.setTNodeNum(nodeNum);

    const int fromID = annotated.getFromID();
    const int toID = annotated.getToID();
    const int amount = annotated.getTranAmount();

    int fromBalance = 100;
    const auto fromIt = balances.find(fromID);
    if (fromIt != balances.end()) {
        fromBalance = fromIt->second;
    }

    int toBalance = 100;
    const auto toIt = balances.find(toID);
    if (toIt != balances.end()) {
        toBalance = toIt->second;
    }

    if (!canAcceptTran(annotated)) {
        ++rejectedTransactions;
        if (logStream != nullptr) {
            const bool duplicateId = transactionIds.find(annotated.getTranID()) != transactionIds.end();
            const bool invalidFields = annotated.getTranID() < 0 || annotated.getFromID() < 0 ||
                                      annotated.getToID() < 0 || annotated.getTranAmount() <= 0;
            if (duplicateId) {
                *logStream << "Rejected transaction (duplicate id) in node " << nodeNum << std::endl;
            } else if (invalidFields) {
                *logStream << "Rejected transaction (invalid fields) in node " << nodeNum << std::endl;
            } else {
                *logStream << "Rejected transaction (insufficient funds) in node " << nodeNum << std::endl;
            }
        }
        return false;
    }

    annotated.setFromValue(fromBalance);
    annotated.setToValue(toBalance);

    balances[fromID] = fromBalance - amount;
    balances[toID] = toBalance + amount;

    if (bChain.front().getCurrNumTran() >= maxTransactionsPerBlock) {
        const int newBlockNumber = currentNumBlocks + 1;
        block newBlock(newBlockNumber, maxTransactionsPerBlock);
        newBlock.setPrevHash(bChain.front().getHash());
        newBlock.insertTran(annotated);
        newBlock.computeHash();
        insertBlockFront(std::move(newBlock));
    } else {
        bChain.front().insertTran(annotated);
        bChain.front().computeHash();
    }

    transactionIds.insert(annotated.getTranID());

    if (logStream != nullptr) {
        *logStream << "Inserting transaction to block #" << currentNumBlocks
                   << " in node " << annotated.getTNodeNum() << std::endl;
    }
    return true;
}

void blockChain::insertBlockFront(block b) {
    bChain.push_front(std::move(b));
    currentNumBlocks = static_cast<int>(bChain.size());
}

bool blockChain::verifyChain() const {
    std::string prev = "0";
    for (auto it = bChain.crbegin(); it != bChain.crend(); ++it) {
        if (it->calculateHash() != it->getHash()) {
            return false;
        }
        if (it->getPrevHash() != prev) {
            return false;
        }
        prev = it->getHash();
    }
    return true;
}

void blockChain::tamperPrevHash(std::size_t index, const std::string &newPrev) {
    auto it = bChain.begin();
    std::size_t i = 0;
    while (i < index && it != bChain.end()) {
        ++it;
        ++i;
    }
    if (it != bChain.end()) {
        it->setPrevHash(newPrev);
    }
}

void blockChain::setLogStream(std::ostream *out) {
    logStream = out;
}

void blockChain::setCurrNumBlocks(int cnb) {
    currentNumBlocks = cnb;
}

void blockChain::setNodeNum(int node) {
    nodeNum = node;
}

int blockChain::getCurrNumBlocks() const {
    return currentNumBlocks;
}

int blockChain::getTotalTransactions() const {
    int total = 0;
    for (const auto &blk : bChain) {
        total += blk.getCurrNumTran();
    }
    return total;
}

int blockChain::getRejectedTransactions() const {
    return rejectedTransactions;
}

std::string blockChain::getTipHash() const {
    if (bChain.empty()) {
        return "";
    }
    return bChain.front().getHash();
}

std::string blockChain::getConsensusFingerprint() const {
    std::string previousBlockFingerprint = "0";
    for (auto it = bChain.crbegin(); it != bChain.crend(); ++it) {
        std::string blockData = previousBlockFingerprint;
        blockData.push_back('|');
        blockData += std::to_string(it->getBlockNumber());

        const int transactionCount = it->getCurrNumTran();
        for (int txIndex = 0; txIndex < transactionCount; ++txIndex) {
            const transaction &tx = it->getTran(static_cast<std::size_t>(txIndex));
            blockData.push_back('|');
            blockData += std::to_string(tx.getTranID());
            blockData.push_back(',');
            blockData += std::to_string(tx.getFromID());
            blockData.push_back(',');
            blockData += std::to_string(tx.getFromValue());
            blockData.push_back(',');
            blockData += std::to_string(tx.getToID());
            blockData.push_back(',');
            blockData += std::to_string(tx.getToValue());
            blockData.push_back(',');
            blockData += std::to_string(tx.getTranAmount());
            blockData.push_back(',');
            blockData += tx.getTimeStamp();
        }

        previousBlockFingerprint = crypto::sha256_hex(blockData);
    }
    return previousBlockFingerprint;
}

const block &blockChain::getBack() const {
    return bChain.back();
}

const block &blockChain::getFront() const {
    return bChain.front();
}

int blockChain::getNodeNum() const {
    return nodeNum;
}

const block *blockChain::getBlockFromOldest(int blockNumber) const {
    if (blockNumber <= 0) {
        return nullptr;
    }

    int currBlockNum = 1;
    for (auto it = bChain.crbegin(); it != bChain.crend(); ++it) {
        if (currBlockNum == blockNumber) {
            return &(*it);
        }
        ++currBlockNum;
    }
    return nullptr;
}

bool blockChain::searchID(int transactionId) const {
    for (const auto &blk : bChain) {
        if (blk.searchTrans(transactionId)) {
            return true;
        }
    }
    return false;
}
