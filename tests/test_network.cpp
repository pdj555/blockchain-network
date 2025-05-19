#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "blockNetwork.h"

TEST(BlockNetworkTest, DisplayOutput) {
    blockNetwork net(1, 2);
    transaction t(0, 1, 1, 2, 5, "ts");
    net.insertTranToNode(0, t);

    testing::internal::CaptureStdout();
    net.display();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Node 0"), std::string::npos);
    EXPECT_NE(output.find("Block Number"), std::string::npos);
}

