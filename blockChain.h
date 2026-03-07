#pragma once
#include <cstddef>
#include <iosfwd>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include "block.h"

class blockChain
{
public:
    enum class RejectionReason { duplicateId, invalidFields, insufficientFunds };

private:
    int currentNumBlocks;
    std::list<block> bChain;
    int nodeNum;
    std::ostream *logStream;
    int maxTransactionsPerBlock;
    std::unordered_map<int, int> balances;
    std::unordered_set<int> transactionIds;
    int rejectedTransactions;
public:
    blockChain();
    blockChain(int tPerB);
    bool canAcceptTran(const transaction &t) const;
    void recordRejectedTran(const transaction &t);
    void recordRejectedTran(RejectionReason reason);
    bool insertTran(const transaction &t);
    void insertBlockFront(block b);

    bool verifyChain() const;

    // Intentionally modify the prevHash of a block for testing
    void tamperPrevHash(std::size_t index, const std::string &newPrev);

    void setLogStream(std::ostream *out);

    void setCurrNumBlocks(int cnb);
    void setNodeNum(int node);

    int getCurrNumBlocks() const;
    int getTotalTransactions() const;
    int getRejectedTransactions() const;
    std::string getTipHash() const;
    std::string getConsensusFingerprint() const;
    const block &getBack() const;
    const block &getFront() const;
    int getNodeNum() const;

    const block *getBlockFromOldest(int blockNumber) const;
    bool searchID(int transactionId) const;
};
