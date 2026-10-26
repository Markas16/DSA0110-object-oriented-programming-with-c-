#include <iostream>
#include <iomanip>
#include <cstdint>
#include <climits>
#include <limits>
#include "../Task02/task02_constants.hpp"

// ============================================================================
// TASK 3: Data Types, Compiler Measurements, Limits, and Precision Analysis
// ============================================================================

int main() {
    std::cout << "========================================================\n";
    std::cout << "  TASK 3: DATA TYPES, SIZEOF MEASUREMENTS & COMPATIBILITY\n";
    std::cout << "========================================================\n\n";

    // 1. Data Type Selection Matrix & Machine Sizeof Measurements
    std::cout << "[1] System Data Type Selection & Actual Compiler Measurements:\n";
    std::cout << std::left 
              << std::setw(22) << "Domain Quantity" 
              << std::setw(16) << "Selected Type" 
              << std::setw(14) << "sizeof (bytes)" 
              << std::setw(20) << "Standard Header" 
              << "Design Justification\n";
    std::cout << std::string(95, '-') << "\n";

    std::cout << std::left 
              << std::setw(22) << "Money / Balance" 
              << std::setw(16) << "int64_t" 
              << std::setw(14) << sizeof(int64_t) 
              << std::setw(20) << "<cstdint>" 
              << "Exact integer paise, prevents overflow up to Rs 9.22e16\n";

    std::cout << std::left 
              << std::setw(22) << "Trip Fare" 
              << std::setw(16) << "int64_t" 
              << std::setw(14) << sizeof(int64_t) 
              << std::setw(20) << "<cstdint>" 
              << "Exact integer arithmetic, safe with surcharge operations\n";

    std::cout << std::left 
              << std::setw(22) << "Time (Hour)" 
              << std::setw(16) << "int / uint8_t" 
              << std::setw(14) << sizeof(int) 
              << std::setw(20) << "built-in" 
              << "Range 0..23; fits in 1 byte, standard int avoids promotion\n";

    std::cout << std::left 
              << std::setw(22) << "Metro Zone" 
              << std::setw(16) << "Zone (enum)" 
              << std::setw(14) << sizeof(MetroFare::Zone) 
              << std::setw(20) << "custom enum" 
              << "Scoped enum class backed by uint8_t; strict type safety\n";

    std::cout << std::left 
              << std::setw(22) << "Peak / Status Flags" 
              << std::setw(16) << "bool" 
              << std::setw(14) << sizeof(bool) 
              << std::setw(20) << "built-in" 
              << "Boolean flags (peak, weekend, concession, pass active)\n";

    std::cout << std::left 
              << std::setw(22) << "Journey Counter" 
              << std::setw(16) << "uint64_t" 
              << std::setw(14) << sizeof(uint64_t) 
              << std::setw(20) << "<cstdint>" 
              << "Non-negative counter supporting millions of transactions\n";

    std::cout << "\n[2] Relevant Limits from <climits> and <limits>:\n";
    std::cout << "  - int32_t Min / Max : " << INT32_MIN << " to " << INT32_MAX << "\n";
    std::cout << "  - int64_t Min / Max : " << INT64_MIN << " to " << INT64_MAX << "\n";
    std::cout << "  - double Digits10   : " << std::numeric_limits<double>::digits10 << " decimal digits precision\n";
    std::cout << "  - float Digits10    : " << std::numeric_limits<float>::digits10  << " decimal digits precision\n\n";

    // 2. Demonstration of IEEE 754 Floating-Point Inaccuracy vs Exact Integer Paise
    std::cout << "[3] Reproducible Precision Comparison: Floating-Point vs Integer Paise:\n";
    double rupeeAcc = 0.0;
    int64_t paiseAcc = 0;
    const int iterations = 100000;
    // Repeatedly add Rs 0.10 (10 paise) 100,000 times: Expected total = Rs 10,000.00
    for (int i = 0; i < iterations; ++i) {
        rupeeAcc += 0.10;
        paiseAcc += 10;
    }

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "  - Adding Rs 0.10 repeatedly 100,000 times:\n";
    std::cout << "    * Expected Total         : Rs 10000.0000000000\n";
    std::cout << "    * Floating-Point (double): Rs " << rupeeAcc << " (Error: " << (rupeeAcc - 10000.0) << ")\n";
    std::cout << "    * Integer Paise (int64)  : Rs " << (paiseAcc / 100.0) << " (Error: 0.0000000000 EXACT)\n";

    // 3. Reproducible 32-bit Integer Overflow Demonstration
    std::cout << "\n[4] Integer Overflow Demonstration in Fare Accumulation:\n";
    std::cout << std::setprecision(2);
    volatile int32_t int32Total = 0;
    int64_t int64Total = 0;
    // Simulate 3,000,000 journeys with average fare 2500 paise (Rs 25.00)
    // Mathematical total = 3,000,000 * 2500 = 7,500,000,000 paise (Rs 75,000,000)
    int64_t tripCount = 3000000;
    int64_t fareSample = 2500;
    for (int64_t i = 0; i < tripCount; ++i) {
        int32Total += static_cast<int32_t>(fareSample);
        int64Total += fareSample;
    }
    std::cout << "  - Accumulating 3,000,000 journeys @ 2500 paise:\n";
    std::cout << "    * Expected Total (Mathematical) : 7,500,000,000 paise (Rs 75,000,000.00)\n";
    std::cout << "    * int32_t Accumulator (32-bit)  : " << int32Total << " paise (OVERFLOW/WRAP-AROUND!)\n";
    std::cout << "    * int64_t Accumulator (64-bit)  : " << int64Total << " paise (CORRECT & STABLE)\n\n";

    // 4. Complexity & Trade-Off Summary
    std::cout << "[5] Complexity & Design Trade-off Comparison:\n";
    std::cout << "  +-------------------+------------+-------------+--------------------------------------+\n";
    std::cout << "  | Representation    | Time Comp. | Space Comp. | Precision & Safety                   |\n";
    std::cout << "  +-------------------+------------+-------------+--------------------------------------+\n";
    std::cout << "  | int64_t (paise)   | O(1)       | O(1) 8B     | Exact integer, immune to float drift |\n";
    std::cout << "  | int32_t (paise)   | O(1)       | O(1) 4B     | Overflows at ~2.14 crore paise       |\n";
    std::cout << "  | double (rupees)   | O(1)       | O(1) 8B     | Accumulated rounding error           |\n";
    std::cout << "  +-------------------+------------+-------------+--------------------------------------+\n";

    std::cout << "\nTask 3 executed successfully.\n";
    return 0;
}
