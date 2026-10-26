# DSA01 C++ Assignment 1: City Metro Smart-Card Fare Logic System

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Compiler](https://img.shields.io/badge/Compiler-GCC%2016.2.0-brightgreen.svg)](https://www.gnu.org/software/gcc/)
[![Platform](https://img.shields.io/badge/Platform-Windows%2011-lightgrey.svg)](https://microsoft.com/windows)
[![Tests](https://img.shields.io/badge/Tests-All%20Passing-success.svg)](docs/TEST_RESULTS.md)

## 1. Student and Project Information
- **Student Name:** Markas
- **Register Number:** 192421250
- **Course:** DSA01 — C++ and Object-Oriented Programming
- **Assignment:** Assignment 1
- **Project Title:** Design, Implement and Validate the Fare Logic of a City Metro Smart-Card System
- **GitHub Repository:** [https://github.com/Markas16/DSA0110-object-oriented-programming-with-c-](https://github.com/Markas16/DSA0110-object-oriented-programming-with-c-)

---

## 2. Project Architecture & Repository Layout

```
DSA01-192421250-Assignment1/
├── README.md                          # Main project documentation and quickstart
├── .gitignore                         # Build and binary exclusions
├── docs/
│   ├── Assignment_Report.md           # Submission-ready formal technical report (Tasks 1-12)
│   ├── UML.md                         # Mermaid class, sequence, and state diagrams
│   ├── ASSUMPTIONS.md                 # Explicit financial and business assumptions
│   └── TEST_RESULTS.md                # Verification matrix and empirical test results
├── Task01/
│   └── task01_analysis.md             # Domain modeling, OOP pillars, and O(1) analysis
├── Task02/
│   ├── task02_tokens.cpp              # Token classification and macro pitfall demo
│   └── task02_constants.hpp           # Centralized type-safe constexpr constants
├── Task03/
│   └── task03_datatypes.cpp           # Machine sizeof, limits, and float drift demo
├── Task04/
│   ├── task04_prediction.cpp          # Assignment prediction program (3 3.5 / C 68 / ...)
│   └── task04_safe_conversions.cpp    # Safe static_cast and brace init narrowing checks
├── Task05/
│   ├── task05_prediction.cpp          # Assignment prediction program (11 16 / 10 6 4 / ...)
│   └── task05_operators.cpp           # Operator precedence, 24-hr table lookup, and i=i++
├── Task06/
│   └── task06_fare_control.cpp        # Completed TODO 1 & 2 program + boundary hour tests
├── Task07/
│   ├── procedural_fare.cpp            # Procedural baseline & balance corruption demo
│   └── oop_fare.cpp                   # OOP SmartCard enforcing invariants via encapsulation
├── Task08/
│   └── task08_safe_io.cpp             # Stream fail recovery, cin.clear(), and pretty table
├── Task09/
│   └── (integrated with src/ and tests/)
├── Task10/
│   └── task10_scale_simulation.cpp    # Scale simulation (20k, 500k, 3M) & int32 overflow demo
├── Task11/
│   └── task11_design_defence.md       # Final architectural defence & constexpr vs startup table
├── Task12/
│   ├── task12_day4_challenge.cpp      # Airport zone, weekend discount, and MonthlyPass
│   └── day4_challenge_report.md       # Day-4 parts A, B, C, D impact and defense
├── src/
│   ├── fare_system.hpp                # Production Metro System header
│   ├── fare_system.cpp                # Production implementation
│   └── main.cpp                       # Interactive CLI driver application
├── tests/
│   ├── test_fare_system.cpp           # Automated unit and integration test harness
│   └── test_cases.md                  # Test matrix specifications and manual audit
├── scripts/
│   ├── build.ps1                      # Automated build script for all 14 targets
│   └── run_tests.ps1                  # Test runner and evidence collection script
├── bin/                               # Compiled binaries directory (excluded in Git)
└── Evidence/                          # Captured terminal outputs and benchmark logs
```

---

## 3. Toolchain & Prerequisites

- **Operating System:** Windows 11 (x64)
- **Compiler:** `g++` (GCC) 16.2.0 (MSYS2 UCRT64 toolchain)
- **Compiler Flags:** `-std=c++17 -Wall -Wextra -Wpedantic -O2`
- **Shell:** Windows PowerShell

---

## 4. How to Build and Run

### 4.1 Automated One-Click Build
Run the provided PowerShell build script from the project directory:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\build.ps1
```

This compiles all 14 project targets into `bin/` with zero warnings.

### 4.2 Automated Test & Evidence Execution
Run the comprehensive test runner to execute all test suites, record benchmarks, and generate raw output logs in `Evidence/`:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\run_tests.ps1
```

### 4.3 Running the Interactive Application
Launch the interactive CLI driver:

```powershell
.\bin\fare_system_app.exe
```

---

## 5. Direct Manual Compilation Commands

You can compile any individual module directly with `g++`:

```powershell
# Task 4 Prediction Program
g++ -std=c++17 -Wall -Wextra -Wpedantic Task04\task04_prediction.cpp -o bin\task04_prediction.exe
.\bin\task04_prediction.exe

# Task 5 Prediction Program
g++ -std=c++17 -Wall -Wextra -Wpedantic Task05\task05_prediction.cpp -o bin\task05_prediction.exe
.\bin\task05_prediction.exe

# Task 6 Completed Fare Logic
g++ -std=c++17 -Wall -Wextra -Wpedantic Task06\task06_fare_control.cpp -o bin\task06_fare_control.exe
.\bin\task06_fare_control.exe

# Task 9 Automated Test Suite
g++ -std=c++17 -Wall -Wextra -Wpedantic tests\test_fare_system.cpp src\fare_system.cpp -o bin\test_fare_system.exe
.\bin\test_fare_system.exe

# Task 10 Scale Simulation (20k, 500k, 3M trips)
g++ -std=c++17 -Wall -Wextra -Wpedantic Task10\task10_scale_simulation.cpp -o bin\task10_scale_simulation.exe
.\bin\task10_scale_simulation.exe

# Task 12 Day-4 Design Challenge
g++ -std=c++17 -Wall -Wextra -Wpedantic Task12\task12_day4_challenge.cpp -o bin\task12_day4_challenge.exe
.\bin\task12_day4_challenge.exe
```

---

## 6. Summary of Task Accomplishments

- **Task 1:** Complete domain modeling, assumptions, UML class diagram, and OOP pillars in transit domain.
- **Task 2:** Classification of tokens, `#define` vs `constexpr` comparison, scoped `enum class`, and centralized constants.
- **Task 3:** Machine `sizeof` measurements, limits, IEEE 754 precision drift, and 32-bit overflow demonstration.
- **Task 4:** Verification of expected prediction output (`3 3.5`, `C 68`, `1 true`, `s >= u`), line-by-line explanation, and safe conversions.
- **Task 5:** Verification of expected prediction output (`11 16`, `10 6 4`, `6 0`, `3 1 -3 -1`, `18`), 24-hour table lookup vs logical expressions, and `i = i++` hazard analysis.
- **Task 6:** Completed TODOs in fare program, matching output:
  ```
  Fare Rs 10.00 balance Rs 90
  Fare Rs 36.00 balance Rs 54
  Denied (low balance) balance Rs 54
  ```
  with explicit tests for exact boundary hours (10:00 AM, 08:00 PM, 08:00 AM, 05:00 PM).
- **Task 7:** Procedural baseline versus OOP `SmartCard` guaranteeing invariant $\text{balance} \ge \text{₹}50.00$.
- **Task 8:** Safe stream I/O with `cin.fail()` recovery, negative recharge rejection, and `<iomanip>` formatted reports.
- **Task 9:** Reusable production fare engine and automated 20-case test suite including 5-journey manual audit match.
- **Task 10:** Scale benchmark for 20k, 500k, and 3M journeys demonstrating `int32_t` overflow and `int64_t` stability.
- **Task 11:** Evidence-based architectural defence and startup configurable table vs `constexpr` trade-offs.
- **Task 12:** Day-4 challenge implementing Airport zone (₹60), 10% weekend discount, exclusivity rules, and `MonthlyPass` class.

---

## 7. Known Assumptions & Invariants

1. **Integer Arithmetic:** All monetary transactions are in integer paise ($1\text{ Rupee } = 100\text{ paise}$).
2. **Minimum Balance Invariant:** Cards must retain $\ge 5000\text{ paise}$ post-journey.
3. **Mutual Exclusivity:** Weekend discount (-10%) and peak surcharge (+20%) are mutually exclusive.
4. **Monthly Pass Entry Policy:** Cards with passes travel free in covered zones, but still require ₹50 security deposit at turnstiles.
