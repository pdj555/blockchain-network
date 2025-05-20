#ifndef GTEST_GTEST_H
#define GTEST_GTEST_H

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <sstream>

namespace testing {

struct TestInfo {
    const char* name;
    std::function<void()> func;
};

inline std::vector<TestInfo>& getTests() {
    static std::vector<TestInfo> tests;
    return tests;
}

inline int& failureCount() {
    static int count = 0;
    return count;
}

class TestRegistrar {
public:
    TestRegistrar(const char* name, std::function<void()> func) {
        getTests().push_back({name, func});
    }
};

namespace internal {

inline std::streambuf*& coutBuf() {
    static std::streambuf* buf = nullptr;
    return buf;
}

inline std::ostringstream& captured() {
    static std::ostringstream ss;
    return ss;
}

inline void CaptureStdout() {
    captured().str("");
    captured().clear();
    coutBuf() = std::cout.rdbuf(captured().rdbuf());
}

inline std::string GetCapturedStdout() {
    if (coutBuf()) {
        std::cout.rdbuf(coutBuf());
        coutBuf() = nullptr;
    }
    return captured().str();
}

} // namespace internal

} // namespace testing

#define TEST(SuiteName, TestName) \
    void SuiteName##_##TestName(); \
    static testing::TestRegistrar registrar_##SuiteName##_##TestName(#SuiteName "." #TestName, SuiteName##_##TestName); \
    void SuiteName##_##TestName()

#define EXPECT_EQ(val1, val2) \
    do { \
        if (!((val1) == (val2))) { \
            std::cerr << __FILE__ << ":" << __LINE__ << ": expected equality\n"; \
            testing::failureCount()++; \
        } \
    } while (0)

#define EXPECT_NE(val1, val2) \
    do { \
        if (!((val1) != (val2))) { \
            std::cerr << __FILE__ << ":" << __LINE__ << ": expected inequality\n"; \
            testing::failureCount()++; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << __FILE__ << ":" << __LINE__ << ": expected true\n"; \
            testing::failureCount()++; \
        } \
    } while (0)

#define EXPECT_FALSE(cond) \
    do { \
        if (cond) { \
            std::cerr << __FILE__ << ":" << __LINE__ << ": expected false\n"; \
            testing::failureCount()++; \
        } \
    } while (0)

#endif // GTEST_GTEST_H
