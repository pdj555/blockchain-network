#include <iostream>
#include "blockNetwork.h"

using namespace std;

int main()
{
    int numNodesInNetwork;
    int numEdges;
    cin >> numNodesInNetwork;
    cout << "Number of nodes: " << numNodesInNetwork << endl;

    int numTransactionsPerBlock;
    cin >> numTransactionsPerBlock;
    cout << "Number of transactions per block: " << numTransactionsPerBlock << endl;

    int totalNumTransactions;
    cin >> totalNumTransactions;
    cout << "Total number of transactions: " << totalNumTransactions << endl;

    blockNetwork *n1 = new blockNetwork(numNodesInNetwork, numTransactionsPerBlock);

    int uNode;
    int vNode;
    cin >> numEdges;
    for (int i = 0; i < numEdges; i++) {
        cin >> uNode >> vNode;
        n1->addEdge(uNode, vNode);
    }

    int node;
    int transactionID;
    int fromID;
    int toID;
    int amountTrans;
    string timeStamp;

    while (cin >> node >> transactionID >> fromID >> toID >> amountTrans >> timeStamp) {
        transaction* t = new transaction(node, transactionID, fromID, toID, amountTrans, timeStamp);
        n1->insertTranToNode(node, t);
    }

    n1->display();
    return 0;
}

