#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "transaction.h"
using namespace std;

class block
{
    int blockNumber;
    int currentNumTransactions;
    int maxNumTransactions;
    vector<transaction> bTransactions;
    block* nextBlock;
    int newFromValue;
    int newToValue;
    int numNodesInNetwork;
    std::string hash;
    std::string prevHash;
public:
    block();
    block(int bNumber, int maxTransactions);
    void inseartTran(transaction t);

    void computeHash();
    std::string getHash() const;
    void setPrevHash(const std::string &pHash);
    std::string getPrevHash() const;
    std::string calculateHash() const;

    void setBlockNumber(int bN);
    void setCurrNumTran(int cnt);
    void setMaxNumTran(int mnt);
    void setNextBlock(block* bA);
    void setNumNodesInNetwork(int numNodes);

    int getBlockNumber();
    int getCurrNumTran();
    int getMaxNumTran();
    block* getNextBlock();
    int getNumNodesInNetwork();

    transaction getTran(int index);
    void displayTransctions(std::unordered_map<int, int> &balances);
    bool searchTrans(int id);
};
