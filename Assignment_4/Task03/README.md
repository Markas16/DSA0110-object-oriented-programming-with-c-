# Task 3: Constructors in Derived Classes and Member Classes

**Bloom's Level: K5 – Evaluate | Total Marks: 9**

---

## 3.1 Pass Values to Base Constructors (3 Marks)

### Constructor Implementation with Member Initializer Lists
```cpp
// Student Constructor
Student(const std::string& n, const std::string& city, const std::string& pr, const std::string& crs)
    : Person(n, city), prog(pr), enrolledCourse(crs) {}

// Staff Constructor
Staff(const std::string& n, const std::string& city, const std::string& d)
    : Person(n, city), dept(d) {}

// TeachingAssistant Constructor (Most-Derived Class)
TeachingAssistant(const std::string& n, const std::string& city,
                  const std::string& pr, const std::string& crs,
                  const std::string& d, int h)
    : Person(n, city),            // Direct initialization of virtual base!
      Student(n, city, pr, crs),  // Intermediate base
      Staff(n, city, d),          // Intermediate base
      hours(h) {}
```

### Why the Most-Derived Class Must Call the Virtual Base Constructor:
In a virtual inheritance diamond, the virtual base class (`Person`) is shared between multiple derivation branches (`Student` and `Staff`). If intermediate derived classes were allowed to construct the virtual base, an irresolvable conflict would occur if `Student` and `Staff` passed different arguments to `Person`.

To enforce consistency:
1. The C++ standard mandates that **only the most-derived class currently being instantiated (`TeachingAssistant`) invokes the constructor of the virtual base class**.
2. Any constructor calls to `Person` present in the member initializer lists of intermediate classes (`Student`, `Staff`) are **silently ignored** by the compiler during `TeachingAssistant` construction.

---

## 3.2 Member Classes (Composition) (2 Marks)

- **`Address`** is embedded as a member class inside `Person`.
- **`Course`** is embedded as a member class inside `Student`.

### Initialization Chain:
When `Person` is constructed, its member object `Address addr;` must be constructed before the `Person` constructor body runs.
```cpp
class Person {
protected:
    std::string name;
    Address addr;
public:
    Person(const std::string& n, const std::string& city)
        : name(n), addr(city) {} // Passing value to Address member constructor
};
```

---

## 3.3 Trace the Order of Construction and Destruction (3 Marks)

### Execution Trace for `TeachingAssistant`:
```
1.  [Construct] Address(Salem)          <-- Member of Virtual Base
2.  [Construct] Person(Karthik)          <-- Virtual Base Class
3.  [Construct] Course(AI201)           <-- Member of 1st Direct Base
4.  [Construct] Student(M.Tech)          <-- 1st Direct Base Class
5.  [Construct] Staff(CSE)               <-- 2nd Direct Base Class
6.  [Construct] TeachingAssistant(15 hrs)<-- Most-Derived Class Body
----------------------------------------------------------------------
7.  [Destruct]  ~TeachingAssistant(15 hrs)<-- Most-Derived Destructor Body
8.  [Destruct]  ~Staff(CSE)              <-- 2nd Direct Base Destructor
9.  [Destruct]  ~Student(M.Tech)         <-- 1st Direct Base Destructor
10. [Destruct]  ~Course(AI201)          <-- Member of 1st Direct Base
11. [Destruct]  ~Person(Karthik)         <-- Virtual Base Destructor
12. [Destruct]  ~Address(Salem)          <-- Member of Virtual Base
```

### The Universal Rule for Object Lifecycle:
1. **Construction Order:**
   - **Step 1:** Virtual base classes anywhere in the hierarchy are constructed first, in depth-first, left-to-right order.
   - **Step 2:** Non-virtual direct base classes are constructed in the exact order they appear in the class derivation header list.
   - **Step 3:** Non-static member objects are constructed in the exact order they are **declared in the class definition**, irrespective of the order listed in the constructor's member initializer list.
   - **Step 4:** The derived class constructor body executes.
2. **Destruction Order:**
   - Destruction proceeds in the **exact reverse order** of construction.

---

## 3.4 Experiment: Removing the `virtual` Keyword (1 Mark)

| Feature | With `virtual Person` | Without `virtual` (Normal Inheritance) |
| :--- | :--- | :--- |
| **Number of `Person` instances in TA** | **1 shared sub-object** | **2 distinct sub-objects** (one via Student, one via Staff) |
| **Construction Trace** | `Address -> Person -> Course -> Student -> Staff -> TA` | `Address -> Person -> Course -> Student -> Address -> Person -> Staff -> TA` |
| **`sizeof(TeachingAssistant)`** | **168 bytes** (Includes single Person + vtable/vbase pointers) | **192 bytes** (Duplicated strings for name, phone, Address within both branches) |

---

## Predict the Output (Task 3)

### Given Program:
```cpp
#include <iostream>

struct Engine {
Engine() { std::cout << "Engine "; }
~Engine() { std::cout << "~Engine "; }
};

struct Vehicle {
Vehicle() { std::cout << "Vehicle "; }
~Vehicle() { std::cout << "~Vehicle "; }
};

struct Car : Vehicle {
Engine e;
Car() { std::cout << "Car "; }
~Car() { std::cout << "~Car "; }
};

int main()
{
{
Car c;
std::cout << "| ";
}
std::cout << '\n';
}
```

### 1. My Predicted Output (Written BEFORE running):
```
Vehicle Engine Car | ~Car ~Engine ~Vehicle 
```

### 2. Actual Output after Compilation and Execution:
```
Vehicle Engine Car | ~Car ~Engine ~Vehicle 
```

### 3. Difference & Explanation:
- Prediction exactly matches actual output.
- **Rule:**
  1. Base class `Vehicle` constructor runs first (`Vehicle `).
  2. Embedded member object `Engine e` constructor runs second (`Engine `).
  3. Derived class `Car` constructor body runs third (`Car `).
  4. Stream prints `| `.
  5. Object `c` goes out of scope at the closing brace `}`.
  6. Destructors execute in reverse: `~Car `, then `~Engine `, then `~Vehicle `.

---

## Complete the Code (TODO 1)

### Solution:
```cpp
class Student : public Person {
    long long regNo;
    Address home;

public:
    // TODO 1: Initialise the Person base, regNo and the home Address member in the initializer list
    Student(const std::string& n, int a, long long r, const std::string& c, int p)
        : Person(n, a), regNo(r), home(c, p) {}
...
```

### Expected & Verified Output:
```
192311001 Kavya (19) Salem-636001
```

### Swapped Order Experiment & Compiler Warning:
When the initializer list is written with `home(c, p)` before `regNo(r)`:
```cpp
Student(...) : Person(n, a), home(c, p), regNo(r) {}
```
Compiling with `g++ -std=c++17 -Wall -Wextra` produces:
```
reorder_warning_demo.cpp: In constructor 'Student::Student(...)':
reorder_warning_demo.cpp:32:13: warning: 'Address Student::home' will be initialized after [-Wreorder]
   32 |     Address home;
      |             ^~~~
reorder_warning_demo.cpp:31:15: warning:   'long long int Student::regNo' [-Wreorder]
   31 |     long long regNo;
      |               ^~~~~
```
**Explanation:** In C++, member variables are ALWAYS initialized in the order of their declaration within the class body (`regNo` first, `home` second). The order in the initializer list is ignored by the code generator. The `-Wreorder` flag alerts developers that code execution does not match the visual order of the initializer list, which could cause undefined behavior if one member depended on another.

---

## Alternative Concept and Complexity Analysis

### Alternative Concept: Dynamic Pointer to Address (`Address* home = new Address(...)`)
Instead of value composition (`Address home`), `Student` could hold a raw pointer to a heap-allocated `Address`.

```cpp
class StudentWithPtr : public Person {
    long long regNo;
    Address* home; // Heap pointer
public:
    StudentWithPtr(const std::string& n, int a, long long r, const std::string& c, int p)
        : Person(n, a), regNo(r), home(new Address(c, p)) {}
    
    ~StudentWithPtr() {
        delete home; // Manual cleanup required
    }
    
    // Copy Constructor (Deep Copy required by Rule of Three)
    StudentWithPtr(const StudentWithPtr& other)
        : Person(other), regNo(other.regNo), home(new Address(*other.home)) {}
    
    // Copy Assignment Operator
    StudentWithPtr& operator=(const StudentWithPtr& other) {
        if (this != &other) {
            Person::operator=(other);
            regNo = other.regNo;
            delete home;
            home = new Address(*other.home);
        }
        return *this;
    }
};
```

### Complexity and Performance Comparison:

| Dimension | Direct Member Object (`Address home`) | Dynamic Pointer (`Address* home = new ...`) |
| :--- | :--- | :--- |
| **Time Overhead** | Zero allocation overhead; contiguous stack memory allocation ($O(1)$). | Slower heap allocation (`malloc`/`operator new`) and deallocation (`free`/`operator delete`) with OS lock and fragmentation ($O(1)$ amortized but high constant factor). |
| **Space Overhead** | Exactly `sizeof(Address)` (32 bytes). Data is contiguous, optimizing CPU cache lines. | Adds 8 bytes for the pointer + 16 bytes malloc chunk header metadata + memory alignment padding. |
| **Lifetime & Safety** | Automatic RAII. Impossible to leak; automatically destroyed when `Student` is destroyed. | Prone to memory leaks, dangling pointers, and double-free errors if the **Rule of Three / Five** is violated. |
