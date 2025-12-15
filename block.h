#pragma once
#include <cstddef>
#include <iosfwd>
#include <vector>
#include <string>
#include "transaction.h"

class block
{
    int blockNumber;
    int currentNumTransactions;
    int maxNumTransactions;
    std::vector<transaction> bTransactions;
    std::string hash;
    std::string prevHash;
public:
    block();
    block(int bNumber, int maxTransactions);
    void insertTran(const transaction &t);

    void computeHash();
    std::string getHash() const;
    void setPrevHash(const std::string &pHash);
    std::string getPrevHash() const;
    std::string calculateHash() const;

    void setBlockNumber(int bN);
    void setCurrNumTran(int cnt);
    void setMaxNumTran(int mnt);

    int getBlockNumber() const;
    int getCurrNumTran() const;
    int getMaxNumTran() const;

    const transaction &getTran(std::size_t index) const;
    void displayTransactions(std::ostream &out) const;
    void displayTransactions() const;
    bool searchTrans(int id) const;
};
