#ifndef FARE_SYSTEM_HPP
#define FARE_SYSTEM_HPP

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <optional>

namespace MetroSystem {

enum class Zone : uint8_t {
    Inner = 0,
    Middle = 1,
    Outer = 2,
    Airport = 3,
    Unknown = 255
};

std::string_view zoneToString(Zone z) noexcept;
Zone stringToZone(std::string_view sv) noexcept;

struct JourneyRecord {
    uint64_t journeyId;
    Zone destinationZone;
    int travelHour;
    bool isWeekend;
    int64_t farePaise;
    bool authorized;
    int64_t balanceAfterPaise;
    std::string notes;
};

class FareEngine {
public:
    static constexpr int64_t DEFAULT_BASE_INNER   = 1000LL; // Rs 10.00
    static constexpr int64_t DEFAULT_BASE_MIDDLE  = 2000LL; // Rs 20.00
    static constexpr int64_t DEFAULT_BASE_OUTER   = 3000LL; // Rs 30.00
    static constexpr int64_t DEFAULT_BASE_AIRPORT = 6000LL; // Rs 60.00
    static constexpr int64_t MIN_BALANCE_PAISE    = 5000LL; // Rs 50.00
    static constexpr int DEFAULT_PEAK_SURCHARGE   = 20;     // 20%
    static constexpr int DEFAULT_WEEKEND_DISCOUNT = 10;     // 10%

    // Interval definitions:
    // Morning peak: [08:00, 10:00)
    // Evening peak: [17:00, 20:00)
    static bool isPeakHour(int hour) noexcept;
    
    // Calculates fare in paise. Returns -1 if zone or hour is invalid.
    // In Day-4 / Task 12: Peak surcharge and weekend discount do NOT combine (mutually exclusive).
    static int64_t calculateFare(Zone zone, int hour, bool isWeekend = false) noexcept;

    static std::string formatRupees(int64_t paise);
};

class SmartCard {
private:
    uint64_t cardId_;
    int64_t balancePaise_;
    bool active_;
    std::vector<JourneyRecord> journeyHistory_;
    uint64_t nextJourneyId_{1};

public:
    explicit SmartCard(uint64_t cardId, int64_t initialBalancePaise = 10000LL);

    uint64_t getCardId() const noexcept { return cardId_; }
    int64_t getBalancePaise() const noexcept { return balancePaise_; }
    double getBalanceRupees() const noexcept { return balancePaise_ / 100.0; }
    bool isActive() const noexcept { return active_; }
    const std::vector<JourneyRecord>& getHistory() const noexcept { return journeyHistory_; }

    // Core Invariant Check: (balance - fare >= 5000)
    bool canTravel(int64_t farePaise) const noexcept;

    // Deduct fare and log journey record. Returns true if permitted, false if denied.
    bool processTrip(Zone zone, int hour, bool isWeekend = false);

    // Recharge funds
    bool recharge(int64_t amountPaise);

    // Print journey report table to stdout
    void printReport() const;
};

} // namespace MetroSystem

#endif // FARE_SYSTEM_HPP
