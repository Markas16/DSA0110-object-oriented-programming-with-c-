#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - Task A3: Payer Hierarchy & Open/Closed Principle
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

// Forward declaration of Bill
class Bill {
public:
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
    virtual std::string getPatientName() const = 0;
};

// Concrete Bill stub for demonstrating Payer calculations
class MockInPatientBill : public Bill {
    std::string patient;
    double billedTotal;

public:
    MockInPatientBill(const std::string& p, double t) : patient(p), billedTotal(t) {}
    double total() const override { return billedTotal; }
    std::string type() const override { return "InPatient"; }
    std::string getPatientName() const override { return patient; }
};

// Abstract Class: Payer
class Payer {
protected:
    std::string payerName;

public:
    explicit Payer(const std::string& name) : payerName(name) {}
    virtual ~Payer() = default;

    // Pure virtual function defining policy coverage contract
    virtual double amountCovered(const Bill& bill) const = 0;

    virtual void printCoverage(const Bill& bill) const {
        double billed = bill.total();
        double covered = amountCovered(bill);
        double patientShare = (billed > covered) ? (billed - covered) : 0.0;

        std::cout << "Payer: " << std::left << std::setw(22) << payerName
                  << " | Billed: Rs. " << std::right << std::setw(8) << std::fixed << std::setprecision(2) << billed
                  << " | Covered: Rs. " << std::setw(8) << covered
                  << " | Patient Due: Rs. " << std::setw(8) << patientShare << '\n';
    }

    const std::string& getName() const noexcept { return payerName; }
};

// Derived Payer 1: SelfPay (Patient pays 100%, coverage = 0)
class SelfPay : public Payer {
public:
    SelfPay() : Payer("Self-Pay (Direct)") {}

    double amountCovered(const Bill& /*bill*/) const override {
        return 0.0; // Zero external coverage
    }
};

// Derived Payer 2: Insurance (Subject to policy cover limit and co-payment percentage)
class Insurance : public Payer {
private:
    double coverLimit;
    double coPayPercentage; // e.g. 10.0 for 10%

public:
    Insurance(const std::string& insurerName, double limit, double coPay)
        : Payer(insurerName), coverLimit(limit), coPayPercentage(coPay) {}

    double amountCovered(const Bill& bill) const override {
        double billed = bill.total();
        double eligibleAmount = std::min(billed, coverLimit);
        double patientCoPay = eligibleAmount * (coPayPercentage / 100.0);
        return eligibleAmount - patientCoPay;
    }
};

// Derived Payer 3: GovernmentScheme (Fixed capped package rates per bill category)
class GovernmentScheme : public Payer {
private:
    std::string schemeCode;
    double inPatientPackageCap;
    double outPatientPackageCap;

public:
    GovernmentScheme(const std::string& code, double ipCap = 30000.0, double opCap = 1500.0)
        : Payer("Govt Scheme [" + code + "]"), schemeCode(code),
          inPatientPackageCap(ipCap), outPatientPackageCap(opCap) {}

    double amountCovered(const Bill& bill) const override {
        double billed = bill.total();
        if (bill.type() == "InPatient") {
            return std::min(billed, inPatientPackageCap);
        } else if (bill.type() == "OutPatient") {
            return std::min(billed, outPatientPackageCap);
        }
        return std::min(billed, 10000.0); // Default emergency cap
    }
};

// ============================================================================
// TASK A3.2: EXTENSION - Corporate Tie-up Payer
// Added WITHOUT modifying Payer, SelfPay, Insurance, or GovernmentScheme!
// ============================================================================
class CorporateTieUp : public Payer {
private:
    std::string corporationName;
    double negotiatedDiscountRate; // e.g. 15% discount
    double employeeCreditLine;     // Maximum company-sponsored credit

public:
    CorporateTieUp(const std::string& corp, double discount, double credit)
        : Payer("Corporate [" + corp + "]"), corporationName(corp),
          negotiatedDiscountRate(discount), employeeCreditLine(credit) {}

    double amountCovered(const Bill& bill) const override {
        double billed = bill.total();
        double discountedBill = billed * (1.0 - (negotiatedDiscountRate / 100.0));
        return std::min(discountedBill, employeeCreditLine);
    }
};

int main() {
    std::cout << "===================================================================================\n";
    std::cout << "               HOSPITAL INSURANCE & PAYER SETTLEMENT SYSTEM                        \n";
    std::cout << "===================================================================================\n";

    MockInPatientBill bill("Arun Vijay", 50000.0);
    std::cout << "Processing Claim for Patient: " << bill.getPatientName() 
              << " | Total Billed: Rs. " << bill.total() << "\n\n";

    std::vector<std::unique_ptr<Payer>> payers;
    payers.push_back(std::make_unique<SelfPay>());
    payers.push_back(std::make_unique<Insurance>("Star Health Gold", 40000.0, 10.0));
    payers.push_back(std::make_unique<GovernmentScheme>("AB-PMJAY", 30000.0));
    payers.push_back(std::make_unique<CorporateTieUp>("Tata Consultancy Services", 15.0, 45000.0));

    for (const auto& p : payers) {
        p->printCoverage(bill);
    }

    std::cout << "===================================================================================\n";
    return 0;
}
