#pragma once
#include <iostream>
#include <list>
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

    void setCurrNumBlocks(int cnb);
    void setNodeNum(int node);

    int getCurrNumBlocks();
    block getBack();
    block getFront();
    int getNodeNum();

    void displayTrans(int bNum, vector<int> &idNum, vector<int> &idVal);
    int searchID(int ID);
};
