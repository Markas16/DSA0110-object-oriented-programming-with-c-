#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "../src/fare_system.hpp"

using namespace MetroSystem;

// Lightweight Test Framework
static int gPassedTests = 0;
static int gFailedTests = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { \
            std::cout << "  [PASS] " << msg << "\n"; \
            gPassedTests++; \
        } else { \
            std::cerr << "  [FAIL] " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            gFailedTests++; \
        } \
    } while(0)

int main() {
    std::cout << "========================================================\n";
    std::cout << "  TASK 9: AUTOMATED UNIT & INTEGRATION TEST HARNESS\n";
    std::cout << "========================================================\n\n";

    // ------------------------------------------------------------------------
    // Group 1: Every Original Zone Peak & Off-Peak Rates
    // ------------------------------------------------------------------------
    std::cout << "[Group 1] Original Zone Base & Peak Rates:\n";
    // Inner: Base 1000, Peak 1200
    TEST_ASSERT(FareEngine::calculateFare(Zone::Inner, 12) == 1000, "Inner Zone Off-Peak = 1000 paise (Rs 10.00)");
    TEST_ASSERT(FareEngine::calculateFare(Zone::Inner, 9)  == 1200, "Inner Zone Peak (09:00) = 1200 paise (Rs 12.00)");

    // Middle: Base 2000, Peak 2400
    TEST_ASSERT(FareEngine::calculateFare(Zone::Middle, 14) == 2000, "Middle Zone Off-Peak = 2000 paise (Rs 20.00)");
    TEST_ASSERT(FareEngine::calculateFare(Zone::Middle, 18) == 2400, "Middle Zone Peak (18:00) = 2400 paise (Rs 24.00)");

    // Outer: Base 3000, Peak 3600
    TEST_ASSERT(FareEngine::calculateFare(Zone::Outer, 22) == 3000, "Outer Zone Off-Peak = 3000 paise (Rs 30.00)");
    TEST_ASSERT(FareEngine::calculateFare(Zone::Outer, 8)  == 3600, "Outer Zone Peak (08:00) = 3600 paise (Rs 36.00)");

    // ------------------------------------------------------------------------
    // Group 2: Boundary Hours (08:00, 10:00, 17:00, 20:00)
    // ------------------------------------------------------------------------
    std::cout << "\n[Group 2] Temporal Boundary Tests:\n";
    // 08:00 is Morning Peak Start [08:00 inclusive)
    TEST_ASSERT(FareEngine::isPeakHour(8) == true, "08:00 is PEAK (Morning start boundary)");
    TEST_ASSERT(FareEngine::calculateFare(Zone::Outer, 8) == 3600, "08:00 Outer fare applies +20% surcharge");

    // 10:00 is Morning Peak End [10:00 exclusive -> Off-Peak)
    TEST_ASSERT(FareEngine::isPeakHour(10) == false, "10:00 is OFF-PEAK (Morning end boundary)");
    TEST_ASSERT(FareEngine::calculateFare(Zone::Outer, 10) == 3000, "10:00 Outer fare is standard base fare");

    // 17:00 is Evening Peak Start [17:00 inclusive)
    TEST_ASSERT(FareEngine::isPeakHour(17) == true, "17:00 is PEAK (Evening start boundary)");
    TEST_ASSERT(FareEngine::calculateFare(Zone::Outer, 17) == 3600, "17:00 Outer fare applies +20% surcharge");

    // 20:00 is Evening Peak End [20:00 exclusive -> Off-Peak)
    TEST_ASSERT(FareEngine::isPeakHour(20) == false, "20:00 is OFF-PEAK (Evening end boundary)");
    TEST_ASSERT(FareEngine::calculateFare(Zone::Outer, 20) == 3000, "20:00 Outer fare is standard base fare");

    // ------------------------------------------------------------------------
    // Group 3: Solvency Boundary Tests (Leaving Exactly Rs 50 vs Rs 49.99)
    // ------------------------------------------------------------------------
    std::cout << "\n[Group 3] Minimum Solvency Balance (Rs 50.00 = 5000 paise):\n";
    // Test Card starting with 6000 paise. Inner off-peak fare = 1000 paise.
    // Remaining balance after trip = 6000 - 1000 = 5000 paise (EXACTLY Rs 50.00). Must be permitted!
    SmartCard boundaryCard(201, 6000);
    bool boundaryTripAllowed = boundaryCard.processTrip(Zone::Inner, 12);
    TEST_ASSERT(boundaryTripAllowed == true, "Trip leaving EXACTLY 5000 paise (Rs 50.00) is PERMITTED");
    TEST_ASSERT(boundaryCard.getBalancePaise() == 5000, "Remaining balance is exactly 5000 paise");

    // Next trip from same card: fare = 1000 paise.
    // If deducted, balance would become 4000 < 5000. Must be DENIED!
    bool secondTripAllowed = boundaryCard.processTrip(Zone::Inner, 12);
    TEST_ASSERT(secondTripAllowed == false, "Trip leaving < 5000 paise is STRICTLY DENIED");
    TEST_ASSERT(boundaryCard.getBalancePaise() == 5000, "Card balance remained 5000 paise (undeducted)");

    // Test Card starting with 5999 paise. Inner fare 1000 -> remaining 4999 paise (Rs 49.99). Must be DENIED!
    SmartCard edgeCard(202, 5999);
    bool edgeTripAllowed = edgeCard.processTrip(Zone::Inner, 12);
    TEST_ASSERT(edgeTripAllowed == false, "Trip leaving 4999 paise (Rs 49.99) is DENIED");
    TEST_ASSERT(edgeCard.getBalancePaise() == 5999, "Balance remains 5999 paise");

    // ------------------------------------------------------------------------
    // Group 4: Input Validation, Unknown Zones & Negative Recharge
    // ------------------------------------------------------------------------
    std::cout << "\n[Group 4] Input Validation & Edge Conditions:\n";
    TEST_ASSERT(FareEngine::calculateFare(Zone::Unknown, 12) == -1, "Unknown Zone returns error code -1");
    TEST_ASSERT(FareEngine::calculateFare(Zone::Inner, 24) == -1, "Hour 24 returns error code -1");
    TEST_ASSERT(FareEngine::calculateFare(Zone::Inner, -1) == -1, "Negative hour returns error code -1");

    SmartCard rechargeCard(203, 10000);
    TEST_ASSERT(rechargeCard.recharge(-500) == false, "Negative recharge (-500 paise) is REJECTED");
    TEST_ASSERT(rechargeCard.recharge(0) == false, "Zero recharge is REJECTED");
    TEST_ASSERT(rechargeCard.recharge(2500) == true, "Positive recharge (+2500 paise) is ACCEPTED");
    TEST_ASSERT(rechargeCard.getBalancePaise() == 12500, "Balance correctly increased to 12500 paise");

    // ------------------------------------------------------------------------
    // Group 5: Five Sequential Journeys with Manual Verification
    // ------------------------------------------------------------------------
    std::cout << "\n[Group 5] 5-Journey Sequential Audit Verification:\n";
    // Card initialized with Rs 150.00 (15000 paise)
    // Journey 1: Middle Off-Peak (14:00) -> Fare: 2000 paise. Balance: 15000 - 2000 = 13000 paise.
    // Journey 2: Outer Peak (08:30)      -> Fare: 3600 paise. Balance: 13000 - 3600 = 9400 paise.
    // Journey 3: Inner Peak (18:00)      -> Fare: 1200 paise. Balance: 9400 - 1200 = 8200 paise.
    // Journey 4: Outer Off-Peak (21:00)  -> Fare: 3000 paise. Balance: 8200 - 3000 = 5200 paise.
    // Journey 5: Middle Off-Peak (11:00) -> Fare: 2000 paise. Balance: 5200 - 2000 = 3200 < 5000 -> DENIED!
    // Expected Sum of Permitted Fares: 2000 + 3600 + 1200 + 3000 = 9800 paise (Rs 98.00).
    // Final Expected Balance: 5200 paise (Rs 52.00).
    SmartCard auditCard(501, 15000);

    bool j1 = auditCard.processTrip(Zone::Middle, 14); // 2000
    bool j2 = auditCard.processTrip(Zone::Outer, 8);   // 3600
    bool j3 = auditCard.processTrip(Zone::Inner, 18);  // 1200
    bool j4 = auditCard.processTrip(Zone::Outer, 21);  // 3000
    bool j5 = auditCard.processTrip(Zone::Middle, 11); // 2000 -> denied!

    TEST_ASSERT(j1 == true, "Journey 1 (Middle Off-Peak) Approved");
    TEST_ASSERT(j2 == true, "Journey 2 (Outer Peak) Approved");
    TEST_ASSERT(j3 == true, "Journey 3 (Inner Peak) Approved");
    TEST_ASSERT(j4 == true, "Journey 4 (Outer Off-Peak) Approved");
    TEST_ASSERT(j5 == false, "Journey 5 (Middle Off-Peak) Denied (would breach Rs 50 min)");

    int64_t manualTotalDeducted = 2000 + 3600 + 1200 + 3000; // 9800 paise
    int64_t manualFinalBalance  = 15000 - manualTotalDeducted; // 5200 paise

    int64_t actualTotalDeducted = 0;
    for (const auto& r : auditCard.getHistory()) {
        if (r.authorized) actualTotalDeducted += r.farePaise;
    }

    TEST_ASSERT(actualTotalDeducted == manualTotalDeducted, "Total Fare Deducted Matches Manual Calculation (9800 paise = Rs 98.00)");
    TEST_ASSERT(auditCard.getBalancePaise() == manualFinalBalance, "Final Card Balance Matches Manual Calculation (5200 paise = Rs 52.00)");

    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "TEST HARNESS SUMMARY: " << gPassedTests << " Passed, " << gFailedTests << " Failed\n";
    std::cout << "========================================================\n";

    return (gFailedTests == 0) ? 0 : 1;
}
