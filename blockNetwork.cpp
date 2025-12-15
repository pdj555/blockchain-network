#include "blockNetwork.h"
#include <cstddef>
#include <iostream>
#include <utility>

blockNetwork::blockNetwork()
    : numNodes(0),
      allNodes(),
      adjList() {}

blockNetwork::blockNetwork(int numberOfNodes, int maxTranPerBlock)
    : numNodes(numberOfNodes > 0 ? numberOfNodes : 0),
      allNodes(),
      adjList(static_cast<std::size_t>(numNodes)) {
    const int maxTransactions = maxTranPerBlock > 0 ? maxTranPerBlock : 1;
    allNodes.reserve(static_cast<std::size_t>(numNodes));
    for (int i = 0; i < numNodes; ++i) {
        blockChain newChain(maxTransactions);
        newChain.setNodeNum(i);
        allNodes.push_back(std::move(newChain));
    }
}

void blockNetwork::insertTranToNode(int node, const transaction &tran) {
    if (node < 0 || static_cast<std::size_t>(node) >= allNodes.size()) {
        return;
    }
    allNodes[static_cast<std::size_t>(node)].insertTran(tran);
}

void blockNetwork::setLogStream(std::ostream *out) {
    for (auto &chain : allNodes) {
        chain.setLogStream(out);
    }
}

int blockNetwork::getNumNodes() const {
    return numNodes;
}

int blockNetwork::getNodeBlockCount(int node) const {
    if (node < 0 || static_cast<std::size_t>(node) >= allNodes.size()) {
        return 0;
    }
    return allNodes[static_cast<std::size_t>(node)].getCurrNumBlocks();
}

int blockNetwork::getNodeTransactionCount(int node) const {
    if (node < 0 || static_cast<std::size_t>(node) >= allNodes.size()) {
        return 0;
    }
    return allNodes[static_cast<std::size_t>(node)].getTotalTransactions();
}

void blockNetwork::addEdge(int uNode, int vNode) {
    const std::size_t numAdjNodes = adjList.size();
    if (uNode >= 0 && static_cast<std::size_t>(uNode) < numAdjNodes && vNode >= 0 &&
        static_cast<std::size_t>(vNode) < numAdjNodes) {
        adjList[static_cast<std::size_t>(uNode)].push_back(vNode);
    }
}

void blockNetwork::display(std::ostream &out) const {
    for (int i = 0; i < numNodes; ++i) {
        out << "~~~ Node " << i << ": " << std::endl;
        const int numOfBlocksInNode = allNodes[static_cast<std::size_t>(i)].getCurrNumBlocks();
        out << "Current number of blocks: " << numOfBlocksInNode << std::endl;

        for (int blockNum = 1; blockNum <= numOfBlocksInNode; ++blockNum) {
            const block *blk = allNodes[static_cast<std::size_t>(i)].getBlockFromOldest(blockNum);
            const int numTransInBlock = blk != nullptr ? blk->getCurrNumTran() : 0;

            out << "Block Number: " << blockNum << " -- Number of Transactions:" << numTransInBlock << std::endl;
            if (blk != nullptr) {
                blk->displayTransactions(out);
            }
        }
    }
}

void blockNetwork::display() const {
    display(std::cout);
}

bool blockNetwork::verifyAllChains() const {
    for (const auto &chain : allNodes) {
        if (!chain.verifyChain()) {
            return false;
        }
    }
    return true;
}

void blockNetwork::tamperPrevHash(std::size_t nodeIndex, std::size_t blockIndex, const std::string &newPrev) {
    if (nodeIndex < allNodes.size()) {
        allNodes[nodeIndex].tamperPrevHash(blockIndex, newPrev);
    }
}
