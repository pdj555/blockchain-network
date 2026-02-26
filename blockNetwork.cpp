#include "blockNetwork.h"
#include <cstddef>
#include <queue>
#include <iostream>
#include <utility>

blockNetwork::blockNetwork()
    : numNodes(0),
      allNodes(),
      adjList(),
      propagateTransactions(true) {}

blockNetwork::blockNetwork(int numberOfNodes, int maxTranPerBlock)
    : numNodes(numberOfNodes > 0 ? numberOfNodes : 0),
      allNodes(),
      adjList(static_cast<std::size_t>(numNodes)),
      propagateTransactions(true) {
    const int maxTransactions = maxTranPerBlock > 0 ? maxTranPerBlock : 1;
    allNodes.reserve(static_cast<std::size_t>(numNodes));
    for (int i = 0; i < numNodes; ++i) {
        blockChain newChain(maxTransactions);
        newChain.setNodeNum(i);
        allNodes.push_back(std::move(newChain));
    }
}

bool blockNetwork::insertTranToNode(int node, const transaction &tran) {
    if (node < 0 || static_cast<std::size_t>(node) >= allNodes.size()) {
        return false;
    }

    if (!propagateTransactions) {
        return allNodes[static_cast<std::size_t>(node)].insertTran(tran);
    }

    std::vector<bool> visited(allNodes.size(), false);
    std::queue<int> toVisit;
    toVisit.push(node);
    visited[static_cast<std::size_t>(node)] = true;

    bool allAccepted = true;
    while (!toVisit.empty()) {
        const int currentNode = toVisit.front();
        toVisit.pop();

        const bool accepted = allNodes[static_cast<std::size_t>(currentNode)].insertTran(tran);
        if (!accepted) {
            allAccepted = false;
        }

        const auto &neighbors = adjList[static_cast<std::size_t>(currentNode)];
        for (int neighbor : neighbors) {
            if (neighbor < 0 || static_cast<std::size_t>(neighbor) >= visited.size()) {
                continue;
            }
            if (!visited[static_cast<std::size_t>(neighbor)]) {
                visited[static_cast<std::size_t>(neighbor)] = true;
                toVisit.push(neighbor);
            }
        }
    }

    return allAccepted;
}

void blockNetwork::setLogStream(std::ostream *out) {
    for (auto &chain : allNodes) {
        chain.setLogStream(out);
    }
}

void blockNetwork::setPropagationEnabled(bool enabled) {
    propagateTransactions = enabled;
}

bool blockNetwork::isPropagationEnabled() const {
    return propagateTransactions;
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

int blockNetwork::getNodeRejectedTransactionCount(int node) const {
    if (node < 0 || static_cast<std::size_t>(node) >= allNodes.size()) {
        return 0;
    }
    return allNodes[static_cast<std::size_t>(node)].getRejectedTransactions();
}

int blockNetwork::getNumEdges() const {
    int count = 0;
    for (const auto &neighbors : adjList) {
        count += static_cast<int>(neighbors.size());
    }
    return count;
}

int blockNetwork::getReachableNodeCount(int startNode) const {
    if (startNode < 0 || static_cast<std::size_t>(startNode) >= adjList.size()) {
        return 0;
    }

    std::vector<bool> visited(adjList.size(), false);
    std::queue<int> toVisit;
    toVisit.push(startNode);
    visited[static_cast<std::size_t>(startNode)] = true;

    int reachableCount = 0;
    while (!toVisit.empty()) {
        const int currentNode = toVisit.front();
        toVisit.pop();
        ++reachableCount;

        const auto &neighbors = adjList[static_cast<std::size_t>(currentNode)];
        for (int neighbor : neighbors) {
            if (neighbor < 0 || static_cast<std::size_t>(neighbor) >= visited.size()) {
                continue;
            }
            if (!visited[static_cast<std::size_t>(neighbor)]) {
                visited[static_cast<std::size_t>(neighbor)] = true;
                toVisit.push(neighbor);
            }
        }
    }

    return reachableCount;
}

int blockNetwork::getMaxReachableNodeCount() const {
    int maxReachableCount = 0;
    for (int node = 0; node < numNodes; ++node) {
        const int reachableCount = getReachableNodeCount(node);
        if (reachableCount > maxReachableCount) {
            maxReachableCount = reachableCount;
        }
    }
    return maxReachableCount;
}

int blockNetwork::getIsolatedNodeCount() const {
    if (adjList.empty()) {
        return 0;
    }

    std::vector<int> inDegree(adjList.size(), 0);
    for (const auto &neighbors : adjList) {
        for (int neighbor : neighbors) {
            if (neighbor >= 0 && static_cast<std::size_t>(neighbor) < inDegree.size()) {
                ++inDegree[static_cast<std::size_t>(neighbor)];
            }
        }
    }

    int isolatedCount = 0;
    for (std::size_t node = 0; node < adjList.size(); ++node) {
        if (adjList[node].empty() && inDegree[node] == 0) {
            ++isolatedCount;
        }
    }
    return isolatedCount;
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
