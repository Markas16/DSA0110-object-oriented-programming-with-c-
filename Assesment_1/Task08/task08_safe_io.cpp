#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>
#include <vector>
#include <cstdint>

// ============================================================================
// TASK 8: Robust Input / Output Handling and Stream Recovery
// ============================================================================

enum class Zone { Inner, Middle, Outer, Invalid };

struct JourneyRecord {
    std::string zoneStr;
    int hour;
    int64_t farePaise;
    bool approved;
    int64_t remainingBalancePaise;
};

// Safe integer reader using cin extraction, fail detection, and clear/ignore recovery
int readValidInteger(const std::string& prompt, int minVal, int maxVal) {
    int val = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            if (val >= minVal && val <= maxVal) {
                // Clear any leftover characters on line
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return val;
            } else {
                std::cout << "  [Error] Value must be between " << minVal << " and " << maxVal << ".\n";
            }
        } else {
            if (std::cin.eof()) {
                return minVal; // Graceful EOF fallback
            }
            std::cout << "  [Error] Invalid numeric input! Clearing stream...\n";
            std::cin.clear(); // Clear error state flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad tokens
        }
    }
}

// Alternative input method using getline() + stoi() + try-catch
bool parseIntegerWithTryCatch(const std::string& input, int& result, int minVal, int maxVal) {
    try {
        size_t idx = 0;
        int parsed = std::stoi(input, &idx);
        // Ensure entire string was consumed
        if (idx == input.length() && parsed >= minVal && parsed <= maxVal) {
            result = parsed;
            return true;
        }
    } catch (const std::invalid_argument&) {
        // Non-numeric
    } catch (const std::out_of_range&) {
        // Overflow
    }
    return false;
}

Zone parseZoneInput(const std::string& z) {
    if (z == "Inner" || z == "inner" || z == "1") return Zone::Inner;
    if (z == "Middle" || z == "middle" || z == "2") return Zone::Middle;
    if (z == "Outer" || z == "outer" || z == "3") return Zone::Outer;
    return Zone::Invalid;
}

std::string formatMoney(int64_t paise) {
    std::ostringstream oss;
    oss << "Rs " << std::fixed << std::setprecision(2) << (paise / 100.0);
    return oss.str();
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "  TASK 8: SAFE INPUT / OUTPUT VALIDATION SYSTEM\n";
    std::cout << "========================================================\n\n";

    // Demonstration of both methods
    std::cout << "[1] Testing Alternative: getline() + stoi() + try-catch Validation:\n";
    std::vector<std::string> testInputs = {"25", "-5", "abc", "18", "99999999999999999"};
    for (const auto& str : testInputs) {
        int parsed = 0;
        bool ok = parseIntegerWithTryCatch(str, parsed, 0, 23);
        std::cout << "  Input: \"" << std::setw(20) << std::left << str << "\" -> "
                  << (ok ? "VALID (" + std::to_string(parsed) + ")" : "REJECTED (Safe error handling)") 
                  << "\n";
    }

    std::cout << "\n[2] Interactive Robust Input Simulator:\n";
    // Reading travel hour with strict range [0, 23]
    int hour = readValidInteger("Enter journey hour (0 - 23): ", 0, 23);
    std::cout << "Accepted Hour: " << hour << "\n";

    // Reading Zone
    Zone selectedZone = Zone::Invalid;
    std::string zoneInput;
    while (selectedZone == Zone::Invalid) {
        std::cout << "Enter Zone (Inner, Middle, Outer): ";
        if (std::cin >> zoneInput) {
            selectedZone = parseZoneInput(zoneInput);
            if (selectedZone == Zone::Invalid) {
                std::cout << "  [Error] Unrecognized zone! Please enter Inner, Middle, or Outer.\n";
            }
        } else {
            if (std::cin.eof()) {
                selectedZone = Zone::Outer; // Default on EOF
                break;
            }
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Reading Recharge Amount
    int rechargePaise = readValidInteger("Enter recharge amount in paise (positive integer): ", 1, 1000000);
    std::cout << "Accepted Recharge: " << formatMoney(rechargePaise) << "\n\n";

    // Pretty-Printed Journey Report Table
    std::cout << "[3] Formatted Journey Report Table with <iomanip>:\n";
    std::cout << "+" << std::string(10, '-') << "+" << std::string(12, '-') 
              << "+" << std::string(8, '-') << "+" << std::string(14, '-') 
              << "+" << std::string(12, '-') << "+" << std::string(16, '-') << "+\n";
    std::cout << "| " << std::left << std::setw(8)  << "Trip ID"
              << "| " << std::left << std::setw(10) << "Zone"
              << "| " << std::left << std::setw(6)  << "Hour"
              << "| " << std::left << std::setw(12) << "Fare"
              << "| " << std::left << std::setw(10) << "Status"
              << "| " << std::left << std::setw(14) << "Balance"
              << "|\n";
    std::cout << "+" << std::string(10, '-') << "+" << std::string(12, '-') 
              << "+" << std::string(8, '-') << "+" << std::string(14, '-') 
              << "+" << std::string(12, '-') << "+" << std::string(16, '-') << "+\n";

    std::vector<JourneyRecord> sampleRecords = {
        {"Inner", 8, 1200, true, 8800},
        {"Middle", 14, 2000, true, 6800},
        {"Outer", 18, 3600, false, 6800},
        {"Outer", 22, 3000, true, 3800}
    };

    int id = 1;
    for (const auto& rec : sampleRecords) {
        std::cout << "| " << std::left << std::setw(8) << ("#" + std::to_string(id++))
                  << "| " << std::left << std::setw(10) << rec.zoneStr
                  << "| " << std::left << std::setw(6) << rec.hour
                  << "| " << std::left << std::setw(12) << formatMoney(rec.farePaise)
                  << "| " << std::left << std::setw(10) << (rec.approved ? "APPROVED" : "DENIED")
                  << "| " << std::left << std::setw(14) << formatMoney(rec.remainingBalancePaise)
                  << "|\n";
    }
    std::cout << "+" << std::string(10, '-') << "+" << std::string(12, '-') 
              << "+" << std::string(8, '-') << "+" << std::string(14, '-') 
              << "+" << std::string(12, '-') << "+" << std::string(16, '-') << "+\n\n";

    // Comparison of Approaches
    std::cout << "[4] Comparison of Input Methods:\n";
    std::cout << "  - cin >> val : Direct stream extraction, fast O(1), minimal buffer, requires cin.clear() recovery.\n";
    std::cout << "  - getline/stoi: Line-oriented, robust against stray characters, allocates std::string buffer O(N).\n";

    return 0;
}
