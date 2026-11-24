# DSA01 – Assignment 4: Designing Reusable Class Hierarchies

**Course:** Object Oriented Programming with C++ (DSA01)  
**Course Outcome:** CO4 – Analyse and implement C++ applications using inheritance, types of inheritance, virtual base classes, abstract classes, pointers and polymorphism to design reusable and extensible programs.  
**Nature of Assignment:** Individual | **Bloom’s Level Assessed:** K5 – Evaluate  
**Repository Name:** `DSA01-<RegisterNumber>-Assignment4`  

---

## Student & Environment Information

- **Student Name:** [Enter Your Name]
- **Register Number:** [Enter Your Register Number]
- **Course Code:** DSA01
- **Compiler & Version:** `g++ (GCC) 13.2.0` / `g++ 13` (MinGW-w64 / GCC on Linux / MSYS2)
- **Standard Compilation Command:**
  ```bash
  g++ -std=c++17 -Wall -Wextra <filename.cpp> -o <output.exe>
  ```
- **AddressSanitizer Command (Task 4):**
  ```bash
  g++ -std=c++17 -Wall -Wextra -fsanitize=address -g account_buggy.cpp -o account_buggy.exe
  ```

---

## Repository Structure

```
ass_4/
├── README.md                          <-- Master README with compilation and running guidelines
├── REPORT.md                          <-- Comprehensive academic K5 evaluation report with UML & traces
├── run_all_tasks.bat                  <-- Master batch script executing all tasks sequentially
│
├── Task01/                            <-- Task 1: Class Design Review (College Payroll)
│   ├── payroll_redesign.cpp           <-- Repaired OOP class hierarchy
│   ├── payroll_monolithic_alt.cpp     <-- Monolithic switch-based alternative for empirical analysis
│   ├── compile_and_run.bat            <-- Windows one-click build and execution script
│   ├── output.txt                     <-- Captured program execution output
│   └── README.md                      <-- Detailed Task 1 report (Deficiencies, UML, Defence, Big-O)
│
├── Task02/                            <-- Task 2: University People Management
│   ├── university_hierarchy.cpp       <-- Complete diamond hierarchy with virtual inheritance
│   ├── diamond_prediction.cpp         <-- Predict the Output program (with virtual)
│   ├── diamond_nonvirtual.cpp         <-- Non-virtual experiment showing duplicated Person
│   ├── compile_and_run.bat
│   ├── output.txt
│   └── README.md                      <-- Detailed Task 2 report (Diamond, Access table, Prediction)
│
├── Task03/                            <-- Task 3: Constructors in Derived and Member Classes
│   ├── student_complete.cpp           <-- Completed code for TODO 1 (Member initializer list)
│   ├── reorder_warning_demo.cpp       <-- Swapped order demonstrating -Wreorder warning
│   ├── construction_trace.cpp         <-- Complete constructor and destructor trace with sizeof
│   ├── engine_vehicle_predict.cpp     <-- Predict the Output program (Engine, Vehicle, Car)
│   ├── compile_and_run.bat
│   ├── output.txt
│   └── README.md                      <-- Detailed Task 3 report (Order rules, Composition, Rule of 3)
│
├── Task04/                            <-- Task 4: Pointers to Derived Classes Forensics
│   ├── account_buggy.cpp              <-- Buggy code demonstrating non-virtual defects & slicing
│   ├── account_repaired.cpp           <-- Repaired code with virtual destructor and unique_ptr
│   ├── account_predict.cpp            <-- Predict the Output program (rate vs vrate vs slice)
│   ├── sanitizer_report.txt           <-- AddressSanitizer 800-byte heap memory leak report
│   ├── compile_and_run.bat
│   ├── output.txt
│   └── README.md                      <-- Detailed Task 4 report (Forensics, Evidence, Prevention rules)
│
├── Task05/                            <-- Task 5: Pointers to Objects and the this Pointer
│   ├── cart_complete.cpp              <-- Completed code for TODO 1 & 2 (this pointer & chaining)
│   ├── cart_four_ways.cpp             <-- Demonstration of four ways to reach an object
│   ├── cart_array_search.cpp          <-- Array of Cart pointers and highest total search
│   ├── compile_and_run.bat
│   ├── output.txt
│   └── README.md                      <-- Detailed Task 5 report (Evaluation, this analysis, vector alt)
│
└── Task06/                            <-- Task 6: Design Challenge (Vehicle Rental Fleet)
    ├── vehicle_rental_fleet.cpp       <-- Full system with Bicycle, TwoWheeler, Car, EV, and ElectricBus
    ├── compile_and_run.bat
    ├── output.txt
    └── README.md                      <-- Detailed Task 6 report (Stages 1-5, SIMATS Pseudocode, UML)
```

---

## Steps to Compile and Run Each Task

### Quick Master Execution (Windows):
Double-click `run_all_tasks.bat` or run:
```powershell
.\run_all_tasks.bat
```

### Manual Individual Commands:

#### Task 1: College Payroll System
```bash
cd Task01
g++ -std=c++17 -Wall -Wextra payroll_redesign.cpp -o payroll_redesign.exe
./payroll_redesign.exe
g++ -std=c++17 -Wall -Wextra payroll_monolithic_alt.cpp -o payroll_monolithic_alt.exe
./payroll_monolithic_alt.exe
```

#### Task 2: University People Management
```bash
cd Task02
g++ -std=c++17 -Wall -Wextra university_hierarchy.cpp -o university_hierarchy.exe
./university_hierarchy.exe
g++ -std=c++17 -Wall -Wextra diamond_prediction.cpp -o diamond_prediction.exe
./diamond_prediction.exe
```

#### Task 3: Constructors & Member Classes
```bash
cd Task03
g++ -std=c++17 -Wall -Wextra student_complete.cpp -o student_complete.exe
./student_complete.exe
g++ -std=c++17 -Wall -Wextra reorder_warning_demo.cpp -o reorder_warning_demo.exe
g++ -std=c++17 -Wall -Wextra construction_trace.cpp -o construction_trace.exe
./construction_trace.exe
```

#### Task 4: Forensics & Polymorphism
```bash
cd Task04
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g account_buggy.cpp -o account_buggy.exe
./account_buggy.exe
g++ -std=c++17 -Wall -Wextra account_repaired.cpp -o account_repaired.exe
./account_repaired.exe
g++ -std=c++17 -Wall -Wextra account_predict.cpp -o account_predict.exe
./account_predict.exe
```

#### Task 5: Pointers to Objects & `this` Pointer
```bash
cd Task05
g++ -std=c++17 -Wall -Wextra cart_complete.cpp -o cart_complete.exe
./cart_complete.exe
g++ -std=c++17 -Wall -Wextra cart_four_ways.cpp -o cart_four_ways.exe
./cart_four_ways.exe
g++ -std=c++17 -Wall -Wextra cart_array_search.cpp -o cart_array_search.exe
./cart_array_search.exe
```

#### Task 6: Vehicle Rental Fleet Design Challenge
```bash
cd Task06
g++ -std=c++17 -Wall -Wextra vehicle_rental_fleet.cpp -o vehicle_rental_fleet.exe
./vehicle_rental_fleet.exe
```

---

## Summary of Results Across All Tasks

| Task | Core Concept Evaluated | Result Summary |
| :--- | :--- | :--- |
| **Task 01** | Class Design Review & OCP | Repaired 7 critical OOP flaws; designed `Staff` hierarchy with pure virtual `pay()`; verified unified single-loop payroll processing (Total: Rs. 296,940.00). Proved $O(1)$ space efficiency over monolithic class. |
| **Task 02** | Diamond Problem & Virtual Inheritance | Correctly resolved ambiguity in `TeachingAssistant` inheriting from `Student` and `Staff`. Verified single construction of `Person`. Produced comprehensive access visibility matrix. |
| **Task 03** | Constructors, Member Classes & Order | Completed `TODO 1` using member initializer list (`192311001 Kavya (19) Salem-636001`); documented `-Wreorder` compiler warning; recorded full lifecycle traces; proved virtual base rules. |
| **Task 04** | Polymorphism & Destructor Forensics | Uncovered static binding bug (3.5% vs 4.0%), undefined behavior leak (800 bytes caught via AddressSanitizer), and object slicing. Repaired with virtual destructor and `std::unique_ptr<Account>`. |
| **Task 05** | Object Access & `this` Pointer | Completed `TODO 1` & `TODO 2` enabling method chaining (`324.00`); demonstrated 4 access paradigms; implemented pointer arithmetic search to find highest customer cart (`Rs. 1575.00`); analyzed vector pointer invalidation. |
| **Task 06** | Full System Design Challenge | Designed and built full vehicle rental fleet (Bicycle, TwoWheeler, Car, EV); implemented SIMATS standard pseudocode for rental/returns with late penalties; proved Open/Closed Principle by adding `ElectricBus` with zero modification to base classes. |

---

## Progressive Commit History Guidelines

To adhere to the mandatory submission instructions:
```bash
git init
git add README.md REPORT.md
git commit -m "chore: initial commit with assignment documentation and structure"

git add Task01/
git commit -m "feat(task1): complete college payroll redesign and diagnosis"

git add Task02/
git commit -m "feat(task2): resolve diamond problem in university people hierarchy"

git add Task03/
git commit -m "feat(task3): implement derived constructors, member classes and order tracing"

git add Task04/
git commit -m "feat(task4): forensic analysis of non-virtual defects and AddressSanitizer leak proof"

git add Task05/
git commit -m "feat(task5): implement this pointer method chaining and array search"

git add Task06/
git commit -m "feat(task6): implement vehicle rental fleet management system with OCP extension"
```
