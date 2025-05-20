#include "blockNetwork.h"

blockNetwork::blockNetwork() {
    numNodes = 0;
}

blockNetwork::blockNetwork(int numberOfNodes, int maxTranPerBlock) {
    numNodes = 0;
    adjList.resize(numberOfNodes);
    for (int i = 0; i < numberOfNodes; i++) {
        blockChain newChain(maxTranPerBlock);
        numNodes++;
        newChain.setNodeNum(numNodes);
        allNodes.push_back(newChain);
    }
}

void blockNetwork::insertTranToNode(int node, const transaction &tran) {
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
    if (uNode >= 0 && uNode < adjList.size() && vNode >= 0 && vNode < adjList.size()) {
        adjList[uNode].push_back(vNode);
    }
}

int blockNetwork::getValueOfID(int id) {
    auto it = idMap.find(id);
    if (it != idMap.end()) {
        return it->second;
    }
    return -1;
}

void blockNetwork::setValueOfID(int id, int value) {
    idMap[id] = value;
}

int blockNetwork::searchID(int id) {
    auto it = idMap.find(id);
    if (it != idMap.end()) {
        return it->second;
    }
    return -1;
}

void blockNetwork::addID(int id, int value) {
    idMap[id] = value;
}

void blockNetwork::clearID() {
    idMap.clear();
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
            allNodes[i].displayTrans(blockNum, idMap);
        }
        idMap.clear();
    }
}

bool blockNetwork::verifyAllChains() const {
    for (const auto &chain : allNodes) {
        if (!chain.verifyChain()) {
            return false;
        }
    }
    return true;
}

void blockNetwork::tamperPrevHash(size_t nodeIndex, size_t blockIndex, const std::string &newPrev) {
    if (nodeIndex < allNodes.size()) {
        allNodes[nodeIndex].tamperPrevHash(blockIndex, newPrev);
    }
}


