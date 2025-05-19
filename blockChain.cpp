#include "blockChain.h"

blockChain::blockChain() {
    currentNumBlocks = 0;
}

blockChain::blockChain(int tPerB) {
    bChain.push_front(block(currentNumBlocks, tPerB));
    currentNumBlocks = 1;
}

void blockChain::insertTran(transaction* t) {
    if (bChain.empty() || bChain.front().getCurrNumTran() == bChain.front().getMaxNumTran()) {
        block nB(currentNumBlocks, bChain.front().getMaxNumTran());
        nB.inseartTran(*t);
        insertBlockFront(nB);
        cout << "Inserting transaction to block #" << currentNumBlocks << " in node " << t->getTNodeNum() << endl;
    } else {
        bChain.front().inseartTran(*t);
        cout << "Inserting transaction to block #" << currentNumBlocks << " in node " << t->getTNodeNum() << endl;
    }
}

void blockChain::insertBlockFront(block b) {
    b.setNextBlock(&bChain.front());
    bChain.push_front(b);
    currentNumBlocks++;
}

void blockChain::setCurrNumBlocks(int cnb) {
    currentNumBlocks = cnb;
}

void blockChain::setNodeNum(int node) {
    nodeNum = node;
}

int blockChain::getCurrNumBlocks() {
    return currentNumBlocks;
}

block blockChain::getBack() {
    return bChain.back();
}

block blockChain::getFront() {
    return bChain.front();
}

int blockChain::getNodeNum() {
    return nodeNum;
}

void blockChain::displayTrans(int bNum, vector<int> &idNum, vector<int> &idVal) {
    list<block> copy;
    copy.assign(bChain.begin(), bChain.end());
    copy.reverse();
    int currBlockNum = 1;
    for (list<block>::iterator it = copy.begin(); it != copy.end(); ++it) {
        if (currBlockNum == bNum) {
            it->displayTransctions(idNum, idVal);
            break;
        }
        currBlockNum++;
    }
}

int blockChain::searchID(int ID) {
    for (int i = 0; i < bChain.size(); i++) {
        bChain.front().searchTrans(ID);
    }
    return false;
}


