# Task A2: Understand Runtime Binding

**Bloom’s Level: K5 – Evaluate | Total Marks: 5**

---

## A2.1 Early and Late Binding (2 Marks)

When calling a member function through a pointer to a base class (`Bill* b = new OPBill;`):

1. **Early Binding (Static / Compile-Time Binding):**
   - Occurs when the function is **non-virtual** (or called directly on a value object).
   - The compiler examines strictly the *static type* of the pointer (`Bill*`).
   - The memory address of the target function is hardcoded directly into the assembly instruction at compile/link time: `call Bill::someFunction`.
   - Cannot adapt if the underlying object at runtime is actually a derived `OPBill`.

2. **Late Binding (Dynamic / Run-Time Binding):**
   - Occurs when the function is declared **`virtual`** and called through a pointer (`Bill*`) or reference (`Bill&`).
   - The compiler cannot predict the exact derived type of the object at compile time.
   - The compiler generates code that queries the object's hidden virtual table pointer (`vptr`) at runtime to look up the appropriate function pointer in the class's `vtable`, emitting an indirect jump: `call [rax + 8]`.
   - Enables polymorphism: the correct derived implementation (`OPBill::total()`) executes automatically.

---

## A2.2 The Virtual Table (Vtable) Architecture (2 Marks)

Every class with at least one virtual function receives a compiler-generated **virtual method table (`vtable`)** stored in the read-only static memory segment (`.rodata`). Every instantiated object includes an implicit **virtual table pointer (`vptr`)** as its first 8 bytes.

```
       Bill* b (Address: 0x1000)
       +-----------------------+
0x1000 | vptr (0x8000) --------+-------> OPBill Vtable (Address: 0x8000)
       | patientName           |        +-----------------------------------+
       | consultationFee       | slot 0 | &OPBill::~OPBill()                |
       | diagnosticTestsFee    | slot 1 | &OPBill::total()                  |
       +-----------------------+        +-----------------------------------+
```

### Tracing the Execution of `b->total()`:
1. The CPU reads pointer `b` containing address `0x1000`.
2. It dereferences offset `0` of the object to obtain `vptr` (`0x8000`).
3. It indexes slot 1 in the `vtable` (`0x8000 + 8 bytes`) to find the function pointer `&OPBill::total()`.
4. It executes an indirect call to `&OPBill::total()` passing `0x1000` as the implicit `this` pointer in register `RDI`/`RCX`.
5. Returns `500.0`.
- **Time Complexity:** Exactly 1 pointer dereference + 1 indexed array access ($\mathcal{O}(1)$ constant time).

*(See `vtable_diagram.txt` for comprehensive byte layout).*

---

## A2.3 Virtual Destructors (1 Mark)

### Why `Bill` Must Have a Virtual Destructor:
When dynamically allocated objects are managed through base pointers (`Bill* b = new OPBill; delete b;`), deleting `b` invokes the destructor of `Bill`.
- If `~Bill()` is **non-virtual**, static binding applies: only `Bill::~Bill()` executes. The derived destructor `OPBill::~OPBill()` is completely bypassed, causing undefined behavior per the C++ standard and leaking all resources held by `OPBill` (e.g., dynamic buffers, open files).
- If `~Bill()` is **`virtual`**, the runtime looks up slot 0 in the vtable, invoking `OPBill::~OPBill()`, which automatically chains to `Bill::~Bill()`.

---

## Predict the Output (Task A2)

### Given Program:
```cpp
#include <iostream>

struct Bill {
virtual ~Bill() { std::cout << "~Bill "; }
virtual double total() const = 0;
void show() const { std::cout << total() << ' '; }
};

struct OPBill : Bill {
~OPBill() override { std::cout << "~OPBill "; }
double total() const override { return 500; }
};

int main()
{
Bill* b = new OPBill;
b->show();
delete b;
std::cout << '\n';
}
```

### 1. My Predicted Output (Written BEFORE running):
```
500 ~OPBill ~Bill 
```

### 2. Actual Output after Compilation & Run:
```
500 ~OPBill ~Bill 
```

### 3. Forensic Explanation:
- **How `show()`, a non-virtual function, reached `OPBill::total()`:**  
  `show()` is defined in `Bill`. Inside its body, the expression `total()` is implicitly `this->total()`. The parameter `this` is a pointer (`const Bill* const this`). Because `total()` is declared **`virtual`**, calling it through `this` triggers **runtime dynamic dispatch**. Since `this` points to an instance of `OPBill`, the vtable lookup resolves to `OPBill::total()`, printing `500`.
- **Removing `virtual` from `~Bill`:**  
  - **New Output:** `500 ~Bill `
  - **Reason:** With a non-virtual destructor, `delete b;` statically binds to `Bill::~Bill()`. `~OPBill` is bypassed.
  - **AddressSanitizer Warning:** ASan or `-Wall` warns: `warning: deleting object of polymorphic class type 'Bill' which has non-virtual destructor might cause undefined behavior [-Wdelete-non-virtual-dtor]`.

---

## Alternative Concept and Complexity Analysis

### Alternative Concept: `dynamic_cast` Type Testing Ladder
Instead of virtual function calls, `total()` is made non-virtual, and callers inspect the runtime type of each bill using a chain of `dynamic_cast` calls:

```cpp
double computeTotal(Bill* b) {
    if (auto op = dynamic_cast<OPBill*>(b)) return op->total();
    if (auto ip = dynamic_cast<InPatientBill*>(b)) return ip->total();
    if (auto dc = dynamic_cast<DayCareSurgeryBill*>(b)) return dc->total();
    if (auto em = dynamic_cast<EmergencyBill*>(b)) return em->total();
    return 0.0;
}
```

### Complexity and Trade-off Comparison:

| Evaluation Metric | Virtual Function Dispatch (`b->total()`) | Type Testing Ladder (`dynamic_cast`) |
| :--- | :--- | :--- |
| **Time Complexity per Call** | **$\mathcal{O}(1)$ Constant Time** (1 array lookup in vtable, ~2–3 nanoseconds) | **$\mathcal{O}(k)$ Linear in Number of Types** ($k$ string comparisons / type descriptor traversals down the RTTI graph; worst-case latency scales with number of classes) |
| **Memory Overhead** | 8 bytes per object (`vptr`) | 8 bytes per object (`vptr` is **still required** by the compiler to support RTTI for `dynamic_cast`) |
| **Maintainability (OCP)** | Zero modification to existing code when adding a new bill type. | Must edit every `dynamic_cast` ladder in every dispatch function in the program. |
