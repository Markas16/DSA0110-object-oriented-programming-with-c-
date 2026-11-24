#include <iomanip>
#include <iostream>
#include <string>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 5.2: Array of Pointers to Cart Objects
 * Finding the Cart with the Highest Total using Pointer Arithmetic & Indexing
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

    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100.0;
    }

    const std::string& getCustomerName() const { return customerName; }

    void display() const {
        std::cout << "Customer: " << std::left << std::setw(12) << customerName 
                  << " | Items: " << n 
                  << " | Discount: " << std::right << std::setw(2) << static_cast<int>(discount) << "%"
                  << " | Total: Rs. " << std::fixed << std::setprecision(2) << total() << '\n';
    }
};

int main() {
    std::cout << "=======================================================\n";
    std::cout << " ACTIVE CUSTOMER CARTS - ARRAY OF POINTERS SEARCH      \n";
    std::cout << "=======================================================\n";

    const int NUM_CUSTOMERS = 4;
    Cart* activeCarts[NUM_CUSTOMERS];

    // Allocate dynamic cart objects for 4 customers
    activeCarts[0] = new Cart("Rohan");
    activeCarts[0]->add("Atta", 320.0).add("Sugar", 45.0).setDiscount(5.0); // Total: (365)*0.95 = 346.75

    activeCarts[1] = new Cart("Divya");
    activeCarts[1]->add("Olive Oil", 650.0).add("Almonds", 450.0).setDiscount(15.0); // Total: (1100)*0.85 = 935.00

    activeCarts[2] = new Cart("Ganesh");
    activeCarts[2]->add("Rice", 1200.0).add("Ghee", 550.0).setDiscount(10.0); // Total: (1750)*0.90 = 1575.00

    activeCarts[3] = new Cart("Meera");
    activeCarts[3]->add("Tea", 180.0).add("Milk", 60.0).setDiscount(0.0); // Total: 240.00

    std::cout << "\n--- Current Active Carts in System ---\n";
    for (int i = 0; i < NUM_CUSTOMERS; ++i) {
        activeCarts[i]->display();
    }

    // Task 5.2: Finding cart with highest total using pointer arithmetic & indexing
    Cart** highestCartPtr = activeCarts; // Pointer to pointer (pointer arithmetic)
    double maxTotal = (*highestCartPtr)->total();

    for (Cart** it = activeCarts + 1; it < activeCarts + NUM_CUSTOMERS; ++it) {
        double currentTotal = (*it)->total();
        if (currentTotal > maxTotal) {
            maxTotal = currentTotal;
            highestCartPtr = it;
        }
    }

    std::cout << "\n-------------------------------------------------------\n";
    std::cout << "Cart with Highest Total (Located via Pointer Arithmetic):\n";
    (*highestCartPtr)->display();
    std::cout << "Winner: " << (*highestCartPtr)->getCustomerName() 
              << " with billing total Rs. " << std::fixed << std::setprecision(2) << maxTotal << '\n';
    std::cout << "-------------------------------------------------------\n";

    // Clean up heap memory
    for (int i = 0; i < NUM_CUSTOMERS; ++i) {
        delete activeCarts[i];
        activeCarts[i] = nullptr;
    }
    std::cout << "All active customer cart memory safely deallocated.\n";

    return 0;
}
