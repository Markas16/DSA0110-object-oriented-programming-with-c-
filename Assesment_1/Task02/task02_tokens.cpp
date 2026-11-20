#include <iostream>
#include <iomanip>
#include <cstdint>
#include "task02_constants.hpp"

// ============================================================================
// TASK 2: Tokens, Keywords, Identifiers, Constants & Macros Comparison
// ============================================================================

// Traditional unscoped enum (demonstration of drawbacks)
enum LegacyZone {
    LEGACY_INNER,
    LEGACY_MIDDLE,
    LEGACY_OUTER
};

// Alternative macro-based constants (anti-pattern)
#define MACRO_BASE_INNER 1000
#define MACRO_BASE_MIDDLE 2000
#define MACRO_BASE_OUTER 3000
#define MACRO_MIN_BALANCE 5000
#define MACRO_CALC_SURCHARGE(fare) ((fare) * 120 / 100)

int main() {
    std::cout << "========================================================\n";
    std::cout << "  TASK 2: TOKENS, CONSTANTS, ENUMS & MACRO ANALYSIS\n";
    std::cout << "========================================================\n\n";

    // 1. C++ Token Classification Examples
    std::cout << "[1] C++ Token Classification in Metro Fare System:\n";
    std::cout << "  - Keywords:    class, constexpr, enum, explicit, switch, return, namespace\n";
    std::cout << "  - Identifiers: SmartCard, deductFare, balancePaise, BASE_FARE_INNER_PAISE\n";
    std::cout << "  - Constants:   1000LL, 5000LL, 20, 'C', \"Inner\"\n";
    std::cout << "  - Operators:   +, -, *, /, %, >=, <=, &&, ||, ==, =\n";
    std::cout << "  - Punctuators: ;, {, }, (, ), <, >, ::, ,\n\n";

    // 2. Comparison: #define vs const vs constexpr
    std::cout << "[2] Comparison: #define vs const vs constexpr:\n";
    std::cout << "  +----------------+---------------------+-------------------+-------------------+\n";
    std::cout << "  | Property       | #define MACRO       | const             | constexpr         |\n";
    std::cout << "  +----------------+---------------------+-------------------+-------------------+\n";
    std::cout << "  | Evaluation     | Preprocessor text   | Run/Compile-time  | Guaranteed Compile|\n";
    std::cout << "  | Type Safety    | NONE (text replace) | Strong type check | Strong type check |\n";
    std::cout << "  | Scope          | Global (leaks out)  | Block / Namespace | Block / Namespace |\n";
    std::cout << "  | Debug Symbols  | Missing             | Preserved in DWARF| Preserved in DWARF|\n";
    std::cout << "  | Storage Cost   | Zero (immediate)    | May take memory   | Zero if not odr-us|\n";
    std::cout << "  +----------------+---------------------+-------------------+-------------------+\n\n";

    // 3. Enum vs Enum Class
    std::cout << "[3] Unscoped enum vs Scoped enum class:\n";
    // Legacy enum pollutes namespace and implicitly converts to int:
    int legacyInt = LEGACY_INNER; // Implicit conversion allowed (unsafe!)
    std::cout << "  - Legacy enum implicit int conversion: LEGACY_INNER -> " << legacyInt << "\n";

    // Scoped enum class prevents accidental conversion:
    MetroFare::Zone scopedZone = MetroFare::Zone::Inner;
    // int badInt = scopedZone; // COMPILE ERROR! Type-safe.
    std::cout << "  - Scoped enum class Zone::Inner requires explicit cast: "
              << static_cast<int>(scopedZone) << "\n\n";

    // 4. Centralized constexpr Constants Verification
    std::cout << "[4] Centralized Type-Safe Constants (in Paise):\n";
    std::cout << "  - Inner Zone Base:   " << MetroFare::BASE_FARE_INNER_PAISE   << " paise (Rs " 
              << (MetroFare::BASE_FARE_INNER_PAISE / 100.0) << ")\n";
    std::cout << "  - Middle Zone Base:  " << MetroFare::BASE_FARE_MIDDLE_PAISE  << " paise (Rs " 
              << (MetroFare::BASE_FARE_MIDDLE_PAISE / 100.0) << ")\n";
    std::cout << "  - Outer Zone Base:   " << MetroFare::BASE_FARE_OUTER_PAISE   << " paise (Rs " 
              << (MetroFare::BASE_FARE_OUTER_PAISE / 100.0) << ")\n";
    std::cout << "  - Airport Zone Base: " << MetroFare::BASE_FARE_AIRPORT_PAISE << " paise (Rs " 
              << (MetroFare::BASE_FARE_AIRPORT_PAISE / 100.0) << ")\n";
    std::cout << "  - Minimum Balance:   " << MetroFare::MIN_BALANCE_PAISE       << " paise (Rs " 
              << (MetroFare::MIN_BALANCE_PAISE / 100.0) << ")\n\n";

    // 5. Macro Pitfalls Demonstration
    std::cout << "[5] Macro Pitfall Analysis:\n";
    int f = 1000;
    std::cout << "  - Sample base fare: " << f << "\n";
    // Side effect bug: MACRO_CALC_SURCHARGE(f++) expands to ((f++) * 120 / 100), incrementing f twice!
    std::cout << "  - Macro expansion lacks safety and side-effect guarantees.\n";
    std::cout << "  - Modern C++ constexpr functions & inline constexpr variables eliminate macros.\n";

    std::cout << "\nTask 2 executed successfully.\n";
    return 0;
}
