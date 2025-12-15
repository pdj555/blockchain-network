#include <gtest/gtest.h>
#include "blockChain.h"
#include "transaction.h"

TEST(BlockChainTest, InsertBlocks) {
    blockChain chain(2); // max two transactions per block
    chain.setLogStream(nullptr);
    transaction t1(0, 1, 2, 3, 10, "t1");
    transaction t2(0, 2, 2, 3, 20, "t2");
    transaction t3(0, 3, 3, 4, 30, "t3");

    chain.insertTran(t1);
    EXPECT_EQ(chain.getCurrNumBlocks(), 1);
    EXPECT_EQ(chain.getFront().getCurrNumTran(), 1);

    chain.insertTran(t2);
    EXPECT_EQ(chain.getCurrNumBlocks(), 1);
    EXPECT_EQ(chain.getFront().getCurrNumTran(), 2);

    chain.insertTran(t3);
    EXPECT_EQ(chain.getCurrNumBlocks(), 2);
    EXPECT_EQ(chain.getFront().getCurrNumTran(), 1);
}

TEST(BlockChainTest, VerifyChainIntegrity) {
    blockChain chain(2);
    chain.setLogStream(nullptr);
    transaction t1(0, 1, 1, 2, 10, "t1");
    transaction t2(0, 2, 2, 3, 20, "t2");
    chain.insertTran(t1);
    chain.insertTran(t2);
    EXPECT_TRUE(chain.verifyChain());
}

TEST(BlockChainTest, DetectTampering) {
    blockChain chain(2);
    chain.setLogStream(nullptr);
    transaction t1(0, 1, 1, 2, 10, "t1");
    transaction t2(0, 2, 2, 3, 20, "t2");
    chain.insertTran(t1);
    chain.insertTran(t2);
    chain.tamperPrevHash(0, "badhash");
    EXPECT_TRUE(!chain.verifyChain());
}

TEST(BlockChainTest, RecordsPreTransactionBalances) {
    blockChain chain(1);
    chain.setLogStream(nullptr);

    transaction t1(0, 1, 1, 2, 10, "t1");
    transaction t2(0, 2, 2, 1, 5, "t2");

    chain.insertTran(t1);
    chain.insertTran(t2);

    const block *first = chain.getBlockFromOldest(1);
    const block *second = chain.getBlockFromOldest(2);
    EXPECT_TRUE(first != nullptr);
    EXPECT_TRUE(second != nullptr);

    EXPECT_EQ(first->getTran(0).getFromValue(), 100);
    EXPECT_EQ(first->getTran(0).getToValue(), 100);

    EXPECT_EQ(second->getTran(0).getFromValue(), 110);
    EXPECT_EQ(second->getTran(0).getToValue(), 90);
}
