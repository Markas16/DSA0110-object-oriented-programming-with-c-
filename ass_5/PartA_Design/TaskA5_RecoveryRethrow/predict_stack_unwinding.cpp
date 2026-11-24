#include <iostream>
#include <string>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - Task A5: Predict the Output - Stack Unwinding & RAII Guards
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

struct Guard {
    std::string n;
    explicit Guard(const std::string& s) : n(s) {}
    ~Guard() { std::cout << '~' << n << ' '; }
};

void f()
{
    Guard g("f");
    throw 42;
}

int main()
{
    try {
        Guard m("main");
        f();
        std::cout << "not printed ";
    } catch (int x) {
        std::cout << "caught " << x;
    } catch (...) {
        std::cout << "other";
    }
    std::cout << '\n';
    return 0;
}
