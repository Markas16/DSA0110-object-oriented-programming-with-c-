#include <iostream>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - Task A2: Predict the Output & Vtable Binding Mechanics
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

struct Bill {
    virtual ~Bill() { std::cout << "~Bill "; }
    virtual double total() const = 0;
    void show() const { std::cout << total() << ' '; }
};

struct OPBill : Bill {
    ~OPBill() override { std::cout << "~OPBill "; }
    double total() const override { return 500; }
};

int main()
{
    std::cout << "--- 1. Execution with Virtual Destructor ---\n";
    Bill* b = new OPBill;
    b->show();
    delete b;
    std::cout << '\n';

    return 0;
}
