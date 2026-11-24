#include <iostream>
#include <vector>
#include <memory>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 4: Repaired Account Hierarchy
 * Repairs:
 *  1. virtual double rate() with dynamic dispatch and override keyword
 *  2. virtual ~Account() to guarantee complete destructor chain execution
 *  3. std::unique_ptr<Account> in std::vector to prevent object slicing and leaks
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

class Account {
public:
    // REPAIR 1: Declared virtual for dynamic polymorphism
    virtual double rate() const { return 3.5; }

    // REPAIR 2: Virtual destructor guarantees derived destructor is invoked
    virtual ~Account() {
        std::cout << "[Account Destructor] Base account safely destroyed\n";
    }
};

class SeniorAccount : public Account {
    double* history = nullptr;

public:
    SeniorAccount() : history(new double[100]) {
        for (int i = 0; i < 100; ++i) history[i] = 0.0;
    }

    // REPAIR 1: Explicit override keyword
    double rate() const override { return 4.0; }

    ~SeniorAccount() override {
        std::cout << "[SeniorAccount Destructor] Safely deallocating history array (800 bytes)\n";
        delete[] history;
    }

    // Disable copy to prevent slicing accidental bugs (Rule of 5)
    SeniorAccount(const SeniorAccount&) = delete;
    SeniorAccount& operator=(const SeniorAccount&) = delete;

    // Enable move semantics
    SeniorAccount(SeniorAccount&& other) noexcept : history(other.history) {
        other.history = nullptr;
    }
    SeniorAccount& operator=(SeniorAccount&& other) noexcept {
        if (this != &other) {
            delete[] history;
            history = other.history;
            other.history = nullptr;
        }
        return *this;
    }
};

int main()
{
    std::cout << "=======================================================\n";
    std::cout << " REPAIRED BANK ACCOUNT SYSTEM - RUNTIME POLYMORPHISM   \n";
    std::cout << "=======================================================\n";

    // REPAIR 3: Storing std::unique_ptr<Account> in std::vector prevents slicing
    std::vector<std::unique_ptr<Account>> accounts;
    accounts.push_back(std::make_unique<Account>());
    accounts.push_back(std::make_unique<SeniorAccount>());

    std::cout << "\n--- Iterating through accounts (Dynamic Dispatch) ---\n";
    for (const auto& a : accounts) {
        std::cout << "Account Interest Rate: " << a->rate() << "%\n";
    }

    std::cout << "\n--- Automatic destruction via RAII (Virtual Destructors) ---\n";
    accounts.clear(); // Safely triggers full destructor chain

    std::cout << "All accounts destroyed safely without any memory leak!\n";
    return 0;
}
