#include "fare_system.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace MetroSystem {

std::string_view zoneToString(Zone z) noexcept {
    switch (z) {
        case Zone::Inner:   return "Inner";
        case Zone::Middle:  return "Middle";
        case Zone::Outer:   return "Outer";
        case Zone::Airport: return "Airport";
        default:            return "Unknown";
    }
}

Zone stringToZone(std::string_view sv) noexcept {
    if (sv == "Inner" || sv == "inner" || sv == "1")     return Zone::Inner;
    if (sv == "Middle" || sv == "middle" || sv == "2")   return Zone::Middle;
    if (sv == "Outer" || sv == "outer" || sv == "3")     return Zone::Outer;
    if (sv == "Airport" || sv == "airport" || sv == "4") return Zone::Airport;
    return Zone::Unknown;
}

bool FareEngine::isPeakHour(int hour) noexcept {
    if (hour < 0 || hour > 23) return false;
    // Morning: 08:00 inclusive to 10:00 exclusive
    // Evening: 17:00 inclusive to 20:00 exclusive
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

int64_t FareEngine::calculateFare(Zone zone, int hour, bool isWeekend) noexcept {
    if (hour < 0 || hour > 23) return -1;

    int64_t baseFare = 0;
    switch (zone) {
        case Zone::Inner:   baseFare = DEFAULT_BASE_INNER; break;
        case Zone::Middle:  baseFare = DEFAULT_BASE_MIDDLE; break;
        case Zone::Outer:   baseFare = DEFAULT_BASE_OUTER; break;
        case Zone::Airport: baseFare = DEFAULT_BASE_AIRPORT; break;
        default:            return -1; // Unknown zone
    }

    // Business Precedence Rule:
    // Weekend discount (10%) must NOT combine with peak surcharge (20%).
    // On weekends, peak surcharges do not apply, and eligible trips receive a 10% discount.
    if (isWeekend) {
        // Apply 10% weekend discount using integer arithmetic:
        // fare = baseFare - (baseFare * 10) / 100
        return baseFare - (baseFare * DEFAULT_WEEKEND_DISCOUNT) / 100;
    }

    // On weekdays: apply 20% peak surcharge if within peak hours
    if (isPeakHour(hour)) {
        return baseFare + (baseFare * DEFAULT_PEAK_SURCHARGE) / 100;
    }

    return baseFare;
}

std::string FareEngine::formatRupees(int64_t paise) {
    std::ostringstream oss;
    oss << "Rs " << std::fixed << std::setprecision(2) << (paise / 100.0);
    return oss.str();
}

SmartCard::SmartCard(uint64_t cardId, int64_t initialBalancePaise)
    : cardId_(cardId), balancePaise_(initialBalancePaise), active_(true) {}

bool SmartCard::canTravel(int64_t farePaise) const noexcept {
    if (!active_ || farePaise < 0) return false;
    // Rule: balance - fare must be at least 5000 paise (Rs 50.00)
    return (balancePaise_ - farePaise) >= FareEngine::MIN_BALANCE_PAISE;
}

bool SmartCard::processTrip(Zone zone, int hour, bool isWeekend) {
    int64_t fare = FareEngine::calculateFare(zone, hour, isWeekend);
    JourneyRecord rec{};
    rec.journeyId = nextJourneyId_++;
    rec.destinationZone = zone;
    rec.travelHour = hour;
    rec.isWeekend = isWeekend;
    rec.farePaise = fare;

    if (fare < 0) {
        rec.authorized = false;
        rec.balanceAfterPaise = balancePaise_;
        rec.notes = "Denied: Invalid Zone or Hour";
        journeyHistory_.push_back(rec);
        return false;
    }

    if (canTravel(fare)) {
        balancePaise_ -= fare;
        rec.authorized = true;
        rec.balanceAfterPaise = balancePaise_;
        rec.notes = "Approved";
        journeyHistory_.push_back(rec);
        return true;
    } else {
        rec.authorized = false;
        rec.balanceAfterPaise = balancePaise_;
        rec.notes = "Denied: Low Balance (violates Rs 50 minimum rule)";
        journeyHistory_.push_back(rec);
        return false;
    }
}

bool SmartCard::recharge(int64_t amountPaise) {
    if (amountPaise <= 0) return false; // Reject zero or negative recharge
    balancePaise_ += amountPaise;
    return true;
}

void SmartCard::printReport() const {
    std::cout << "\n========================================================================================\n";
    std::cout << "  SMART-CARD JOURNEY AUDIT REPORT | Card ID: #" << cardId_ << "\n";
    std::cout << "  Current Balance: " << FareEngine::formatRupees(balancePaise_) 
              << " (" << balancePaise_ << " paise)\n";
    std::cout << "========================================================================================\n";
    std::cout << "| " << std::left << std::setw(6)  << "Trip"
              << "| " << std::left << std::setw(10) << "Zone"
              << "| " << std::left << std::setw(6)  << "Hour"
              << "| " << std::left << std::setw(9)  << "Day"
              << "| " << std::left << std::setw(12) << "Fare"
              << "| " << std::left << std::setw(11) << "Status"
              << "| " << std::left << std::setw(14) << "Balance"
              << "| " << std::left << std::setw(18) << "Audit Notes"
              << "|\n";
    std::cout << "|-------+-----------+-------+----------+-------------+------------+---------------+--------------------|\n";

    for (const auto& r : journeyHistory_) {
        std::cout << "| " << std::left << std::setw(6)  << ("#" + std::to_string(r.journeyId))
                  << "| " << std::left << std::setw(10) << zoneToString(r.destinationZone)
                  << "| " << std::left << std::setw(6)  << (std::to_string(r.travelHour) + ":00")
                  << "| " << std::left << std::setw(9)  << (r.isWeekend ? "Weekend" : "Weekday")
                  << "| " << std::left << std::setw(12) << (r.farePaise >= 0 ? FareEngine::formatRupees(r.farePaise) : "N/A")
                  << "| " << std::left << std::setw(11) << (r.authorized ? "APPROVED" : "DENIED")
                  << "| " << std::left << std::setw(14) << FareEngine::formatRupees(r.balanceAfterPaise)
                  << "| " << std::left << std::setw(18) << r.notes
                  << "|\n";
    }
    std::cout << "========================================================================================\n\n";
}

} // namespace MetroSystem
