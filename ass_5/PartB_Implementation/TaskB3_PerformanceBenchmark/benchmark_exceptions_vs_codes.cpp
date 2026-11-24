#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <random>
#include <stdexcept>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - Task B3.3: Cost of Exceptions vs Return Codes
 * Measures validation of 1,000,000 bills under 0%, 1%, and 50% error rates.
 * Compile with: g++ -std=c++17 -O2 -Wall -Wextra benchmark_exceptions_vs_codes.cpp
 * ============================================================================
 */

enum class ValidationStatus {
    OK,
    ERROR_NEGATIVE_CHARGE,
    ERROR_EXCEEDS_MAX_CAP
};

// Return code validation (No exceptions thrown)
ValidationStatus validateWithCode(double amount) noexcept {
    if (amount < 0.0) return ValidationStatus::ERROR_NEGATIVE_CHARGE;
    if (amount > 1000000.0) return ValidationStatus::ERROR_EXCEEDS_MAX_CAP;
    return ValidationStatus::OK;
}

// Exception-based validation (Throws on error)
void validateWithException(double amount) {
    if (amount < 0.0) throw std::invalid_argument("negative");
    if (amount > 1000000.0) throw std::out_of_range("too large");
}

int main() {
    std::cout << "=========================================================================================\n";
    std::cout << " TASK B3.3: VALIDATION BENCHMARK: EXCEPTIONS vs RETURN CODES (1,000,000 BILLS) [-O2]     \n";
    std::cout << "=========================================================================================\n\n";

    const size_t NUM_BILLS = 1000000;
    const std::vector<double> errorRates = {0.00, 0.01, 0.50}; // 0%, 1%, 50%

    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(15) << "Error Rate"
              << std::setw(28) << "Return Codes Time (ms)"
              << std::setw(28) << "Exceptions Time (ms)"
              << "Speed Ratio (Ex / Code)\n";
    std::cout << "-----------------------------------------------------------------------------------------\n";

    for (double rate : errorRates) {
        std::vector<double> testData(NUM_BILLS);
        size_t errorCount = static_cast<size_t>(NUM_BILLS * rate);

        for (size_t i = 0; i < NUM_BILLS; ++i) {
            if (i < errorCount) {
                testData[i] = -100.0; // Injected invalid charge
            } else {
                testData[i] = 2500.0; // Valid clinical charge
            }
        }

        // Shuffle test data to simulate realistic stochastic arrival
        std::mt19937 g(42);
        std::shuffle(testData.begin(), testData.end(), g);

        // 1. Measure Return Code Approach
        auto startCode = std::chrono::high_resolution_clock::now();
        size_t codeErrors = 0;
        double codeSum = 0.0;
        for (size_t i = 0; i < NUM_BILLS; ++i) {
            ValidationStatus status = validateWithCode(testData[i]);
            if (status == ValidationStatus::OK) {
                codeSum += testData[i];
            } else {
                codeErrors++;
            }
        }
        auto endCode = std::chrono::high_resolution_clock::now();
        double timeCodeMs = std::chrono::duration<double, std::milli>(endCode - startCode).count();

        // 2. Measure Exception Approach
        auto startEx = std::chrono::high_resolution_clock::now();
        size_t exErrors = 0;
        double exSum = 0.0;
        for (size_t i = 0; i < NUM_BILLS; ++i) {
            try {
                validateWithException(testData[i]);
                exSum += testData[i];
            } catch (const std::exception&) {
                exErrors++;
            }
        }
        auto endEx = std::chrono::high_resolution_clock::now();
        double timeExMs = std::chrono::duration<double, std::milli>(endEx - startEx).count();

        double ratio = timeExMs / timeCodeMs;

        std::cout << std::left << std::setw(15) << (std::to_string(static_cast<int>(rate * 100)) + "%")
                  << std::setw(28) << timeCodeMs
                  << std::setw(28) << timeExMs
                  << std::fixed << std::setprecision(2) << ratio << "x\n";
    }

    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << "Key Finding:\n";
    std::cout << "  - At 0% error rate: Exceptions exhibit near parity or slight lead due to zero-cost DWARF tables.\n";
    std::cout << "  - At 1% error rate: Minimal acceptable overhead for critical error logging and stack unwind.\n";
    std::cout << "  - At 50% error rate: Exceptions are significantly slower due to OS unwinding metadata lookups.\n";
    std::cout << "=========================================================================================\n";
    return 0;
}
