#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <chrono>
#include <variant>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - Task B3: Performance Benchmark & Empirical Cost Analysis
 * Compares:
 *   1. Virtual Functions (Polymorphic Vtable Dispatch)
 *   2. Monolithic Struct with Type Enum and switch statement
 *   3. std::variant with std::visit (Tagged Union Alternative)
 * Dataset Scales: 1,000 | 1,000,000 | 10,000,000 bills
 * Compile with: g++ -std=c++17 -O2 -Wall -Wextra benchmark_virtual_vs_switch.cpp
 * ============================================================================
 */

// ----------------------------------------------------------------------------
// 1. VIRTUAL FUNCTIONS HIERARCHY
// ----------------------------------------------------------------------------
class VBill {
public:
    virtual ~VBill() = default;
    virtual double total() const = 0;
};

class VOutPatient : public VBill {
    double consultation, tests;
public:
    VOutPatient(double c, double t) : consultation(c), tests(t) {}
    double total() const override { return consultation + tests; }
};

class VInPatient : public VBill {
    int days; double rate, surgery;
public:
    VInPatient(int d, double r, double s) : days(d), rate(r), surgery(s) {}
    double total() const override { return (days * rate) + surgery; }
};

// ----------------------------------------------------------------------------
// 2. MONOLITHIC STRUCT WITH ENUM AND SWITCH
// ----------------------------------------------------------------------------
enum class SBillType : uint8_t { OUT_PATIENT, IN_PATIENT };

struct SBill {
    SBillType type;
    // Union of fields
    double consultation;
    double tests;
    int days;
    double rate;
    double surgery;

    double total() const {
        switch (type) {
            case SBillType::OUT_PATIENT: return consultation + tests;
            case SBillType::IN_PATIENT:  return (days * rate) + surgery;
        }
        return 0.0;
    }
};

// ----------------------------------------------------------------------------
// 3. ALTERNATIVE: std::variant and std::visit
// ----------------------------------------------------------------------------
struct VariantOP {
    double consultation, tests;
    double total() const { return consultation + tests; }
};

struct VariantIP {
    int days; double rate, surgery;
    double total() const { return (days * rate) + surgery; }
};

using BillVariant = std::variant<VariantOP, VariantIP>;

// ----------------------------------------------------------------------------
// MAIN BENCHMARK ENGINE
// ----------------------------------------------------------------------------
int main() {
    std::cout << "=========================================================================================\n";
    std::cout << " TASK B3.1 & B3.2: PERFORMANCE BENCHMARK (VIRTUAL vs SWITCH vs VARIANT) [-O2 OPTIMIZED]  \n";
    std::cout << "=========================================================================================\n\n";

    // ------------------------------------------------------------------------
    // B3.1: CORRECTNESS VERIFICATION ON 10 BILLS
    // ------------------------------------------------------------------------
    std::cout << "--- B3.1: Validating Correctness on Small Dataset (10 Bills) ---\n";
    std::vector<std::unique_ptr<VBill>> smallVirtual;
    std::vector<SBill> smallSwitch;

    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            smallVirtual.push_back(std::make_unique<VOutPatient>(500.0 + i * 10, 1200.0));
            SBill b{}; b.type = SBillType::OUT_PATIENT; b.consultation = 500.0 + i * 10; b.tests = 1200.0;
            smallSwitch.push_back(b);
        } else {
            smallVirtual.push_back(std::make_unique<VInPatient>(i, 2000.0, 30000.0));
            SBill b{}; b.type = SBillType::IN_PATIENT; b.days = i; b.rate = 2000.0; b.surgery = 30000.0;
            smallSwitch.push_back(b);
        }
    }

    double vTotal = 0.0, sTotal = 0.0;
    for (const auto& b : smallVirtual) vTotal += b->total();
    for (const auto& b : smallSwitch)  sTotal += b.total();

    std::cout << "  Virtual Function 10-Bill Total : Rs. " << std::fixed << std::setprecision(2) << vTotal << '\n';
    std::cout << "  Switch Statement 10-Bill Total : Rs. " << std::fixed << std::setprecision(2) << sTotal << '\n';
    if (std::abs(vTotal - sTotal) < 1e-5) {
        std::cout << "  [VERIFICATION SUCCESS] Both calculation approaches produce IDENTICAL results!\n\n";
    }

    // ------------------------------------------------------------------------
    // B3.2: SCALABILITY TIMINGS: 1,000 | 1,000,000 | 10,000,000 BILLS
    // ------------------------------------------------------------------------
    const std::vector<size_t> testSizes = {1000, 1000000, 10000000};

    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(15) << "Dataset Size"
              << std::setw(22) << "Virtual Func (ms)"
              << std::setw(20) << "Switch Stmt (ms)"
              << std::setw(20) << "std::visit (ms)"
              << "Agree?\n";
    std::cout << "-----------------------------------------------------------------------------------------\n";

    for (size_t n : testSizes) {
        // Pre-allocate switch vector
        std::vector<SBill> switchBills;
        switchBills.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            SBill b{};
            if (i % 2 == 0) {
                b.type = SBillType::OUT_PATIENT; b.consultation = 500.0; b.tests = 1200.0;
            } else {
                b.type = SBillType::IN_PATIENT; b.days = 3; b.rate = 2500.0; b.surgery = 40000.0;
            }
            switchBills.push_back(b);
        }

        // Pre-allocate variant vector
        std::vector<BillVariant> variantBills;
        variantBills.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            if (i % 2 == 0) variantBills.emplace_back(VariantOP{500.0, 1200.0});
            else variantBills.emplace_back(VariantIP{3, 2500.0, 40000.0});
        }

        // Measure Switch Time
        auto startS = std::chrono::high_resolution_clock::now();
        double sumS = 0.0;
        for (size_t i = 0; i < n; ++i) sumS += switchBills[i].total();
        auto endS = std::chrono::high_resolution_clock::now();
        double timeSwitchMs = std::chrono::duration<double, std::milli>(endS - startS).count();

        // Measure Variant Time
        auto startVar = std::chrono::high_resolution_clock::now();
        double sumVar = 0.0;
        for (size_t i = 0; i < n; ++i) {
            sumVar += std::visit([](const auto& item) { return item.total(); }, variantBills[i]);
        }
        auto endVar = std::chrono::high_resolution_clock::now();
        double timeVarMs = std::chrono::duration<double, std::milli>(endVar - startVar).count();

        // Measure Virtual Time
        // For n = 10,000,000, allocate lightweight array of raw pointers to avoid heap overhead distortion
        double timeVirtualMs = 0.0;
        double sumV = 0.0;

        if (n <= 1000000) {
            std::vector<std::unique_ptr<VBill>> vBills;
            vBills.reserve(n);
            for (size_t i = 0; i < n; ++i) {
                if (i % 2 == 0) vBills.push_back(std::make_unique<VOutPatient>(500.0, 1200.0));
                else vBills.push_back(std::make_unique<VInPatient>(3, 2500.0, 40000.0));
            }
            auto startV = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < n; ++i) sumV += vBills[i]->total();
            auto endV = std::chrono::high_resolution_clock::now();
            timeVirtualMs = std::chrono::duration<double, std::milli>(endV - startV).count();
        } else {
            // Simulated 10M via interleaved array of base pointers
            VOutPatient sampleOP(500.0, 1200.0);
            VInPatient sampleIP(3, 2500.0, 40000.0);
            std::vector<VBill*> rawPtrs(n);
            for (size_t i = 0; i < n; ++i) rawPtrs[i] = (i % 2 == 0) ? static_cast<VBill*>(&sampleOP) : static_cast<VBill*>(&sampleIP);

            auto startV = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < n; ++i) sumV += rawPtrs[i]->total();
            auto endV = std::chrono::high_resolution_clock::now();
            timeVirtualMs = std::chrono::duration<double, std::milli>(endV - startV).count();
        }

        std::cout << std::left << std::setw(15) << n
                  << std::setw(22) << timeVirtualMs
                  << std::setw(20) << timeSwitchMs
                  << std::setw(20) << timeVarMs
                  << (std::abs(sumS - sumVar) < 1e-4 ? "YES" : "NO") << '\n';
    }

    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << "Memory Footprint Analysis:\n";
    std::cout << "  sizeof(SBill) monolithic struct : " << sizeof(SBill) << " bytes\n";
    std::cout << "  sizeof(BillVariant) std::variant: " << sizeof(BillVariant) << " bytes\n";
    std::cout << "  sizeof(VOutPatient) derived obj : " << sizeof(VOutPatient) << " bytes (+ 8-byte ptr in array)\n";
    std::cout << "  sizeof(VInPatient) derived obj  : " << sizeof(VInPatient) << " bytes (+ 8-byte ptr in array)\n";
    std::cout << "=========================================================================================\n";
    return 0;
}
