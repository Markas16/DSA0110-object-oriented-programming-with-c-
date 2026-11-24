# Task 2: From Requirement to Hierarchy — University People Management

**Bloom's Level: K5 – Evaluate | Total Marks: 8**

---

## 2.1 Identify the Relationships (2 Marks)

| Relationship | Type (`is-a` vs `has-a`) | Inheritance Category | Justification |
| :--- | :--- | :--- | :--- |
| **`Student` to `Person`** | `is-a` | **Hierarchical / Single Inheritance** | A student is a specialized kind of person with academic credentials. |
| **`Staff` to `Person`** | `is-a` | **Hierarchical / Single Inheritance** | A staff member is a person with employment attributes. |
| **`TeachingAssistant` to `Student` and `Staff`** | `is-a` (both) | **Multiple / Hybrid (Diamond) Inheritance** | A TA concurrently holds student status (program/CGPA) and staff status (dept/stipend). |
| **`ResearchScholar` to `Student`** | `is-a` | **Multilevel Inheritance** | A research scholar is a specialized graduate student pursuing doctoral research. |
| **`Fellowship` in `ResearchScholar`** | `has-a` | **Composition (Member Class)** | A research scholar *holds* a fellowship grant; the fellowship is an independent funding entity with grant title, stipend, and tenure. |

---

## 2.2 Resolve the Diamond (2 Marks)

### The Diamond Problem in C++
When `TeachingAssistant` inherits from both `Student` and `Staff`, which both derive from `Person`, the inheritance graph forms a closed diamond:
```
         Person
        /      \
    Student   Staff
        \      /
     TeachingAssistant
```

Without special language mechanisms, this structure presents two fatal flaws:
1. **Name and Member Ambiguity:**  
   `TeachingAssistant` receives two independent copies of the `Person` base sub-object (one via `Student`, one via `Staff`). Attempting to access `ta.name` or `ta.getId()` generates a compiler error:
   ```
   error: request for member 'name' is ambiguous
   note: candidates are: 'std::string Person::name' (via Student)
   note:                 'std::string Person::name' (via Staff)
   ```
2. **State Duplication and Inconsistency:**  
   The object occupies duplicate memory for `name`, `id`, and `phone`. Modifying `Student::Person::name` leaves `Staff::Person::name` unchanged, causing critical data corruption.

### Resolution: Virtual Base Classes
By declaring `virtual` inheritance in the intermediate classes:
```cpp
struct Student : virtual public Person { ... };
struct Staff   : virtual public Person { ... };
```
The C++ runtime and compiler ensure that **only one shared instance of `Person` exists** inside any instantiated `TeachingAssistant`. The compiler injects a virtual base pointer/table (`vbptr`/`vbtable`) to resolve offsets to the single shared `Person` sub-object at runtime.

---

## 2.3 Design the Classes (3 Marks)

### Class Declarations and Access Specifiers
```cpp
class Person {
protected:
    std::string name;
    int id;
    std::string phone;
public:
    Person(const std::string& n, int i, const std::string& p);
    virtual ~Person() = default;
    virtual void display() const;
};

class Student : virtual public Person {
protected:
    std::string programme;
    double cgpa;
public:
    Student(const std::string& n, int i, const std::string& p, const std::string& prog, double gpa);
    void display() const override;
};

class Staff : virtual public Person {
protected:
    std::string department;
    double salary;
public:
    Staff(const std::string& n, int i, const std::string& p, const std::string& dept, double sal);
    void display() const override;
};

class TeachingAssistant : public Student, public Staff {
private:
    int teachingHoursPerWeek;
public:
    TeachingAssistant(const std::string& n, int i, const std::string& p,
                      const std::string& prog, double gpa,
                      const std::string& dept, double sal, int hours);
    void display() const override;
};
```

### Inheritance Mode Justification:
- **Public Inheritance (`public Person`, `public Student`, `public Staff`)** is chosen because every relationship is a genuine **Liskov-substitutable `is-a` relationship**. A `TeachingAssistant` *must* be usable anywhere a `Person*`, `Student*`, or `Staff*` is expected.
- Protected or private inheritance would hide base members from client code and break polymorphic upcasting.

---

## 2.4 Access Table (1 Mark)

Visibility of `Person` members in `Student` under the three inheritance specifiers:

| Base Member Specifier in `Person` | Public Inheritance (`public Person`) | Protected Inheritance (`protected Person`) | Private Inheritance (`private Person`) |
| :--- | :--- | :--- | :--- |
| **`public` member** | **`public`** in `Student` | **`protected`** in `Student` | **`private`** in `Student` |
| **`protected` member** | **`protected`** in `Student` | **`protected`** in `Student` | **`private`** in `Student` |
| **`private` member** | **Inaccessible directly** (Accessible only via Person's public/protected methods) | **Inaccessible directly** | **Inaccessible directly** |

---

## Predict the Output (Task 2)

### Given Program:
```cpp
#include <iostream>

struct Person { Person() { std::cout << "Person "; } };
struct Student : virtual Person { Student() { std::cout << "Student "; } };
struct Staff : virtual Person { Staff() { std::cout << "Staff "; } };
struct TeachingAssistant : Student, Staff {
TeachingAssistant() { std::cout << "TA "; }
};

int main()
{
TeachingAssistant ta;
std::cout << '\n';
}
```

### 1. My Predicted Output (Written BEFORE running):
```
Person Student Staff TA 
```

### 2. Actual Output after Compilation and Run:
```
Person Student Staff TA 
```

### 3. Explanation of Rules:
- **Why `Person` is printed only once:**  
  Under virtual inheritance, the shared virtual base class (`Person`) is constructed **first and exactly once** by the most-derived class (`TeachingAssistant`) before any non-virtual base classes are initialized.
- Direct non-virtual base classes are then constructed in their declaration order in the class header: `Student` first, followed by `Staff`.
- Finally, the body of the `TeachingAssistant` constructor runs.

### 4. Non-Virtual Experiment (Removing `virtual`):
When `virtual` is removed:
```cpp
struct Student : Person { Student() { std::cout << "Student "; } };
struct Staff   : Person { Staff()   { std::cout << "Staff "; } };
```
- **New Output:**  
  `Person Student Person Staff TA `
- **Reason:**  
  `Student` independently constructs its own `Person` instance (`Person Student`), and then `Staff` independently constructs its own separate `Person` instance (`Person Staff`).
- **Compiler Ambiguity Message:**  
  If any code tries to access `ta.Person::...` or upcast `Person* p = &ta;`, the compiler emits:
  ```
  error: 'Person' is an ambiguous base of 'TeachingAssistant'
  ```

---

## Alternative Concept and Complexity Analysis

### Alternative Concept: Composition (`has-a`) instead of Diamond Inheritance
Instead of virtual multiple inheritance, `TeachingAssistant` can be designed to contain a `Student` object and a `Staff` object as member fields:

```cpp
class TeachingAssistant {
private:
    Student studentRecord;
    Staff staffRecord;
    int teachingHours;
public:
    const std::string& getName() const { return studentRecord.getName(); }
};
```

### Complexity and Trade-off Comparison:

| Criterion | Virtual Inheritance Hierarchy | Composition (`has-a` Members) |
| :--- | :--- | :--- |
| **Space per Object (`sizeof`)** | Stores **one `Person` sub-object** + virtual base table pointer (`vbptr` = 8 bytes per branch). Total memory is compact and prevents redundant strings. | Stores **two full `Person` sub-objects** (`studentRecord.person` and `staffRecord.person`). Wastes redundant memory for duplicated `name`, `id`, and `phone` strings. |
| **Time to Reach `name`** | Direct dereference with small offset calculation via `vbptr` ($O(1)$). | Explicit delegation call ($O(1)$) to `studentRecord.getName()`. |
| **Polymorphism & Upcasting** | Direct polymorphic conversion to `Person*`, `Student*`, or `Staff*`. | Cannot implicitly pass `TeachingAssistant` where a `Student` or `Staff` is required without adapter interfaces. |
| **Name Ambiguity** | Completely eliminated by compiler's virtual base mechanism. | Manually eliminated by explicitly naming `studentRecord` or `staffRecord`. |
