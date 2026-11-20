#include <iostream>

// ============================================================================
// TASK 5: Supplied Prediction Program
// ============================================================================
// PREDICTED OUTPUT BEFORE EXECUTION:
// ----------------------------------
// Line 1: 11 16
// Line 2: 10 6 4
// Line 3: 6 0
// Line 4: 3 1 -3 -1
// Line 5: 18
//
// LINE-BY-LINE EXPLANATION:
// Line 1 (11 16):
//   Operator Precedence: Multiplication (*) has higher precedence than addition (+).
//   1 + 2 * 5 evaluates as 1 + (2 * 5) = 1 + 10 = 11.
//   (1 + 2) * 5 + 1 forces addition first: (3 * 5) + 1 = 15 + 1 = 16.
//
// Line 2 (10 6 4):
//   Conditional (Ternary) Operator:
//   With a = 10 and b = 6, the expression (a > b ? a - b : b - a) evaluates to 10 - 6 = 4.
//   Printing a, b, and the result outputs: 10 6 4.
//
// Line 3 (6 0):
//   Increment and Short-Circuit Evaluation:
//   Starting with i = 5, ++i increments i to 6 and evaluates to 6.
//   In the logical expression (i < 5 && ++counter), since (6 < 5) is false,
//   the right-hand side is short-circuited and not evaluated.
//   Printing i and the boolean expression result (as int) outputs: 6 0.
//
// Line 4 (3 1 -3 -1):
//   Integer Division & Remainder with Negative Operands (C++11/C++17):
//   Division truncates towards zero:
//     7 / 2 = 3,       7 % 2 = 1
//    -7 / 2 = -3,     -7 % 2 = -1
//   The relationship (a/b)*b + (a%b) == a holds strictly: (-3)*2 + (-1) = -7.
//
// Line 5 (18):
//   20% Peak Surcharge Calculation:
//   Base fare of 15 units with 20% surcharge applied:
//   (base * 120) / 100 = (15 * 120) / 100 = 1800 / 100 = 18.
// ============================================================================

int main() {
    // Line 1: Precedence of arithmetic operators
    int p1 = 1 + 2 * 5;
    int p2 = (1 + 2) * 5 + 1;
    std::cout << p1 << " " << p2 << "\n";

    // Line 2: Conditional ternary operator
    int a = 10;
    int b = 6;
    int diff = (a > b) ? (a - b) : (b - a);
    std::cout << a << " " << b << " " << diff << "\n";

    // Line 3: Prefix increment and short-circuit evaluation
    int i = 5;
    int inc = ++i; // i becomes 6
    int shortCircuit = (inc < 5 && false); // false, 0
    std::cout << inc << " " << shortCircuit << "\n";

    // Line 4: Division and remainder with negative numbers
    std::cout << (7 / 2) << " " << (7 % 2) << " "
              << (-7 / 2) << " " << (-7 % 2) << "\n";

    // Line 5: Peak surcharge calculation (15 base + 20% = 18)
    int baseFare = 15;
    int peakFare = (baseFare * 120) / 100;
    std::cout << peakFare << "\n";

    return 0;
}
