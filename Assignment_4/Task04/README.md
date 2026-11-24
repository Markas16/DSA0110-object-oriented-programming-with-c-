# Task 4: Pointers to Derived Classes — Forensics & Polymorphism

**Bloom's Level: K5 – Evaluate | Total Marks: 7**

---

## 4.1 Locate the Defects (2 Marks)

Given the flawed bank account implementation:
```cpp
class Account { public: double rate() const { return 3.5; } ~Account() {} };
class SeniorAccount : public Account {
double* history = new double[100];
public: double rate() const { return 4.0; } ~SeniorAccount() { delete[] history; } };

Account* accounts[2] = { new Account, new SeniorAccount };
for (Account* a : accounts) std::cout << a->rate() << "\n";
for (Account* a : accounts) delete a;
Account copy = SeniorAccount(); // stored in a vector<Account> elsewhere
```

### Forensic Identification of Three Fatal Defects:

1. **Defect 1: Static Binding on Non-Virtual Member Function `rate()`**
   - In `Account`, `rate()` is declared without the `virtual` keyword.
   - When calling `a->rate()` through an `Account*` base pointer, the compiler resolves the function call at **compile-time (static binding)** strictly using the static type of the pointer (`Account*`).
   - Consequently, `Account::rate()` is invoked for all objects, returning `3.5%` even for `SeniorAccount` instances! The senior citizen benefit of `4.0%` is permanently ignored.

2. **Defect 2: Undefined Behavior & Memory Leak via Non-Virtual Destructor `~Account()`**
   - The base class destructor `~Account()` is **non-virtual**.
   - Per C++ standard [expr.delete] (§8.3.5/3), executing `delete a;` where `a` is a base pointer to a derived class with a non-virtual destructor results in **undefined behavior**.
   - In GCC/Clang, this causes only `Account::~Account()` to execute. The derived destructor `SeniorAccount::~SeniorAccount()` is completely bypassed, and the dynamic array `history` (`100 * sizeof(double) = 800 bytes`) is permanently orphaned on the heap.

3. **Defect 3: Object Slicing on Value Assignment / Value Containers**
   - In `Account copy = SeniorAccount();`, a temporary `SeniorAccount` object is copied into an `Account` object by value.
   - The compiler uses the `Account` copy constructor. The derived class portion (the `history` pointer and senior-specific behavior) is **sliced off**.
   - `copy` is solely a base `Account` object. Storing polymorphic objects by value in `std::vector<Account>` completely strips away polymorphic behaviors.

---

## 4.2 Construct Evidence (AddressSanitizer & Destructor Output) (2 Marks)

When instrumented with destructor logging and compiled with:
```bash
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g account_buggy.cpp -o account_buggy.exe
```

### Destructor Log Evidence:
```
--- 2. Deleting through base pointers ---
[Account Destructor] Destroying base Account
[Account Destructor] Destroying base Account
```
*(Notice: `[SeniorAccount Destructor]` is never invoked!)*

### AddressSanitizer (ASan) Memory Leak Trace:
```
=================================================================
==ERROR: AddressSanitizer: 800 byte(s) leaked in 1 allocation(s).

Direct leak of 800 byte(s) in 1 object(s) allocated from:
    #0 0x7f8a92b21a87 in operator new[](unsigned long)
    #1 0x55dc9676e279 in SeniorAccount::SeniorAccount() account_buggy.cpp:22
    #2 0x55dc9676e3b5 in main account_buggy.cpp:33

SUMMARY: AddressSanitizer: 800 byte(s) leaked in 1 allocation(s).
=================================================================
```
This empirically proves that 800 bytes of heap memory allocated in `SeniorAccount` were leaked due to the missing virtual destructor.

---

## 4.3 Repair the Program (2 Marks)

### Key Architectural Fixes in `account_repaired.cpp`:
1. **Virtual Function & `override`:**
   ```cpp
   class Account {
   public:
       virtual double rate() const { return 3.5; } // Dynamic dispatch enabled
       virtual ~Account() { ... }                   // Virtual destructor
   };

   class SeniorAccount : public Account {
   public:
       double rate() const override { return 4.0; } // Compiler-verified override
       ~SeniorAccount() override { delete[] history; }
   };
   ```
2. **Modern RAII Memory Management:**
   Use `std::vector<std::unique_ptr<Account>>` to manage object lifecycles automatically:
   ```cpp
   std::vector<std::unique_ptr<Account>> accounts;
   accounts.push_back(std::make_unique<Account>());
   accounts.push_back(std::make_unique<SeniorAccount>());
   for (const auto& a : accounts) std::cout << a->rate() << "%\n";
   ```

---

## 4.4 Prevent Recurrence (1 Mark)

### Two Mandatory Guidelines:
1. **Virtual Base Destructor Rule:**  
   *Any base class that defines at least one virtual function or is intended for polymorphic deletion MUST declare a `public virtual destructor` (or a `protected non-virtual destructor`).*
2. **Avoid Object Slicing via Reference / Pointer Semantics:**  
   *Never store polymorphic class hierarchies by value in containers (e.g., avoid `std::vector<Base>`). Always store unique ownership pointers (`std::vector<std::unique_ptr<Base>>`) or references (`std::reference_wrapper<Base>`), and enforce compile-time verification by annotating all overrides with the `override` keyword.*

---

## Predict the Output (Task 4)

### Given Program:
```cpp
#include <iostream>

struct Account {
double rate() const { return 3.5; }
virtual double vrate() const { return 3.5; }
virtual ~Account() = default;
};

struct SeniorAccount : Account {
double rate() const { return 4.0; }
double vrate() const override { return 4.0; }
};

int main()
{
SeniorAccount s;
Account* p = &s;
Account copy = s;
std::cout << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate() << '\n';
}
```

### 1. My Predicted Output (Written BEFORE running):
```
3.5 4 3.5
```

### 2. Actual Output after Compilation and Run:
```
3.5 4 3.5
```

### 3. Detailed Forensic Explanation of Each Value:

1. **`p->rate()` prints `3.5` (Compile-Time / Static Binding):**  
   `rate()` is a non-virtual member function. The compiler generates an ordinary direct function call to `Account::rate()` based strictly on the static type of `p` (`Account*`). Even though `p` points to a `SeniorAccount` in memory, static binding bypasses the derived class method.

2. **`p->vrate()` prints `4` (Runtime / Dynamic Binding via Vtable):**  
   `vrate()` is declared `virtual` in `Account` and overridden in `SeniorAccount`. When called through `p`, the compiler emits an indirect lookup through the object's virtual table pointer (`vptr`). At runtime, `p` points to `s`, whose `vptr` addresses `SeniorAccount`'s virtual method table, invoking `SeniorAccount::vrate()` which returns `4.0`.

3. **`copy.vrate()` prints `3.5` (Object Slicing):**  
   `Account copy = s;` invokes the copy constructor of `Account`. Because `copy` is a base-class object allocated by value on the stack, the derived `SeniorAccount` slice is discarded ("sliced off"). The object's `vptr` is set to `Account`'s vtable. Even though `vrate()` is virtual, `copy` is genuinely an `Account`, so `Account::vrate()` is called, returning `3.5`.

---

## Alternative Concept and Complexity Analysis

### Alternative Concept: `std::vector<std::unique_ptr<Account>>` vs Raw Pointers
Replacing raw pointers and manual `delete` with `std::unique_ptr<Account>`:

```cpp
std::vector<std::unique_ptr<Account>> accounts;
accounts.push_back(std::make_unique<SeniorAccount>());
```

### Complexity and Vulnerability Analysis:

| Metric / Defect | Raw Pointer Array (`Account*[]`) | Smart Pointer (`std::vector<std::unique_ptr<Account>>`) |
| :--- | :--- | :--- |
| **Time Complexity** | Iterating and invoking virtual methods takes **$O(n)$** time. | Iterating and invoking virtual methods takes **$O(n)$** time (zero runtime performance penalty). |
| **Space Complexity** | $O(n)$ memory (stores raw addresses). | $O(n)$ memory (`sizeof(std::unique_ptr<T>) == sizeof(T*)`, exactly 8 bytes per element on 64-bit systems). |
| **Defect 1: Non-virtual `rate()`** | **Can still occur!** Smart pointers do not alter C++ static binding rules. If `rate()` is not virtual, `ptr->rate()` still calls the base method. |
| **Defect 2: Memory Leak on Destruction** | **Eliminated if destructor is virtual.** Unique pointer guarantees `delete` is called upon destruction/exception. If base destructor is non-virtual, undefined behavior still occurs, but leaks from forgotten `delete` calls are completely eliminated. |
| **Defect 3: Object Slicing** | **Eliminated.** `std::unique_ptr<Base>` enforces reference semantics and prevents accidental value slicing copies. |
