#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>
#include "blockNetwork.h"

TEST(BlockNetworkTest, DisplayOutput) {
    blockNetwork net(1, 2);
    net.setLogStream(nullptr);
    transaction t(0, 1, 1, 2, 5, "ts");
    net.insertTranToNode(0, t);

    std::ostringstream oss;
    net.display(oss);
    std::string output = oss.str();

    EXPECT_NE(output.find("Node 0"), std::string::npos);
    EXPECT_NE(output.find("Block Number"), std::string::npos);
}

TEST(BlockNetworkTest, VerifyAllChains) {
    blockNetwork net(1, 2);
    net.setLogStream(nullptr);
    transaction t(0, 1, 1, 2, 5, "ts");
    net.insertTranToNode(0, t);
    EXPECT_TRUE(net.verifyAllChains());
}

TEST(BlockNetworkTest, DetectTampering) {
    blockNetwork net(1, 2);
    net.setLogStream(nullptr);
    transaction t1(0, 1, 1, 2, 5, "ts");
    transaction t2(0, 2, 2, 3, 7, "ts2");
    net.insertTranToNode(0, t1);
    net.insertTranToNode(0, t2);
    net.tamperPrevHash(0, 0, "bad");
    EXPECT_TRUE(!net.verifyAllChains());
}

TEST(BlockNetworkTest, ExposesRejectedTransactionCounts) {
    blockNetwork net(1, 2);
    net.setLogStream(nullptr);
    transaction t1(0, 1, 10, 20, 1000, "ts");
    transaction t2(0, 1, 20, 30, 5, "ts2");

    EXPECT_TRUE(!net.insertTranToNode(0, t1));
    EXPECT_TRUE(net.insertTranToNode(0, t2));
    EXPECT_EQ(net.getNodeRejectedTransactionCount(0), 1);
}
