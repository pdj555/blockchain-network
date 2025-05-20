#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <string>

struct TestEntry {
    std::string name;
    std::function<void()> func;
};

inline int &GetFailureCount() {
    static int count = 0;
    return count;
}

inline std::vector<TestEntry>& GetTests() {
    static std::vector<TestEntry> tests;
    return tests;
}

struct TestRegistrar {
    TestRegistrar(const std::string& n, std::function<void()> f) {
        GetTests().push_back({n, f});
    }
};

#define TEST(suite, name) \
    void suite##_##name(); \
    static TestRegistrar registrar_##suite##_##name(#suite "." #name, suite##_##name); \
    void suite##_##name()

#define EXPECT_EQ(val1, val2) \
    do { \
        auto v1 = (val1); \
        auto v2 = (val2); \
        if (!(v1 == v2)) { \
            std::cerr << __FILE__ << ":" << __LINE__ << " Expectation failed: " \
                      << #val1 << " == " << #val2 \
                      << " (" << v1 << " vs " << v2 << ")" << std::endl; \
            ++GetFailureCount(); \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << __FILE__ << ":" << __LINE__ << " Expectation failed: " \
                      << #cond << std::endl; \
            ++GetFailureCount(); \
        } \
    } while (0)

#define EXPECT_NE(val1, val2) \
    do { \
        auto v1 = (val1); \
        auto v2 = (val2); \
        if (v1 == v2) { \
            std::cerr << __FILE__ << ":" << __LINE__ << " Expectation failed: " \
                      << #val1 << " != " << #val2 << std::endl; \
            ++GetFailureCount(); \
        } \
    } while (0)

