#include <iostream>
#include "blockNetwork.h"

using namespace std;

int main()
{
    int numNodesInNetwork;
    int numEdges;
    if (!(cin >> numNodesInNetwork) || numNodesInNetwork <= 0) {
        cerr << "Invalid number of nodes" << endl;
        return 1;
    }
    cout << "Number of nodes: " << numNodesInNetwork << endl;

    int numTransactionsPerBlock;
    if (!(cin >> numTransactionsPerBlock) || numTransactionsPerBlock <= 0) {
        cerr << "Invalid number of transactions per block" << endl;
        return 1;
    }
    cout << "Number of transactions per block: " << numTransactionsPerBlock << endl;

    int totalNumTransactions;
    if (!(cin >> totalNumTransactions) || totalNumTransactions < 0) {
        cerr << "Invalid total number of transactions" << endl;
        return 1;
    }
    cout << "Total number of transactions: " << totalNumTransactions << endl;

    blockNetwork n1(numNodesInNetwork, numTransactionsPerBlock);

    int uNode;
    int vNode;
    if (!(cin >> numEdges) || numEdges < 0) {
        cerr << "Invalid number of edges" << endl;
        return 1;
    }
    for (int i = 0; i < numEdges; i++) {
        if (!(cin >> uNode >> vNode)) {
            cerr << "Unexpected end of edge list" << endl;
            return 1;
        }
        if (uNode < 0 || uNode >= numNodesInNetwork ||
            vNode < 0 || vNode >= numNodesInNetwork) {
            cerr << "Invalid edge " << uNode << " -> " << vNode << endl;
            return 1;
        }
        n1.addEdge(uNode, vNode);
    }

    int node;
    int transactionID;
    int fromID;
    int toID;
    int amountTrans;
    string timeStamp;

    while (true) {
        if (!(cin >> node >> transactionID >> fromID >> toID >> amountTrans >> timeStamp)) {
            if (cin.eof()) {
                break;
            }
            cerr << "Malformed transaction entry" << endl;
            return 1;
        }
        if (node < 0 || node >= numNodesInNetwork) {
            cerr << "Invalid node in transaction" << endl;
            return 1;
        }
        if (amountTrans <= 0) {
            cerr << "Invalid transaction amount" << endl;
            return 1;
        }
        transaction t(node, transactionID, fromID, toID, amountTrans, timeStamp);
        n1.insertTranToNode(node, t);
    }

    n1.display();
    return 0;
}

