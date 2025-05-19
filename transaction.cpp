#include "transaction.h"

transaction::transaction() {}

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

void transaction::setTranID(int tranID) {
    tID = tranID;
}

void transaction::setFromID(int fID) {
    fromID = fID;
}

void transaction::setToID(int targetID) {
    toID = targetID;
}

void transaction::setTranAmount(int tranAmount) {
    tAmount = tranAmount;
}

void transaction::setTimeStamp(string tS) {
    timeStamp = tS;
}

void transaction::setTNodeNum(int node) {
    nodeNum = node;
}

void transaction::setFromValue(int fV) {
    fromValue = fV;
}

void transaction::setToValue(int tV) {
    toValue = tV;
}

int transaction::getTranID() const {
    return tID;
}

int transaction::getFromID() const {
    return fromID;
}

int transaction::getToID() const {
    return toID;
}

int transaction::getTranAmount() const {
    return tAmount;
}

string transaction::getTimeStamp() const {
    return timeStamp;
}

int transaction::getTNodeNum() const {
    return nodeNum;
}

int transaction::getFromValue() const {
    return fromValue;
}

int transaction::getToValue() const {
    return toValue;
}

void transaction::displayTransaction(int newFromValue, int newToValue) {
    cout << tID << " " << fromID << " " << newFromValue + tAmount << " " << toID << " " << newToValue - tAmount << " " << tAmount << " " << timeStamp << endl;
    fromValue = newFromValue;
    toValue = newToValue;
}


