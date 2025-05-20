#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>
#include "blockNetwork.h"

TEST(BlockNetworkTest, DisplayOutput) {
    blockNetwork net(1, 2);
    transaction t(0, 1, 1, 2, 5, "ts");
    net.insertTranToNode(0, t);

    std::ostringstream oss;
    auto* old = std::cout.rdbuf(oss.rdbuf());
    net.display();
    std::cout.rdbuf(old);
    std::string output = oss.str();

    EXPECT_NE(output.find("Node 0"), std::string::npos);
    EXPECT_NE(output.find("Block Number"), std::string::npos);
}

TEST(BlockNetworkTest, VerifyAllChains) {
    blockNetwork net(1, 2);
    transaction t(0, 1, 1, 2, 5, "ts");
    net.insertTranToNode(0, t);
    EXPECT_TRUE(net.verifyAllChains());
}

