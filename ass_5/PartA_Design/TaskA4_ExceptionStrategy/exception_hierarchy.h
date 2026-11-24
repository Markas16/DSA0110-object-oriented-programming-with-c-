#ifndef EXCEPTION_HIERARCHY_H
#define EXCEPTION_HIERARCHY_H

#include <stdexcept>
#include <string>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 5 - Task A4: Hospital Enterprise Exception Hierarchy
 * All domain exceptions derive from std::exception for robust polymorphic catching.
 * ============================================================================
 */

// Root Domain Exception
class HospitalException : public std::exception {
protected:
    std::string errorMessage;

public:
    explicit HospitalException(const std::string& msg) : errorMessage(msg) {}
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

// Branch 1: Billing and Ledger Violations
class BillingError : public HospitalException {
public:
    explicit BillingError(const std::string& msg) : HospitalException("BillingError: " + msg) {}
};

class NegativeChargeException : public BillingError {
public:
    explicit NegativeChargeException(const std::string& item, double value)
        : BillingError("Negative monetary charge rejected for [" + item + "]: Rs. " + std::to_string(value)) {}
};

class InvalidBillDataException : public BillingError {
public:
    explicit InvalidBillDataException(const std::string& msg)
        : BillingError("Invalid bill data: " + msg) {}
};

// Branch 2: Insurance and Claim Processing Errors
class ClaimError : public HospitalException {
public:
    explicit ClaimError(const std::string& msg) : HospitalException("ClaimError: " + msg) {}
};

class ClaimLimitExceededException : public ClaimError {
public:
    ClaimLimitExceededException(double billed, double cover)
        : ClaimError("Claim amount Rs. " + std::to_string(billed) + 
                     " exceeds policy cover limit of Rs. " + std::to_string(cover)) {}
};

class UnknownPackageException : public ClaimError {
public:
    explicit UnknownPackageException(const std::string& pkgCode)
        : ClaimError("Unrecognized surgical package code: [" + pkgCode + "]") {}
};

class PatientRecordNotFoundException : public HospitalException {
public:
    explicit PatientRecordNotFoundException(const std::string& patientId)
        : HospitalException("PatientRecordNotFound: No medical record found for ID: " + patientId) {}
};

#endif // EXCEPTION_HIERARCHY_H
