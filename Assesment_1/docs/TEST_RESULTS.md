# Test Execution & Verification Results

**Student:** Markas  
**Register No:** 192421250  
**Environment:** Windows 11, GCC 16.2.0 (MSYS2 UCRT64), C++17  

---

## 1. Summary of Test Harnesses

| Suite Name | Source File | Total Tests | Passed | Failed | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Task 2 Tokens & Constants** | `Task02/task02_tokens.cpp` | 5 checks | 5 | 0 | **ALL PASS** |
| **Task 3 Data Types & Limits**| `Task03/task03_datatypes.cpp` | 4 checks | 4 | 0 | **ALL PASS** |
| **Task 4 Prediction Program** | `Task04/task04_prediction.cpp`| 4 lines | 4 | 0 | **ALL PASS** |
| **Task 4 Safe Conversions**   | `Task04/task04_safe_conversions.cpp` | 3 checks | 3 | 0 | **ALL PASS** |
| **Task 5 Prediction Program** | `Task05/task05_prediction.cpp`| 5 lines | 5 | 0 | **ALL PASS** |
| **Task 5 Operators & Peak**   | `Task05/task05_operators.cpp` | 24 hours | 24 | 0 | **ALL PASS** |
| **Task 6 Completed Fare Logic**| `Task06/task06_fare_control.cpp`| 7 scenarios| 7 | 0 | **ALL PASS** |
| **Task 7 Procedural vs OOP**  | `Task07/procedural_fare.cpp` & `oop_fare.cpp` | 6 checks | 6 | 0 | **ALL PASS** |
| **Task 8 Safe I/O Handling**  | `Task08/task08_safe_io.cpp`   | 5 inputs | 5 | 0 | **ALL PASS** |
| **Task 9 Core Test Suite**    | `tests/test_fare_system.cpp`  | 20 tests | 20 | 0 | **ALL PASS** |
| **Task 10 Scale Simulation**  | `Task10/task10_scale_simulation.cpp` | 3 scale runs | 3 | 0 | **ALL PASS** |
| **Task 12 Day-4 Challenge**   | `Task12/task12_day4_challenge.cpp` | 7 scenarios| 7 | 0 | **ALL PASS** |

---

## 2. Evidence Files Captured

The following raw execution logs have been generated and archived in the `Evidence/` directory:

1. `Evidence/task02_output.txt`: Tokens, constants, macro pitfall analysis.
2. `Evidence/task03_output.txt`: Compiler `sizeof` measurements, IEEE 754 precision drift, 32-bit overflow.
3. `Evidence/task04_prediction_output.txt`: Verifies expected output (`3 3.5`, `C 68`, `1 true`, `s >= u`).
4. `Evidence/task04_safe_output.txt`: Safe conversions and brace initialization narrowing prevention.
5. `Evidence/task05_prediction_output.txt`: Verifies expected output (`11 16`, `10 6 4`, `6 0`, `3 1 -3 -1`, `18`).
6. `Evidence/task05_operators_output.txt`: Logical peak check vs `bool peak[24]` lookup table.
7. `Evidence/task06_output.txt`: Verifies expected output:
   ```
   Fare Rs 10.00 balance Rs 90
   Fare Rs 36.00 balance Rs 54
   Denied (low balance) balance Rs 54
   ```
   and boundary hours (10:00, 20:00, 08:00, 17:00).
8. `Evidence/task07_procedural_output.txt`: Procedural flow and balance corruption vulnerability.
9. `Evidence/task07_oop_output.txt`: OOP SmartCard encapsulation enforcing invariants.
10. `Evidence/task08_output.txt`: Safe I/O recovery and pretty-printed audit report.
11. `Evidence/task09_test_suite_output.txt`: Automated assert-based unit and integration test suite.
12. `Evidence/task10_scale_output.txt`: Scale measurements for 20k, 500k, and 3M journeys.
13. `Evidence/task12_output.txt`: Airport zone, weekend discount, and monthly pass tests.
14. `Evidence/cli_sample_session.txt`: Interactive CLI driver demonstration log.
