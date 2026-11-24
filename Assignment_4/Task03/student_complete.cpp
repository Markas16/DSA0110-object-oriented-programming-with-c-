#include <iostream>
#include <string>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 3: Complete the Code (TODO 1)
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

class Person {
protected:
    std::string name;
    int age;

public:
    Person(const std::string& n, int a) : name(n), age(a) {}
};

class Address { // used as a member class (composition)
    std::string city;
    int pin;

public:
    Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); }
};

class Student : public Person {
    long long regNo;
    Address home;

public:
    // TODO 1 Completed: Initialise the Person base, regNo and the home Address member in the initializer list
    Student(const std::string& n, int a, long long r, const std::string& c, int p)
        : Person(n, a), regNo(r), home(c, p) {}

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
