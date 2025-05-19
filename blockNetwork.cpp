#include "blockNetwork.h"

blockNetwork::blockNetwork() {
    numNodes = 0;
}

blockNetwork::blockNetwork(int numberOfNodes, int maxTranPerBlock) {
    numNodes = 0;
    for (int i = 0; i < numberOfNodes; i++) {
        blockChain newChain(maxTranPerBlock);
        numNodes++;
        newChain.setNodeNum(numNodes);
        allNodes.push_back(newChain);
    }
}

void blockNetwork::insertTranToNode(int node, transaction* tran) {
    if (node >= 0 && node < numNodes) {
        allNodes[node].insertTran(tran);
    }
}

void blockNetwork::setNumNodes(int nN) {
    numNodes = nN;
}

void blockNetwork::setNumIDs(int nI) {
    numIDs = nI;
}

int blockNetwork::getNumNodes() {
    return numNodes;
}

int blockNetwork::getNumIDs() {
    return numIDs;
}

void blockNetwork::addEdge(int uNode, int vNode) {
    u.push_back(uNode);
    v.push_back(vNode);
}

int blockNetwork::getValueOfID(int index) {
    return idValue[index];
}

void blockNetwork::setValueOfID(int index, int value) {
    idValue[index] = value;
}

int blockNetwork::searchID(int id) {
    for (int i = 0; i < idNumber.size(); i++) {
        if (idNumber[i] == id) {
            return i;
        }
    }
    return -1;
}

void blockNetwork::addID(int id, int value) {
    idNumber.push_back(id);
    idValue.push_back(value);
}

void blockNetwork::clearID() {
    idNumber.clear();
    idValue.clear();
}

void blockNetwork::display() {
    int numOfBlocksInNode;
    int blockNum;
    int numTransInBlock;
    int numTrans = 0;

    for (int i = 0; i < numNodes; i++) {
        cout << "~~~ Node " << i << ": " << endl;
        numOfBlocksInNode = allNodes[i].getCurrNumBlocks();
        cout << "Current number of blocks: " << numOfBlocksInNode << endl;
        for (int j = 0; j < numOfBlocksInNode; j++){
            blockNum = j+1;
            if (blockNum == numOfBlocksInNode) {
                numTransInBlock = allNodes[i].getFront().getCurrNumTran();
                numTrans += allNodes[i].getFront().getCurrNumTran();
            }
            else {
                numTransInBlock = allNodes[i].getBack().getCurrNumTran();
                numTrans += allNodes[i].getBack().getCurrNumTran();
            }
            cout << "Block Number: " << blockNum << " -- Number of Transactions:";
            cout << numTransInBlock << endl;
            allNodes[i].displayTrans(blockNum, idNumber, idValue);
        }
        idValue.clear();
        idNumber.clear();
    }
}


