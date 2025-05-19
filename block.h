#pragma once
#include <iostream>
#include <vector>
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
public:
    block();
    block(int bNumber, int maxTransactions);
    void inseartTran(transaction t);

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
    void displayTransctions(vector<int> &idN, vector<int> &idV);
    bool searchTrans(int id);
};
