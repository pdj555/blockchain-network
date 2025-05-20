#include "block.h"

block::block() {
    blockNumber = 0;
    maxNumTransactions = 10;
    currentNumTransactions = 0;
}

block::block(int bNumber, int maxTransactions) {
    blockNumber = bNumber;
    maxNumTransactions = maxTransactions;
    currentNumTransactions = 0;
}

void block::computeHash() {
    hash = calculateHash();
}

std::string block::calculateHash() const {
    std::string data = prevHash + std::to_string(blockNumber);
    for (const auto &t : bTransactions) {
        data += std::to_string(t.getTranID());
        data += std::to_string(t.getFromID());
        data += std::to_string(t.getToID());
        data += std::to_string(t.getTranAmount());
        data += t.getTimeStamp();
    }
    return std::to_string(std::hash<std::string>{}(data));
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

void block::inseartTran(transaction t) {
    bTransactions.push_back(t);
    currentNumTransactions++;
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

void block::setNextBlock(block* bA) {
    nextBlock = bA;
}

void block::setNumNodesInNetwork(int numNodes) {
    numNodesInNetwork = numNodes;
}

int block::getBlockNumber() {
    return blockNumber;
}

int block::getCurrNumTran() {
    return currentNumTransactions;
}

int block::getMaxNumTran() {
    return maxNumTransactions;
}

block* block::getNextBlock() {
    return nextBlock;
}

int block::getNumNodesInNetwork() {
    return numNodesInNetwork;
}

transaction block::getTran(int index) {
    return bTransactions[index];
}

void block::displayTransctions(vector<int> &idN, vector<int> &idV) {
    int newFromVal;
    int newToVal;
    for (int i = 0; i < bTransactions.size(); i++) {
        int fIdx = -1;
        int tIdx = -1;
        for (int j = 0; j < idN.size(); j++) {
            if (idN[j] == bTransactions[i].getFromID()) {
                fIdx = j;
            }
            if (idN[j] == bTransactions[i].getToID()) {
                tIdx = j;
            }
        }
        if (fIdx != -1 && tIdx != -1) {
            newFromVal = idV[fIdx]-bTransactions[i].getTranAmount();
            newToVal = idV[tIdx]+bTransactions[i].getTranAmount();
            idN[fIdx] = bTransactions[i].getFromID();
            idV[fIdx] = newFromVal;
            idN[tIdx] = bTransactions[i].getToID();
            idV[tIdx] = newToVal;
            bTransactions[i].displayTransaction(newFromVal, newToVal);
        }
        else if (fIdx != -1 && tIdx == -1) {
            newFromVal = idV[fIdx]-bTransactions[i].getTranAmount();
            newToVal = 100+bTransactions[i].getTranAmount();
            idN[fIdx] = bTransactions[i].getFromID();
            idV[fIdx] = newFromVal;
            idN.push_back(bTransactions[i].getToID());
            idV.push_back(newToVal);
            bTransactions[i].displayTransaction(newFromVal, newToVal);
        }
        else if (fIdx == -1 && tIdx != -1) {
            newFromVal = 100-bTransactions[i].getTranAmount();
            newToVal = idV[tIdx]+bTransactions[i].getTranAmount();
            idN.push_back(bTransactions[i].getFromID());
            idV.push_back(newFromVal);
            idN[tIdx] = bTransactions[i].getToID();
            idV[tIdx] = newToVal;
            bTransactions[i].displayTransaction(newFromVal, newToVal);
        }
        else {
            newFromVal = 100-bTransactions[i].getTranAmount();
            newToVal = 100+bTransactions[i].getTranAmount();
            idN.push_back(bTransactions[i].getFromID());
            idV.push_back(newFromVal);
            idN.push_back(bTransactions[i].getToID());
            idV.push_back(newToVal);
            bTransactions[i].displayTransaction(newFromVal, newToVal);
        }
    }
}

bool block::searchTrans(int id) {
    bool find = false;
    for (int i = 0; i < bTransactions.size(); i++) {
        if (bTransactions[i].getTranID() == id) {
            find = true;
        }
    }
    return find;
}


