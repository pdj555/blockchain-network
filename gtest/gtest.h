#ifndef FAKE_GTEST_H
#define FAKE_GTEST_H
#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include <sstream>
#include <iostream>

namespace testing {
struct TestEntry { std::string name; std::function<void()> fn; };
inline std::vector<TestEntry>& registry() { static std::vector<TestEntry> r; return r; }
inline void addTest(const std::string& name, std::function<void()> fn) { registry().push_back({name, fn}); }
inline int RunAllTests() {
    int failures = 0;
    for (auto& t : registry()) {
        try {
            t.fn();
            std::cout << "[ OK ] " << t.name << std::endl;
        } catch (const std::exception& e) {
            ++failures;
            std::cout << "[FAIL] " << t.name << " " << e.what() << std::endl;
        } catch (...) {
            ++failures;
            std::cout << "[FAIL] " << t.name << " unknown exception" << std::endl;
        }
    }
    return failures;
}
namespace internal {
    inline std::stringstream captured;
    inline std::streambuf* old_buf = nullptr;
    inline void CaptureStdout() {
        captured.str("");
        captured.clear();
        old_buf = std::cout.rdbuf(captured.rdbuf());
    }
    inline std::string GetCapturedStdout() {
        std::cout.rdbuf(old_buf);
        old_buf = nullptr;
        return captured.str();
    }
} // namespace internal
} // namespace testing

#define TEST(suite, name) \
    void suite##_##name(); \
    namespace { struct suite##_##name##_registrar { \
        suite##_##name##_registrar() { testing::addTest(#suite "." #name, suite##_##name); } \
    }; static suite##_##name##_registrar suite##_##name##_instance; } \
    void suite##_##name()

#define EXPECT_EQ(val1, val2) do { if (!((val1) == (val2))) throw std::runtime_error("EXPECT_EQ failed"); } while(0)
#define EXPECT_NE(val1, val2) do { if (!((val1) != (val2))) throw std::runtime_error("EXPECT_NE failed"); } while(0)
#define EXPECT_TRUE(cond) do { if (!(cond)) throw std::runtime_error("EXPECT_TRUE failed"); } while(0)

#endif // FAKE_GTEST_H
