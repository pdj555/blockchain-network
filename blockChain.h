#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include "block.h"
using namespace std;

class blockChain
{
    int currentNumBlocks;
    list<block> bChain;
    int nodeNum;
public:
    blockChain();
    blockChain(int tPerB);
    void insertTran(const transaction &t);
    void insertBlockFront(block b);

    bool verifyChain() const;

    // Intentionally modify the prevHash of a block for testing
    void tamperPrevHash(size_t index, const std::string &newPrev);

    void setCurrNumBlocks(int cnb);
    void setNodeNum(int node);

    int getCurrNumBlocks();
    block getBack();
    block getFront();
    int getNodeNum();

    void displayTrans(int bNum, std::unordered_map<int, int> &balances);
    int searchID(int ID);
};
