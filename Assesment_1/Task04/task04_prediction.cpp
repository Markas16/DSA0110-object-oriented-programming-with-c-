#include <iostream>

// ============================================================================
// TASK 4: Supplied Prediction Program
// ============================================================================
// PREDICTED OUTPUT BEFORE EXECUTION:
// ----------------------------------
// Line 1: 3 3.5
// Line 2: C 68
// Line 3: 1 true
// Line 4: s >= u
//
// LINE-BY-LINE EXPLANATION:
// 1. (7 / 2) performs integer division (truncates towards zero) -> 3.
//    (static_cast<double>(7) / 2) promotes 2 to double -> 3.5.
// 2. char 'C' prints as character 'C'.
//    ('C' + 1) undergoes integral promotion to int (ASCII 67 + 1 = 68).
// 3. bool 'true' without boolalpha prints numeric 1.
//    After std::boolalpha, it prints textual "true".
// 4. In (s < u) where s = -1 (int) and u = 1 (unsigned int), usual arithmetic
//    conversions convert s to unsigned int (0xFFFFFFFF or 4,294,967,295).
//    Since 4294967295 < 1 is FALSE, the else branch executes: "s >= u".
// ============================================================================

int main() {
    // Prediction verification output header
    // Line 1: Integer division and floating-point promotion
    int a = 7;
    int b = 2;
    std::cout << (a / b) << " " << (static_cast<double>(a) / b) << "\n";

    // Line 2: Character representation and integral promotion
    char c = 'C';
    std::cout << c << " " << (c + 1) << "\n";

    // Line 3: Boolean stream formatting
    bool flag = true;
    std::cout << flag << " " << std::boolalpha << flag << "\n";

    // Line 4: Signed vs. Unsigned comparison trap
    int s = -1;
    unsigned int u = 1;
    if (s < u) {
        std::cout << "s < u\n";
    } else {
        std::cout << "s >= u\n";
    }

    return 0;
}
