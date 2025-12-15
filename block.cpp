#include "block.h"
#include "sha256.h"
#include <iostream>
#include <stdexcept>

block::block()
    : blockNumber(0),
      currentNumTransactions(0),
      maxNumTransactions(10),
      bTransactions(),
      hash(),
      prevHash() {}

block::block(int bNumber, int maxTransactions)
    : blockNumber(bNumber),
      currentNumTransactions(0),
      maxNumTransactions(maxTransactions),
      bTransactions(),
      hash(),
      prevHash() {}

void block::computeHash() {
    hash = calculateHash();
}

std::string block::calculateHash() const {
    std::string data;
    data.reserve(prevHash.size() + 32U + (bTransactions.size() * 64U));
    data.append(prevHash);
    data.push_back('|');
    data.append(std::to_string(blockNumber));
    for (const auto &t : bTransactions) {
        data.push_back('|');
        data += std::to_string(t.getTNodeNum());
        data.push_back(',');
        data += std::to_string(t.getTranID());
        data.push_back(',');
        data += std::to_string(t.getFromID());
        data.push_back(',');
        data += std::to_string(t.getFromValue());
        data.push_back(',');
        data += std::to_string(t.getToID());
        data.push_back(',');
        data += std::to_string(t.getToValue());
        data.push_back(',');
        data += std::to_string(t.getTranAmount());
        data.push_back(',');
        data += t.getTimeStamp();
    }
    return crypto::sha256_hex(data);
}

std::string block::getHash() const {
    return hash;
}

void block::setPrevHash(const std::string &pHash) {
    prevHash = pHash;
}

std::string block::getPrevHash() const {
    return prevHash;
}

void block::insertTran(const transaction &t) {
    bTransactions.push_back(t);
    currentNumTransactions = static_cast<int>(bTransactions.size());
}

void block::setBlockNumber(int bN) {
    blockNumber = bN;
}

void block::setCurrNumTran(int cnt) {
    currentNumTransactions = cnt;
}

void block::setMaxNumTran(int mnt) {
    maxNumTransactions = mnt;
}

int block::getBlockNumber() const {
    return blockNumber;
}

int block::getCurrNumTran() const {
    return currentNumTransactions;
}

int block::getMaxNumTran() const {
    return maxNumTransactions;
}

const transaction &block::getTran(std::size_t index) const {
    if (index >= bTransactions.size()) {
        throw std::out_of_range("transaction index out of range");
    }
    return bTransactions[index];
}

void block::displayTransactions(std::ostream &out) const {
    for (const auto &tran : bTransactions) {
        tran.displayTransaction(out);
    }
}

void block::displayTransactions() const {
    displayTransactions(std::cout);
}

bool block::searchTrans(int id) const {
    for (std::size_t i = 0; i < bTransactions.size(); ++i) {
        if (bTransactions[i].getTranID() == id) {
            return true;
        }
    }
    return false;
}
