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

TEST(BlockNetworkTest, PropagatesTransactionsAcrossEdges) {
    blockNetwork net(3, 2);
    net.setLogStream(nullptr);
    net.addEdge(0, 1);
    net.addEdge(1, 2);

    transaction t(0, 1, 1, 2, 5, "ts");
    EXPECT_TRUE(net.insertTranToNode(0, t));

    EXPECT_EQ(net.getNodeTransactionCount(0), 1);
    EXPECT_EQ(net.getNodeTransactionCount(1), 1);
    EXPECT_EQ(net.getNodeTransactionCount(2), 1);
}

TEST(BlockNetworkTest, SupportsOriginOnlyMode) {
    blockNetwork net(2, 2);
    net.setLogStream(nullptr);
    net.setPropagationEnabled(false);
    net.addEdge(0, 1);

    transaction t(0, 1, 1, 2, 5, "ts");
    EXPECT_TRUE(net.insertTranToNode(0, t));

    EXPECT_EQ(net.getNodeTransactionCount(0), 1);
    EXPECT_EQ(net.getNodeTransactionCount(1), 0);
}

TEST(BlockNetworkTest, ReportsReachabilityAndIsolationHealth) {
    blockNetwork net(4, 2);
    net.addEdge(0, 1);
    net.addEdge(1, 2);

    EXPECT_EQ(net.getNumEdges(), 2);
    EXPECT_EQ(net.getReachableNodeCount(0), 3);
    EXPECT_EQ(net.getMaxReachableNodeCount(), 3);
    EXPECT_EQ(net.getIsolatedNodeCount(), 1);
}
