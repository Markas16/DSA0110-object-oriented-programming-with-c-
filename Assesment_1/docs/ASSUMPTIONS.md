# Explicit Project Assumptions & Engineering Decisions

**Project Title:** Design, Implement and Validate the Fare Logic of a City Metro Smart-Card System  
**Course:** DSA01 — C++ and Object-Oriented Programming  
**Student:** Markas  
**Register No:** 192421250  
**Compiler:** GCC 16.2.0 (C++17 standard)  

---

## 1. Currency Unit and Precision
- **Internal Representation:** All financial values (balances, base fares, peak surcharges, discounts, and recharge amounts) are stored internally as 64-bit signed integers (`int64_t`) in **paise** ($1\text{ Rupee } = 100\text{ paise}$).
- **Floating-Point Avoidance:** Floating-point data types (`float`, `double`) are strictly prohibited in the core business logic and balance mutation paths to avoid IEEE 754 precision drift and non-associative penny loss. Floating-point conversions occur only at the user I/O formatting boundary (`cout << (paise / 100.0)`).
- **Rounding Rules:** Whenever fractional paise arise during percentage calculations (e.g. 20% surcharge, 10% discount), integer division truncates fractions towards zero ($(\text{fare} \times \text{pct}) / 100$), adhering to municipal fare collection rounding standards.

## 2. Fare Structure & Zonal Tiers
- **Inner Zone:** Base fare = 1000 paise (₹10.00).
- **Middle Zone:** Base fare = 2000 paise (₹20.00).
- **Outer Zone:** Base fare = 3000 paise (₹30.00).
- **Airport Zone (Day-4 / Task 12):** Base fare = 6000 paise (₹60.00).
- **Flat Zonal Exit Model:** Travel fare is computed based on the destination zone reached by the commuter during exit turnstile validation.

## 3. Peak Congestion Intervals
- **Morning Peak Window:** From 08:00 inclusive to 10:00 exclusive ($[08:00, 10:00)$).
- **Evening Peak Window:** From 17:00 inclusive to 20:00 exclusive ($[17:00, 20:00)$).
- **Boundary Hours Behavior:**
  - `08:00` $\to$ PEAK (+20% surcharge applied).
  - `10:00` $\to$ OFF-PEAK (Standard base fare).
  - `17:00` $\to$ PEAK (+20% surcharge applied).
  - `20:00` $\to$ OFF-PEAK (Standard base fare).

## 4. Minimum Retained Solvency Invariant
- **Rule:** A commuter card must maintain a minimum post-journey balance of **5000 paise (₹50.00)**.
- **Formula:** $\text{Current Balance} - \text{Journey Fare} \ge 5000\text{ paise}$.
- If a journey would leave the card with 4999 paise or less, gate turnstiles immediately lock and the transaction is aborted.
- Under zero circumstances will a deduction occur if the invariant check fails.

## 5. Day-4 Precedence & Exclusivity Policy
- **Mutual Exclusivity:** Weekend discounts (-10%) and peak surcharges (+20%) are mutually exclusive. Weekend travel is treated as non-congested off-peak; therefore, weekend trips receive a 10% discount from the base fare and peak surcharges are never levied.
- **Monthly Pass Turnstile Solvency Policy:** Commuters holding a valid Monthly Pass for their destination zone incur ₹0.00 deduction. However, to guarantee anti-passback security and physical card authenticity, the card must still hold at least ₹50.00 in security balance to trigger the turnstile entry sensor.
- **Concessions Policy:** Student (25%) and Senior (30%) concession discounts are clearly marked as configurable parameters, applied strictly after zonal base calculations.
