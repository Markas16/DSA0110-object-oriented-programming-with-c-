#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - Task B1: Full Hospital Polymorphic Billing Implementation
 * Validates All 4 Bill Kinds with All 3 Payers
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

// Base Bill Abstract Class
class Bill {
protected:
    std::string patient;
    int billId;

public:
    Bill(const std::string& p, int id) : patient(p), billId(id) {}
    virtual ~Bill() = default; // Essential virtual destructor

    virtual double total() const = 0;
    virtual std::string type() const = 0;

    virtual void print() const {
        std::cout << "Bill #" << billId << " | " << std::left << std::setw(16) << type()
                  << " | Patient: " << std::setw(14) << patient
                  << " | Total: Rs. " << std::right << std::setw(9) << std::fixed << std::setprecision(2) << total();
    }

    const std::string& getPatient() const noexcept { return patient; }
    int getId() const noexcept { return billId; }
};

// 1. Out-Patient Bill
class OutPatientBill : public Bill {
    double consultation;
    double diagnosticTests;

public:
    OutPatientBill(const std::string& p, int id, double c, double t)
        : Bill(p, id), consultation(c), diagnosticTests(t) {}

    double total() const override { return consultation + diagnosticTests; }
    std::string type() const override { return "OutPatient"; }
};

// 2. In-Patient Bill
class InPatientBill : public Bill {
    int days;
    double perDayRoomRate;
    double surgery;
    double pharmacy;

public:
    InPatientBill(const std::string& p, int id, int d, double rate, double s, double rx)
        : Bill(p, id), days(d), perDayRoomRate(rate), surgery(s), pharmacy(rx) {}

    double total() const override { return (days * perDayRoomRate) + surgery + pharmacy; }
    std::string type() const override { return "InPatient"; }
};

// 3. Day-Care Surgery Bill
class DayCareSurgeryBill : public Bill {
    std::string packageCode;
    double packagePrice;

public:
    DayCareSurgeryBill(const std::string& p, int id, const std::string& code, double price)
        : Bill(p, id), packageCode(code), packagePrice(price) {}

    double total() const override { return packagePrice; }
    std::string type() const override { return "DayCareSurgery"; }
};

// 4. Emergency Bill
class EmergencyBill : public Bill {
    double baseCharge;
    double hourlyRate;
    int hours;

public:
    EmergencyBill(const std::string& p, int id, double base, double rate, int h)
        : Bill(p, id), baseCharge(base), hourlyRate(rate), hours(h) {}

    double total() const override { return baseCharge + (hourlyRate * hours); }
    std::string type() const override { return "Emergency"; }
};

// Base Payer Abstract Class
class Payer {
protected:
    std::string payerName;

public:
    explicit Payer(const std::string& name) : payerName(name) {}
    virtual ~Payer() = default;

    virtual double amountCovered(const Bill& bill) const = 0;
    const std::string& getName() const noexcept { return payerName; }
};

class SelfPay : public Payer {
public:
    SelfPay() : Payer("SelfPay") {}
    double amountCovered(const Bill& /*bill*/) const override { return 0.0; }
};

class Insurance : public Payer {
    double coverLimit;
    double coPayPercent;

public:
    Insurance(const std::string& name, double limit, double coPay)
        : Payer(name), coverLimit(limit), coPayPercent(coPay) {}

    double amountCovered(const Bill& bill) const override {
        double eligible = std::min(bill.total(), coverLimit);
        return eligible * (1.0 - (coPayPercent / 100.0));
    }
};

class GovernmentScheme : public Payer {
    double inPatientCap;
    double outPatientCap;

public:
    GovernmentScheme(const std::string& name, double ipCap, double opCap)
        : Payer(name), inPatientCap(ipCap), outPatientCap(opCap) {}

    double amountCovered(const Bill& bill) const override {
        if (bill.type() == "InPatient") return std::min(bill.total(), inPatientCap);
        if (bill.type() == "OutPatient") return std::min(bill.total(), outPatientCap);
        return std::min(bill.total(), 15000.0); // Generic package cap
    }
};

int main() {
    std::cout << "=========================================================================================\n";
    std::cout << "          TASK B1: POLYMORPHIC BILLING VERIFICATION & TEST MATRIX                        \n";
    std::cout << "=========================================================================================\n\n";

    // Create one bill of each kind
    std::vector<std::unique_ptr<Bill>> testBills;
    testBills.push_back(std::make_unique<OutPatientBill>("Ravi Kumar", 1001, 600.0, 1400.0));          // Total: 2000.00
    testBills.push_back(std::make_unique<InPatientBill>("Meena Iyer", 1002, 4, 3000.0, 45000.0, 8000.0)); // Total: (4*3000)+45000+8000 = 65000.00
    testBills.push_back(std::make_unique<DayCareSurgeryBill>("Karthik S.", 1003, "CATARACT-01", 25000.0)); // Total: 25000.00
    testBills.push_back(std::make_unique<EmergencyBill>("Anand R.", 1004, 5000.0, 1200.0, 5));          // Total: 5000+(1200*5) = 11000.00

    // Payers
    std::vector<std::unique_ptr<Payer>> payers;
    payers.push_back(std::make_unique<SelfPay>());
    payers.push_back(std::make_unique<Insurance>("Star Health Comprehensive", 50000.0, 10.0));
    payers.push_back(std::make_unique<GovernmentScheme>("AB-PMJAY Scheme", 40000.0, 1500.0));

    for (const auto& bill : testBills) {
        bill->print();
        std::cout << "\n  --- Payer Settlement Matrix ---:\n";
        for (const auto& payer : payers) {
            double billed = bill->total();
            double covered = payer->amountCovered(*bill);
            double patientDue = billed - covered;
            std::cout << "    [" << std::left << std::setw(26) << payer->getName() << "] "
                      << "Covered: Rs. " << std::right << std::setw(8) << std::fixed << std::setprecision(2) << covered
                      << " | Patient Due: Rs. " << std::setw(8) << patientDue << '\n';
        }
        std::cout << "-----------------------------------------------------------------------------------------\n";
    }

    return 0;
}
