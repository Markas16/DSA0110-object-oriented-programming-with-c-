# Task 5: Pointers to Objects and the `this` Pointer

**Bloom's Level: K5 – Evaluate | Total Marks: 7**

---

## 5.1 Four Ways to Reach an Object (3 Marks)

In C++, an object can be accessed and manipulated through four distinct paradigms:

1. **(a) Directly by Name (Stack Allocation):**
   ```cpp
   Cart c;
   c.add("Rice", 60.0);
   ```
   Direct identifier access. The compiler binds operations directly to the stack frame memory.
2. **(b) Through a Pointer using the Arrow Operator (`->`):**
   ```cpp
   Cart* p = &c;
   p->add("Oil", 180.0);
   ```
   Indirection through memory address. `p->method()` is syntactic sugar for `(*p).method()`.
3. **(c) Through a Reference (Alias):**
   ```cpp
   Cart& ref = c;
   ref.add("Dal", 120.0);
   ```
   An alternative name (alias) for the existing object. Non-null by contract; uses dot syntax `.`.
4. **(d) Through a Pointer Created with `new` and Released with `delete` (Dynamic Allocation):**
   ```cpp
   Cart* heapCart = new Cart();
   heapCart->add("Sugar", 45.0);
   delete heapCart; // Explicit memory deallocation
   ```
   Allocated from the heap (free store) with dynamic lifetime extending beyond function scope.

### The Role of `this` in Method Chaining & Disambiguation:
- **Method Chaining (`return *this;`):**  
  Returning a reference to the invoking instance (`Cart&`) allows fluent cascaded calls:
  ```cpp
  c.add("Rice", 60).add("Dal", 120).setDiscount(10);
  ```
- **Name Disambiguation (`this->discount = discount;`):**  
  When a member variable and a constructor/method parameter share identical names, prefixing with `this->` explicitly tells the compiler that the left operand belongs to the object instance.

---

## 5.2 Array of Pointers to Carts & Maximum Search (2 Marks)

The grocery application maintains an array of pointers to `Cart` objects for active customer carts:
```cpp
const int NUM_CUSTOMERS = 4;
Cart* activeCarts[NUM_CUSTOMERS];
```

### Algorithm to Find the Cart with the Highest Total using Pointer Arithmetic:
```cpp
Cart** highestCartPtr = activeCarts; // Pointer to array element (Cart*)
double maxTotal = (*highestCartPtr)->total();

for (Cart** it = activeCarts + 1; it < activeCarts + NUM_CUSTOMERS; ++it) {
    double currentTotal = (*it)->total();
    if (currentTotal > maxTotal) {
        maxTotal = currentTotal;
        highestCartPtr = it;
    }
}
// (*highestCartPtr) now points to the highest total Cart
```
Time Complexity: $\Theta(n)$ time to scan $n$ pointers.

---

## 5.3 Comparative Evaluation of the Four Ways (2 Marks)

| Dimension | (a) Directly by Name | (b) Pointer with `->` | (c) Reference (`&`) | (d) Heap Pointer (`new`/`delete`) |
| :--- | :--- | :--- | :--- | :--- |
| **Safety** | **Highest:** Automatically scoped, cannot be null or dangling. | **Low:** Can be `nullptr`, uninitialized, or dangling. | **High:** Cannot be null, must be bound on definition. | **Lowest:** Vulnerable to memory leaks, double free, and use-after-free. |
| **Lifetime** | Block / scope lifetime (automatic deallocation on stack unwind). | Tied to the target object it references. | Tied to the target object it references. | Explicit lifetime (persists until manual `delete` is called). |
| **Rebinding** | Cannot be rebound. | Can be reassigned to point to different objects. | Bound permanently at initialization; cannot rebind. | Can be reassigned or deleted. |
| **When to Use** | Default choice for local variables and simple computations. | When optionality (`nullptr`), reseating, or dynamic polymorphism is required. | Function parameters (pass-by-reference), operator overloading, return of `*this`. | Long-lived objects whose lifespan transcends stack scope (preferably wrapped in `unique_ptr`). |

### Why `this` is a Pointer and What `*this` Refers To:
- **Why `this` is a pointer:**  
  When Bjarne Stroustrup designed C++, C compatibility was paramount. Member function invocations `obj.method()` were transformed into C-style functions `Cart_method(&obj)`. Hence, `this` is implemented as an implicit, hidden parameter: `Cart* const this` (a const pointer to a `Cart`). It stores the machine memory address of the invoking instance.
- **What `*this` refers to:**  
  Dereferencing `*this` yields the **lvalue reference** to the current object. When returned by reference (`Cart&`), it enables fluent syntax, chaining consecutive method calls on the exact same underlying object.

---

## Complete the Code (Task 5)

### Completed Implementation:
```cpp
// TODO 1: add(): store item & price if space exists, return *this for chaining
Cart& add(const std::string& item, double price) {
    if (n < 10) {
        items[n] = item;
        prices[n] = price;
        ++n;
    }
    return *this;
}

// TODO 2: setDiscount(): use this-> to disambiguate member from parameter
Cart& setDiscount(double discount) {
    this->discount = discount;
    return *this;
}
```

### What Happens if `add()` Returned `Cart` by Value Instead of `Cart&`:
1. If declared as `Cart add(...)`, each call would invoke the copy constructor to produce an **independent, temporary `Cart` object** on the execution stack.
2. In the chain `c.add("Rice", 60).add("Dal", 120).setDiscount(10);`:
   - `c.add("Rice", 60)` adds "Rice" to `c` and returns a temporary copy `T1`.
   - `.add("Dal", 120)` adds "Dal" to `T1` and returns another temporary copy `T2`.
   - `.setDiscount(10)` sets discount on `T2` and discards it!
3. The original object `c` would only contain "Rice" and have 0% discount!
4. Subsequent calls via pointer `p->add("Oil", 180)` would add "Oil" to `c`, resulting in a final total of $(60 + 180) \times 1.0 = 240.00$ instead of the correct $324.00$.
5. Significant performance penalties occur due to repeated deep copying of strings and arrays.

---

## Alternative Concept and Complexity Analysis

### Alternative Concept: `std::vector<Cart>` (Value Objects) vs Array of Pointers (`Cart*[]`)
Instead of keeping an array of pointers `Cart* activeCarts[10]`, storing cart objects directly inside `std::vector<Cart>`:

```cpp
std::vector<Cart> cartVector;
cartVector.push_back(Cart("Rohan"));
```

### Analysis:

| Metric | Array of Pointers (`Cart*[]`) | Dynamic Vector of Objects (`std::vector<Cart>`) |
| :--- | :--- | :--- |
| **Search Time for Highest Total** | **$O(n)$**: Iterates over $n$ pointers and calls `->total()`. | **$O(n)$**: Iterates over contiguous elements and calls `.total()`. |
| **CPU Cache Efficiency** | Lower: Pointers point to scattered heap locations, causing CPU cache misses. | **Higher**: Objects are packed contiguously in memory, maximizing hardware prefetcher efficiency. |
| **Space Overhead** | Stores 8-byte pointer per entry + separate heap chunk overhead per cart. | Contiguous memory chunk allocated by vector; minimal overhead per element. |
| **Pointer Invalidation Risk** | **None**: The pointer addresses remain stable unless explicitly deleted. | **Critical Risk**: When `vector<Cart>` grows beyond `capacity()`, it reallocates memory, copies/moves all objects, and frees old storage. Any external pointer `Cart* p = &cartVector[0];` becomes an **invalidated dangling pointer**, resulting in segmentation faults or memory corruption upon subsequent access. |
