#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdint>

// ============================================================================
// TASK 6: Control Structures and Completed Fare Logic Program
// ============================================================================
//
// ALGORITHMIC PSEUDOCODE:
// -----------------------
// FUNCTION isPeakHour(hour: Integer) -> Boolean
//     IF (hour >= 8 AND hour < 10) OR (hour >= 17 AND hour < 20) THEN
//         RETURN TRUE
//     ELSE
//         RETURN FALSE
//     END IF
// END FUNCTION
//
// FUNCTION calculateFare(zone: String, hour: Integer) -> Integer (paise)
//     baseFare = 0
//     SWITCH (zone)
//         CASE "Inner":  baseFare = 1000
//         CASE "Middle": baseFare = 2000
//         CASE "Outer":  baseFare = 3000
//         DEFAULT:       RETURN -1 (Invalid Zone)
//     END SWITCH
//
//     IF isPeakHour(hour) THEN
//         fare = baseFare + (baseFare * 20) / 100
//     ELSE
//         fare = baseFare
//     END IF
//     RETURN fare
// END FUNCTION
//
// FUNCTION processJourney(balance: ByRef Integer, zone: String, hour: Integer)
//     fare = calculateFare(zone, hour)
//     IF (balance - fare >= 5000) THEN
//         balance = balance - fare
//         PRINT "Fare Rs " + formatMoney(fare) + " balance Rs " + formatMoney(balance)
//     ELSE
//         PRINT "Denied (low balance) balance Rs " + formatMoney(balance)
//     END IF
// END FUNCTION
// ============================================================================

enum class Zone { Inner, Middle, Outer, Invalid };

Zone parseZone(const std::string& z) {
    if (z == "Inner") return Zone::Inner;
    if (z == "Middle") return Zone::Middle;
    if (z == "Outer") return Zone::Outer;
    return Zone::Invalid;
}

bool isPeakHour(int hour) {
    // Peak intervals: 08:00 inclusive to 10:00 exclusive AND 17:00 inclusive to 20:00 exclusive
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

// TODO 1: Select a zone's base fare with switch and apply 20% peak surcharge using integer paise
int64_t calculateFare(Zone zone, int hour) {
    int64_t baseFare = 0;
    switch (zone) {
        case Zone::Inner:
            baseFare = 1000; // 1000 paise = Rs 10.00
            break;
        case Zone::Middle:
            baseFare = 2000; // 2000 paise = Rs 20.00
            break;
        case Zone::Outer:
            baseFare = 3000; // 3000 paise = Rs 30.00
            break;
        default:
            return -1; // Invalid zone
    }

    if (isPeakHour(hour)) {
        // Integer arithmetic for 20% surcharge
        return baseFare + (baseFare * 20) / 100;
    }
    return baseFare;
}

// Helper to format rupees output exactly matching assignment spec:
// Example: "Fare Rs 10.00 balance Rs 90" (cents shown only when needed or matching format)
void printStatus(int64_t farePaise, int64_t balancePaise, bool permitted) {
    if (permitted) {
        std::cout << "Fare Rs " << std::fixed << std::setprecision(2) << (farePaise / 100.0)
                  << " balance Rs " << (balancePaise / 100);
        if (balancePaise % 100 != 0) {
            std::cout << "." << std::setw(2) << std::setfill('0') << (balancePaise % 100);
        }
        std::cout << "\n";
    } else {
        std::cout << "Denied (low balance) balance Rs " << (balancePaise / 100);
        if (balancePaise % 100 != 0) {
            std::cout << "." << std::setw(2) << std::setfill('0') << (balancePaise % 100);
        }
        std::cout << "\n";
    }
}

// TODO 2: Permit travel only when balance minus fare is at least 5000 paise
bool attemptTravel(int64_t& balancePaise, Zone zone, int hour) {
    int64_t fare = calculateFare(zone, hour);
    if (fare < 0) {
        std::cout << "Denied (invalid zone)\n";
        return false;
    }

    if (balancePaise - fare >= 5000) {
        balancePaise -= fare;
        printStatus(fare, balancePaise, true);
        return true;
    } else {
        printStatus(fare, balancePaise, false);
        return false;
    }
}

int main() {
    // ------------------------------------------------------------------------
    // Part A: Assignment Specification Exact Sequence
    // ------------------------------------------------------------------------
    int64_t balancePaise = 10000; // Rs 100.00

    // Journey 1: Inner, off-peak (e.g., 11:00)
    attemptTravel(balancePaise, Zone::Inner, 11);

    // Journey 2: Outer, peak hour (e.g., 18:00)
    attemptTravel(balancePaise, Zone::Outer, 18);

    // Journey 3: Outer, peak hour (e.g., 18:00) -> will be denied due to low balance
    attemptTravel(balancePaise, Zone::Outer, 18);

    // ------------------------------------------------------------------------
    // Part B: Boundary Tests for Exactly 10 AM (10:00) and Exactly 8 PM (20:00)
    // ------------------------------------------------------------------------
    std::cout << "\n--- Boundary Tests for Exact Boundary Hours ---\n";
    
    // Boundary Test 1: Exactly 10 AM (10:00) should be OFF-PEAK
    std::cout << "[Test 10:00 AM Boundary (Off-Peak expected)]: ";
    int64_t fare10 = calculateFare(Zone::Outer, 10);
    std::cout << "Outer Fare @ 10:00 = " << fare10 << " paise (Rs " << (fare10 / 100.0) << ") -> "
              << (fare10 == 3000 ? "PASS (Off-peak Rs 30.00)" : "FAIL") << "\n";

    // Boundary Test 2: Exactly 8 PM (20:00) should be OFF-PEAK
    std::cout << "[Test 20:00 PM Boundary (Off-Peak expected)]: ";
    int64_t fare20 = calculateFare(Zone::Outer, 20);
    std::cout << "Outer Fare @ 20:00 = " << fare20 << " paise (Rs " << (fare20 / 100.0) << ") -> "
              << (fare20 == 3000 ? "PASS (Off-peak Rs 30.00)" : "FAIL") << "\n";

    // Boundary Test 3: Exactly 08:00 AM should be PEAK
    std::cout << "[Test 08:00 AM Boundary (Peak expected)]:     ";
    int64_t fare08 = calculateFare(Zone::Outer, 8);
    std::cout << "Outer Fare @ 08:00 = " << fare08 << " paise (Rs " << (fare08 / 100.0) << ") -> "
              << (fare08 == 3600 ? "PASS (Peak Rs 36.00)" : "FAIL") << "\n";

    // Boundary Test 4: Exactly 17:00 PM should be PEAK
    std::cout << "[Test 17:00 PM Boundary (Peak expected)]:     ";
    int64_t fare17 = calculateFare(Zone::Outer, 17);
    std::cout << "Outer Fare @ 17:00 = " << fare17 << " paise (Rs " << (fare17 / 100.0) << ") -> "
              << (fare17 == 3600 ? "PASS (Peak Rs 36.00)" : "FAIL") << "\n";

    return 0;
}
