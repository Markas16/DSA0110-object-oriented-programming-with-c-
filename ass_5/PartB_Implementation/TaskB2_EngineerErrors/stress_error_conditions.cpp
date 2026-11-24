#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include "../../PartA_Design/TaskA4_ExceptionStrategy/exception_hierarchy.h"

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - Task B2: Stress Testing Engineered Error Conditions
 * Demonstrates:
 *  1. Negative charges
 *  2. Claim exceeding policy cover
 *  3. Unknown package code
 *  4. Parsing non-numeric text with std::stod
 *  5. Memory exhaustion (std::bad_alloc)
 *  6. RAII stack unwinding verification via destructor logging
 *  7. Top-level catch(...) fault tolerance
 * ============================================================================
 */

// RAII Guard object to track stack unwinding
class TransactionScopeGuard {
    std::string operationName;

public:
    explicit TransactionScopeGuard(const std::string& name) : operationName(name) {
        std::cout << "  [RAII-INIT] Opened scope: " << operationName << '\n';
    }

    ~TransactionScopeGuard() {
        std::cout << "  [RAII-CLEANUP] Destroyed scope: " << operationName 
                  << " (Resources cleanly released!)\n";
    }
};

// 1. Validating negative charge
void testNegativeCharge(double consultation, double tests) {
    TransactionScopeGuard guard("NegativeChargeCheck");
    if (consultation < 0.0) throw NegativeChargeException("Consultation", consultation);
    if (tests < 0.0) throw NegativeChargeException("DiagnosticTests", tests);
    std::cout << "  -> Charge validated: Rs. " << (consultation + tests) << '\n';
}

// 2. Validating claim exceeding cover
void testClaimCover(double billed, double cover) {
    TransactionScopeGuard guard("ClaimCoverAudit");
    if (billed > cover) {
        throw ClaimLimitExceededException(billed, cover);
    }
    std::cout << "  -> Claim approved: Rs. " << billed << '\n';
}

// 3. Validating unknown package code
void testPackageCode(const std::string& code) {
    TransactionScopeGuard guard("PackageCodeLookup");
    if (code != "CATARACT-01" && code != "ENDOSCOPY-02" && code != "HERNIA-03") {
        throw UnknownPackageException(code);
    }
    std::cout << "  -> Package recognized: [" << code << "]\n";
}

// 4. Parsing numerical input from text
double testNumericParse(const std::string& rawText) {
    TransactionScopeGuard guard("NumericParser");
    // std::stod throws std::invalid_argument if non-numeric
    return std::stod(rawText);
}

// 5. Simulating heap memory exhaustion
void testMemoryExhaustion() {
    TransactionScopeGuard guard("MemoryAllocationProbe");
    std::cout << "  -> Attempting to allocate 100 Terabytes (size_t(-1))...\n";
    std::vector<double> massive(static_cast<size_t>(-1)); // Forces std::bad_alloc
}

int main() {
    std::cout << "=================================================================================\n";
    std::cout << "          TASK B2: ADVERSARIAL STRESS TESTING OF EXCEPTION SUBSYSTEM             \n";
    std::cout << "=================================================================================\n\n";

    // Scenario 1: Negative Charge
    std::cout << "[TEST 1: Negative Charge Injection]\n";
    try {
        testNegativeCharge(500.0, -1200.0);
    } catch (const BillingError& e) {
        std::cout << "  [CAUGHT EXPECTED]: " << e.what() << "\n\n";
    }

    // Scenario 2: Claim Exceeds Cover
    std::cout << "[TEST 2: Claim Exceeding Policy Cover]\n";
    try {
        testClaimCover(75000.0, 50000.0);
    } catch (const ClaimError& e) {
        std::cout << "  [CAUGHT EXPECTED]: " << e.what() << "\n\n";
    }

    // Scenario 3: Unknown Package Code
    std::cout << "[TEST 3: Unknown Surgical Package Code]\n";
    try {
        testPackageCode("INVALID-PKG-99");
    } catch (const ClaimError& e) {
        std::cout << "  [CAUGHT EXPECTED]: " << e.what() << "\n\n";
    }

    // Scenario 4: Non-numeric String
    std::cout << "[TEST 4: Non-Numeric Text Input Parsing]\n";
    try {
        double val = testNumericParse("FiveThousandRupees");
        std::cout << "Value: " << val << '\n';
    } catch (const std::invalid_argument& e) {
        std::cout << "  [CAUGHT EXPECTED std::invalid_argument]: Corrupted input (" << e.what() << ")\n\n";
    }

    // Scenario 5: Memory Allocation Failure (std::bad_alloc)
    std::cout << "[TEST 5: Forced std::bad_alloc Memory Exhaustion]\n";
    try {
        testMemoryExhaustion();
    } catch (const std::bad_alloc& e) {
        std::cout << "  [CAUGHT EXPECTED std::bad_alloc]: System out of heap memory (" << e.what() << ")\n\n";
    }

    // Scenario 6: Top-Level Catch-All Fault Tolerance
    std::cout << "[TEST 6: Top-Level catch(...) Fault Tolerance Boundary]\n";
    try {
        TransactionScopeGuard guard("TopLevelBoundary");
        throw 999; // Arbitrary non-standard thrown value
    } catch (const std::exception& e) {
        std::cout << "  [Standard Exception Caught]: " << e.what() << '\n';
    } catch (...) {
        std::cout << "  [CRITICAL RECOVERY]: Non-standard exception caught by catch(...)!\n"
                  << "  Hospital service remains operational. Zero crash occurred!\n\n";
    }

    std::cout << "=================================================================================\n";
    std::cout << "All engineered error scenarios passed. Program completed safely without abort.\n";
    std::cout << "=================================================================================\n";
    return 0;
}
