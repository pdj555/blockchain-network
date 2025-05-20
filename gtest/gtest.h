#ifndef LIGHTWEIGHT_GTEST_H
#define LIGHTWEIGHT_GTEST_H

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace testing {

inline int& tests_failed() {
    static int v = 0;
    return v;
}

namespace internal {
inline std::streambuf*& captured_buf() {
    static std::streambuf* buf = nullptr;
    return buf;
}
inline std::stringstream& captured() {
    static std::stringstream s;
    return s;
}

inline void CaptureStdout() {
    captured().str("");
    captured().clear();
    captured_buf() = std::cout.rdbuf(captured().rdbuf());
}

inline std::string GetCapturedStdout() {
    std::cout.rdbuf(captured_buf());
    return captured().str();
}
} // namespace internal

struct TestInfo {
    const char* name;
    std::function<void()> func;
};

inline std::vector<TestInfo>& registry() {
    static std::vector<TestInfo> r;
    return r;
}

inline void Register(const char* name, std::function<void()> func) {
    registry().push_back({name, func});
}

inline int RunAllTests() {
    for (auto& t : registry()) {
        t.func();
    }
    if (tests_failed()) {
        std::cout << tests_failed() << " tests failed" << std::endl;
    }
    return tests_failed() ? 1 : 0;
}

} // namespace testing

#define TEST(suite, name) \
    void suite##_##name##_impl(); \
    namespace { \
    struct suite##_##name##_registrar { \
        suite##_##name##_registrar() { \
            testing::Register(#suite "." #name, suite##_##name##_impl); \
        } \
    }; \
    static suite##_##name##_registrar suite##_##name##_registrar_instance; \
    } \
    void suite##_##name##_impl()

#define EXPECT_EQ(val1, val2) \
    if (!((val1) == (val2))) { \
        std::cout << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << ": " << (val1) << " != " << (val2) << std::endl; \
        ++testing::tests_failed(); \
    }

#define EXPECT_NE(val1, val2) \
    if (!((val1) != (val2))) { \
        std::cout << "EXPECT_NE failed at " << __FILE__ << ":" << __LINE__ \
                  << ": " << (val1) << " == " << (val2) << std::endl; \
        ++testing::tests_failed(); \
    }

#define EXPECT_TRUE(cond) \
    if (!(cond)) { \
        std::cout << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                  << ": " << #cond << std::endl; \
        ++testing::tests_failed(); \
    }

#endif // LIGHTWEIGHT_GTEST_H
