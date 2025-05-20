#include "gtest.h"
#include <iostream>

std::vector<TestEntry>& GetTests();

int main() {
    int failures = 0;
    for (const auto& t : GetTests()) {
        try {
            t.func();
        } catch (const std::exception& e) {
            std::cerr << t.name << " threw exception: " << e.what() << std::endl;
            ++failures;
            continue;
        }
        failures += GetFailureCount();
        GetFailureCount() = 0;
    }
    if (failures) {
        std::cerr << failures << " tests failed" << std::endl;
        return 1;
    }
    std::cout << "All tests passed" << std::endl;
    return 0;
}
