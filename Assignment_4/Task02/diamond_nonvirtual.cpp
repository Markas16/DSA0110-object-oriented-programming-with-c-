#include <iostream>

struct Person { 
    Person() { std::cout << "Person "; } 
};

// Non-virtual inheritance from Person
struct Student : Person { 
    Student() { std::cout << "Student "; } 
};

// Non-virtual inheritance from Person
struct Staff : Person { 
    Staff() { std::cout << "Staff "; } 
};

struct TeachingAssistant : Student, Staff {
    TeachingAssistant() { std::cout << "TA "; }
};

int main()
{
    TeachingAssistant ta;
    std::cout << '\n';
    std::cout << "Size of non-virtual TA: " << sizeof(ta) << " bytes\n";
    return 0;
}
