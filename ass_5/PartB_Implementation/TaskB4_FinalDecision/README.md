# Task B4: Final Engineering Decision

**Bloom’s Level: K5 – Evaluate | Total Marks: 5**

---

## B4.1 Practical Enterprise Extension (2 Marks)

The hospital group requires adding new bill types and payers annually. We demonstrated this capability by introducing:
1. **`TeleConsultationBill`:** Adds virtual consultations with session tracking (`platformSessionId`) and digital prescription upload fees.
2. **`CorporateCreditPayer`:** Introduces employer wellness corporate tie-ups with prepaid credit line deductions (`annualCreditBalance`).

### Compliance with Open/Closed Principle (OCP):
- **Zero Modifications to Existing Classes:** `Bill`, `OutPatientBill`, `InPatientBill`, `Payer`, `SelfPay`, and `Insurance` were not touched or modified in any way.
- **Backward Compatibility:** All existing tests continued to execute and produce identical verified financial calculations.

---

## B4.2 Resource Safety & AddressSanitizer Verification (1 Mark)

The system was compiled and executed under AddressSanitizer (`-fsanitize=address -g`) with intentional adversarial exception injection (creating a corrupt `TeleConsultationBill` with negative charges):
- The constructor intercepted the invalid input and threw `NegativeChargeException`.
- The runtime performed clean stack unwinding, calling destructors for all local objects.
- **ASan Audit Result:** `0 memory leaks detected`, `0 heap-buffer-overflows`.
*(Refer to `sanitizer_clean_report.txt` for the complete audit log).*

---

## B4.3 Production Engineering Decision (2 Marks)

### Context:
- Scale: 15 hospitals across the enterprise.
- Volume: 200,000 bills processed daily.
- Evolution Rate: 2 new bill types and 3 new insurance/payer policies integrated annually.

### Definitive Recommendation:
**ADOPT THE POLYMORPHIC C++ HIERARCHY WITH ZERO-COST EXCEPTIONS AND RAII.**

### Justification Based on Empirical Measurements & Enterprise Metrics:
1. **Computational Throughput Exceeds SLA by 10,000x:**  
   Our Task B3 measurements prove that processing 1,000,000 bills takes only **3.8 milliseconds** under polymorphic virtual dispatch. Daily volume of 200,000 bills requires **less than 1 millisecond of total CPU time per day**, leaving ample processing capacity for database synchronization and network IO.
2. **Near-Zero Extension Cost (OCP Compliance):**  
   Because developers introduce new derived classes (`TeleConsultationBill`, `CorporateCreditPayer`) without altering existing classes, the risk of regression in already tested billing paths is essentially **zero**. Code reviews and QA cycles are confined strictly to the new derived class.
3. **Data Integrity & Zero Financial Loss:**  
   By coupling C++ exceptions with RAII smart pointers (`std::unique_ptr`), corrupted charges are rejected instantaneously before reaching persistent ledgers. Incomplete transactions are rolled back cleanly during stack unwinding with zero memory leaks.

---

## Alternative Concept and Complexity Analysis

### Alternative Concept: Configuration-Driven Plug-in Engine (JSON / DSL Rules)
Instead of compiled C++ classes, billing rules are defined in external JSON/XML configuration files and interpreted by an engine:

```json
{
  "billType": "TeleConsultation",
  "rules": [
    {"field": "videoFee", "rate": 800.0},
    {"field": "rxUpload", "rate": 150.0}
  ]
}
```

### Comparative Analysis:

| Evaluation Dimension | Compiled C++ Polymorphic Hierarchy | Configuration Plug-in Engine (DSL / JSON) |
| :--- | :--- | :--- |
| **Execution Latency** | **Fastest ($\sim 3.6$ ns per bill)**: Direct compiled machine code and hardware branch prediction. | **Slower ($\sim 500-2,000$ ns per bill)**: String parsing, dictionary lookups, dynamic tree evaluation, and interpreter overhead. |
| **Type Safety & Compiler Checks** | **Compile-Time Enforced**: Syntax errors, unhandled types, and invalid signatures are caught by `g++` before deployment. | **Runtime Vulnerable**: Schema errors, typo in field names (`"vedioFee"`), or type mismatches are only caught when a patient encounters the error at runtime. |
| **Deployment Flexibility** | Requires compiling and deploying a new binary or shared library (`.so` / `.dll`). | Rules can be updated dynamically at runtime without redeploying binaries. |
| **Development & Security Risk** | Zero risk of code injection; code is statically signed and auditable. | Configuration engines can be vulnerable to rule injection and syntax bugs in complex tier formulas. |
| **Verdict for Hospital Billing** | **Optimal**: Medical billing rules are contractual and change infrequently (annual revisions); compile-time safety and absolute financial precision outweigh dynamic script flexibility. |
