#include "transaction.h"
#include <iostream>

transaction::transaction()
    : nodeNum(0),
      tID(0),
      fromID(0),
      fromValue(100),
      toID(0),
      toValue(100),
      tAmount(0),
      timeStamp() {}

transaction::transaction(int tempNode,
                         int temptID,
                         int tempfromID,
                         int temptoID,
                         int temptAmount,
                         const std::string &temptimeStamp)
    : nodeNum(tempNode),
      tID(temptID),
      fromID(tempfromID),
      fromValue(100),
      toID(temptoID),
      toValue(100),
      tAmount(temptAmount),
      timeStamp(temptimeStamp) {}

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

void transaction::setTimeStamp(const std::string &tS) {
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

const std::string &transaction::getTimeStamp() const {
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

void transaction::displayTransaction(std::ostream &out) const {
    out << tID << " " << fromID << " " << fromValue << " " << toID << " " << toValue << " " << tAmount
        << " " << timeStamp << std::endl;
}

void transaction::displayTransaction() const {
    displayTransaction(std::cout);
}
