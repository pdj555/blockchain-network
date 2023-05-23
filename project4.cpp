// Project 4
// CS 2413 Data Structures
// Spring 2023

#include <iostream>
#include <vector> // for array of transactions and array of blockChains
#include <list>   // for blockchain

using namespace std;

class transaction
{
    int nodeNum;      // node to be added to
    int tID;          // transaction id
    int fromID;       // source ID
    int fromValue;    // P4: how much funds does the source ID person have? If first time, then initialize with 100 coins
    int toID;         // target ID
    int toValue;      // P4: how much funds does the source ID person have? If first time, then initialize with 100 coins
    int tAmount;      // how much is being transfered
    string timeStamp; // time of the transaction read from the input file

public:
    transaction();                                                                                 // default constructor
    transaction(int tempNode, int temptID, int tempfromID, int temptoID, int temptAmount, string temptimeStamp); // non default constructor - P4: default 100 for from and to values
    
    // all setters and getters
    void setTranID(int tranID); // sets transaction ID
    void setFromID(int fID); // sets from ID
    void setToID(int targetID); // sets to ID
    void setTranAmount(int tranAmount); // sets transaction amount
    void setTimeStamp(string tS); // sets time stamp
    void setFromValue(int fV); // sets how much funds the from ID person has
    void setToValue(int tV); // sets how much funds the to ID person has
    void setTNodeNum(int node); // sets the node where the transaction is
        
    int getTranID(); // gets transaction ID
    int getFromID(); // gets from ID
    int getToID(); // gets to ID
    int getTranAmount(); // gets transaction amount
    string getTimeStamp(); // gets time stamp
    int getFromValue(); // gets how much funds the from ID person has
    int getToValue(); // gets how much funds the to ID person has
    int getTNodeNum(); // gets which node the transaction is located

    // other methods as needed
    void displayTransaction(int newFromValue, int newToValue);
};

// Default constructor
transaction::transaction() {}

// Non-default constructor
transaction::transaction(int tempNode, int temptID, int tempfromID, int temptoID, int temptAmount, string temptimeStamp)
{
    nodeNum = tempNode;
    tID = temptID;
    fromID = tempfromID;
    toID = temptoID;
    tAmount = temptAmount;
    timeStamp = temptimeStamp;
    fromValue = 100;
    toValue = 100;
}

// Setters
// Sets transaction ID
void transaction::setTranID(int tranID) {
    tID = tranID;
}

// Sets source ID
void transaction::setFromID(int fID) {
    fromID = fID;
}

// Sets target ID
void transaction::setToID(int targetID) {
    toID = targetID;
}

// Sets how much is being transfered
void transaction::setTranAmount(int tranAmount) {
    tAmount = tranAmount;
}

// Sets time of the transaction read from the input file
void transaction::setTimeStamp(string tS) {
    timeStamp = tS;
}

// Sets the node number where the transaction is located
void transaction::setTNodeNum(int node) {
    nodeNum = node;
}

// Sets the value of the from ID
void transaction::setFromValue(int fV) {
    fromValue = fV;
}

// Sets the value of the to ID
void transaction::setToValue(int tV) {
    toValue = tV;
}

// Getters
// Gets transaction ID
int transaction::getTranID() {
    return tID;
}

// Gets source ID
int transaction::getFromID() {
    return fromID;
}

// Gets target ID
int transaction::getToID() {
    return toID;
}

// Gets amount being transfered
int transaction::getTranAmount() {
    return tAmount;
}

// Gets time of the transaction read from the input file
string transaction::getTimeStamp() {
    return timeStamp;
}

// Gets the node number where the transaction is located
int transaction::getTNodeNum() {
    return nodeNum;
}

// Gets the value of the from ID
int transaction::getFromValue() {
    return fromValue;
}

// Gets the value of the to ID
int transaction::getToValue() {
    return toValue;
}

// Displays transaction
void transaction::displayTransaction(int newFromValue, int newToValue) {
    cout << tID << " " << fromID << " " << newFromValue + tAmount << " " << toID << " " << newToValue - tAmount << " " << tAmount << " " << timeStamp << endl;
    fromValue = newFromValue;
    toValue = newToValue;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class block
{
    int blockNumber;                   // block number of the current block
    int currentNumTransactions;        // how many transactions are currently in the block
    int maxNumTransactions;            // how many maximum transactions can be in the block
    vector<transaction> bTransactions; // vector of transaction objects
    block* nextBlock; // keeps up with the previous block in the list
    int newFromValue; // P4: new value of the from ID
    int newToValue; // P4: new value of the to ID
    int numNodesInNetwork; // P4: number of nodes in the network

public:
    block();                                 // default constructor
    block(int bNumber, int maxTransactions); // non default constructor
    
    void inseartTran(transaction t); // insert method to insert a new transaction

    // setters and getters as needed
    void setBlockNumber(int bN); // sets block number
    void setCurrNumTran(int cnt); // sets current number of transactions
    void setMaxNumTran(int mnt); // sets maximum number of tranactions
    void setNextBlock(block* bA); // sets pointer to next block in blockchain
    void setNumNodesInNetwork(int numNodes); // P4: sets the number of nodes in the network
        
    int getBlockNumber(); // gets block number
    int getCurrNumTran(); // gets current number of transactions
    int getMaxNumTran(); // gets maximum number of transactions
    block* getNextBlock(); // gets pointer to next block in blockchain
    int getNumNodesInNetwork(); // P4: gets the number of nodes in the network
    
    transaction getTran(int index); // gets a transaction from the block at a given index

    void displayTransctions(vector<int> &idN, vector<int> &idV); // displays the transactions that the block holds

    bool searchTrans(int id); // P4: search for an ID in the bTransaction vector
};

// Constructors
// Default constructor for block class
block::block() {
    blockNumber = 0;
    maxNumTransactions = 10;
    currentNumTransactions = 0;
}

// Non-Default Constructor for block class
block::block(int bNumber, int maxTransactions) {
    blockNumber = bNumber;
    maxNumTransactions = maxTransactions;
    currentNumTransactions = 0;
}

// Insearts transaction into block's vector
void block::inseartTran(transaction t) {
    bTransactions.push_back(t);
    currentNumTransactions++;
}

// Setters
// Sets block number
void block::setBlockNumber(int bN) {
    blockNumber = bN;
}

// Sets current number of transactions
void block::setCurrNumTran(int cnt) {
    currentNumTransactions = cnt;
}

// Sets maximum number of transactions
void block::setMaxNumTran(int mnt) {
    maxNumTransactions = mnt;
}

// Sets pointer to next block in blockchain
void block::setNextBlock(block* bA) {
    nextBlock = bA;
}

// Getters
// Gets block number
int block::getBlockNumber() {
    return blockNumber;
}

// Gets current number of transactions
int block::getCurrNumTran() {
    return currentNumTransactions;
}

// Gets maximum number of transactions
int block::getMaxNumTran() {
    return maxNumTransactions;
}

// Gets pointer to next block in blockchain
block* block::getNextBlock() {
    return nextBlock;
}

// Returns transaction in block at a given index
transaction block::getTran(int index) {
    return bTransactions[index];
}

// Displays the transactions stored in the block's vector
void block::displayTransctions(vector<int> &idN, vector<int> &idV) {
    
    int newFromVal;
    int newToVal;

     for (int i = 0; i < bTransactions.size(); i++) {
         int fIdx = -1;
         int tIdx = -1;
         for (int j = 0; j < idN.size(); j++) {
             if (idN[j] == bTransactions[i].getFromID()) {
                 fIdx = j;
             }
             if (idN[j] == bTransactions[i].getToID()) {
                 tIdx = j;
             }
         }
         if (fIdx != -1 && tIdx != -1) {
             newFromVal = idV[fIdx]-bTransactions[i].getTranAmount();
             newToVal = idV[tIdx]+bTransactions[i].getTranAmount();

             idN[fIdx] = bTransactions[i].getFromID();
             idV[fIdx] = newFromVal;

             idN[tIdx] = bTransactions[i].getToID();
             idV[tIdx] = newToVal;

             bTransactions[i].displayTransaction(newFromVal, newToVal);
         }
         else if (fIdx != -1 && tIdx == -1) {
             newFromVal = idV[fIdx]-bTransactions[i].getTranAmount();
             newToVal = 100+bTransactions[i].getTranAmount();

             idN[fIdx] = bTransactions[i].getFromID();
             idV[fIdx] = newFromVal;

             idN.push_back(bTransactions[i].getToID());
             idV.push_back(newToVal);

             bTransactions[i].displayTransaction(newFromVal, newToVal);
         }
         else if (fIdx == -1 && tIdx != -1) {
             newFromVal = 100-bTransactions[i].getTranAmount();
             newToVal = idV[tIdx]+bTransactions[i].getTranAmount();

             idN.push_back(bTransactions[i].getFromID());
             idV.push_back(newFromVal);

             idN[tIdx] = bTransactions[i].getToID();
             idV[tIdx] = newToVal;

             bTransactions[i].displayTransaction(newFromVal, newToVal);
         }
         else {
             newFromVal = 100-bTransactions[i].getTranAmount();
             newToVal = 100+bTransactions[i].getTranAmount();

             idN.push_back(bTransactions[i].getFromID());
             idV.push_back(newFromVal);

             idN.push_back(bTransactions[i].getToID());
             idV.push_back(newToVal);

             bTransactions[i].displayTransaction(newFromVal, newToVal);
         }
     }
}

// Searchs for transaction with block
bool block::searchTrans(int id) {
    bool find = false;
    for (int i = 0; i < bTransactions.size(); i++) {
        if (bTransactions[i].getTranID() == id) {
            find = true;
        }
    }
    return find;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class blockChain
{
    int currentNumBlocks; // maintain the size of the list/block chain list
    list<block> bChain;   // blockchain as a linked list
    int nodeNum;

public:
    blockChain();          // default constructor
    blockChain(int tPerB); // non default constructor

    void insertTran(transaction* t); // insert method to insert new transaction into the block chain - add new block if existing block is full
    void insertBlockFront(block b); // while inserting new block into list, insert front
    
    // setters and getters as needed
    void setCurrNumBlocks(int cnb); // sets the current number of blocks in the blockchain
    void setNodeNum(int node); // sets the node that the block chain is in within the block network
    
    int getCurrNumBlocks(); // gets the current number of blocks in the blockchain
    block getBack(); // gets the block at the back of the bChain
    block getFront(); // gets the block at the front of the bChain
    int getNodeNum(); // gets the node number that the block chain is in within the block network

    void displayTrans(int bNum, vector<int> &idNum, vector<int> &idVal); // displays the transactions that are held in the blocks in the blockchain

    int searchID(int ID); // P4: search for an ID across all blocks in bChain
};

// Default constructor for blockChain class
blockChain::blockChain() {
    currentNumBlocks = 0;
}

// Non-Default Constructor for blockChain class
blockChain::blockChain(int tPerB) {
    bChain.push_front(block(currentNumBlocks, tPerB));
    currentNumBlocks = 1;
    //cout << "Adding block #" << currentNumBlocks << endl;
}

// Inserts a transaction into the front-most block in the blockchain. If the front block has reached it's maximum number of
// trasactions, a new block is added to the front of the blockchain
void blockChain::insertTran(transaction* t) {
    if (bChain.empty() || bChain.front().getCurrNumTran() == bChain.front().getMaxNumTran()) {
        block nB(currentNumBlocks, bChain.front().getMaxNumTran());
        
        nB.inseartTran(*t);
        insertBlockFront(nB);
        cout << "Inserting transaction to block #" << currentNumBlocks << " in node " << t -> getTNodeNum() << endl;
    }
    else {
        bChain.front().inseartTran(*t);
        cout << "Inserting transaction to block #" << currentNumBlocks << " in node " << t -> getTNodeNum() << endl;
    }
}

// Inserts a new block at the front of the blockchain
void blockChain::insertBlockFront(block b) {
    b.setNextBlock(&bChain.front());
    bChain.push_front(b);
    currentNumBlocks++;
}

// Setters
// Sets the current number of blocks in the blockchain
void blockChain::setCurrNumBlocks(int cnb) {
    currentNumBlocks = cnb;
}

void blockChain::setNodeNum(int node) {
    nodeNum = node;
}

// Getters
// Gets the current number of blocks in the blockchain
int blockChain::getCurrNumBlocks() {
    return currentNumBlocks;
}

// Gets the block at the back of the bChain
block blockChain::getBack() {
    return bChain.back();
}

// Gets the block at the front of the bChain
block blockChain::getFront() {
    return bChain.front();
}

// Gets the node that the block chain is in within the block network
int blockChain::getNodeNum() {
    return nodeNum;
}

// Displays the transactions in each block of the blockchain
void blockChain::displayTrans(int bNum, vector<int> &idNum, vector<int> &idVal) {
    list<block> copy;
    copy.assign(bChain.begin(), bChain.end());
    copy.reverse();

    int currBlockNum = 1;

    for (list<block>::iterator it = copy.begin(); it != copy.end(); ++it) {
        if (currBlockNum == bNum) {
            it->displayTransctions(idNum, idVal);
            break;
        }
        currBlockNum++;
    }
}

// Returns index of ID if the ID is found in the block chain or -1 if it is not fround in the blockchain
int blockChain::searchID(int ID) {
    for (int i = 0; i < bChain.size(); i++) {
        bChain.front().searchTrans(ID);
    }
    return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class blockNetwork
{
    int numNodes;                // maintain the number of nodes in the network
    vector<blockChain> allNodes; // vector of all the blockChains in the network
    vector<int> u;               // edge list u node
    vector<int> v;               // edge list v node
    vector<int> idValue;         // vector of all the id values in the network
    vector<int> idNumber;        // vector of all the id numbers in the network
    int numIDs;                  // number of IDs in the network

public:
    blockNetwork();                  // default constructor
    blockNetwork(int numberOfNodes, int maxTranPerBlock); // non default constructor
    
    void insertTranToNode(int node, transaction* tran); // insert method to insert new transaction into the block chain in a specific node - there will be a block chain existent in each node (allNodes vector)
    
    // setters and getters as needed
    void setNumNodes(int nN); // sets number of nodes in the network
    void setNumIDs(int nI); // sets number of IDs in the network

    int getNumNodes(); // gets number of nodes in the network
    int getNumIDs(); // gets number of IDs in the network

    void addEdge(int uNode, int vNode); // adds edge
    int getValueOfID(int index); // gets the value of the ID
    void setValueOfID(int index, int value); // sets the value of the ID

    int searchID(int id); // searches for the ID in the network, return index if found, -1 if not found
    void addID(int id, int value); // adds ID to the network
    void clearID(); // clears the ID vector
    
    void display(); // displays the contents of the network
};

// Default constructor
blockNetwork::blockNetwork() {
    numNodes = 0;
}

// Non-default constructor
blockNetwork::blockNetwork(int numberOfNodes, int maxTranPerBlock) {
    numNodes = 0;
    
    for (int i = 0; i < numberOfNodes; i++) {
        blockChain newChain(maxTranPerBlock);
        numNodes++;
        newChain.setNodeNum(numNodes);
        allNodes.push_back(newChain);
    }
}

// Insert transaction to specific node
void blockNetwork::insertTranToNode(int node, transaction* tran) {
    if (node >= 0 && node < numNodes) {
        allNodes[node].insertTran(tran);
    }
}

// Setters
// Sets the number of nodes in the network
void blockNetwork::setNumNodes(int nN) {
    numNodes = nN;
}

// Sets the number of IDs in the network
void blockNetwork::setNumIDs(int nI) {
    numIDs = nI;
}

// Getters
// Gets number of nodes in the network
int blockNetwork::getNumNodes() {
    return numNodes;
}

// Gets number of IDs in the network
int blockNetwork::getNumIDs() {
    return numIDs;
}

// Adds edge
void blockNetwork::addEdge(int uNode, int vNode) {
    u.push_back(uNode);
    v.push_back(vNode);
}

// Gets the value of the ID
int blockNetwork::getValueOfID(int index) {
    return idValue[index];
}

// Sets the value of the ID
void blockNetwork::setValueOfID(int index, int value) {
    idValue[index] = value;
}

// Searches for the ID in the network
int blockNetwork::searchID(int id) {
    for (int i = 0; i < idNumber.size(); i++) {
        if (idNumber[i] == id) {
            return i;
        }
    }
    return -1;
}

// Adds ID to the network
void blockNetwork::addID(int id, int value) {
    idNumber.push_back(id);
    idValue.push_back(value);
}

// Clears the ID vector
void blockNetwork::clearID() {
    idNumber.clear();
    idValue.clear();
}

// Displays the content of the network
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
            
            cout << "Block Number: " << blockNum << " -- Number of Transaction: ";
            cout << numTransInBlock << endl;
            
            allNodes[i].displayTrans(blockNum, idNumber, idValue);
        }
        idValue.clear();
        idNumber.clear();
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main()
{
    int numNodesInNetwork; // number of nodes in the network
    int numEdges; // number of edges in the network

    cin >> numNodesInNetwork;
    cout << "Number of nodes: " << numNodesInNetwork << endl;

    int numTransactionsPerBlock; // max transactions per block

    cin >> numTransactionsPerBlock;
    cout << "Number of transactions per block: " << numTransactionsPerBlock << endl;

    int totalNumTransactions; // total transactions to be read from the input file

    cin >> totalNumTransactions;
    cout << "Total number of transactions: " << totalNumTransactions << endl;

    // object of block network
    blockNetwork *n1 = new blockNetwork(numNodesInNetwork, numTransactionsPerBlock);

    // TODO: input the transaction information and insert to n1
    int uNode;
    int vNode;

    cin >> numEdges;
    for (int i = 0; i < numEdges; i++) {
        cin >> uNode >> vNode;
        n1 -> addEdge(uNode, vNode);
    }

    // Handling transaction information
    int node;
    int transactionID;
    int fromID;
    int toID;
    int amountTrans;
    string timeStamp;

    while (cin >> node >> transactionID >> fromID >> toID >> amountTrans >> timeStamp) {
        transaction* t = new transaction(node, transactionID, fromID, toID, amountTrans, timeStamp);
        n1 -> insertTranToNode(node, t);
    }

    n1->display(); // make sure you write display methods for each class

    return 0;
}
