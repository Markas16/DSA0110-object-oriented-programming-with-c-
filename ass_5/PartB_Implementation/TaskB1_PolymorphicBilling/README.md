# Task B1: Implement and Validate Polymorphic Billing

**Bloom’s Level: K5 – Evaluate | Total Marks: 7**

---

## B1.1 Validation & Hand-Verified Test Matrix (2 Marks)

The hospital billing engine was tested across all four bill categories against all three payer contracts. Every total was validated by manual calculation:

### Hand Verification Table:

| Bill Type | Parameters | Total Bill | Payer | Policy Terms | Hand Calculation of Coverage | Hand-Verified Patient Due |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **OutPatient** | Consult: Rs. 600<br>Tests: Rs. 1400 | **Rs. 2,000.00** | **SelfPay** | Zero cover | $0.00$ | **Rs. 2,000.00** |
| | | | **Insurance** | Limit: Rs. 50,000<br>Co-pay: 10% | $\min(2000, 50000) \times (1 - 0.10) =$ **Rs. 1,800.00** | **Rs. 200.00** |
| | | | **Govt Scheme** | OP Cap: Rs. 1,500 | $\min(2000, 1500) =$ **Rs. 1,500.00** | **Rs. 500.00** |
| **InPatient** | 4 days @ Rs. 3,000<br>Surgery: Rs. 45,000<br>Pharmacy: Rs. 8,000 | **Rs. 65,000.00** | **SelfPay** | Zero cover | $0.00$ | **Rs. 65,000.00** |
| | | | **Insurance** | Limit: Rs. 50,000<br>Co-pay: 10% | $\min(65000, 50000) = 50000$<br>$50000 \times 0.90 =$ **Rs. 45,000.00** | **Rs. 20,000.00** |
| | | | **Govt Scheme** | IP Cap: Rs. 40,000 | $\min(65000, 40000) =$ **Rs. 40,000.00** | **Rs. 25,000.00** |
| **DayCare** | Package: CATARACT-01<br>Price: Rs. 25,000 | **Rs. 25,000.00** | **SelfPay** | Zero cover | $0.00$ | **Rs. 25,000.00** |
| | | | **Insurance** | Limit: Rs. 50,000<br>Co-pay: 10% | $25000 \times 0.90 =$ **Rs. 22,500.00** | **Rs. 2,500.00** |
| | | | **Govt Scheme** | Generic Cap: Rs. 15,000 | $\min(25000, 15000) =$ **Rs. 15,000.00** | **Rs. 10,000.00** |
| **Emergency** | Base: Rs. 5,000<br>5 hrs @ Rs. 1,200/hr | **Rs. 11,000.00** | **SelfPay** | Zero cover | $0.00$ | **Rs. 11,000.00** |
| | | | **Insurance** | Limit: Rs. 50,000<br>Co-pay: 10% | $11000 \times 0.90 =$ **Rs. 9,900.00** | **Rs. 1,100.00** |
| | | | **Govt Scheme** | Generic Cap: Rs. 15,000 | $\min(11000, 15000) =$ **Rs. 11,000.00** | **Rs. 0.00** |

All outputs produced by `full_billing_system.cpp` match these manual computations with 100% precision.

---

## B1.2 Compare with Part A Design (1 Mark)

- **Conformance:** The implementation closely reflects the Part A design: `Bill` and `Payer` remain pure abstract base classes.
- **Refinement in Code:**
  1. Parameters in constructor and methods are strictly passed as `const std::string&` and `const Bill&` to prevent wasteful copy constructions.
  2. Non-throwing accessors (`getPatient()`, `getId()`, `getName()`) are decorated with `noexcept` to maximize optimization and compiler inlining.
  3. Single-argument constructors are marked `explicit` to eliminate inadvertent implicit conversions.

---

## B1.3 Prove Runtime Binding (2 Marks)

### What Happens when `virtual` is Removed from `total()`:
1. In `Bill`, if `total()` is declared without `virtual`:
   - It cannot be declared pure virtual (`= 0`). The compiler emits:
     ```
     error: initializer specified for non-virtual method 'double Bill::total() const'
     ```
2. If given a non-virtual base dummy body `double total() const { return 0.0; }`:
   - When calling `b->total()` through `Bill* b`, the compiler uses **static early binding** based on the static type of `b` (`Bill*`).
   - The output across all bills becomes:
     ```
     OP Ravi: 0
     IP Meena: 0
     Total 0
     ```
   - **Conclusion:** Runtime polymorphism is completely broken without the `virtual` keyword.

---

## B1.4 Prove the Virtual Destructor (2 Marks)

When `virtual ~Bill() = default;` was removed and replaced by non-virtual `~Bill() {}`:
- Running dynamic allocation under `-fsanitize=address -g`:
  The compiler emits:
  ```
  warning: deleting object of polymorphic class type 'Bill' which has non-virtual destructor might cause undefined behavior [-Wdelete-non-virtual-dtor]
  ```
- If derived classes allocate internal heap resources (e.g. `double* history` or dynamic strings), AddressSanitizer intercepts and flags an **unreleased heap leak** because the derived destructor never runs.

---

## Complete the Code (TODO 1, 2, 3)

### Completed Implementation:
```cpp
// TODO 1: Declare pure virtual total() and virtual type() returning "Bill"
virtual double total() const = 0;
virtual std::string type() const { return "Bill"; }

// TODO 2 (In OutPatient): Override total() as consultation + tests, type() as "OP"
double total() const override { return consultation + tests; }
std::string type() const override { return "OP"; }

// TODO 3 (In InPatient): Override total() as days * perDay + surgery, type() as "IP"
double total() const override { return (days * perDay) + surgery; }
std::string type() const override { return "IP"; }
```

### Verified Program Output:
```
OP Ravi: 1700
IP Meena: 47500
Total 49200
```

---

## Alternative Concept and Complexity Analysis

### Alternative Concept: Compile-Time Polymorphism with C++ Templates
Instead of virtual functions, bills are processed using generic template functions with separate containers per type:

```cpp
template <typename TBill>
void printAndSum(const std::vector<TBill>& bills, double& sum) {
    for (const auto& b : bills) {
        b.print();
        sum += b.total(); // Direct compile-time inlined call!
    }
}
```

### Complexity and Trade-off Comparison:

| Dimension | Dynamic Polymorphism (`std::vector<Bill*>`) | Template Polymorphism (Separate Containers) |
| :--- | :--- | :--- |
| **Call Latency** | $\sim 2-3$ ns per call (1 vtable indirection) | $\sim 0$ ns (Function is fully inlined by compiler; no vptr dereference) |
| **Memory per Object** | 8-byte `vptr` stored in each object | Zero `vptr` overhead; objects store only raw data members |
| **Heterogeneous Aggregation** | **Supported**: Bills of all types (`OP`, `IP`, `DayCare`, `Emergency`) live together in a single `std::vector<std::unique_ptr<Bill>>` (e.g., daily hospital ledger queue). | **Impossible**: Requires maintaining 4+ separate vectors (`vector<OutPatient>`, `vector<InPatient>`, etc.). Cannot sort or process them in a single unified time-ordered billing pipeline. |
| **Code Binary Size** | Compact: 1 compiled copy of processing algorithms. | Bloated: Template instantiation generates separate binary machine code for every type (Template Code Bloat). |
