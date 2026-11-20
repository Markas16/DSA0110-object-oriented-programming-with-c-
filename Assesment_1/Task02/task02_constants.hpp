#ifndef FARE_CONSTANTS_HPP
#define FARE_CONSTANTS_HPP

#include <cstdint>
#include <string_view>

namespace MetroFare {

// Zone enumeration: scoped enum class provides strict type safety
enum class Zone : uint8_t {
    Inner = 0,
    Middle = 1,
    Outer = 2,
    Airport = 3 // Introduced in Task 12
};

// Centralized, type-safe, compile-time constants (constexpr)
// Stored in integer paise (1 Rupee = 100 Paise)
inline constexpr int64_t BASE_FARE_INNER_PAISE   = 1000LL; // Rs 10.00
inline constexpr int64_t BASE_FARE_MIDDLE_PAISE  = 2000LL; // Rs 20.00
inline constexpr int64_t BASE_FARE_OUTER_PAISE   = 3000LL; // Rs 30.00
inline constexpr int64_t BASE_FARE_AIRPORT_PAISE = 6000LL; // Rs 60.00

inline constexpr int64_t MIN_BALANCE_PAISE       = 5000LL; // Rs 50.00 (Minimum required balance)
inline constexpr int64_t INITIAL_BALANCE_PAISE   = 10000LL; // Rs 100.00 (Standard starting balance)

// Peak hour surcharge percentage
inline constexpr int PEAK_SURCHARGE_PERCENT = 20; // 20%
inline constexpr int WEEKEND_DISCOUNT_PERCENT = 10; // 10% (Task 12)

// Peak hour interval definitions (24-hour clock)
// Morning: [08:00, 10:00)
inline constexpr int MORNING_PEAK_START = 8;
inline constexpr int MORNING_PEAK_END   = 10;

// Evening: [17:00, 20:00)
inline constexpr int EVENING_PEAK_START = 17;
inline constexpr int EVENING_PEAK_END   = 20;

// Helper function to convert Zone to human-readable string_view
constexpr std::string_view getZoneName(Zone z) noexcept {
    switch (z) {
        case Zone::Inner:   return "Inner";
        case Zone::Middle:  return "Middle";
        case Zone::Outer:   return "Outer";
        case Zone::Airport: return "Airport";
        default:            return "Unknown";
    }
}

} // namespace MetroFare

#endif // FARE_CONSTANTS_HPP
