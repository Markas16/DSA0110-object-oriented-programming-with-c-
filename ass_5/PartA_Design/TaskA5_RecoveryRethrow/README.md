# Task A5: Design Recovery and Rethrowing

**Bloom’s Level: K5 – Evaluate | Total Marks: 4**

---

## A5.1 Multi-Tiered Catch, Log & Rethrow Architecture (2 Marks)

A reliable medical billing architecture enforces a clean separation between **error detection**, **audit logging**, and **user notification**:

```
[Layer 1: Validation / Core Engine]
       | Throws specific domain exception (e.g., ClaimLimitExceededException)
       v
[Layer 2: Service / Audit Intermediary]
       | Catches domain error -> Writes entry to persistent audit log
       | Executes 'throw;' (Rethrows exact same polymorphic exception)
       v
[Layer 3: Top-Level Workflow / UI Controller]
       | Final catch block handles recovery, displays user message, rolls back state
```

### Strategic Placement:
1. **Where to Detect & Throw:**  
   In the leaf computation or constructor where the invariant is broken (e.g. `InPatientBill::InPatientBill` throws `NegativeChargeException`).
2. **Where to Catch, Audit & Rethrow (`throw;`):**  
   In intermediate business layers (e.g., `settleClaim()`). It catches `const ClaimError& e`, records the forensic breach to the compliance audit log, and executes **`throw;`** to re-propagate the active exception object without slicing.
3. **Where to Finally Catch & Recover:**  
   In the outer transactional loop / UI layer (`main()` or HTTP handler). It prevents program termination, notifies the user with a sanitized message, and continues processing the next patient in the queue.

---

## A5.2 Stack Unwinding and RAII Resource Safety (2 Marks)

### What is Stack Unwinding?
When an exception is thrown, normal sequential execution halts. The C++ runtime library searches up the execution call stack to find an enclosing `try/catch` block that can handle the exception type. As the runtime exits each stack frame, it performs **stack unwinding**:
- It destroys all fully-constructed local (automatic) objects within each exiting scope in the **exact reverse order of their declaration**.
- The destructors for those local objects execute automatically.

### The Role of RAII (Resource Acquisition Is Initialization):
- If heap memory is wrapped inside **RAII containers** (such as `std::unique_ptr<Bill>`, `std::vector<double>`, or `std::ofstream`), their destructors are invoked during stack unwinding.
- Even if an exception aborts a function halfway through, all dynamically allocated heap memory, locks, and file descriptors are reclaimed. **Zero memory leaks occur.**

---

## Predict the Output (Task A5)

### Given Program:
```cpp
#include <iostream>
#include <string>

struct Guard {
std::string n;
explicit Guard(const std::string& s) : n(s) {}
~Guard() { std::cout << '~' << n << ' '; }
};

void f()
{
Guard g("f");
throw 42;
}

int main()
{
try {
Guard m("main");
f();
std::cout << "not printed ";
} catch (int x) {
std::cout << "caught " << x;
} catch (...) {
std::cout << "other";
}
std::cout << '\n';
}
```

### 1. My Predicted Output (Written BEFORE running):
```
~f ~main caught 42
```

### 2. Actual Output after Compilation & Execution:
```
~f ~main caught 42
```

### 3. Detailed Forensic Explanation:
1. In `main()`, inside the `try` block, `Guard m("main")` is constructed on the stack.
2. Function `f()` is called.
3. Inside `f()`, `Guard g("f")` is constructed on the stack.
4. `f()` executes `throw 42;`.
5. The runtime initiates stack unwinding:
   - First, the scope of `f()` is exited. `g`'s destructor executes, printing `~f `.
   - Next, the runtime unwinds out of `main`'s `try` block. `m`'s destructor executes, printing `~main `.
6. Control jumps immediately to the matching handler `catch (int x)`.
7. Because the unwinding bypasses all statements following `f()`, `std::cout << "not printed ";` **never executes**.
8. The catch block executes, printing `caught 42`.

---

## Alternative Concept and Complexity Analysis

### Alternative Concept: Manual `delete` inside Catch Blocks
Instead of RAII, raw pointers are allocated and manual `delete` statements are placed inside each `catch` block:

```cpp
void processRaw(double amount) {
    Bill* b = new InPatientBill("Ravi", 3, 2500, 40000);
    try {
        validate(amount); // Might throw!
        delete b;         // Normal cleanup
    } catch (...) {
        delete b;         // Manual cleanup on error
        throw;
    }
}
```

### Comparative Analysis:

| Metric | RAII Objects (`std::unique_ptr<Bill>`) | Manual `delete` in Every `catch` |
| :--- | :--- | :--- |
| **Execution Time** | Same ($\mathcal{O}(1)$ inline destructor execution) | Same ($\mathcal{O}(1)$ `delete` execution) |
| **Code Bloat & Cognitive Load** | Minimal (Zero catch boilerplate; destructor runs automatically) | High (Every single `try-catch` block must repeat cleanup logic for every active resource) |
| **Vulnerability to Memory Leaks** | **Zero Risk**: Guaranteed by C++ language standard during stack unwinding. | **Catastrophic Risk**: If a developer adds a new `throw` or early `return` before `delete`, or forgets to catch a specific exception, or an exception is thrown inside `delete`, heap memory is leaked permanently. |
| **Number of Points of Failure** | 0 places | $N \times M$ places (Number of resources $\times$ number of error paths) |
