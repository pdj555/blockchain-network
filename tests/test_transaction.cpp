#include <gtest/gtest.h>
#include "transaction.h"

TEST(TransactionTest, Creation) {
    transaction t(1, 10, 2, 3, 50, "now");
    EXPECT_EQ(t.getTNodeNum(), 1);
    EXPECT_EQ(t.getTranID(), 10);
    EXPECT_EQ(t.getFromID(), 2);
    EXPECT_EQ(t.getToID(), 3);
    EXPECT_EQ(t.getTranAmount(), 50);
    EXPECT_EQ(t.getTimeStamp(), "now");
    EXPECT_EQ(t.getFromValue(), 100);
    EXPECT_EQ(t.getToValue(), 100);
}

