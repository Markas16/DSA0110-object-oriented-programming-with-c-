#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>

// ============================================================================
// TASK 7 (Part 2): Object-Oriented SmartCard Implementation
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

class SmartCard {
private:
    uint64_t cardId_;
    int64_t balancePaise_; // Strictly private: guarded against illegal direct manipulation
    static constexpr int64_t MIN_BALANCE_PAISE = 5000; // Rs 50.00 invariant

public:
    SmartCard(uint64_t id, int64_t initialBalancePaise)
        : cardId_(id), balancePaise_(initialBalancePaise) {
        if (balancePaise_ < MIN_BALANCE_PAISE) {
            std::cout << "[Warning] Card " << cardId_ 
                      << " initialized below minimum operating balance!\n";
        }
    }

    uint64_t getCardId() const noexcept { return cardId_; }
    int64_t getBalancePaise() const noexcept { return balancePaise_; }
    double getBalanceRupees() const noexcept { return balancePaise_ / 100.0; }

    // Core validation: Card can only travel if post-deduction balance >= 5000 paise
    bool canTravel(int64_t farePaise) const noexcept {
        if (farePaise < 0) return false;
        return (balancePaise_ - farePaise) >= MIN_BALANCE_PAISE;
    }

    // Atomic deduction: Guaranteed to never violate minimum balance
    bool deduct(int64_t farePaise) {
        if (!canTravel(farePaise)) {
            return false; // Invariant enforced: transaction rejected
        }
        balancePaise_ -= farePaise;
        return true;
    }

    // Safe recharge: rejects negative or zero amounts
    bool recharge(int64_t amountPaise) {
        if (amountPaise <= 0) {
            std::cout << "[OOP] Recharge rejected: amount must be strictly positive.\n";
            return false;
        }
        balancePaise_ += amountPaise;
        return true;
    }
};

class FareEngine {
public:
    static bool isPeakHour(int hour) noexcept {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    static int64_t calculateFare(Zone zone, int hour) noexcept {
        int64_t base = 0;
        switch (zone) {
            case Zone::Inner:  base = 1000; break; // Rs 10.00
            case Zone::Middle: base = 2000; break; // Rs 20.00
            case Zone::Outer:  base = 3000; break; // Rs 30.00
        }
        if (isPeakHour(hour)) {
            return (base * 120) / 100;
        }
        return base;
    }
};

int main() {
    std::cout << "========================================================\n";
    std::cout << "  TASK 7: OBJECT-ORIENTED SMARTCARD DEMONSTRATION\n";
    std::cout << "========================================================\n\n";

    SmartCard card(101, 10000); // Card #101 starting with Rs 100.00

    std::cout << "Created Card ID: " << card.getCardId() 
              << " | Initial Balance: Rs " << std::fixed << std::setprecision(2) 
              << card.getBalanceRupees() << "\n\n";

    // Scenario 1: Inner off-peak (1000 paise = Rs 10.00)
    int64_t fare1 = FareEngine::calculateFare(Zone::Inner, 12);
    if (card.deduct(fare1)) {
        std::cout << "[OOP] Trip 1 Success: Inner Zone, Fare Rs " << (fare1 / 100.0) 
                  << " | Remaining: Rs " << card.getBalanceRupees() << "\n";
    }

    // Scenario 2: Outer peak (3600 paise = Rs 36.00)
    int64_t fare2 = FareEngine::calculateFare(Zone::Outer, 18);
    if (card.deduct(fare2)) {
        std::cout << "[OOP] Trip 2 Success: Outer Zone (Peak), Fare Rs " << (fare2 / 100.0) 
                  << " | Remaining: Rs " << card.getBalanceRupees() << "\n";
    }

    // Scenario 3: Outer peak (3600 paise = Rs 36.00) -> Must be Denied!
    int64_t fare3 = FareEngine::calculateFare(Zone::Outer, 18);
    if (!card.deduct(fare3)) {
        std::cout << "[OOP] Trip 3 DENIED! Balance Rs " << card.getBalanceRupees() 
                  << " insufficient to maintain Rs 50.00 minimum after fare Rs " 
                  << (fare3 / 100.0) << "\n";
    }

    // Recharging card
    std::cout << "\nRecharging Card with Rs 50.00 (5000 paise)...\n";
    card.recharge(5000);
    std::cout << "New Balance: Rs " << card.getBalanceRupees() << "\n";

    // Retrying Trip 3 after recharge
    if (card.deduct(fare3)) {
        std::cout << "[OOP] Trip 3 Success after recharge: Fare Rs " << (fare3 / 100.0) 
                  << " | Remaining: Rs " << card.getBalanceRupees() << "\n";
    }

    // Discussion on Public vs Private Balance
    std::cout << "\n[Encapsulation & Security Analysis]:\n";
    std::cout << "1. Private member 'balancePaise_' guarantees that invariant (>= Rs 50)\n"
              << "   CANNOT be bypassed by external components.\n";
    std::cout << "2. If 'balancePaise_' were public, any component could do 'card.balancePaise_ = 0;'\n"
              << "   or forget to call 'canTravel()', creating financial deficits.\n";
    std::cout << "3. Method 'deduct()' couples validation and state transition atomically.\n";

    return 0;
}
