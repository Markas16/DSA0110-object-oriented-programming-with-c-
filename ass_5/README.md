# DSA01 – Assignment 5: Reliable Software with Runtime Polymorphism and Exceptions

**Course:** Object Oriented Programming with C++ (DSA01)  
**Course Outcome:** CO5 – Build robust C++ applications by applying virtual functions, runtime polymorphism and exception handling mechanisms to develop reliable and error-resilient software solutions.  
**Nature of Assignment:** Individual | **Bloom’s Level Assessed:** K5 – Evaluate  
**Repository Name:** `DSA01-<RegisterNumber>-Assignment5`  

---

## Student & Environment Information

- **Student Name:** [Enter Your Name]
- **Register Number:** [Enter Your Register Number]
- **Course Code:** DSA01
- **Compiler & Version:** `g++ (GCC) 13.2.0` / `g++ 13` (MinGW-w64 / Linux GCC / MSYS2)
- **Standard Compilation Command:**
  ```bash
  g++ -std=c++17 -Wall -Wextra <source.cpp> -o <binary.exe>
  ```
- **AddressSanitizer Command:**
  ```bash
  g++ -std=c++17 -Wall -Wextra -fsanitize=address -g <source.cpp> -o <binary.exe>
  ```
- **High-Performance Optimization Command (Task B3):**
  ```bash
  g++ -std=c++17 -O2 -Wall -Wextra <source.cpp> -o <binary.exe>
  ```

---

## Repository Structure

```
ass_5/
├── README.md                                    <-- Master repository README
├── REPORT.md                                    <-- Formal Academic Evaluation Report (Part A & B)
├── run_all_tasks.bat                            <-- Windows master execution script
│
├── PartA_Design/
│   ├── TaskA1_BillHierarchy/
│   │   ├── bill_hierarchy_pseudocode.txt        <-- SIMATS standard algorithmic pseudocode
│   │   └── README.md                            <-- Interface, UML diagram, Big-O analysis
│   ├── TaskA2_RuntimeBinding/
│   │   ├── predict_vtable.cpp                   <-- Predict the Output program
│   │   ├── vtable_diagram.txt                   <-- Detailed memory & vtable architecture
│   │   ├── compile_and_run.bat
│   │   ├── output.txt
│   │   └── README.md                            <-- Early/late binding, vtable mechanics & dynamic_cast alt
│   ├── TaskA3_AbstractClasses/
│   │   ├── payer_hierarchy.cpp                  <-- Abstract Payer, SelfPay, Insurance, Govt, Corporate
│   │   ├── compile_and_run.bat
│   │   ├── output.txt
│   │   └── README.md                            <-- Payer design, OCP extension & std::function alt
│   ├── TaskA4_ExceptionStrategy/
│   │   ├── exception_hierarchy.h                <-- Custom hospital domain exception hierarchy
│   │   └── README.md                            <-- Error taxonomy, noexcept rules & return codes alt
│   └── TaskA5_RecoveryRethrow/
│       ├── predict_stack_unwinding.cpp          <-- Predict the Output program (RAII Guard)
│       ├── compile_and_run.bat
│       ├── output.txt
│       └── README.md                            <-- Multi-tier catch/rethrow, RAII stack unwinding
│
└── PartB_Implementation/
    ├── TaskB1_PolymorphicBilling/
    │   ├── complete_billing.cpp                 <-- Completed code for TODO 1, 2, 3
    │   ├── full_billing_system.cpp              <-- Full matrix: 4 bill kinds x 3 payers
    │   ├── compile_and_run.bat
    │   ├── output.txt
    │   └── README.md                            <-- Test matrix, hand verification & template alt
    ├── TaskB2_EngineerErrors/
    │   ├── complete_claims.cpp                  <-- Completed code for TODO 1 & 2
    │   ├── predict_validate_process.cpp         <-- Predict the Output program
    │   ├── stress_error_conditions.cpp          <-- Injected 5 errors, bad_alloc & top-level catch
    │   ├── compile_and_run.bat
    │   ├── output.txt
    │   └── README.md                            <-- Error analysis, throw vs throw e, optional alt
    ├── TaskB3_PerformanceBenchmark/
    │   ├── benchmark_virtual_vs_switch.cpp      <-- Benchmark for 1K, 1M, 10M bills (Virtual vs Switch vs Variant)
    │   ├── benchmark_exceptions_vs_codes.cpp    <-- Benchmark for 1M validations (Codes vs Exceptions @ 0%, 1%, 50%)
    │   ├── compile_and_run.bat
    │   ├── output.txt
    │   └── README.md                            <-- Timings, charts, trade-off matrix
    └── TaskB4_FinalDecision/
        ├── enterprise_hospital_system.cpp       <-- 15-hospital production system with TeleConsult & CorporateCredit
        ├── sanitizer_clean_report.txt           <-- AddressSanitizer zero-leak verification
        ├── compile_and_run.bat
        ├── output.txt
        └── README.md                            <-- Production decision & configuration engine alt
```

---

## Steps to Compile and Run Each Task

### Quick Master Execution (Windows):
Double-click `run_all_tasks.bat` or run:
```powershell
.\run_all_tasks.bat
```

### Manual Individual Commands:

#### Task A2: Runtime Binding & Vtables
```bash
cd PartA_Design/TaskA2_RuntimeBinding
g++ -std=c++17 -Wall -Wextra predict_vtable.cpp -o predict_vtable.exe
./predict_vtable.exe
```

#### Task A3: Payer Hierarchy
```bash
cd PartA_Design/TaskA3_AbstractClasses
g++ -std=c++17 -Wall -Wextra payer_hierarchy.cpp -o payer_hierarchy.exe
./payer_hierarchy.exe
```

#### Task A5: Stack Unwinding & RAII
```bash
cd PartA_Design/TaskA5_RecoveryRethrow
g++ -std=c++17 -Wall -Wextra predict_stack_unwinding.cpp -o predict_stack_unwinding.exe
./predict_stack_unwinding.exe
```

#### Task B1: Polymorphic Billing Validation
```bash
cd PartB_Implementation/TaskB1_PolymorphicBilling
g++ -std=c++17 -Wall -Wextra complete_billing.cpp -o complete_billing.exe
./complete_billing.exe
g++ -std=c++17 -Wall -Wextra full_billing_system.cpp -o full_billing_system.exe
./full_billing_system.exe
```

#### Task B2: Engineered Errors & Claims
```bash
cd PartB_Implementation/TaskB2_EngineerErrors
g++ -std=c++17 -Wall -Wextra complete_claims.cpp -o complete_claims.exe
./complete_claims.exe
g++ -std=c++17 -Wall -Wextra predict_validate_process.cpp -o predict_validate_process.exe
./predict_validate_process.exe
g++ -std=c++17 -Wall -Wextra stress_error_conditions.cpp -o stress_error_conditions.exe
./stress_error_conditions.exe
```

#### Task B3: Performance Benchmarks
```bash
cd PartB_Implementation/TaskB3_PerformanceBenchmark
g++ -std=c++17 -O2 -Wall -Wextra benchmark_virtual_vs_switch.cpp -o benchmark_virtual_vs_switch.exe
./benchmark_virtual_vs_switch.exe
g++ -std=c++17 -O2 -Wall -Wextra benchmark_exceptions_vs_codes.cpp -o benchmark_exceptions_vs_codes.exe
./benchmark_exceptions_vs_codes.exe
```

#### Task B4: Final Production System & AddressSanitizer
```bash
cd PartB_Implementation/TaskB4_FinalDecision
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g enterprise_hospital_system.cpp -o enterprise_hospital_system.exe
./enterprise_hospital_system.exe
```

---

## Summary of Results Across All Tasks

| Task | Component Evaluated | Result Summary |
| :--- | :--- | :--- |
| **Task A1** | Bill Interface & SIMATS Pseudocode | Formulated abstract base `Bill`; created Mermaid UML diagram; produced SIMATS algorithmic pseudocode for daily ledger; proved $\mathcal{O}(1)$ space superiority over monolithic switch. |
| **Task A2** | Runtime Binding & Vtables | Predicted output (`500 ~OPBill ~Bill `); traced exact byte-level vtable dereferences; analyzed $\mathcal{O}(k)$ penalty of `dynamic_cast` ladder. |
| **Task A3** | Abstract Classes & OCP Extension | Designed `Payer` contract with `SelfPay`, `Insurance`, `GovernmentScheme`; extended seamlessly with `CorporateTieUp` with zero changes to existing classes. |
| **Task A4** | Enterprise Exception Hierarchy | Built custom taxonomy derived from `std::exception`; established `noexcept` specifications; justified why destructors must never throw. |
| **Task A5** | Recovery, Rethrow & RAII | Predicted output (`~f ~main caught 42`); detailed multi-tier catch architecture; proved automatic stack unwinding resource reclamation over manual `delete`. |
| **Task B1** | Polymorphic Billing Implementation | Completed `TODO 1–3` (`Total 49200`); constructed $4 \times 3$ test matrix; verified all computations by hand; demonstrated compiler failure when removing `virtual`. |
| **Task B2** | Engineered Adversarial Errors | Completed claim settlement `TODO 1 & 2`; predicted output; stress-tested negative charges, bad package, `std::bad_alloc`, non-numeric text; proved `throw;` prevents slicing. |
| **Task B3** | Scalability & Exception Benchmarks | Proved exact agreement on 10 bills; measured 10,000,000 bills (Virtual: 36.2 ms, Switch: 19.8 ms, Variant: 24.5 ms); proved exceptions are faster on 0% error path (1.14 ms vs 1.42 ms). |
| **Task B4** | Enterprise Decision & Clean ASan | Integrated `TeleConsultationBill` and `CorporateCreditPayer` with zero edits to base classes; verified zero memory leaks under AddressSanitizer; formulated formal decision for 15-hospital group. |

---

## Progressive Commit History Guidelines

```bash
git init
git add README.md REPORT.md run_all_tasks.bat
git commit -m "chore: initial commit with assignment structure and documentation"

git add PartA_Design/TaskA1_BillHierarchy/
git commit -m "feat(taskA1): design polymorphic bill hierarchy and SIMATS pseudocode"

git add PartA_Design/TaskA2_RuntimeBinding/
git commit -m "feat(taskA2): vtable architecture, memory layout and runtime binding trace"

git add PartA_Design/TaskA3_AbstractClasses/
git commit -m "feat(taskA3): abstract payer hierarchy with corporate extension"

git add PartA_Design/TaskA4_ExceptionStrategy/
git commit -m "feat(taskA4): hospital exception hierarchy and noexcept specification"

git add PartA_Design/TaskA5_RecoveryRethrow/
git commit -m "feat(taskA5): multi-tier recovery design and RAII stack unwinding trace"

git add PartB_Implementation/TaskB1_PolymorphicBilling/
git commit -m "feat(taskB1): implement polymorphic billing matrix and complete code TODOs"

git add PartB_Implementation/TaskB2_EngineerErrors/
git commit -m "feat(taskB2): adversarial error engineering, claim settlement and throw; semantics"

git add PartB_Implementation/TaskB3_PerformanceBenchmark/
git commit -m "feat(taskB3): large-scale benchmark for virtual vs switch and exceptions cost"

git add PartB_Implementation/TaskB4_FinalDecision/
git commit -m "feat(taskB4): enterprise hospital group deployment and clean AddressSanitizer audit"
```
