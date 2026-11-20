#include <iostream>
#include <string>
#include <utility>

class Prescription {
private:
    std::string* medicines;
    int count;

public:
    // Constructor
    explicit Prescription(int size)
        : medicines(size > 0 ? new std::string[size] : nullptr),
          count(size > 0 ? size : 0) {
    }

    // Deep copy constructor
    Prescription(const Prescription& other)
        : medicines(other.count > 0
                        ? new std::string[other.count]
                        : nullptr),
          count(other.count) {

        for (int i = 0; i < count; ++i) {
            medicines[i] = other.medicines[i];
        }

        std::cout << "Copy constructor called.\n";
    }

    // Copy assignment operator
    Prescription& operator=(const Prescription& other) {
        if (this != &other) {
            std::string* newMedicines =
                other.count > 0
                    ? new std::string[other.count]
                    : nullptr;

            for (int i = 0; i < other.count; ++i) {
                newMedicines[i] = other.medicines[i];
            }

            delete[] medicines;
            medicines = newMedicines;
            count = other.count;
        }

        std::cout << "Copy assignment called.\n";
        return *this;
    }

    // Destructor
    ~Prescription() {
        delete[] medicines;
        std::cout << "Destructor called.\n";
    }

    void setMedicine(int index, const std::string& name) {
        if (index >= 0 && index < count) {
            medicines[index] = name;
        }
    }

    void display() const {
        for (int i = 0; i < count; ++i) {
            std::cout << medicines[i] << '\n';
        }
    }

    int size() const {
        return count;
    }
};

// Pass by value: creates a copy
void passByValue(Prescription p) {
    std::cout << "Inside pass-by-value function:\n";
    p.display();
}

// Pass by reference: does not create a copy
void passByReference(const Prescription& p) {
    std::cout << "Inside pass-by-reference function:\n";
    p.display();
}

int main() {
    std::cout << "1. Original Prescription\n";

    Prescription first(3);
    first.setMedicine(0, "Paracetamol");
    first.setMedicine(1, "Vitamin C");
    first.setMedicine(2, "Cetirizine");
    first.display();

    std::cout << "\n2. Copy Constructor\n";

    Prescription second = first;
    second.setMedicine(0, "Changed Medicine");

    std::cout << "Original:\n";
    first.display();

    std::cout << "Copied:\n";
    second.display();

    std::cout << "\n3. Copy Assignment\n";

    Prescription third(1);
    third = first;
    third.display();

    std::cout << "\n4. Self Assignment\n";

    first = first;
    first.display();

    std::cout << "\n5. Pass by Value\n";
    passByValue(first);

    std::cout << "\n6. Pass by Reference\n";
    passByReference(first);

    std::cout << "\nEnd of program.\n";

    return 0;
}