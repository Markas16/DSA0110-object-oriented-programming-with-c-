#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 2: University People Management Hierarchy
 * Demonstrating Diamond Resolution with Virtual Base Classes and Member Classes
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

// Root Base Class
class Person {
protected:
    std::string name;
    int id;
    std::string phone;

public:
    Person(const std::string& n = "Unknown", int i = 0, const std::string& p = "0000000000")
        : name(n), id(i), phone(p) {}

    virtual ~Person() = default;

    virtual void display() const {
        std::cout << "ID: " << id << " | Name: " << std::left << std::setw(16) << name 
                  << " | Phone: " << phone;
    }

    const std::string& getName() const { return name; }
    int getId() const { return id; }
    const std::string& getPhone() const { return phone; }
};

// Student: Virtual Public Inheritance from Person
class Student : virtual public Person {
protected:
    std::string programme;
    double cgpa;

public:
    Student(const std::string& n, int i, const std::string& p,
            const std::string& prog, double gpa)
        : Person(n, i, p), programme(prog), cgpa(gpa) {}

    void display() const override {
        Person::display();
        std::cout << " | Prog: " << programme << " | CGPA: " << std::fixed << std::setprecision(2) << cgpa;
    }

    const std::string& getProgramme() const { return programme; }
    double getCGPA() const { return cgpa; }
};

// Staff: Virtual Public Inheritance from Person
class Staff : virtual public Person {
protected:
    std::string department;
    double salary;

public:
    Staff(const std::string& n, int i, const std::string& p,
          const std::string& dept, double sal)
        : Person(n, i, p), department(dept), salary(sal) {}

    void display() const override {
        Person::display();
        std::cout << " | Dept: " << department << " | Salary: Rs. " << std::fixed << std::setprecision(2) << salary;
    }

    const std::string& getDepartment() const { return department; }
    double getSalary() const { return salary; }
};

// TeachingAssistant: Multiple Inheritance from Student and Staff (Diamond Resolved via Virtual Inheritance)
class TeachingAssistant : public Student, public Staff {
private:
    int teachingHoursPerWeek;

public:
    // The most-derived class is responsible for initializing the virtual base Person
    TeachingAssistant(const std::string& n, int i, const std::string& p,
                      const std::string& prog, double gpa,
                      const std::string& dept, double sal,
                      int hours)
        : Person(n, i, p),
          Student(n, i, p, prog, gpa),
          Staff(n, i, p, dept, sal),
          teachingHoursPerWeek(hours) {}

    void display() const override {
        // Direct unambiguous access to name and id from shared Person sub-object!
        std::cout << "[TA Record] ID: " << id << " | Name: " << name << " | Phone: " << phone << '\n'
                  << "            Programme: " << programme << " | CGPA: " << cgpa << '\n'
                  << "            Dept: " << department << " | Stipend: Rs. " << salary << '\n'
                  << "            Teaching Load: " << teachingHoursPerWeek << " hrs/week\n";
    }
};

// Fellowship: Member Class (has-a relationship)
class Fellowship {
private:
    std::string grantTitle;
    double stipend;
    int tenureMonths;

public:
    Fellowship(const std::string& title = "Institutional Grant", double stip = 35000.0, int tenure = 36)
        : grantTitle(title), stipend(stip), tenureMonths(tenure) {}

    void printFellowship() const {
        std::cout << "Grant: " << grantTitle << " | Monthly Stipend: Rs. " 
                  << std::fixed << std::setprecision(2) << stipend 
                  << " | Tenure: " << tenureMonths << " months";
    }
};

// ResearchScholar: Multilevel Inheritance from Student, with Fellowship member (has-a)
class ResearchScholar : public Student {
private:
    std::string researchTopic;
    Fellowship fellowship; // has-a relationship (Composition)

public:
    ResearchScholar(const std::string& n, int i, const std::string& p,
                    const std::string& prog, double gpa,
                    const std::string& topic,
                    const Fellowship& f)
        : Person(n, i, p), Student(n, i, p, prog, gpa),
          researchTopic(topic), fellowship(f) {}

    void display() const override {
        std::cout << "[Scholar Record] ";
        Student::display();
        std::cout << "\n                 Topic: " << researchTopic << '\n'
                  << "                 ";
        fellowship.printFellowship();
        std::cout << '\n';
    }
};

int main() {
    std::cout << "=======================================================================\n";
    std::cout << "               UNIVERSITY PEOPLE MANAGEMENT SYSTEM                    \n";
    std::cout << "=======================================================================\n";

    // Polymorphic collection
    std::vector<std::unique_ptr<Person>> people;

    people.push_back(std::make_unique<Student>("Aravind S.", 202401, "9876543210", "B.Tech CSE", 9.15));
    people.push_back(std::make_unique<Staff>("Dr. Deepa M.", 10045, "9443210987", "Computer Science", 115000.0));
    
    // Teaching Assistant inheriting Student and Staff with single shared Person
    people.push_back(std::make_unique<TeachingAssistant>(
        "Siddharth V.", 202188, "9123456780", 
        "M.Tech AI", 8.85, 
        "CSE Dept", 25000.0, 12
    ));

    // Research Scholar with Fellowship composition
    Fellowship csirGrant("CSIR-JRF Fellowship", 37000.0, 60);
    people.push_back(std::make_unique<ResearchScholar>(
        "Ananya Ramesh", 201977, "9012345678",
        "Ph.D Computing", 9.40,
        "Quantum Machine Learning Algorithms",
        csirGrant
    ));

    for (const auto& p : people) {
        p->display();
        std::cout << "-----------------------------------------------------------------------\n";
    }

    std::cout << "Size of TeachingAssistant: " << sizeof(TeachingAssistant) << " bytes\n";
    return 0;
}
