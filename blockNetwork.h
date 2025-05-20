#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "blockChain.h"
using namespace std;

class blockNetwork
{
    int numNodes;
    vector<blockChain> allNodes;
    vector<vector<int>> adjList;
    unordered_map<int, int> idMap;
    int numIDs;
public:
    blockNetwork();
    blockNetwork(int numberOfNodes, int maxTranPerBlock);

    void insertTranToNode(int node, const transaction &tran);

    void setNumNodes(int nN);
    void setNumIDs(int nI);

    int getNumNodes();
    int getNumIDs();

    void addEdge(int uNode, int vNode);
    int getValueOfID(int id);
    void setValueOfID(int id, int value);

    int searchID(int id);
    void addID(int id, int value);
    void clearID();

    void display();

    bool verifyAllChains() const;

    // Modify prevHash of a block in a given node for testing
    void tamperPrevHash(size_t nodeIndex, size_t blockIndex, const std::string &newPrev);
};
