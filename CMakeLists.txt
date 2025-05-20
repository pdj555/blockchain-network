cmake_minimum_required(VERSION 3.10)
project(BlockchainNetwork)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(blockchain
    block.cpp
    blockChain.cpp
    blockNetwork.cpp
    transaction.cpp
)

# headers are in source directory
target_include_directories(blockchain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(p4 main.cpp)
target_link_libraries(p4 PRIVATE blockchain)

# ---- Tests ----
enable_testing()
add_executable(tests
    tests/test_transaction.cpp
    tests/test_blockchain.cpp
    tests/test_network.cpp
    gtest/gtest_main.cpp
)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests PRIVATE blockchain)
add_test(NAME all_tests COMMAND tests)

