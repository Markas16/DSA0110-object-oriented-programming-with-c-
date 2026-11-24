#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 1.2: Repaired Class Hierarchy for College Payroll
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

// Common Abstract Base Class
class Staff {
protected:
    std::string name;
    int id;

public:
    Staff(const std::string& name, int id)
        : name(name), id(id) {}

    virtual ~Staff() = default; // Essential for polymorphic deletion

    // Pure virtual function making Staff an abstract class
    virtual double pay() const = 0;

    virtual void display() const {
        std::cout << "ID: " << id << " | Name: " << std::left << std::setw(18) << name;
    }

    const std::string& getName() const { return name; }
    int getId() const { return id; }
};

// Intermediate class for staff receiving basic salary + dearness allowance (DA)
class SalariedStaff : public Staff {
protected:
    double basic;
    static constexpr double DA_RATE = 0.42; // Replaces magic number 0.42

public:
    SalariedStaff(const std::string& name, int id, double basic)
        : Staff(name, id), basic(basic) {}

    double calculateBaseWithDA() const {
        return basic + (basic * DA_RATE);
    }

    double getBasic() const { return basic; }
    static double getDARate() { return DA_RATE; }
};

// Professor: SalariedStaff + Research Allowance
class Professor : public SalariedStaff {
private:
    double researchAllowance;

public:
    Professor(const std::string& name, int id, double basic, double researchAllowance)
        : SalariedStaff(name, id, basic), researchAllowance(researchAllowance) {}

    double pay() const override {
        return calculateBaseWithDA() + researchAllowance;
    }

    void display() const override {
        Staff::display();
        std::cout << " | Role: Professor     | Pay: Rs. " << std::fixed << std::setprecision(2) << pay() << '\n';
    }
};

// LabAssistant: SalariedStaff (Basic + DA)
class LabAssistant : public SalariedStaff {
public:
    LabAssistant(const std::string& name, int id, double basic)
        : SalariedStaff(name, id, basic) {}

    double pay() const override {
        return calculateBaseWithDA();
    }

    void display() const override {
        Staff::display();
        std::cout << " | Role: Lab Assistant | Pay: Rs. " << std::fixed << std::setprecision(2) << pay() << '\n';
    }
};

// GuestFaculty: Paid hourly (hours * rate)
class GuestFaculty : public Staff {
private:
    double hours;
    double rate;

public:
    GuestFaculty(const std::string& name, int id, double hours, double rate)
        : Staff(name, id), hours(hours), rate(rate) {}

    double pay() const override {
        return hours * rate;
    }

    void display() const override {
        Staff::display();
        std::cout << " | Role: Guest Faculty | Pay: Rs. " << std::fixed << std::setprecision(2) << pay() << '\n';
    }
};

// Extension Example: Demonstrating Open/Closed Principle
class AdministrativeStaff : public SalariedStaff {
private:
    double administrativeAllowance;

public:
    AdministrativeStaff(const std::string& name, int id, double basic, double adminAllowance)
        : SalariedStaff(name, id, basic), administrativeAllowance(adminAllowance) {}

    double pay() const override {
        return calculateBaseWithDA() + administrativeAllowance;
    }

    void display() const override {
        Staff::display();
        std::cout << " | Role: Admin Staff   | Pay: Rs. " << std::fixed << std::setprecision(2) << pay() << '\n';
    }
};

int main() {
    std::cout << "=======================================================================\n";
    std::cout << "            COLLEGE PAYROLL SYSTEM - POLYMORPHIC PROCESSING            \n";
    std::cout << "=======================================================================\n";

    // Polymorphic collection of staff pointers using std::unique_ptr for memory safety
    std::vector<std::unique_ptr<Staff>> payrollList;

    payrollList.push_back(std::make_unique<Professor>("Dr. Arulmozhi", 101, 85000.0, 15000.0));
    payrollList.push_back(std::make_unique<LabAssistant>("Kannan R.", 201, 32000.0));
    payrollList.push_back(std::make_unique<GuestFaculty>("Priya Sundaram", 301, 45.0, 1200.0));
    payrollList.push_back(std::make_unique<AdministrativeStaff>("Muthu Kumar", 401, 40000.0, 5000.0));

    double totalPayroll = 0.0;

    // Single unified loop processing all staff types
    for (const auto& staffMember : payrollList) {
        staffMember->display();
        totalPayroll += staffMember->pay();
    }

    std::cout << "-----------------------------------------------------------------------\n";
    std::cout << "Total Monthly Payroll Expenditure: Rs. " 
              << std::fixed << std::setprecision(2) << totalPayroll << '\n';
    std::cout << "=======================================================================\n";

    return 0;
}
