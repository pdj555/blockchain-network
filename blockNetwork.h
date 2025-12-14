#pragma once
#include <cstddef>
#include <iosfwd>
#include <vector>
#include "blockChain.h"

class blockNetwork
{
    int numNodes;
    std::vector<blockChain> allNodes;
    std::vector<std::vector<int>> adjList;
public:
    blockNetwork();
    blockNetwork(int numberOfNodes, int maxTranPerBlock);

    void insertTranToNode(int node, const transaction &tran);
    void setLogStream(std::ostream *out);

    int getNumNodes() const;
    int getNodeBlockCount(int node) const;
    int getNodeTransactionCount(int node) const;

    void addEdge(int uNode, int vNode);

    void display(std::ostream &out) const;
    void display() const;

    bool verifyAllChains() const;

    // Modify prevHash of a block in a given node for testing
    void tamperPrevHash(std::size_t nodeIndex, std::size_t blockIndex, const std::string &newPrev);
};
