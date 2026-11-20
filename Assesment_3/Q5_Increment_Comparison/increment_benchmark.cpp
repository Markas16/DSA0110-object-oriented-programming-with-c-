#include <iostream>
#include <chrono>

struct Counter {
    int value = 0;

    Counter& operator++() {
        ++value;
        return *this;
    }

    Counter operator++(int) {
        Counter old = *this;
        ++value;
        return old;
    }
};

using Clock = std::chrono::steady_clock;

int main() {
    const int iterations = 10000000;

    Counter prefixCounter;
    Counter postfixCounter;

    auto startPrefix = Clock::now();

    for (int i = 0; i < iterations; ++i) {
        ++prefixCounter;
    }

    auto endPrefix = Clock::now();

    auto startPostfix = Clock::now();

    for (int i = 0; i < iterations; ++i) {
        postfixCounter++;
    }

    auto endPostfix = Clock::now();

    double prefixTime =
        std::chrono::duration<double, std::milli>(
            endPrefix - startPrefix
        ).count();

    double postfixTime =
        std::chrono::duration<double, std::milli>(
            endPostfix - startPostfix
        ).count();

    std::cout << "Iterations: " << iterations << '\n';

    std::cout << "Prefix time: "
              << prefixTime << " ms\n";

    std::cout << "Postfix time: "
              << postfixTime << " ms\n";

    std::cout << "Prefix final value: "
              << prefixCounter.value << '\n';

    std::cout << "Postfix final value: "
              << postfixCounter.value << '\n';

    return 0;
}