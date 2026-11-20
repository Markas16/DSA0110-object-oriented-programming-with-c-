#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>

// ============================================================================
// TASK 7 (Part 1): Procedural Metro Fare Implementation
// ============================================================================
// Characteristics:
// - Direct manipulation of primitive variables.
// - State is not encapsulated; any function can modify 'balancePaise'.
// - Lack of invariants: nothing prevents balancePaise from going below 5000
//   if caller bypasses check.
// ============================================================================

enum class Zone { Inner, Middle, Outer };

const char* getZoneName(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "Unknown";
    }
}

bool isPeakHour(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

int64_t calculateFare(Zone zone, int hour) {
    int64_t base = 0;
    switch (zone) {
        case Zone::Inner:  base = 1000; break; // 1000 paise (Rs 10.00)
        case Zone::Middle: base = 2000; break; // 2000 paise (Rs 20.00)
        case Zone::Outer:  base = 3000; break; // 3000 paise (Rs 30.00)
    }
    if (isPeakHour(hour)) {
        return (base * 120) / 100; // +20%
    }
    return base;
}

bool canTravel(int64_t balancePaise, int64_t farePaise) {
    return (balancePaise - farePaise) >= 5000; // Min balance 5000 paise (Rs 50)
}

bool processJourneyProcedural(int64_t& balancePaise, Zone zone, int hour) {
    int64_t fare = calculateFare(zone, hour);
    if (!canTravel(balancePaise, fare)) {
        std::cout << "[Procedural] Denied! Insufficient balance. Required post-trip >= Rs 50.00, Current: Rs " 
                  << std::fixed << std::setprecision(2) << (balancePaise / 100.0) 
                  << ", Fare: Rs " << (fare / 100.0) << "\n";
        return false;
    }

    balancePaise -= fare;
    std::cout << "[Procedural] Success! Zone: " << getZoneName(zone) 
              << ", Hour: " << hour 
              << ", Fare: Rs " << std::fixed << std::setprecision(2) << (fare / 100.0)
              << ", Remaining Balance: Rs " << (balancePaise / 100.0) << "\n";
    return true;
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "  TASK 7: PROCEDURAL METRO FARE DEMONSTRATION\n";
    std::cout << "========================================================\n\n";

    int64_t balancePaise = 10000; // Rs 100.00

    std::cout << "Initial Balance: Rs " << (balancePaise / 100.0) << "\n\n";

    // Trip 1: Inner zone off-peak (1000 paise)
    processJourneyProcedural(balancePaise, Zone::Inner, 12);

    // Trip 2: Outer zone peak (3600 paise)
    processJourneyProcedural(balancePaise, Zone::Outer, 18);

    // Trip 3: Outer zone peak (3600 paise) -> Denied
    processJourneyProcedural(balancePaise, Zone::Outer, 18);

    // Demonstration of Procedural Flaw: Direct accidental corruption
    std::cout << "\n[Demonstrating Procedural Vulnerability]:\n";
    std::cout << "Because balancePaise is an unencapsulated variable, any rogue code\n"
              << "can execute 'balancePaise = 1000;' directly without invariant checks!\n";
    balancePaise = 1000; // Violated business invariant directly!
    std::cout << "Corrupted balance: Rs " << (balancePaise / 100.0) << " (below minimum limit!)\n";

    return 0;
}
