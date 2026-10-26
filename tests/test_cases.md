# Comprehensive Test Specification & Verification Matrix

This document defines the formal test suite for the City Metro Smart-Card Fare Logic System, implemented for DSA01 Assignment 1 (Markas, Register No: 192421250).

## 1. Test Execution Matrix

| Test ID | Test Category | Input Vector (Zone, Hour, Day, Balance) | Expected Result | Actual Result | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **TC-01** | Base Fare | Zone: `Inner`, Hour: `12`, Day: `Weekday`, Bal: `10000` | Fare: 1000 paise (Rs 10.00), Status: Approved, Post-Bal: 9000 | Fare: 1000, Approved, Post-Bal: 9000 | **PASS** |
| **TC-02** | Peak Surcharge | Zone: `Inner`, Hour: `09`, Day: `Weekday`, Bal: `10000` | Fare: 1200 paise (Rs 12.00), Status: Approved, Post-Bal: 8800 | Fare: 1200, Approved, Post-Bal: 8800 | **PASS** |
| **TC-03** | Base Fare | Zone: `Middle`, Hour: `14`, Day: `Weekday`, Bal: `10000` | Fare: 2000 paise (Rs 20.00), Status: Approved, Post-Bal: 8000 | Fare: 2000, Approved, Post-Bal: 8000 | **PASS** |
| **TC-04** | Peak Surcharge | Zone: `Middle`, Hour: `18`, Day: `Weekday`, Bal: `10000` | Fare: 2400 paise (Rs 24.00), Status: Approved, Post-Bal: 7600 | Fare: 2400, Approved, Post-Bal: 7600 | **PASS** |
| **TC-05** | Base Fare | Zone: `Outer`, Hour: `22`, Day: `Weekday`, Bal: `10000` | Fare: 3000 paise (Rs 30.00), Status: Approved, Post-Bal: 7000 | Fare: 3000, Approved, Post-Bal: 7000 | **PASS** |
| **TC-06** | Peak Surcharge | Zone: `Outer`, Hour: `08`, Day: `Weekday`, Bal: `10000` | Fare: 3600 paise (Rs 36.00), Status: Approved, Post-Bal: 6400 | Fare: 3600, Approved, Post-Bal: 6400 | **PASS** |
| **TC-07** | Boundary Hour | Zone: `Outer`, Hour: `08:00` (Morning Peak Start) | Fare: 3600 paise (+20% surcharge applied) | Fare: 3600 paise | **PASS** |
| **TC-08** | Boundary Hour | Zone: `Outer`, Hour: `10:00` (Morning Peak End/Exclusive) | Fare: 3000 paise (Base fare, off-peak) | Fare: 3000 paise | **PASS** |
| **TC-09** | Boundary Hour | Zone: `Outer`, Hour: `17:00` (Evening Peak Start) | Fare: 3600 paise (+20% surcharge applied) | Fare: 3600 paise | **PASS** |
| **TC-10** | Boundary Hour | Zone: `Outer`, Hour: `20:00` (Evening Peak End/Exclusive) | Fare: 3000 paise (Base fare, off-peak) | Fare: 3000 paise | **PASS** |
| **TC-11** | Solvency Bound | Zone: `Inner`, Hour: `12`, Initial Balance: `6000` paise | Fare: 1000 paise, Post-Bal: 5000 paise (EXACTLY Rs 50.00) -> Approved | Approved, Post-Bal: 5000 | **PASS** |
| **TC-12** | Solvency Breach | Zone: `Inner`, Hour: `12`, Initial Balance: `5000` paise | Trip Denied (Balance would drop to 4000 < 5000) | Denied, Balance: 5000 | **PASS** |
| **TC-13** | Solvency Sub-cent| Zone: `Inner`, Hour: `12`, Initial Balance: `5999` paise | Trip Denied (Post-balance would be 4999 paise = Rs 49.99) | Denied, Balance: 5999 | **PASS** |
| **TC-14** | Invalid Zone | Zone: `Unknown`, Hour: `12` | Error return code `-1`, trip rejected | Return code: -1 | **PASS** |
| **TC-15** | Invalid Hour High| Zone: `Inner`, Hour: `24` | Error return code `-1`, rejected | Return code: -1 | **PASS** |
| **TC-16** | Invalid Hour Low | Zone: `Inner`, Hour: `-1` | Error return code `-1`, rejected | Return code: -1 | **PASS** |
| **TC-17** | Negative Recharge| Recharge: `-500` paise | Rejected: False, Balance unchanged | False, Balance unchanged | **PASS** |
| **TC-18** | Zero Recharge | Recharge: `0` paise | Rejected: False, Balance unchanged | False, Balance unchanged | **PASS** |
| **TC-19** | Valid Recharge | Recharge: `+2500` paise, Starting Bal: `10000` | Accepted: True, Balance becomes 12500 paise | True, Balance: 12500 | **PASS** |
| **TC-20** | 5-Trip Audit | Initial: 15000 paise. 4 valid trips + 1 denied trip | Sum deducted: 9800 paise, Final Balance: 5200 paise | Sum: 9800, Final: 5200 | **PASS** |

## 2. 5-Journey Manual Audit Breakdown

```
Card Initial Balance = 15,000 paise (Rs 150.00)
-------------------------------------------------------------------------------------------------
Journey 1: Zone Middle, Hour 14:00 (Off-Peak) -> Base 2000 paise.
           New Balance = 15,000 - 2,000 = 13,000 paise (Rs 130.00). Approved.
Journey 2: Zone Outer,  Hour 08:30 (Peak)     -> 3000 + 20% = 3600 paise.
           New Balance = 13,000 - 3,600 = 9,400 paise (Rs 94.00). Approved.
Journey 3: Zone Inner,  Hour 18:00 (Peak)     -> 1000 + 20% = 1200 paise.
           New Balance = 9,400 - 1,200 = 8,200 paise (Rs 82.00). Approved.
Journey 4: Zone Outer,  Hour 21:00 (Off-Peak) -> Base 3000 paise.
           New Balance = 8,200 - 3,000 = 5,200 paise (Rs 52.00). Approved.
Journey 5: Zone Middle, Hour 11:00 (Off-Peak) -> Base 2000 paise.
           Check: 5,200 - 2,000 = 3,200 paise < 5,000 minimum required balance.
           Result: DENIED. Balance remains 5,200 paise (Rs 52.00).
-------------------------------------------------------------------------------------------------
Total Fares Deducted: 2000 + 3600 + 1200 + 3000 = 9800 paise (Rs 98.00).
Final Retained Balance: 5200 paise (Rs 52.00).
System Verification: Matches automated assert results with 100% precision.
```
