# Task A3: Design with Abstract Classes — Hospital Payer Hierarchy

**Bloom’s Level: K5 – Evaluate | Total Marks: 5**

---

## A3.1 Payer Interface Design (3 Marks)

Hospital billing operates across diverse payment models. We define an abstract base class `Payer` establishing the contract for claim settlement:

```cpp
class Payer {
protected:
    std::string payerName;
public:
    explicit Payer(const std::string& name) : payerName(name) {}
    virtual ~Payer() = default;

    // Pure virtual function enforcing policy coverage calculation
    virtual double amountCovered(const Bill& bill) const = 0;
};
```

### Derived Payer Models:
1. **`SelfPay`:**  
   The patient assumes 100% financial liability.  
   `amountCovered(bill) = 0.0`.
2. **`Insurance`:**  
   Governed by an annual coverage ceiling (`coverLimit`) and a mandatory patient co-payment fraction (`coPayPercentage`):  
   $$\text{Eligible} = \min(\text{Billed}, \text{CoverLimit})$$  
   $$\text{AmountCovered} = \text{Eligible} \times \left(1 - \frac{\text{CoPay}\%}{100}\right)$$
3. **`GovernmentScheme`:**  
   Enforces statutory package price caps based on classification (e.g., In-patient capped at Rs. 30,000; Out-patient capped at Rs. 1,500):  
   $$\text{AmountCovered} = \min(\text{Billed}, \text{PackageCap})$$

---

## A3.2 Extension: Adding a New Payer (2 Marks)

Under the **Open/Closed Principle (OCP)**, a new payer type such as **`CorporateTieUp`** (providing negotiated institutional discounts and employer-backed credit lines) is added simply by declaring a new derived class:

```cpp
class CorporateTieUp : public Payer {
private:
    std::string corporationName;
    double negotiatedDiscountRate; // e.g. 15% discount
    double employeeCreditLine;     // e.g. Rs. 45,000 credit limit

public:
    CorporateTieUp(const std::string& corp, double discount, double credit)
        : Payer("Corporate [" + corp + "]"), corporationName(corp),
          negotiatedDiscountRate(discount), employeeCreditLine(credit) {}

    double amountCovered(const Bill& bill) const override {
        double discountedBill = bill.total() * (1.0 - (negotiatedDiscountRate / 100.0));
        return std::min(discountedBill, employeeCreditLine);
    }
};
```
### Zero Modification to Existing Code:
Existing classes (`Payer`, `SelfPay`, `Insurance`, `GovernmentScheme`, `Bill`) remain 100% untouched and require no recompilation.

---

## Alternative Concept and Complexity Analysis

### Alternative Concept: `std::function` / Function Pointers (Policy-Based Design)
Instead of class inheritance, each payer policy is represented as a callable object:
```cpp
using PayerPolicy = std::function<double(const Bill&)>;

PayerPolicy createInsurancePolicy(double limit, double coPay) {
    return [=](const Bill& b) {
        double eligible = std::min(b.total(), limit);
        return eligible * (1.0 - coPay / 100.0);
    };
}
```

### Comparative Analysis:

| Evaluation Dimension | Abstract Class Hierarchy (Our Design) | Functional Objects (`std::function`) |
| :--- | :--- | :--- |
| **Time Complexity per Call** | **$\mathcal{O}(1)$** (1 direct vtable pointer dereference) | **$\mathcal{O}(1)$** (Type-erased functor call through virtual stub) |
| **Space Complexity per Object** | **$\mathcal{O}(1)$ Optimal** (Class fields stored directly in object layout; 1 `vptr` = 8 bytes) | **$\mathcal{O}(1)$ Higher Overhead** (`std::function` object size is 32–48 bytes; capturing large lambda state forces secondary heap allocations) |
| **State Encapsulation & Identity** | **Strong**: Grouping attributes (`payerName`, `coverLimit`, `coPayPercentage`) with inspection methods (`printCoverage()`, `getName()`) is natural. | **Weak**: State is trapped inside lambda closures; inspecting or logging policy parameters requires manual custom struct wrappers. |
| **Runtime Mutability & Dynamic Extension** | High: Adding new payer types conforms strictly to class-based OOP and UML standards. | Good for simple lambdas, but cumbersome for complex multi-method contracts. |
