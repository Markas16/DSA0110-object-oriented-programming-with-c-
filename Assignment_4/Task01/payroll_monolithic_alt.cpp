#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 1: Alternative Concept (Monolithic Staff with switch-case)
 * Used for Empirical Space & Complexity Analysis
 * ============================================================================
 */

enum class StaffType {
    PROFESSOR,
    LAB_ASSISTANT,
    GUEST_FACULTY,
    ADMIN_STAFF // Adding this requires editing multiple locations!
};

class MonolithicStaff {
private:
    StaffType type;
    std::string name;
    int id;

    // Fields required across ALL staff categories (many remain unused per instance)
    double basic;
    double daRate;
    double researchAllowance;
    double hours;
    double rate;
    double adminAllowance;

public:
    // Large constructor with many optional/unused parameters
    MonolithicStaff(StaffType t, const std::string& n, int i, 
                    double b = 0.0, double res = 0.0, 
                    double h = 0.0, double r = 0.0, 
                    double adm = 0.0)
        : type(t), name(n), id(i), basic(b), daRate(0.42),
          researchAllowance(res), hours(h), rate(r), adminAllowance(adm) {}

    // Calculation with switch statement
    double pay() const {
        switch (type) {
            case StaffType::PROFESSOR:
                return basic + (basic * daRate) + researchAllowance;
            case StaffType::LAB_ASSISTANT:
                return basic + (basic * daRate);
            case StaffType::GUEST_FACULTY:
                return hours * rate;
            case StaffType::ADMIN_STAFF:
                return basic + (basic * daRate) + adminAllowance;
            default:
                return 0.0;
        }
    }

    void display() const {
        std::cout << "ID: " << id << " | Name: " << std::left << std::setw(18) << name;
        switch (type) {
            case StaffType::PROFESSOR:
                std::cout << " | Role: Professor     | Pay: Rs. " << pay() << '\n';
                break;
            case StaffType::LAB_ASSISTANT:
                std::cout << " | Role: Lab Assistant | Pay: Rs. " << pay() << '\n';
                break;
            case StaffType::GUEST_FACULTY:
                std::cout << " | Role: Guest Faculty | Pay: Rs. " << pay() << '\n';
                break;
            case StaffType::ADMIN_STAFF:
                std::cout << " | Role: Admin Staff   | Pay: Rs. " << pay() << '\n';
                break;
        }
    }

    static size_t objectSize() {
        return sizeof(MonolithicStaff);
    }
};

int main() {
    std::cout << "Size of MonolithicStaff object: " << MonolithicStaff::objectSize() << " bytes\n";
    std::cout << "(Note: Every object wastes memory for fields not relevant to its type!)\n\n";

    std::vector<MonolithicStaff> list = {
        MonolithicStaff(StaffType::PROFESSOR, "Dr. Arulmozhi", 101, 85000.0, 15000.0),
        MonolithicStaff(StaffType::LAB_ASSISTANT, "Kannan R.", 201, 32000.0),
        MonolithicStaff(StaffType::GUEST_FACULTY, "Priya Sundaram", 301, 0.0, 0.0, 45.0, 1200.0),
        MonolithicStaff(StaffType::ADMIN_STAFF, "Muthu Kumar", 401, 40000.0, 0.0, 0.0, 0.0, 5000.0)
    };

    double total = 0.0;
    for (const auto& s : list) {
        s.display();
        total += s.pay();
    }
    std::cout << "Total Payroll: Rs. " << std::fixed << std::setprecision(2) << total << '\n';
    return 0;
}
