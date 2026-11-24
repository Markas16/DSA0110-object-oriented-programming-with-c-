#include <iostream>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 4: Forensics Demonstration of Defects
 * Defects:
 *  1. Non-virtual rate() causes static binding (returns 3.5 instead of 4.0)
 *  2. Non-virtual destructor causes memory leak (SeniorAccount::~SeniorAccount never runs)
 *  3. Value copy causes Object Slicing
 * Compile with: g++ -std=c++17 -Wall -Wextra -fsanitize=address -g account_buggy.cpp
 * ============================================================================
 */

class Account {
public:
    double rate() const { return 3.5; }
    ~Account() {
        std::cout << "[Account Destructor] Destroying base Account\n";
    }
};

class SeniorAccount : public Account {
    double* history = new double[100]; // 800 bytes dynamically allocated

public:
    double rate() const { return 4.0; }
    ~SeniorAccount() {
        std::cout << "[SeniorAccount Destructor] Releasing history array (800 bytes)\n";
        delete[] history;
    }
};

int main()
{
    std::cout << "--- 1. Testing rate() with raw base pointer ---\n";
    Account* accounts[2] = { new Account, new SeniorAccount };
    for (Account* a : accounts) {
        std::cout << "Rate: " << a->rate() << "%\n"; // DEFECT 1: Prints 3.5 for both!
    }

    std::cout << "\n--- 2. Deleting through base pointers ---\n";
    for (Account* a : accounts) {
        delete a; // DEFECT 2: SeniorAccount destructor NEVER runs! Memory leak!
    }

    std::cout << "\n--- 3. Testing Object Slicing ---\n";
    Account copy = SeniorAccount(); // DEFECT 3: Object Slicing!
    std::cout << "Copy Rate: " << copy.rate() << "%\n";

    return 0;
}
