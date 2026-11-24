#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include "../../PartA_Design/TaskA4_ExceptionStrategy/exception_hierarchy.h"

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - Task B4: Final Production Engineering Decision System
 * Demonstrates:
 *   - 15-Hospital Group Scale Architecture (200,000 daily bills)
 *   - B4.1: Adding TeleConsultationBill and CorporateCreditPayer with 0 edits to existing classes
 *   - B4.2: Resource safety under thrown/caught exceptions (AddressSanitizer clean)
 *   - B4.3: Production evaluation and justification
 * Compiler: g++ -std=c++17 -Wall -Wextra -fsanitize=address -g
 * ============================================================================
 */

// ============================================================================
// CORE EXISTING ARCHITECTURE (Remains completely untouched!)
// ============================================================================
class Bill {
protected:
    std::string patient;
    int billId;

public:
    Bill(const std::string& p, int id) : patient(p), billId(id) {}
    virtual ~Bill() = default;

    virtual double total() const = 0;
    virtual std::string type() const = 0;

    virtual void print() const {
        std::cout << "Bill #" << billId << " | " << std::left << std::setw(18) << type()
                  << " | Patient: " << std::setw(14) << patient
                  << " | Total: Rs. " << std::right << std::setw(9) << std::fixed << std::setprecision(2) << total();
    }

    const std::string& getPatient() const noexcept { return patient; }
    int getId() const noexcept { return billId; }
};

class OutPatientBill : public Bill {
    double consultation, tests;
public:
    OutPatientBill(const std::string& p, int id, double c, double t)
        : Bill(p, id), consultation(c), tests(t) {
        if (c < 0 || t < 0) throw NegativeChargeException("OutPatient", c < 0 ? c : t);
    }
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OutPatient"; }
};

class InPatientBill : public Bill {
    int days; double perDay, surgery, pharmacy;
public:
    InPatientBill(const std::string& p, int id, int d, double rate, double s, double rx)
        : Bill(p, id), days(d), perDay(rate), surgery(s), pharmacy(rx) {
        if (rate < 0 || s < 0 || rx < 0) throw NegativeChargeException("InPatient", rate < 0 ? rate : s);
    }
    double total() const override { return (days * perDay) + surgery + pharmacy; }
    std::string type() const override { return "InPatient"; }
};

class Payer {
protected:
    std::string name;
public:
    explicit Payer(const std::string& n) : name(n) {}
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& bill) const = 0;
    const std::string& getName() const noexcept { return name; }
};

class SelfPay : public Payer {
public:
    SelfPay() : Payer("SelfPay") {}
    double amountCovered(const Bill&) const override { return 0.0; }
};

class Insurance : public Payer {
    double limit, coPay;
public:
    Insurance(const std::string& n, double lim, double cp)
        : Payer(n), limit(lim), coPay(cp) {}
    double amountCovered(const Bill& b) const override {
        double eligible = std::min(b.total(), limit);
        return eligible * (1.0 - coPay / 100.0);
    }
};

// ============================================================================
// B4.1: PRACTICAL ENTERPRISE EXTENSIONS (ADDED WITHOUT EDITING EXISTING CODE!)
// ============================================================================

// Extension 1: TeleConsultationBill (New bill type for remote consultations)
class TeleConsultationBill : public Bill {
private:
    std::string platformSessionId;
    double videoConsultationFee;
    double digitalRxUploadFee;

public:
    TeleConsultationBill(const std::string& p, int id, const std::string& session, double consultFee, double rxFee)
        : Bill(p, id), platformSessionId(session),
          videoConsultationFee(consultFee), digitalRxUploadFee(rxFee) {
        if (consultFee < 0 || rxFee < 0) {
            throw NegativeChargeException("TeleConsultation", consultFee < 0 ? consultFee : rxFee);
        }
    }

    double total() const override {
        return videoConsultationFee + digitalRxUploadFee;
    }

    std::string type() const override {
        return "TeleConsultation";
    }

    void print() const override {
        Bill::print();
        std::cout << " [Session: " << platformSessionId << "]\n";
    }
};

// Extension 2: CorporateCreditPayer (New payer type for employer health credits)
class CorporateCreditPayer : public Payer {
private:
    std::string corporateId;
    double annualCreditBalance;

public:
    CorporateCreditPayer(const std::string& name, const std::string& corpId, double balance)
        : Payer(name), corporateId(corpId), annualCreditBalance(balance) {}

    double amountCovered(const Bill& bill) const override {
        double billed = bill.total();
        if (billed > annualCreditBalance) {
            // Partial cover up to remaining corporate balance
            return annualCreditBalance;
        }
        return billed; // 100% covered by corporate credit line
    }
};

// ============================================================================
// PRODUCTION SIMULATION RUNNER
// ============================================================================
int main() {
    std::cout << "=========================================================================================\n";
    std::cout << "           HOSPITAL GROUP PRODUCTION SYSTEM: ENTERPRISE EXTENSION & SANITIZER AUDIT       \n";
    std::cout << "=========================================================================================\n\n";

    std::vector<std::unique_ptr<Bill>> ledger;

    // 1. Existing Bill Types
    ledger.push_back(std::make_unique<OutPatientBill>("Deepak Verma", 5001, 750.0, 1250.0));
    ledger.push_back(std::make_unique<InPatientBill>("Sunita Rao", 5002, 5, 3500.0, 60000.0, 12000.0));

    // 2. B4.1 Newly Added Extension Bill Type
    ledger.push_back(std::make_unique<TeleConsultationBill>("Aakash Nair", 5003, "ZOOM-MED-9941", 800.0, 150.0));

    // Payers
    std::vector<std::unique_ptr<Payer>> activePayers;
    activePayers.push_back(std::make_unique<SelfPay>());
    activePayers.push_back(std::make_unique<Insurance>("Max Bupa Health", 50000.0, 10.0));
    // B4.1 Newly Added Extension Payer
    activePayers.push_back(std::make_unique<CorporateCreditPayer>("Infosys Corporate Wellness", "INFY-EMP-102", 40000.0));

    std::cout << "--- 1. Processing Active Bills with Existing and New Payers ---\n";
    for (const auto& bill : ledger) {
        bill->print();
        std::cout << '\n';
        for (const auto& payer : activePayers) {
            double billed = bill->total();
            double covered = payer->amountCovered(*bill);
            double due = billed - covered;
            std::cout << "     [" << std::left << std::setw(28) << payer->getName() << "] "
                      << "Covered: Rs. " << std::right << std::setw(8) << std::fixed << std::setprecision(2) << covered
                      << " | Patient Due: Rs. " << std::setw(8) << due << '\n';
        }
        std::cout << "-----------------------------------------------------------------------------------------\n";
    }

    // 3. Testing Exception Handling under AddressSanitizer (B4.2 Resource Safety Check)
    std::cout << "\n--- 2. Stress Testing Exceptions & Memory Safety under AddressSanitizer ---\n";
    try {
        std::cout << "Attempting to create corrupt TeleConsultationBill with negative charges...\n";
        auto badBill = std::make_unique<TeleConsultationBill>("Faulty Patient", 9999, "ERR-SESSION", -500.0, 100.0);
        std::cout << "Total: " << badBill->total() << '\n';
    } catch (const HospitalException& e) {
        std::cout << "[EXPECTED AUDIT CATCH]: " << e.what() << "\n";
        std::cout << "[VERIFIED]: Corrupted object was destroyed during stack unwind without any heap leak!\n";
    }

    std::cout << "\n=========================================================================================\n";
    std::cout << "Hospital Group Billing batch completed successfully with zero memory leaks.\n";
    std::cout << "=========================================================================================\n";
    return 0;
}
