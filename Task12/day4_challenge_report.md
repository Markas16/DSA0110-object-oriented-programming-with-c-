# Task 12: Day-4 Design Challenge Report

**Student:** Markas  
**Register No:** 192421250  
**Course:** DSA01 — C++ and Object-Oriented Programming  

---

## Part A: Impact Analysis (2 Marks)

The addition of an **Airport Zone (₹60 base)**, a **10% Weekend Discount**, **Mutual Exclusivity Rules**, and a **2-Zone Monthly Pass** impacts several layers of the architecture:

1. **Enumerations & Constants**:
   - `enum class Zone`: Extended with `Airport = 3`.
   - Constants Added: `BASE_FARE_AIRPORT = 6000LL` (paise), `WEEKEND_DISCOUNT_PERCENT = 10`.
2. **Expressions & Precedence Logic**:
   - The pricing pipeline requires conditional branches to enforce **mutual exclusivity**: weekend travel disables the peak-hour surcharge calculation and applies the 10% discount.
3. **Control Structures (Switches)**:
   - `calculateFare()` switches must add `case Zone::Airport: base = 6000; break;`.
   - String parser maps `"Airport"`, `"airport"`, and `"4"` to `Zone::Airport`.
4. **Class Models & Interfaces**:
   - New Class: `MonthlyPass` encapsulating two eligible `Zone` tokens and expiry checks.
   - `SmartCard` Interface: Extended to allow attaching/querying a `MonthlyPass`.
   - Solvency Policy Decision: Verification of whether ₹50 minimum balance check applies when fare is 0 paise.

---

## Part B: Evaluate the Original Design (2 Marks)

### Strengths of Original Design:
- **Integer Paise Arithmetic**: The original foundation using integer paise seamlessly accommodated the 10% weekend discount ($6000 - 600 = 5400\text{ paise}$) with zero floating-point drift.
- **Encapsulated `canTravel()` Check**: Gate turnstile checks prevented negative balance vulnerabilities.

### Limitations & Pain Points in Original Design:
1. **Hardcoded Switch Statements**: Adding the Airport zone required modifying every switch statement across procedural functions.
2. **Lack of Temporal Awareness (Day of Week)**: Original functions only accepted `hour` ($0 \dots 23$); day-of-week context (`isWeekend`) was absent from the signature.
3. **Rigid Tariff Model**: No abstraction existed for subscription passes or concessionary tariffs. If attempted in the original design, `calculateFare()` would require messy nested ternary logic.

---

## Part C: Adapt or Redesign (2 Marks)

The redesign introduced two architectural enhancements:
1. **`MonthlyPass` Component**:
   ```cpp
   class MonthlyPass {
       bool active_;
       Zone zone1_;
       Zone zone2_;
   public:
       bool coversZone(Zone z) const noexcept;
   };
   ```
2. **Centralized Hierarchical Fare Policy**:
   A deterministic pricing pipeline enforcing clear precedence:
   - **Step 1 (Pass Check)**: If commuter holds an active `MonthlyPass` covering the destination zone, $\text{Fare} = 0\text{ paise}$.
   - **Step 2 (Weekend Exclusivity)**: If `isWeekend == true`, apply $-10\%$ on base fare. Peak surcharges are strictly skipped.
   - **Step 3 (Weekday Peak Check)**: If `isWeekend == false` and `isPeakHour(hour) == true`, apply $+20\%$ surcharge on base fare.
   - **Step 4 (Concessions)**: Configurable Concession rate applied to the calculated fare.

---

## Part D: Evidence-Based Defence & Empirical Results (2 Marks)

### 1. Test Results Summary

| Scenario Tested | Input Parameters | Expected Result | Actual Result | Verification |
| :--- | :--- | :--- | :--- | :--- |
| **Airport Base Fare** | Zone: Airport, Hour: 12, Weekday | 6000 paise (₹60.00) | 6000 paise (₹60.00) | **PASS** |
| **Airport Peak (+20%)** | Zone: Airport, Hour: 08, Weekday | 7200 paise (₹72.00) | 7200 paise (₹72.00) | **PASS** |
| **Airport Weekend (-10%)** | Zone: Airport, Hour: 12, Weekend | 5400 paise (₹54.00) | 5400 paise (₹54.00) | **PASS** |
| **Weekend Peak Exclusivity** | Zone: Airport, Hour: 08, Weekend | 5400 paise (No surcharge) | 5400 paise | **PASS** |
| **Monthly Pass Covered Trip** | Pass: [Inner, Airport], Trip: Airport | 0 paise (₹0.00 deducted) | 0 paise deducted | **PASS** |
| **Monthly Pass Non-Covered** | Pass: [Inner, Airport], Trip: Middle | 2000 paise deducted | 2000 paise deducted | **PASS** |
| **Pass with Low Balance** | Card Balance: ₹30.00, Trip: Airport | DENIED (Security Invariant) | DENIED (Balance < ₹50) | **PASS** |

### 2. Documented Minimum Balance Policy for Monthly Pass Holders:
**Policy Statement:** *“To prevent fraudulent tailgating and maintain physical smart-card solvency, every commuter must maintain a minimum security deposit of ₹50.00 (5000 paise) on their card. Even when an active Monthly Pass waives 100% of the travel fare (₹0.00 deduction), turnstiles will lock if card balance drops below ₹50.00.”*
