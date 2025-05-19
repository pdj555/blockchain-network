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

    int getTranID();
    int getFromID();
    int getToID();
    int getTranAmount();
    string getTimeStamp();
    int getFromValue();
    int getToValue();
    int getTNodeNum();

    void displayTransaction(int newFromValue, int newToValue);
};
