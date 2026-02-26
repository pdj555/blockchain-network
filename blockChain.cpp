#include "blockChain.h"
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
    balances.emplace(annotated.getFromID(), 100);
    balances.emplace(annotated.getToID(), 100);

    const int fromID = annotated.getFromID();
    const int toID = annotated.getToID();
    const int amount = annotated.getTranAmount();

    int &fromBalance = balances[fromID];
    int &toBalance = balances[toID];

    const int transactionId = annotated.getTranID();
    if (transactionIds.find(transactionId) != transactionIds.end()) {
        ++rejectedTransactions;
        if (logStream != nullptr) {
            *logStream << "Rejected transaction (duplicate id) in node " << nodeNum << std::endl;
        }
        return false;
    }

    if (amount > fromBalance) {
        ++rejectedTransactions;
        if (logStream != nullptr) {
            *logStream << "Rejected transaction (insufficient funds) in node " << nodeNum << std::endl;
        }
        return false;
    }

    annotated.setFromValue(fromBalance);
    annotated.setToValue(toBalance);

    fromBalance -= amount;
    toBalance += amount;

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

    transactionIds.insert(transactionId);

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
