#include <iomanip>
#include <iostream>
#include <string>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 5.1: Four Ways to Reach an Object in C++
 * Demonstrates:
 *   (a) Directly by name
 *   (b) Through a pointer with ->
 *   (c) Through a reference
 *   (d) Through dynamic allocation with new and delete
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

class Cart {
    std::string customerName;
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0; // percent

public:
    Cart(const std::string& customer = "Guest") : customerName(customer) {}

    // Uses this pointer for method chaining
    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    // Disambiguates parameter name from member variable using this->
    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100.0;
    }

    void display() const {
        std::cout << "Cart [" << customerName << "] | Items: " << n 
                  << " | Discount: " << discount << "% | Total: Rs. " 
                  << std::fixed << std::setprecision(2) << total() << '\n';
    }
};

int main() {
    std::cout << "=======================================================\n";
    std::cout << " FOUR WAYS TO REACH AND MANIPULATE A CART OBJECT       \n";
    std::cout << "=======================================================\n";

    // (a) Directly by Name
    std::cout << "\n[Way A: Directly by Name]\n";
    Cart cartA("Alice");
    cartA.add("Apple", 50.0).add("Banana", 30.0).setDiscount(5.0);
    cartA.display();

    // (b) Through a Pointer with ->
    std::cout << "\n[Way B: Through Pointer with ->]\n";
    Cart* ptrB = &cartA;
    ptrB->add("Milk", 40.0);
    ptrB->display();

    // (c) Through a Reference
    std::cout << "\n[Way C: Through Reference]\n";
    Cart& refC = cartA;
    refC.add("Bread", 35.0);
    refC.display();

    // (d) Through Dynamic Allocation with new and delete
    std::cout << "\n[Way D: Heap Pointer with new and delete]\n";
    Cart* heapCart = new Cart("David");
    heapCart->add("Coffee", 150.0).add("Biscuits", 45.0).setDiscount(10.0);
    heapCart->display();
    
    // Explicit deallocation to prevent memory leak
    delete heapCart;
    heapCart = nullptr;
    std::cout << "Heap object successfully deallocated with delete.\n";

    return 0;
}
