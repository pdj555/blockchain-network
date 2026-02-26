#pragma once
#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>
#include "blockChain.h"

class blockNetwork
{
    int numNodes;
    std::vector<blockChain> allNodes;
    std::vector<std::vector<int>> adjList;
    std::vector<std::vector<int>> reachableNodesCache;
    bool reachabilityCacheValid;
    bool propagateTransactions;

    void rebuildReachabilityCache();
public:
    blockNetwork();
    blockNetwork(int numberOfNodes, int maxTranPerBlock);

    bool insertTranToNode(int node, const transaction &tran);
    void setLogStream(std::ostream *out);
    void setPropagationEnabled(bool enabled);
    bool isPropagationEnabled() const;

    int getNumNodes() const;
    int getNodeBlockCount(int node) const;
    int getNodeTransactionCount(int node) const;
    int getNodeRejectedTransactionCount(int node) const;
    int getNumEdges() const;
    int getReachableNodeCount(int startNode) const;
    int getMaxReachableNodeCount() const;
    int getIsolatedNodeCount() const;

    void addEdge(int uNode, int vNode);

    void display(std::ostream &out) const;
    void display() const;

    bool verifyAllChains() const;

    // Modify prevHash of a block in a given node for testing
    void tamperPrevHash(std::size_t nodeIndex, std::size_t blockIndex, const std::string &newPrev);
};
