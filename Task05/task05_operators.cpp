#include <iostream>
#include <iomanip>
#include <array>
#include <chrono>

// ============================================================================
// TASK 5: Operators, Precedence, Peak Logic & Lookup Table Comparison
// ============================================================================

// Method A: Combined Logical Expression
// Precedence: >=, < are evaluated before &&, and && evaluates before ||.
constexpr bool isPeakLogical(int hour) noexcept {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

// Method B: Lookup Table (bool peak[24])
// Pre-computed lookup table stored in static memory
static constexpr std::array<bool, 24> PEAK_LOOKUP = {
    false, false, false, false, false, false, false, false, // 00 - 07
    true,  true,                                            // 08, 09 (Morning Peak)
    false, false, false, false, false, false, false,        // 10 - 16
    true,  true,  true,                                     // 17, 18, 19 (Evening Peak)
    false, false, false, false                              // 20 - 23
};

constexpr bool isPeakLookup(int hour) noexcept {
    if (hour >= 0 && hour < 24) {
        return PEAK_LOOKUP[hour];
    }
    return false;
}

// 20% Peak surcharge calculated strictly with integer arithmetic
constexpr int64_t applyPeakSurcharge(int64_t baseFarePaise) noexcept {
    // baseFare + 20% = (baseFare * 120) / 100
    return (baseFarePaise * 120LL) / 100LL;
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "  TASK 5: OPERATORS, PRECEDENCE & LOOKUP TABLE ANALYSIS\n";
    std::cout << "========================================================\n\n";

    // 1. Verification of Peak Hours across all 24 Hours
    std::cout << "[1] 24-Hour Peak Classification Comparison:\n";
    std::cout << "Hour | Logical Exp | Table Lookup | Match?\n";
    std::cout << "-----+-------------+--------------+--------\n";
    bool allMatch = true;
    for (int h = 0; h < 24; ++h) {
        bool mLog = isPeakLogical(h);
        bool mTab = isPeakLookup(h);
        if (mLog != mTab) allMatch = false;
        if (h == 8 || h == 9 || h == 10 || h == 17 || h == 19 || h == 20) {
            std::cout << std::setw(4) << h << " | "
                      << std::setw(11) << (mLog ? "PEAK" : "Off-Peak") << " | "
                      << std::setw(12) << (mTab ? "PEAK" : "Off-Peak") << " | "
                      << (mLog == mTab ? "YES" : "NO") << "\n";
        }
    }
    std::cout << "All 24 hours identical: " << (allMatch ? "TRUE (VERIFIED)" : "FALSE") << "\n\n";

    // 2. Performance & Complexity Comparison
    std::cout << "[2] Complexity and Design Comparison:\n";
    std::cout << "  +----------------------+------------+-------------+----------------------------------+\n";
    std::cout << "  | Approach             | Time Comp. | Space Comp. | Trade-offs                       |\n";
    std::cout << "  +----------------------+------------+-------------+----------------------------------+\n";
    std::cout << "  | Logical Expression   | O(1)       | O(1) 0 extra| Branch-predictor dependent       |\n";
    std::cout << "  | bool peak[24] Table  | O(1)       | O(1) 24 B   | Direct index, branchless, cache  |\n";
    std::cout << "  +----------------------+------------+-------------+----------------------------------+\n\n";

    // 3. Prefix vs Postfix Increment Explanation
    std::cout << "[3] Prefix vs Postfix Increment Semantics:\n";
    int val = 10;
    std::cout << "  - Initial: val = " << val << "\n";
    int pre = ++val; // Increments first, returns modified lvalue
    std::cout << "  - After ++val : pre = " << pre << ", val = " << val << "\n";
    int post = val++; // Saves copy, increments, returns copy rvalue
    std::cout << "  - After val++ : post = " << post << ", val = " << val << "\n\n";

    // 4. Why 'i = i++' must be avoided
    std::cout << "[4] Analysis of 'i = i++' Undefined/Unspecified Behavior:\n";
    std::cout << "  - In C++03, modifying 'i' more than once between sequence points causes UNDEFINED BEHAVIOR.\n";
    std::cout << "  - In C++17 (P0145R3), assignment evaluates right side before left side, but\n";
    std::cout << "    writing 'i = i++' produces conflicting write intents, confusing compilers and human readers.\n";
    std::cout << "  - It must strictly be replaced with simple '++i;' or 'i += 1;'.\n";

    return 0;
}
