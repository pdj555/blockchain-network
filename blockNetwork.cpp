#include "blockNetwork.h"
#include <cstddef>
#include <queue>
#include <iostream>
#include <utility>
#include <algorithm>
#include <unordered_map>

blockNetwork::blockNetwork()
    : numNodes(0),
      allNodes(),
      adjList(),
      reachableNodesCache(),
      reachabilityCacheValid(false),
      propagateTransactions(true),
      networkTransactionIds() {}

blockNetwork::blockNetwork(int numberOfNodes, int maxTranPerBlock)
    : numNodes(numberOfNodes > 0 ? numberOfNodes : 0),
      allNodes(),
      adjList(static_cast<std::size_t>(numNodes)),
      reachableNodesCache(),
      reachabilityCacheValid(false),
      propagateTransactions(true),
      networkTransactionIds() {
    const int maxTransactions = maxTranPerBlock > 0 ? maxTranPerBlock : 1;
    allNodes.reserve(static_cast<std::size_t>(numNodes));
    for (int i = 0; i < numNodes; ++i) {
        blockChain newChain(maxTransactions);
        newChain.setNodeNum(i);
        allNodes.push_back(std::move(newChain));
    }
}

void blockNetwork::rebuildReachabilityCache() {
    reachableNodesCache.assign(adjList.size(), std::vector<int>());

    for (std::size_t start = 0; start < adjList.size(); ++start) {
        std::vector<bool> visited(adjList.size(), false);
        std::queue<int> toVisit;
        toVisit.push(static_cast<int>(start));
        visited[start] = true;

        auto &reachable = reachableNodesCache[start];
        while (!toVisit.empty()) {
            const int currentNode = toVisit.front();
            toVisit.pop();
            reachable.push_back(currentNode);

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
    }

    reachabilityCacheValid = true;
}

bool blockNetwork::insertTranToNode(int node, const transaction &tran) {
    if (node < 0 || static_cast<std::size_t>(node) >= allNodes.size()) {
        return false;
    }

    const int transactionId = tran.getTranID();
    if (networkTransactionIds.find(transactionId) != networkTransactionIds.end()) {
        allNodes[static_cast<std::size_t>(node)].recordRejectedTran(blockChain::RejectionReason::duplicateId);
        return false;
    }

    if (!propagateTransactions) {
        const bool inserted = allNodes[static_cast<std::size_t>(node)].insertTran(tran);
        if (inserted) {
            networkTransactionIds.insert(transactionId);
        }
        return inserted;
    }

    if (!reachabilityCacheValid) {
        rebuildReachabilityCache();
    }

    const auto &reachableNodes = reachableNodesCache[static_cast<std::size_t>(node)];
    for (int currentNode : reachableNodes) {
        if (!allNodes[static_cast<std::size_t>(currentNode)].canAcceptTran(tran)) {
            allNodes[static_cast<std::size_t>(node)].recordRejectedTran(blockChain::RejectionReason::duplicateId);
            return false;
        }
    }

    for (int currentNode : reachableNodes) {
        allNodes[static_cast<std::size_t>(currentNode)].insertTran(tran);
    }

    networkTransactionIds.insert(transactionId);
    return true;
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

    if (!reachabilityCacheValid) {
        const_cast<blockNetwork *>(this)->rebuildReachabilityCache();
    }

    return static_cast<int>(reachableNodesCache[static_cast<std::size_t>(startNode)].size());
}

int blockNetwork::getMaxReachableNodeCount() const {
    if (!reachabilityCacheValid) {
        const_cast<blockNetwork *>(this)->rebuildReachabilityCache();
    }

    int maxReachableCount = 0;
    for (const auto &reachable : reachableNodesCache) {
        const int reachableCount = static_cast<int>(reachable.size());
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

int blockNetwork::getLargestConsensusGroupSize() const {
    if (allNodes.empty()) {
        return 0;
    }

    std::unordered_map<std::string, int> groupCounts;
    int largestGroup = 0;
    for (const auto &chain : allNodes) {
        const int count = ++groupCounts[chain.getConsensusFingerprint()];
        if (count > largestGroup) {
            largestGroup = count;
        }
    }
    return largestGroup;
}

double blockNetwork::getConsensusRatio() const {
    if (allNodes.empty()) {
        return 0.0;
    }
    return static_cast<double>(getLargestConsensusGroupSize()) /
           static_cast<double>(allNodes.size());
}

void blockNetwork::addEdge(int uNode, int vNode) {
    const std::size_t numAdjNodes = adjList.size();
    if (uNode >= 0 && static_cast<std::size_t>(uNode) < numAdjNodes && vNode >= 0 &&
        static_cast<std::size_t>(vNode) < numAdjNodes) {
        auto &neighbors = adjList[static_cast<std::size_t>(uNode)];
        if (std::find(neighbors.begin(), neighbors.end(), vNode) == neighbors.end()) {
            neighbors.push_back(vNode);
            reachabilityCacheValid = false;
        }
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
