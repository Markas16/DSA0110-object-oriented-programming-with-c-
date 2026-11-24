#include <iostream>
#include <string>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 3: Experiment with Swapped Initializer List Order
 * Compiling with -Wall triggers -Wreorder warning
 * ============================================================================
 */

class Person {
protected:
    std::string name;
    int age;

public:
    Person(const std::string& n, int a) : name(n), age(a) {}
};

class Address {
    std::string city;
    int pin;

public:
    Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); }
};

class Student : public Person {
    long long regNo; // Declared FIRST
    Address home;    // Declared SECOND

public:
    // SWAPPED ORDER IN INITIALIZER LIST: home before regNo
    // Triggers compiler warning: -Wreorder
    Student(const std::string& n, int a, long long r, const std::string& c, int p)
        : Person(n, a), home(c, p), regNo(r) {}

    void print() const
    {
        std::cout << regNo << ' ' << name << " (" << age << ") " << home.str() << '\n';
    }
};

int main()
{
    Student s("Kavya", 19, 192311001, "Salem", 636001);
    s.print();
    return 0;
}
