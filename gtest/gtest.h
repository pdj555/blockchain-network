#ifndef GTEST_GTEST_H
#define GTEST_GTEST_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

namespace testing {

struct Test {
    std::string name;
    std::function<void()> func;
};

inline std::vector<Test>& registry() {
    static std::vector<Test> r;
    return r;
}

inline void RegisterTest(const std::string& name, std::function<void()> func) {
    registry().push_back({name, func});
}

inline int RunAllTests() {
    int failed = 0;
    for (const auto& t : registry()) {
        try {
            t.func();
            std::cout << "[       OK ] " << t.name << "\n";
        } catch (const std::exception& e) {
            std::cout << "[  FAILED  ] " << t.name << " - " << e.what() << "\n";
            failed++;
        }
    }
    if (failed) {
        std::cout << failed << " test(s) failed\n";
    } else {
        std::cout << "All tests passed\n";
    }
    return failed;
}

namespace internal {
inline std::stringstream& captured() {
    static std::stringstream cap;
    return cap;
}
inline std::streambuf*& oldBuf() {
    static std::streambuf* buf = nullptr;
    return buf;
}

inline void CaptureStdout() {
    oldBuf() = std::cout.rdbuf(captured().rdbuf());
}

inline std::string GetCapturedStdout() {
    std::cout.rdbuf(oldBuf());
    oldBuf() = nullptr;
    std::string s = captured().str();
    captured().str("");
    captured().clear();
    return s;
}
} // namespace internal

} // namespace testing

#define TEST(Suite, Name) \
    void Suite##_##Name(); \
    namespace { \
    struct Suite##_##Name##_Reg { \
        Suite##_##Name##_Reg() { testing::RegisterTest(#Suite "." #Name, Suite##_##Name); } \
    } Suite##_##Name##_instance; \
    } \
    void Suite##_##Name()

#define EXPECT_TRUE(expr) \
    do { if(!(expr)) throw std::runtime_error("EXPECT_TRUE failed: " #expr); } while(0)

#define EXPECT_EQ(a,b) \
    do { if(!((a)==(b))) throw std::runtime_error("EXPECT_EQ failed"); } while(0)

#define EXPECT_NE(a,b) \
    do { if(!((a)!=(b))) throw std::runtime_error("EXPECT_NE failed"); } while(0)

#define EXPECT_LT(a,b) \
    do { if(!((a)<(b))) throw std::runtime_error("EXPECT_LT failed"); } while(0)

#endif // GTEST_GTEST_H
