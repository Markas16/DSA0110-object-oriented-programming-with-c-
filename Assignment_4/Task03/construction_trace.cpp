#include <iostream>
#include <string>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 3: Order of Construction and Destruction Trace
 * Includes Member Classes (Address, Course) and Virtual Inheritance
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

// Member Class for Person
class Address {
    std::string city;
public:
    Address(const std::string& c = "Chennai") : city(c) {
        std::cout << "  [Construct] Address(" << city << ")\n";
    }
    ~Address() {
        std::cout << "  [Destruct]  ~Address(" << city << ")\n";
    }
};

// Member Class for Student
class Course {
    std::string code;
public:
    Course(const std::string& cd = "CS101") : code(cd) {
        std::cout << "  [Construct] Course(" << code << ")\n";
    }
    ~Course() {
        std::cout << "  [Destruct]  ~Course(" << code << ")\n";
    }
};

// Base Class with Address member class
class Person {
protected:
    std::string name;
    Address addr; // Member class (Composition)

public:
    Person(const std::string& n = "DefaultPerson", const std::string& city = "Salem")
        : name(n), addr(city) {
        std::cout << " [Construct] Person(" << name << ")\n";
    }
    virtual ~Person() {
        std::cout << " [Destruct]  ~Person(" << name << ")\n";
    }
};

// Student: Virtual Base Person + Course member class
class Student : virtual public Person {
protected:
    std::string prog;
    Course enrolledCourse; // Member class

public:
    Student(const std::string& n = "DefaultStudent", const std::string& city = "Trichy",
            const std::string& pr = "B.Tech", const std::string& crs = "CS301")
        : Person(n, city), prog(pr), enrolledCourse(crs) {
        std::cout << " [Construct] Student(" << prog << ")\n";
    }
    ~Student() override {
        std::cout << " [Destruct]  ~Student(" << prog << ")\n";
    }
};

// Staff: Virtual Base Person
class Staff : virtual public Person {
protected:
    std::string dept;

public:
    Staff(const std::string& n = "DefaultStaff", const std::string& city = "Coimbatore",
          const std::string& d = "CSE")
        : Person(n, city), dept(d) {
        std::cout << " [Construct] Staff(" << dept << ")\n";
    }
    ~Staff() override {
        std::cout << " [Destruct]  ~Staff(" << dept << ")\n";
    }
};

// TeachingAssistant: Most-derived class in diamond
class TeachingAssistant : public Student, public Staff {
    int hours;

public:
    TeachingAssistant(const std::string& n, const std::string& city,
                      const std::string& pr, const std::string& crs,
                      const std::string& d, int h)
        // Most-derived class explicitly calls virtual base Person
        : Person(n, city),
          Student(n, city, pr, crs),
          Staff(n, city, d),
          hours(h) {
        std::cout << " [Construct] TeachingAssistant(" << hours << " hrs)\n";
    }
    ~TeachingAssistant() override {
        std::cout << " [Destruct]  ~TeachingAssistant(" << hours << " hrs)\n";
    }
};

int main() {
    std::cout << "=======================================================\n";
    std::cout << " 1. TRACING CONSTRUCTION & DESTRUCTION OF STUDENT     \n";
    std::cout << "=======================================================\n";
    {
        Student s("Kavya", "Madurai", "B.Tech", "DSA01");
    }

    std::cout << "\n=======================================================\n";
    std::cout << " 2. TRACING CONSTRUCTION & DESTRUCTION OF STAFF       \n";
    std::cout << "=======================================================\n";
    {
        Staff st("Prof. Raman", "Tirunelveli", "ECE");
    }

    std::cout << "\n=======================================================\n";
    std::cout << " 3. TRACING CONSTRUCTION & DESTRUCTION OF TA          \n";
    std::cout << "=======================================================\n";
    {
        TeachingAssistant ta("Karthik", "Salem", "M.Tech", "AI201", "CSE", 15);
    }

    std::cout << "\n=======================================================\n";
    std::cout << " SIZEOF COMPARISON                                     \n";
    std::cout << "=======================================================\n";
    std::cout << "sizeof(Address):           " << sizeof(Address) << " bytes\n";
    std::cout << "sizeof(Course):            " << sizeof(Course) << " bytes\n";
    std::cout << "sizeof(Person):            " << sizeof(Person) << " bytes\n";
    std::cout << "sizeof(Student):           " << sizeof(Student) << " bytes\n";
    std::cout << "sizeof(Staff):             " << sizeof(Staff) << " bytes\n";
    std::cout << "sizeof(TeachingAssistant): " << sizeof(TeachingAssistant) << " bytes\n";

    return 0;
}
