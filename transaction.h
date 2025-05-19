#pragma once
#include <iostream>
#include <string>
using namespace std;

class transaction
{
    int nodeNum;
    int tID;
    int fromID;
    int fromValue;
    int toID;
    int toValue;
    int tAmount;
    string timeStamp;
public:
    transaction();
    transaction(int tempNode, int temptID, int tempfromID, int temptoID, int temptAmount, string temptimeStamp);

    void setTranID(int tranID);
    void setFromID(int fID);
    void setToID(int targetID);
    void setTranAmount(int tranAmount);
    void setTimeStamp(string tS);
    void setFromValue(int fV);
    void setToValue(int tV);
    void setTNodeNum(int node);

    int getTranID() const;
    int getFromID() const;
    int getToID() const;
    int getTranAmount() const;
    string getTimeStamp() const;
    int getFromValue() const;
    int getToValue() const;
    int getTNodeNum() const;

    void displayTransaction(int newFromValue, int newToValue);
};
