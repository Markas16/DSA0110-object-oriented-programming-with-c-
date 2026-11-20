#include <iostream>

class ShallowPrescription {
private:
    int* medicines;
    int count;

public:
    explicit ShallowPrescription(int size)
        : medicines(new int[size]), count(size) {
        for (int i = 0; i < count; ++i) {
            medicines[i] = 100 + i;
        }
    }

    // Compiler-generated copy constructor performs shallow copying.

    void changeFirstMedicine(int value) {
        medicines[0] = value;
    }

    void display() const {
        std::cout << "Memory address: "
                  << static_cast<const void*>(medicines) << '\n';

        std::cout << "Medicines: ";

        for (int i = 0; i < count; ++i) {
            std::cout << medicines[i] << ' ';
        }

        std::cout << '\n';
    }

    // No destructor is provided in this demonstration.
    // This avoids double deletion when demonstrating shallow copying.
};

int main() {
    ShallowPrescription first(3);

    ShallowPrescription second = first;

    std::cout << "Original object:\n";
    first.display();

    std::cout << "\nCopied object:\n";
    second.display();

    std::cout << "\nChanging the first medicine through copied object.\n";
    second.changeFirstMedicine(999);

    std::cout << "\nOriginal object after modification:\n";
    first.display();

    std::cout << "\nCopied object after modification:\n";
    second.display();

    return 0;
}