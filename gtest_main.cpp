#include "gtest/gtest.h"
#include <iostream>

int main() {
    int failures = 0;
    for (const auto &test : testing::getTests()) {
        try {
            test.func();
        } catch (...) {
            std::cerr << "Exception thrown in test " << test.name << std::endl;
            testing::failureCount()++;
        }
    }
    failures = testing::failureCount();
    if (failures) {
        std::cerr << failures << " test(s) failed\n";
    } else {
        std::cout << "All tests passed\n";
    }
    return failures ? 1 : 0;
}
