#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <cstdint>
#include <cassert>

// ============================================================================
// TASK 12: Day-4 Design Challenge Implementation
// ============================================================================
// Features Implemented:
//   1. 4th Zone: Airport (Base Fare = 6000 paise = Rs 60.00)
//   2. 10% Weekend Discount
//   3. Surcharge & Discount Mutual Exclusivity:
//      Weekend trips NEVER incur peak surcharges; weekend discount applies.
//   4. MonthlyPass Class:
//      Allows unlimited travel in 2 designated zones.
//   5. Documented Policy on Minimum Balance with Pass:
//      Security Policy: Card must still hold >= 5000 paise (Rs 50.00)
//      to pass the turnstile, but Rs 0.00 is deducted for covered zones!
//   6. Configurable Concession Policy:
//      Student Concession (25% off base) or Senior Concession (30% off base)
//      with explicit, documented precedence rules!
// ============================================================================

enum class Zone : uint8_t {
    Inner = 0,
    Middle = 1,
    Outer = 2,
    Airport = 3,
    Unknown = 255
};

const char* zoneToString(Zone z) {
    switch (z) {
        case Zone::Inner:   return "Inner";
        case Zone::Middle:  return "Middle";
        case Zone::Outer:   return "Outer";
        case Zone::Airport: return "Airport";
        default:            return "Unknown";
    }
}

// Configurable Concession Type
enum class ConcessionType {
    None,
    Student, // Illustrative: 25% discount
    Senior   // Illustrative: 30% discount
};

// Monthly Pass model allowing unlimited transit in 2 designated zones
class MonthlyPass {
private:
    bool active_;
    Zone zone1_;
    Zone zone2_;
    uint64_t passExpiryTimestamp_;

public:
    MonthlyPass() : active_(false), zone1_(Zone::Unknown), zone2_(Zone::Unknown), passExpiryTimestamp_(0) {}
    MonthlyPass(Zone z1, Zone z2, uint64_t expiry = 9999999999ULL)
        : active_(true), zone1_(z1), zone2_(z2), passExpiryTimestamp_(expiry) {}

    bool isActive() const noexcept { return active_; }

    // Returns true if destination zone is covered by this pass
    bool coversZone(Zone z) const noexcept {
        if (!active_) return false;
        return (z == zone1_ || z == zone2_);
    }

    std::string getDescription() const {
        if (!active_) return "No Active Pass";
        return std::string("Monthly Pass [Zones: ") + zoneToString(zone1_) + ", " + zoneToString(zone2_) + "]";
    }
};

// Day-4 Configurable Fare Engine
class Day4FareEngine {
public:
    static constexpr int64_t BASE_INNER   = 1000; // Rs 10.00
    static constexpr int64_t BASE_MIDDLE  = 2000; // Rs 20.00
    static constexpr int64_t BASE_OUTER   = 3000; // Rs 30.00
    static constexpr int64_t BASE_AIRPORT = 6000; // Rs 60.00 (Task 12 New Zone)
    static constexpr int64_t MIN_BALANCE_PAISE = 5000; // Rs 50.00

    static constexpr int PEAK_SURCHARGE_PCT  = 20; // 20%
    static constexpr int WEEKEND_DISCOUNT_PCT = 10; // 10%
    static constexpr int STUDENT_CONCESSION_PCT = 25; // Configurable illustrative concession
    static constexpr int SENIOR_CONCESSION_PCT  = 30; // Configurable illustrative concession

    static bool isPeakHour(int hour) noexcept {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    static int64_t getBaseFare(Zone z) noexcept {
        switch (z) {
            case Zone::Inner:   return BASE_INNER;
            case Zone::Middle:  return BASE_MIDDLE;
            case Zone::Outer:   return BASE_OUTER;
            case Zone::Airport: return BASE_AIRPORT;
            default:            return -1;
        }
    }

    // Comprehensive tariff computation with explicit precedence:
    // Precedence Order:
    // 1. Monthly Pass: if covered, Fare = 0 paise.
    // 2. Concessions: if applicable, applies concession rate on base fare.
    // 3. Weekend vs Peak Exclusivity:
    //    - If isWeekend == true: 10% weekend discount applied. Peak surcharge is NEVER applied.
    //    - If isWeekend == false: 20% peak surcharge applied during peak windows.
    static int64_t calculateFare(Zone zone, int hour, bool isWeekend, 
                                 const MonthlyPass& pass, ConcessionType concession) noexcept {
        if (hour < 0 || hour > 23) return -1;
        int64_t base = getBaseFare(zone);
        if (base < 0) return -1;

        // Rule 1: Monthly Pass coverage grants 100% discount
        if (pass.coversZone(zone)) {
            return 0; // Free travel covered by subscription
        }

        // Rule 2: Weekend Exclusivity (No peak surcharges on weekends)
        if (isWeekend) {
            int64_t discounted = base - (base * WEEKEND_DISCOUNT_PCT) / 100;
            // Apply concession if present
            if (concession == ConcessionType::Student) {
                discounted -= (discounted * STUDENT_CONCESSION_PCT) / 100;
            } else if (concession == ConcessionType::Senior) {
                discounted -= (discounted * SENIOR_CONCESSION_PCT) / 100;
            }
            return discounted;
        }

        // Rule 3: Weekday travel
        int64_t fare = base;
        if (isPeakHour(hour)) {
            fare = base + (base * PEAK_SURCHARGE_PCT) / 100;
        }

        if (concession == ConcessionType::Student) {
            fare -= (fare * STUDENT_CONCESSION_PCT) / 100;
        } else if (concession == ConcessionType::Senior) {
            fare -= (fare * SENIOR_CONCESSION_PCT) / 100;
        }

        return fare;
    }
};

// Day-4 Enhanced SmartCard
class Day4SmartCard {
private:
    uint64_t cardId_;
    int64_t balancePaise_;
    MonthlyPass pass_;
    ConcessionType concession_;

public:
    Day4SmartCard(uint64_t id, int64_t initialBalancePaise, ConcessionType conc = ConcessionType::None)
        : cardId_(id), balancePaise_(initialBalancePaise), pass_(), concession_(conc) {}

    void attachMonthlyPass(const MonthlyPass& p) { pass_ = p; }
    int64_t getBalancePaise() const noexcept { return balancePaise_; }
    double getBalanceRupees() const noexcept { return balancePaise_ / 100.0; }

    // Documented Business Invariant for Monthly Pass Trips:
    // Even if trip fare is 0 paise, the commuter must possess a valid card with
    // balance >= MIN_BALANCE (5000 paise) as an entry deposit / anti-passback guarantee.
    bool canEnterGate(int64_t farePaise) const noexcept {
        return (balancePaise_ - farePaise) >= Day4FareEngine::MIN_BALANCE_PAISE;
    }

    bool tapAndGo(Zone zone, int hour, bool isWeekend) {
        int64_t fare = Day4FareEngine::calculateFare(zone, hour, isWeekend, pass_, concession_);
        if (fare < 0) {
            std::cout << "  [Denied] Invalid zone or temporal parameters.\n";
            return false;
        }

        if (!canEnterGate(fare)) {
            std::cout << "  [Denied] Insufficient security balance! Current: Rs " 
                      << getBalanceRupees() << ", Required >= Rs 50.00.\n";
            return false;
        }

        balancePaise_ -= fare;
        std::cout << "  [Approved] Zone: " << std::left << std::setw(8) << zoneToString(zone)
                  << " | Hour: " << std::setw(2) << hour << ":00"
                  << " | " << (isWeekend ? "Weekend" : "Weekday")
                  << " | Fare: Rs " << std::fixed << std::setprecision(2) << (fare / 100.0)
                  << " | Remaining: Rs " << getBalanceRupees()
                  << (pass_.coversZone(zone) ? " (COVERED BY PASS)" : "") << "\n";
        return true;
    }
};

int main() {
    std::cout << "========================================================\n";
    std::cout << "  TASK 12: DAY-4 DESIGN CHALLENGE VALIDATION SUITE\n";
    std::cout << "========================================================\n\n";

    // ------------------------------------------------------------------------
    // Part D Test 1: Airport Zone Tariffs
    // ------------------------------------------------------------------------
    std::cout << "[Test 1] Airport Zone Tariffs (Base = Rs 60.00):\n";
    MonthlyPass noPass;
    int64_t airBase = Day4FareEngine::calculateFare(Zone::Airport, 12, false, noPass, ConcessionType::None);
    int64_t airPeak = Day4FareEngine::calculateFare(Zone::Airport, 8, false, noPass, ConcessionType::None);
    int64_t airWeekend = Day4FareEngine::calculateFare(Zone::Airport, 12, true, noPass, ConcessionType::None);

    std::cout << "  - Base Fare (Off-Peak)    : Rs " << (airBase / 100.0)    << " (Expected Rs 60.00) -> " 
              << (airBase == 6000 ? "PASS" : "FAIL") << "\n";
    std::cout << "  - Peak Fare (+20%)        : Rs " << (airPeak / 100.0)    << " (Expected Rs 72.00) -> " 
              << (airPeak == 7200 ? "PASS" : "FAIL") << "\n";
    std::cout << "  - Weekend Fare (-10%)     : Rs " << (airWeekend / 100.0) << " (Expected Rs 54.00) -> " 
              << (airWeekend == 5400 ? "PASS" : "FAIL") << "\n\n";

    // ------------------------------------------------------------------------
    // Part D Test 2: Surcharge & Weekend Discount Mutual Exclusivity
    // ------------------------------------------------------------------------
    std::cout << "[Test 2] Surcharge & Weekend Discount Exclusivity:\n";
    // Travelling on Weekend during morning peak hour (08:00):
    // Should NOT apply peak surcharge (+20%). Must ONLY apply 10% weekend discount!
    int64_t airWeekendPeak = Day4FareEngine::calculateFare(Zone::Airport, 8, true, noPass, ConcessionType::None);
    std::cout << "  - Weekend @ 08:00 (Peak window) Fare: Rs " << (airWeekendPeak / 100.0) 
              << " (Expected Rs 54.00, NOT Rs 72 or Rs 64.80) -> " 
              << (airWeekendPeak == 5400 ? "PASS (EXCLUSIVITY VERIFIED)" : "FAIL") << "\n\n";

    // ------------------------------------------------------------------------
    // Part D Test 3: Monthly Pass Unlimited Travel in 2 Selected Zones
    // ------------------------------------------------------------------------
    std::cout << "[Test 3] Monthly Pass for [Inner, Airport]:\n";
    MonthlyPass passInnerAirport(Zone::Inner, Zone::Airport);
    Day4SmartCard passCard(777, 10000); // Rs 100.00 balance
    passCard.attachMonthlyPass(passInnerAirport);

    // Trip A: Inner zone (Covered -> Fare Rs 0)
    std::cout << "Trip 1 (Inner, covered by pass):\n";
    passCard.tapAndGo(Zone::Inner, 9, false);

    // Trip B: Airport zone (Covered -> Fare Rs 0)
    std::cout << "Trip 2 (Airport, covered by pass):\n";
    passCard.tapAndGo(Zone::Airport, 18, false);

    // Trip C: Middle zone (NOT covered by pass -> Standard fare deducted)
    std::cout << "Trip 3 (Middle, NOT covered by pass):\n";
    passCard.tapAndGo(Zone::Middle, 14, false);

    // Trip D: Outer zone on Weekend (NOT covered -> Weekend discounted fare deducted)
    std::cout << "Trip 4 (Outer, Weekend, NOT covered):\n";
    passCard.tapAndGo(Zone::Outer, 11, true);

    // ------------------------------------------------------------------------
    // Part D Test 4: Solvency Invariant Check on Monthly Pass Trips
    // ------------------------------------------------------------------------
    std::cout << "\n[Test 4] Low Balance Invariant on Monthly Pass Journey:\n";
    // Card with only Rs 30.00 (3000 paise). Even though Airport trip fare is 0 with pass,
    // card fails turnstile entry deposit check (must hold >= Rs 50.00).
    Day4SmartCard depletedCard(888, 3000);
    depletedCard.attachMonthlyPass(passInnerAirport);
    std::cout << "Attempting travel on depleted card (Rs 30.00) with pass for Airport:\n";
    bool passDepletedAllowed = depletedCard.tapAndGo(Zone::Airport, 10, false);
    std::cout << "  Result: " << (!passDepletedAllowed ? "PASS (Correctly Denied by Security Invariant)" : "FAIL") << "\n";

    std::cout << "\nTask 12 Day-4 Design Challenge validation completed successfully.\n";
    return 0;
}
