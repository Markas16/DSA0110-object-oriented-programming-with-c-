# ASSIGNMENT 3 – CO3 REPORT

**Name:** [Sukirtheraj Markas.S]  
**Register Number:** [192421250]  
**Course / Section:** [B.Tech IT]  
**Subject:** C++ Programming    
**Compiler and Version:** GNU G++ 
**Programming Standard:** C++17  

---

# Q1 – Constructors and Constructor Overloading (6 Marks)

## 1. Aim

To create `Book` objects using default, title-only and parameterized constructors, demonstrate constructor overloading and delegation, and prevent invalid copy counts.

## 2. Design and Implementation

The `Book` class stores the following data members:

- `title` – stores the book title.
- `author` – stores the author's name.
- `copies` – stores the number of available copies.

The class uses three constructor forms:

1. Default constructor – creates an empty book record.
2. Title-only constructor – creates a donated book with only its title.
3. Parameterized constructor – initializes all book details.

Constructor delegation is used to avoid repeating initialization and validation code. Every constructor uses a member initializer list.

The parameterized constructor validates the number of copies and rejects negative values using `std::invalid_argument`.

## 3. Important Explanations

### 3.1 When does the compiler supply a default constructor?

The compiler supplies an implicit default constructor when a class has no user-declared constructor.

If the programmer declares a constructor, the compiler does not automatically provide the usual default constructor. A default constructor must then be explicitly declared if required.

### 3.2 Why are member initializer lists preferred?

A member initializer list initializes data members directly before the constructor body executes.

Assignment inside the constructor body happens after the members have already been initialized.

Initializer lists are particularly necessary for:
- Constant data members.
- Reference data members.
- Members that do not have a default constructor.

They also avoid unnecessary initialization followed by assignment.

### 3.3 Order of member initialization

C++ initializes class members in their declaration order, not in the order in which they appear in the initializer list.

The `-Wreorder` compiler warning helps identify initializer lists that do not follow declaration order.

### 3.4 Constructor delegation

Constructor delegation allows one constructor to call another constructor of the same class.

It avoids repeated initialization and ensures that common validation logic is maintained in one place.

### 3.5 Explicit constructors

The `explicit` keyword prevents unintended implicit conversions through a single-argument constructor.

Direct initialization is still permitted, but an implicit conversion is prevented.

## 4. Object-State Diagram

The following objects demonstrate the three construction situations:

```text
emptyBook
--------------------------------
Title  : Clean Code
Author : Robert C. Martin
Copies : 3

donatedBook
--------------------------------
Title  : The Alchemist
Author : Unknown / Empty
Copies : 0

fullBook
--------------------------------
Title  : Wings of Fire
Author : A. P. J. Abdul Kalam
Copies : 5
```

Each object is initialized with valid data through its constructor.

## 5. Experimental Investigation

The following experiments are performed:

1. Create and display objects using all three constructor forms.
2. Remove the default constructor and attempt to create `Book books[3]`.
3. Compile the wrong-initialization-order example using `-Wreorder`.
4. Test a negative number of copies.
5. Run the explicit constructor demonstration.
6. Uncomment the implicit-conversion example and observe the compilation error.

### Experimental Results

- The three constructor forms initialize the objects with their expected values.
- Without a default constructor, an array of objects requiring default construction cannot be created.
- The `-Wreorder` option identifies initialization-order problems.
- Negative copy counts are rejected by validation.
- The `explicit` keyword prevents unintended implicit conversion.

**Compiler error for the missing default constructor:**  
[Paste the actual compiler error here.]

**`-Wreorder` warning:**  
[Paste the actual warning here.]

**Negative-copy validation output:**  
[Paste the actual output here.]

**Explicit-constructor compilation result:**  
[Paste the actual compilation result here.]

## 6. Alternative Concept and Complexity Analysis

Instead of using multiple overloaded constructors, a single constructor with default arguments can be used.

Example:

```cpp
Book(std::string title = "",
     std::string author = "",
     int copies = 0);
```

This constructor can create empty, donated and fully specified book objects.

Both approaches have:

- Object creation time complexity: O(1), excluding string processing.
- Additional space complexity: O(1), excluding string storage.

Separate constructors clearly represent different creation situations. A single constructor centralizes validation in one place but may make the purpose of each call less descriptive.

## 7. Conclusion

This question demonstrates default constructors, parameterized constructors, constructor overloading, constructor delegation and member initializer lists. Validation prevents invalid object states, while explicit constructors prevent unintended implicit conversions.

---

# Q2 – Copy Constructors: Shallow and Deep Copies (7 Marks)

## 1. Aim

To understand shallow and deep copying and implement a `Prescription` class that manages a dynamically allocated array of medicine names safely.

## 2. Implementation Summary

The `Prescription` class contains:

- `n` – stores the number of medicines.
- `medicines` – points to a dynamically allocated array of medicine names.

The implementation includes:

1. A parameterized constructor.
2. A deep-copying copy constructor.
3. A copy assignment operator.
4. A destructor.
5. Self-assignment protection.

The copy constructor allocates a new array and copies every medicine name.

The copy assignment operator checks for self-assignment, creates replacement storage, copies the elements, releases the previous storage and returns `*this`.

The destructor releases the dynamically allocated array using `delete[]`.

## 3. Important Explanations

### 3.1 Shallow Copy

A shallow copy copies the pointer address instead of creating separate memory.

As a result, both objects refer to the same dynamically allocated array.

Modifying one object can affect the other object.

If both objects attempt to release the same memory, a double-free error can occur.

### 3.2 Deep Copy

A deep copy creates a separate memory allocation and copies the actual elements.

Each object owns its own array. Therefore, changes to one object do not affect the other.

### 3.3 Rule of Three

The Rule of Three states that when a class manages a resource manually using an owning pointer, it generally needs three special member functions:

1. Destructor.
2. Copy constructor.
3. Copy assignment operator.

These functions work together to manage resource ownership correctly.

### 3.4 Three situations in which a copy constructor is called

A copy constructor is commonly called when:

1. A new object is initialized from an existing object.
2. An object is passed to a function by value.
3. An object is returned by value when copy elision does not apply.

Passing an object by `const` reference does not create a copy of that object.

Passing by value creates a separate object, subject to applicable copy-elision rules.

## 4. Memory Diagrams

### Shallow Copy

```text
Prescription p ─────┐
                    |
                    v
             Shared Medicine Array
             ----------------------
             Paracetamol
             ORS
                    ^
                    |
Prescription q ─────┘
```

Both objects point to the same allocated array.

### Deep Copy

```text
Prescription p
      |
      v
Separate Array A
----------------
Paracetamol
ORS


Prescription q
      |
      v
Separate Array B
----------------
Paracetamol
ORS
```

Each object owns independent memory.

## 5. Expected Output for the Supplied Complete the Code Program

```text
Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS
```

### Explanation

- `p` is modified to contain `Ibuprofen` and `ORS`.
- `q` is an independent copy and contains `Paracetamol` and `Cetirizine`.
- `r` is assigned a deep copy of `p` before `p` is modified, so it contains `Paracetamol` and `ORS`.

The output confirms that the copied objects maintain independent data.

## 6. Experimental Investigation

The following experiments are performed:

1. Implement the class without a copy constructor and observe the shallow-copy problem.
2. Implement the deep-copying copy constructor.
3. Implement copy assignment and the destructor.
4. Modify the copied object and verify that the original remains unchanged.
5. Add a copy-constructor message and count copies when passing by value and by `const` reference.
6. Test self-assignment using `p = p`.
7. Run both versions under AddressSanitizer or Valgrind.
8. Measure copy times for 10, 1,000 and 100,000 medicine entries.

### Sanitizer Results

**Shallow-copy version:**  
[Paste the actual sanitizer diagnostic here.]

**Deep-copy version:**  
[Paste the actual sanitizer output here.]

### Copy Constructor Count

**Pass by value:** [Enter the observed copy count.]

**Pass by const reference:** [Enter the observed copy count.]

### Copy-Time Measurements

| Number of medicines | Measured time |
|---:|---|
| 10 | [Enter measured time] |
| 1,000 | [Enter measured time] |
| 100,000 | [Enter measured time] |

Compiler flags and repetitions used: [Enter actual test details.]

## 7. Alternative Concept and Complexity Analysis

Instead of using a dynamically allocated array, medicine names can be stored using:

```cpp
std::vector<std::string> medicines;
```

The standard library manages memory automatically. Its compiler-generated copy operations copy the elements correctly.

This follows the Rule of Zero because the class does not need to implement its own destructor, copy constructor or copy assignment operator for the vector's memory.

Complexity comparison:

| Operation | Time Complexity | Additional Space |
|---|---|---|
| Shallow pointer copy | O(1) | O(1) |
| Deep copy of n medicines | O(n) | O(n) |
| Vector copy of n medicines | O(n) | O(n) |

A shallow copy is faster in terms of pointer copying, but it does not provide independent ownership and is unsafe for this class.

## 8. Conclusion

Deep copying is necessary when a class owns dynamically allocated memory and copied objects must remain independent. The Rule of Three helps manage memory safely. Standard containers such as `std::vector` simplify resource management through the Rule of Zero.

---

# Q3 – Dynamic Constructors and Destructors (7 Marks)

## 1. Aim

To dynamically allocate a two-dimensional auditorium seat map using a constructor and release all allocated memory using a destructor.

## 2. Implementation Summary

The `SeatMap` class represents an auditorium containing a variable number of rows and seats per row.

The constructor dynamically allocates:

1. An array of row pointers.
2. A separate array for each row.

The seats are initialized during construction.

The class supports:
- Displaying the seat map.
- Booking seats.
- Cancelling seats.
- Creating stack objects.
- Creating objects inside a block.
- Creating heap objects using `new` and `delete`.

The destructor releases every allocated row and then releases the row-pointer array.

## 3. Important Explanations

### 3.1 Object Lifetime

**Local object:** A local object is automatically destroyed when its scope ends.

**Block-scope object:** An object created inside a block is destroyed when execution reaches the closing brace of that block.

**Dynamic object:** An object created using `new` remains alive until it is explicitly destroyed using `delete`.

### 3.2 What happens if delete is forgotten?

If dynamically allocated memory is not released, a memory leak occurs.

The allocated memory remains unavailable for reuse until the program terminates or the memory is otherwise released.

### 3.3 Difference between delete and delete[]

- `delete` releases a single object allocated using `new`.
- `delete[]` releases an array allocated using `new[]`.

Using the wrong form results in undefined behavior.

### 3.4 Why can a destructor not be overloaded?

A destructor has no return type and takes no arguments.

It performs the cleanup operation when an object is destroyed. Since an object has one destruction operation, a destructor cannot be overloaded.

### 3.5 RAII

Resource Acquisition Is Initialization (RAII) connects resource ownership to object lifetime.

The constructor acquires the resources, and the destructor releases them automatically when the object is destroyed.

This reduces the possibility of forgetting to release dynamically allocated memory.

## 4. Memory Layout

```text
SeatMap Object
      |
      v
Row Pointer Array
      |
      |------> Row 0: [O] [O] [O] [O]
      |
      |------> Row 1: [O] [O] [O] [O]
      |
      |------> Row 2: [O] [O] [O] [O]
```

The row-pointer array and each row are separate dynamic allocations.

## 5. Experimental Investigation

The following experiments are performed:

1. Create a local auditorium object.
2. Create an object inside a block and observe its destruction.
3. Create a heap object using `new` and release it using `delete`.
4. Book and cancel seats.
5. Print constructor and destructor messages.
6. Test an event with zero rows.
7. Run AddressSanitizer on the correct program.
8. Create a temporary missing-delete version and investigate the memory leak.
9. Estimate memory for different seat-map sizes.

### Memory Estimation

Assuming one byte per seat:

| Seat Map | Number of Seats | Approximate Seat Storage |
|---|---:|---:|
| 10 × 20 | 200 | 200 bytes |
| 100 × 200 | 20,000 | 20,000 bytes |
| 1,000 × 2,000 | 2,000,000 | 2,000,000 bytes |

These estimates include only seat characters. They exclude row pointers, allocator metadata and alignment overhead.

For a row-pointer implementation, approximate storage is:

```text
Seat storage = rows × seats per row

Row-pointer storage = rows × sizeof(char*)

Total approximate storage =
Seat storage + Row-pointer storage
```

### AddressSanitizer Results

**Missing-delete experiment:**  
[Paste the actual diagnostic or leak-detection result here.]

**Correct implementation:**  
[Paste the actual sanitizer output here.]

### Zero-Row Test

[Record the actual program output for an event with zero rows.]

## 6. Predict the Output – Logger Program

### Prediction Before Running

```text
+A +B -B copyAc use -Ac +P -P end -A
```

### Explanation

1. `Logger a("A")` constructs object A and prints `+A`.
2. The inner block creates object B and prints `+B`.
3. When the block ends, B is destroyed and prints `-B`.
4. `use(a)` passes A by value, so a copy named `Ac` is created and prints `copyAc`.
5. The function prints `use`.
6. The copied parameter is destroyed when the function returns and prints `-Ac`.
7. The heap object P is created using `new` and prints `+P`.
8. `delete p` destroys P and prints `-P`.
9. The program prints `end`.
10. At the end of `main`, A is destroyed and prints `-A`.

The output demonstrates construction, copying, function parameter lifetime, dynamic allocation and destruction order.

## 7. Alternative Concept and Complexity Analysis

Instead of allocating each row separately, the seat map can be allocated as one contiguous block.

The element at row `r` and seat `s` can be accessed using:

```cpp
index = r * seatsPerRow + s;
```

Another alternative is:

```cpp
std::vector<std::vector<char>>
```

Comparison:

| Approach | Number of Allocations | Access Time |
|---|---:|---|
| Row-pointer array | rows + 1 | O(1) |
| Contiguous allocation | 1 | O(1) |
| Vector of vectors | Managed by containers | O(1) indexed access |

A contiguous allocation generally provides better memory locality and less allocation overhead. A vector-based approach provides automatic memory management.

## 8. Conclusion

Dynamic constructors allocate memory according to the required seat-map size. Destructors release all allocated memory when objects are destroyed. RAII makes resource management safer by connecting memory cleanup to object lifetime.

---

# Q4 – Overloading Arithmetic and Stream Operators (7 Marks)

## 1. Aim

To implement a `Money` class that stores monetary values in paise and overloads arithmetic, comparison and stream operators.

## 2. Implementation Summary

The `Money` class stores the amount as a `long long` integer representing paise.

The class demonstrates:

- Addition using `+`.
- Subtraction using `-`.
- Addition assignment using `+=`.
- Equality comparison using `==`.
- Less-than comparison using `<`.
- Input using `>>`.
- Output using `<<`.

Storing money as integer paise avoids common floating-point representation errors in ordinary currency calculations.

## 3. Important Explanations

### 3.1 Why does + return a new object?

The `+` operator calculates a result without modifying the original operands.

Therefore, it returns a new `Money` object containing the result.

### 3.2 Why does += return a reference to *this?

The `+=` operator modifies the existing left-hand object.

Returning `Money&` allows the modified object to be reused and supports chained operations without creating an unnecessary copy.

### 3.3 Member and Friend Operators

Member operators are suitable when the left operand is an object of the class.

Friend non-member operators are useful when an operator needs access to private data but the left operand is not a class object.

### 3.4 Why must << be a non-member?

In:

```cpp
std::cout << amount;
```

The left operand is `std::cout`, which is an `std::ostream` object.

Therefore, the stream insertion operator must be implemented as a non-member function to work with the stream as its left operand.

### 3.5 How does the compiler interpret a + b?

When `+` is implemented as a member operator, the expression:

```cpp
a + b
```

is conceptually translated into:

```cpp
a.operator+(b);
```

## 4. Expected Output for the Supplied Complete the Code Program

```text
Rs 200.00
true
```

The two amounts are:

```text
Money a(120, 75)
Money b(79, 25)
```

Their addition gives:

```text
Rs 120.75 + Rs 79.25 = Rs 200.00
```

The equality comparison checks whether the result equals `Money(200)`.

## 5. Experimental Investigation

The following experiments are performed:

1. Run the supplied Complete the Code program.
2. Test addition involving paise carry.
3. Test subtraction that produces a negative amount.
4. Compare bill calculations with and without operator overloading.
5. Compare integer-paise calculations with a `double`-based implementation over 1,000,000 additions.

### Paise Carry Test

```text
Rs 0.75 + Rs 0.50 = Rs 1.25
```

The 125 paise result is represented as 1 rupee and 25 paise.

### Negative Subtraction

[Record the actual output and explain the policy used for negative monetary values.]

### Double Comparison

| Implementation | Number of Additions | Final Result |
|---|---:|---|
| Integer paise | 1,000,000 | [Enter actual result] |
| Double | 1,000,000 | [Enter actual result] |

**Integer implementation time:** [Enter measured time.]

**Double implementation time:** [Enter measured time.]

Compiler optimization flags: [Enter actual flags.]

## 6. Engineering Decision

Multiplication of one `Money` object by another `Money` object is generally not meaningful for ordinary billing operations.

However, multiplying a monetary amount by a numeric quantity or tax rate can be meaningful if the class defines suitable rounding rules.

Therefore, only operators that represent meaningful monetary operations should be overloaded.

## 7. Alternative Concept and Complexity Analysis

The operators can also be implemented as non-member friend functions or replaced with named functions such as:

```cpp
add(a, b);
equals(a, b);
```

All these approaches have:

- Time complexity: O(1).
- Additional space complexity: O(1).

A non-member operator can support expressions such as:

```cpp
100 + money;
money + 100;
```

when suitable overloads or conversions are provided.

Member operators are convenient when the left operand is a `Money` object, while non-member operators provide greater flexibility for symmetric operations.

## 8. Conclusion

Operator overloading makes monetary calculations easier to read and understand. Integer-paise storage avoids common floating-point representation problems, while stream operators make input and output convenient.

---

# Q5 – Overloading Increment, Comparison and Subscript Operators (6 Marks)

## 1. Aim

To implement prefix and postfix increment operators, comparison operators and checked subscript access for a music playlist.

## 2. Implementation Summary

The assignment uses:

- A `Time` class to represent the current playback position.
- A `Playlist` class to store track names.

The `Time` class overloads:

- Prefix `++`.
- Postfix `++`.
- Equality `==`.
- Less-than `<`.

The `Playlist` class overloads `[]` with both const and non-const versions and checks whether the requested index is valid.

## 3. Important Explanations

### 3.1 Difference between Prefix and Postfix Increment

Prefix increment:

```cpp
++object;
```

Postfix increment:

```cpp
object++;
```

The compiler distinguishes them using the dummy `int` parameter.

Prefix increment does not require this dummy parameter, while postfix increment uses it to identify the postfix form.

### 3.2 Why is Postfix Increment Slower?

Postfix increment returns the original value before incrementing the object.

Therefore, it generally needs to create a temporary copy of the object.

Prefix increment modifies the current object and returns it by reference.

For class types, postfix increment can involve additional copying.

### 3.3 Why does operator[] need const and non-const versions?

A non-const version returns a modifiable reference, allowing an element to be changed.

A const version allows an element to be read through a constant `Playlist` object without allowing modification.

### 3.4 Out-of-Range Index

If an index is negative or greater than or equal to the playlist size, the program must prevent invalid memory access.

The program can report an error or throw `std::out_of_range`.

## 4. Predict the Output – Counter Program

### Prediction Before Running

```text
5 7 7
```

### Explanation

Initially:

```text
c = 5
```

The statement:

```cpp
Counter a = c++;
```

uses postfix increment. It stores the old value 5 in `a` and then increments `c` to 6.

The next statement:

```cpp
Counter b = ++c;
```

uses prefix increment. It increments `c` to 7 first and then stores the updated value in `b`.

Therefore:

```text
a = 5
b = 7
c = 7
```

The postfix version creates a copy of the original object, whereas the prefix version returns the updated object by reference.

## 5. Experimental Investigation

The following experiments are performed:

1. Verify the values of `a` and `b` after `a = t++` and `b = ++t`.
2. Test prefix and postfix increment.
3. Test equality and less-than comparisons.
4. Test `playlist[-1]`.
5. Test `playlist[size]`.
6. Test minute and hour rollover from 59:59 to 1:00:00.
7. Measure 10,000,000 prefix and postfix increments at `-O0` and `-O2`.

### Prefix and Postfix Benchmark

| Optimization Level | Prefix Time | Postfix Time |
|---|---|---|
| -O0 | [Enter measured time] | [Enter measured time] |
| -O2 | [Enter measured time] | [Enter measured time] |

Compiler version: [Enter version.]

Number of increments: 10,000,000.

### Invalid Playlist Index Tests

**Test with index -1:** [Enter actual result.]

**Test with index equal to playlist size:** [Enter actual result.]

### Time Rollover Test

```text
59:59 → 1:00:00
```

[Record the actual program output.]

## 6. Alternative Concept and Complexity Analysis

Playlist access can also be implemented using an `at(i)` function that throws `std::out_of_range`.

An unchecked `operator[]` behaves like built-in array indexing and does not perform automatic range checking.

Comparison:

| Approach | Time Complexity | Safety |
|---|---|---|
| Checked operator[] | O(1) | Checks the index |
| at(i) | O(1) | Throws an exception for invalid index |
| Unchecked operator[] | O(1) | Invalid access can cause undefined behavior |

Checked access adds one constant-time comparison.

The cost of the check can be measured by comparing 10,000,000 accesses with and without the check.

**Measured checked-access time:** [Enter time.]

**Measured unchecked-access time:** [Enter time.]

## 7. Conclusion

Prefix and postfix increment operators provide different behaviors. Prefix increment is generally preferred for class types when the previous value is not required because it can avoid creating an additional copy. Checked subscript access improves safety by preventing invalid playlist indexing.

---

# Q6 – The Rules of Operator Overloading (7 Marks)

## 1. Aim

To implement a dynamic `Text` class and understand the rules and limitations of operator overloading in C++.

## 2. Implementation Summary

The `Text` class stores:

- A dynamically allocated character array.
- The length of the stored text.

The class implements:

1. Constructor.
2. Copy constructor.
3. Copy assignment operator.
4. Destructor.
5. Concatenation using `+`.
6. Equality using `==`.
7. Inequality using `!=`.
8. Output using `<<`.

Deep copying is used to maintain independent memory ownership.

Self-assignment is handled safely.

## 3. Important Explanations

### 3.1 Operators That Cannot Be Overloaded

The following operators cannot be overloaded:

| Operator | Name |
|---|---|
| `::` | Scope resolution |
| `.` | Member access |
| `.*` | Pointer-to-member access |
| `?:` | Conditional operator |
| `sizeof` | Size operator |

These operators have special language-level behavior and cannot be redefined through operator overloading.

### 3.2 Rules of Operator Overloading

1. Operator precedence cannot be changed.
2. Operator associativity cannot be changed.
3. Operator arity cannot be changed.
4. At least one operand must be a user-defined type.
5. Operator overloading cannot create new operators.

### 3.3 Why must operator= be a member?

The assignment operator `operator=` must be implemented as a non-static member function.

It modifies the left-hand object, so it needs access to the current object through `this`.

### 3.4 When should an overloaded operator be a friend?

A friend function is useful when an operator needs access to private data but should be implemented as a non-member function.

For example:

```cpp
std::cout << text;
```

The left operand is an output stream, so stream insertion is implemented as a non-member operator.

### 3.5 Evaluation of a + b + c

The `+` operator is left-associative.

Therefore:

```cpp
a + b + c
```

is evaluated as:

```cpp
(a + b) + c
```

The first operation creates an intermediate result. The second operation uses that intermediate result to create the final text.

Temporary objects and character copying may occur during these operations.

## 4. Experimental Investigation

The following experiments are performed:

1. Create text objects using the constructor.
2. Test deep copying.
3. Test copy assignment.
4. Test self-assignment.
5. Test concatenation using `+`.
6. Test equality and inequality.
7. Test empty messages.
8. Test long messages.
9. Count copy-constructor calls when evaluating `a + b + c`.
10. Run AddressSanitizer and investigate any reported errors.
11. Compare the custom `Text` class with `std::string` for 100,000 concatenations.

### Copy Count

For the expression:

```cpp
a + b + c
```

**Observed copy-constructor calls:** [Enter actual count.]

The exact number depends on the implementation, return-value handling and copy-elision behavior.

### AddressSanitizer Result

[Paste the actual AddressSanitizer output here.]

### Concatenation Benchmark

| Implementation | Number of Concatenations | Measured Time |
|---|---:|---|
| Custom Text class | 100,000 | [Enter time] |
| std::string | 100,000 | [Enter time] |

Compiler version and optimization flags: [Enter actual details.]

## 5. Alternative Concept and Complexity Analysis

The custom `Text` class can be replaced with:

```cpp
std::string
```

The standard library manages memory, copying, assignment, concatenation and comparison automatically.

Another approach is to implement `operator+` using `operator+=` on a copy:

```cpp
Text operator+(Text a, const Text& b)
{
    a += b;
    return a;
}
```

For strings with lengths `n1`, `n2` and `n3`, evaluating:

```cpp
a + b + c
```

creates an intermediate string of length:

```text
n1 + n2
```

The final string has length:

```text
n1 + n2 + n3
```

The total character-copy work for these two concatenations is:

```text
O(n1 + n2 + n3)
```

Temporary storage is required for intermediate and final results.

Using `std::string` reduces the amount of manual memory-management code and provides standard string operations.

## 6. Conclusion

Implementing the `Text` class provides practical understanding of dynamic memory allocation, deep copying, copy assignment, destructors and operator overloading.

Although a custom class is useful for learning and specialized behavior, `std::string` is generally preferable for normal application development because it manages memory automatically.

---

# Q7 – Design a Complex-Number Class from First Principles (10 Marks)

## Stage 1 – Interpret the Problem (2 Marks)

### 1.1 Required Data

A complex number contains two components:

- Real part.
- Imaginary part.

Both components are stored using the `double` data type.

A complex number is represented as:

```text
a + bj
```

Where:
- `a` is the real component.
- `b` is the imaginary component.
- `j` represents the imaginary unit.

### 1.2 Required Operations

The class must support:

1. Addition.
2. Subtraction.
3. Multiplication.
4. Division.
5. Equality comparison.
6. Output using `<<`.
7. Magnitude calculation.

### 1.3 Required Constructors

**Default constructor:** Creates a complex number with real and imaginary parts equal to zero.

**One-argument constructor:** Creates a complex number containing only a real component.

**Two-argument constructor:** Creates a complex number using both real and imaginary components.

### 1.4 Measurable Requirements

The implementation must:

- Correctly calculate all four arithmetic operations.
- Display complex numbers in rectangular form.
- Calculate the magnitude.
- Detect division by a zero complex number.
- Calculate series RLC impedance.
- Calculate parallel RLC impedance.
- Verify the results using mathematical calculations.

---

## Stage 2 – Design (3 Marks)

### 2.1 Class Design

The `Complex` class contains two private data members:

```text
real : double
imag : double
```

The class includes the required constructors and overloaded operators.

### 2.2 Operator Selection

| Operator | Purpose |
|---|---|
| `+` | Complex addition |
| `-` | Complex subtraction |
| `*` | Complex multiplication |
| `/` | Complex division |
| `==` | Equality comparison |
| `<<` | Display complex number |

Arithmetic and comparison operators can be implemented as member functions.

The stream insertion operator is implemented as a friend non-member function because the left operand is an output stream.

### 2.3 Mathematical Formulas

Let:

```text
A = a + bj
B = c + dj
```

**Addition:**

```text
A + B = (a + c) + (b + d)j
```

**Subtraction:**

```text
A - B = (a - c) + (b - d)j
```

**Multiplication:**

```text
A × B = (ac - bd) + (ad + bc)j
```

**Division:**

```text
A / B =
((ac + bd) / (c² + d²))
+
((bc - ad) / (c² + d²))j
```

The denominator is:

```text
c² + d²
```

If the denominator is zero, division must be rejected safely.

**Magnitude:**

```text
|A| = √(a² + b²)
```

---

## Stage 3 – Implement and Verify (3 Marks)

### 3.1 AC Circuit Impedance

The impedance of AC circuit components is represented using complex numbers.

**Resistor:**

```text
ZR = R + 0j
```

**Inductor:**

```text
ZL = 0 + jωL
```

**Capacitor:**

```text
ZC = 0 - j/(ωC)
```

Where:

```text
ω = 2πf
```

Here:
- `R` represents resistance.
- `L` represents inductance.
- `C` represents capacitance.
- `f` represents frequency.
- `ω` represents angular frequency.

### 3.2 Series RLC Circuit

The total series impedance is:

```text
Zseries = ZR + ZL + ZC
```

Therefore:

```text
Zseries = R + j(ωL - 1/(ωC))
```

### 3.3 Parallel RLC Circuit

For a parallel RLC circuit:

```text
1/Zparallel = 1/ZR + 1/ZL + 1/ZC
```

Therefore:

```text
Zparallel = 1 / (1/ZR + 1/ZL + 1/ZC)
```

The complex division operation is used to calculate the final impedance.

### 3.4 Verification Values

The following values are used for verification:

```text
Frequency = 50 Hz
Resistance = 10 Ω
Inductance = 0.01 H
Capacitance = 100 μF
```

Angular frequency:

```text
ω = 2π × 50
ω = 314.1593 rad/s
```

Inductive reactance:

```text
XL = ωL
XL = 314.1593 × 0.01
XL = 3.1416 Ω
```

Capacitive reactance:

```text
XC = 1/(ωC)
XC = 1/(314.1593 × 0.0001)
XC = 31.8310 Ω
```

### 3.5 Series Impedance Verification

```text
Zseries = R + j(XL - XC)

Zseries = 10 + j(3.1416 - 31.8310)

Zseries = 10 - j28.6894 Ω
```

Magnitude:

```text
|Zseries| = √(10² + (-28.6894)²)

|Zseries| ≈ 30.3823 Ω
```

### 3.6 Parallel Impedance Verification

```text
Zparallel = 1 / (1/ZR + 1/ZL + 1/ZC)
```

Using the same component values:

```text
Zparallel ≈ 1.0833 + j3.1080 Ω
```

Magnitude:

```text
|Zparallel| = √(1.0833² + 3.1080²)

|Zparallel| ≈ 3.2914 Ω
```

The calculated values can be compared with the program output.

### 3.7 Division-by-Zero Test

The division operator checks whether the denominator complex number has zero magnitude.

If the denominator is zero, the operation is rejected safely using an exception.

**Actual division-by-zero test output:**  
[Record the output from your program.]

### 3.8 Program Verification

**Series impedance output:** [Record actual output.]

**Series impedance magnitude:** [Record actual output.]

**Parallel impedance output:** [Record actual output.]

**Parallel impedance magnitude:** [Record actual output.]

**Screenshot:** Insert the main Q7 program output screenshot.

---

## Stage 4 – Defend the Design (2 Marks)

### 4.1 Comparison with std::complex<double>

C++ provides the standard library class:

```cpp
std::complex<double>
```

It already supports complex-number arithmetic and other useful operations.

Comparison:

| Custom Complex Class | std::complex<double> |
|---|---|
| Operators are implemented manually | Operators are provided by the standard library |
| Helps understand complex arithmetic | Reduces implementation work |
| Allows custom formatting and validation | Provides standard complex-number operations |
| Requires manual testing and maintenance | Standard library implementation is available |

### 4.2 Why are the Constructors Needed?

The default constructor creates a zero complex number.

The one-argument constructor creates a real-only complex number.

The two-argument constructor initializes both the real and imaginary components.

These constructors make object creation convenient for different situations.

### 4.3 Why are the Operators Needed?

- `+` adds two complex numbers.
- `-` subtracts two complex numbers.
- `*` multiplies two complex numbers.
- `/` divides two complex numbers.
- `==` compares two complex numbers.
- `<<` displays the complex number.

### 4.4 Remaining Limitation

Floating-point values cannot always represent decimal numbers exactly.

Therefore, equality comparisons using `double` may require a suitable tolerance instead of exact equality.

The appropriate tolerance depends on the scale and application.

---

## Alternative Concept and Complexity Analysis

Instead of implementing a custom class, the standard library class `std::complex<double>` can be used.

It provides the required complex arithmetic operations and stores two double values.

| Operation | Custom Complex | std::complex<double> |
|---|---|---|
| Addition | O(1) | O(1) |
| Subtraction | O(1) | O(1) |
| Multiplication | O(1) | O(1) |
| Division | O(1) | O(1) |
| Magnitude | O(1) | O(1) |
| Storage | O(1) | O(1) |

Both implementations require constant time for each arithmetic operation and constant space for each complex number.

The standard library reduces implementation effort. However, application-specific division-by-zero handling, formatting and validation may still be required.

## Conclusion

The complex-number class demonstrates constructors, operator overloading and mathematical operations using real and imaginary components.

The implementation also applies complex arithmetic to series and parallel RLC circuit impedance calculations. Comparing the custom class with `std::complex<double>` demonstrates the advantages of using standard library components.

---

# OVERALL CONCLUSION

This assignment demonstrates important C++ concepts related to constructors, destructors, copy constructors, dynamic memory allocation and operator overloading.

The seven questions provide practical experience in:

1. Constructor overloading and constructor delegation.
2. Shallow copying and deep copying.
3. Dynamic constructors, destructors and RAII.
4. Arithmetic and stream operator overloading.
5. Prefix, postfix, comparison and subscript operators.
6. Rules and limitations of operator overloading.
7. Complex-number arithmetic and AC circuit impedance.

The assignment also highlights the importance of correct object initialization, independent memory ownership, safe resource management, input validation and testing.

Through implementation and experimentation, the concepts of object creation, object destruction and operator behavior become clearer.

---

# REFERENCES

1. C++ programming language concepts.
2. GNU G++ compiler documentation.
3. C++ Standard Library – `std::string`.
4. C++ Standard Library – `std::vector`.
5. C++ Standard Library – `std::complex`.
6. Assignment 3 – CO3 problem statements and course materials.


Each question is maintained in a separate folder, and the work is committed progressively.