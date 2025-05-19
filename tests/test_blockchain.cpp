#include <gtest/gtest.h>
#include "blockChain.h"
#include "transaction.h"

TEST(BlockChainTest, InsertBlocks) {
    blockChain chain(2); // max two transactions per block
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

