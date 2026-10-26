#include <iostream>
#include <iomanip>
#include <cstdint>
#include <utility>

// ============================================================================
// TASK 4: Safe Explicit Conversions and Narrowing Prevention
// ============================================================================

// Safe comparison helper for signed vs unsigned integers
template <typename SignedT, typename UnsignedT>
constexpr bool safe_less_than(SignedT s, UnsignedT u) noexcept {
    if (s < 0) {
        return true; // Negative signed is always less than any unsigned
    }
    // Safe to cast since s >= 0
    return static_cast<UnsignedT>(s) < u;
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "  TASK 4: SAFE CONVERSIONS, BRACE INIT & STATIC_CAST\n";
    std::cout << "========================================================\n\n";

    // 1. Safe Division with static_cast
    int totalPaise = 700;
    int divisor = 200;
    double exactRupees = static_cast<double>(totalPaise) / divisor;
    std::cout << "[1] Controlled static_cast division:\n";
    std::cout << "    totalPaise / divisor = " << exactRupees << " (Rupees)\n\n";

    // 2. Narrowing Prevention with Brace Initialization
    std::cout << "[2] Narrowing Conversion Prevention via Brace Initialization:\n";
    std::cout << "    - C-style assignment 'int x = 3.99;' silently truncates to 3.\n";
    std::cout << "    - Modern C++ brace init 'int x{3.99};' causes a COMPILER ERROR.\n";
    int validInit{5000}; // Clean, type-safe initialization
    std::cout << "    - 'int validInit{5000}' successfully initialized: " << validInit << "\n\n";

    // 3. Safe Signed vs Unsigned Comparison
    int s = -1;
    unsigned int u = 1;
    std::cout << "[3] Safe Signed vs Unsigned Comparison:\n";
    std::cout << "    - Unsafe naive comparison (s < u): " << (s < u ? "s < u" : "s >= u (TRAP!)") << "\n";
    std::cout << "    - Safe helper safe_less_than(s, u): " 
              << (safe_less_than(s, u) ? "s < u (CORRECT)" : "s >= u") << "\n\n";

    // 4. Complexity Discussion
    std::cout << "[4] Complexity & Efficiency Analysis:\n";
    std::cout << "    - Time Complexity : O(1) - single branch or zero-cost static type cast.\n";
    std::cout << "    - Space Complexity: O(1) - zero heap allocations or extra registers.\n";

    return 0;
}
