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

TEST(BlockNetworkTest, DeduplicatesEdgesForStableMetrics) {
    blockNetwork net(2, 2);
    net.addEdge(0, 1);
    net.addEdge(0, 1);

    EXPECT_EQ(net.getNumEdges(), 1);
    EXPECT_EQ(net.getReachableNodeCount(0), 2);
}


TEST(BlockNetworkTest, RejectsPropagatedTransactionAtomically) {
    blockNetwork net(3, 2);
    net.setLogStream(nullptr);
    net.addEdge(0, 1);
    net.addEdge(0, 2);

    EXPECT_TRUE(net.insertTranToNode(0, transaction(0, 10, 1, 2, 60, "seed1")));
    EXPECT_TRUE(net.insertTranToNode(0, transaction(0, 11, 1, 3, 40, "seed2")));
    EXPECT_EQ(net.getNodeTransactionCount(0), 2);
    EXPECT_EQ(net.getNodeTransactionCount(1), 2);
    EXPECT_EQ(net.getNodeTransactionCount(2), 2);

    net.setPropagationEnabled(false);
    EXPECT_TRUE(net.insertTranToNode(0, transaction(0, 12, 2, 1, 50, "origin-only")));
    net.setPropagationEnabled(true);

    EXPECT_TRUE(!net.insertTranToNode(0, transaction(0, 13, 1, 2, 30, "should-reject")));

    EXPECT_EQ(net.getNodeTransactionCount(0), 3);
    EXPECT_EQ(net.getNodeTransactionCount(1), 2);
    EXPECT_EQ(net.getNodeTransactionCount(2), 2);
    EXPECT_EQ(net.getNodeRejectedTransactionCount(0), 1);
    EXPECT_EQ(net.getNodeRejectedTransactionCount(1), 0);
    EXPECT_EQ(net.getNodeRejectedTransactionCount(2), 0);
}

TEST(BlockNetworkTest, AttributesPropagationRejectionToOriginNode) {
    blockNetwork net(2, 2);
    net.setLogStream(nullptr);
    net.addEdge(0, 1);

    net.setPropagationEnabled(false);
    EXPECT_TRUE(net.insertTranToNode(1, transaction(1, 1, 1, 2, 70, "seed")));
    net.setPropagationEnabled(true);

    EXPECT_TRUE(!net.insertTranToNode(0, transaction(0, 2, 1, 3, 50, "reject")));

    EXPECT_EQ(net.getNodeTransactionCount(0), 0);
    EXPECT_EQ(net.getNodeTransactionCount(1), 1);
    EXPECT_EQ(net.getNodeRejectedTransactionCount(0), 1);
    EXPECT_EQ(net.getNodeRejectedTransactionCount(1), 0);
}

TEST(BlockNetworkTest, RejectsDuplicateTransactionIdsAcrossNetwork) {
    blockNetwork net(3, 2);
    net.setLogStream(nullptr);

    EXPECT_TRUE(net.insertTranToNode(0, transaction(0, 99, 1, 2, 10, "t1")));
    EXPECT_TRUE(!net.insertTranToNode(2, transaction(2, 99, 3, 4, 10, "t2")));

    EXPECT_EQ(net.getNodeTransactionCount(0), 1);
    EXPECT_EQ(net.getNodeTransactionCount(1), 0);
    EXPECT_EQ(net.getNodeTransactionCount(2), 0);
    EXPECT_EQ(net.getNodeRejectedTransactionCount(0), 0);
    EXPECT_EQ(net.getNodeRejectedTransactionCount(1), 0);
    EXPECT_EQ(net.getNodeRejectedTransactionCount(2), 1);
}

TEST(BlockNetworkTest, ReportsFullConsensusWithPropagation) {
    blockNetwork net(3, 2);
    net.setLogStream(nullptr);
    net.addEdge(0, 1);
    net.addEdge(1, 2);

    EXPECT_TRUE(net.insertTranToNode(0, transaction(0, 1, 1, 2, 10, "ts1")));

    EXPECT_EQ(net.getLargestConsensusGroupSize(), 3);
    EXPECT_EQ(net.getConsensusRatio(), 1.0);
}

TEST(BlockNetworkTest, ReportsPartialConsensusWhenNodesDiverge) {
    blockNetwork net(3, 2);
    net.setLogStream(nullptr);
    net.addEdge(0, 1);

    EXPECT_TRUE(net.insertTranToNode(0, transaction(0, 1, 1, 2, 10, "ts1")));
    net.setPropagationEnabled(false);
    EXPECT_TRUE(net.insertTranToNode(2, transaction(2, 2, 3, 4, 5, "ts2")));

    EXPECT_EQ(net.getLargestConsensusGroupSize(), 2);
    EXPECT_EQ(net.getConsensusRatio(), 2.0 / 3.0);
}
