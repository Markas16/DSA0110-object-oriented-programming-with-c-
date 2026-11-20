#include <iostream>
#include <iomanip>
#include <chrono>

int main() {
    const int iterations = 1000000;

    double doubleTotal = 0.0;
    long long paiseTotal = 0;

    auto startDouble = std::chrono::steady_clock::now();

    for (int i = 0; i < iterations; ++i) {
        doubleTotal += 0.10;
    }

    auto endDouble = std::chrono::steady_clock::now();

    auto startInteger = std::chrono::steady_clock::now();

    for (int i = 0; i < iterations; ++i) {
        paiseTotal += 10;
    }

    auto endInteger = std::chrono::steady_clock::now();

    double doubleTime =
        std::chrono::duration<double, std::milli>(
            endDouble - startDouble
        ).count();

    double integerTime =
        std::chrono::duration<double, std::milli>(
            endInteger - startInteger
        ).count();

    std::cout << std::fixed << std::setprecision(15);

    std::cout << "DOUBLE CALCULATION\n";
    std::cout << "Total: " << doubleTotal << '\n';
    std::cout << "Expected: 100000.000000000000000\n";
    std::cout << "Time: " << doubleTime << " ms\n";

    std::cout << "\nINTEGER PAISE CALCULATION\n";
    std::cout << "Total paise: " << paiseTotal << '\n';
    std::cout << "Total rupees: "
              << paiseTotal / 100.0 << '\n';
    std::cout << "Time: " << integerTime << " ms\n";

    return 0;
}