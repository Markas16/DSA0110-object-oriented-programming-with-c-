#include <iostream>
#include <vector>
#include <string>

class Prescription {
private:
    std::vector<std::string> medicines;

public:
    void addMedicine(const std::string& name) {
        medicines.push_back(name);
    }

    void display() const {
        for (const std::string& medicine : medicines) {
            std::cout << medicine << '\n';
        }
    }
};

int main() {
    Prescription first;

    first.addMedicine("Paracetamol");
    first.addMedicine("Vitamin C");
    first.addMedicine("Cetirizine");

    Prescription second = first;

    std::cout << "Original Prescription:\n";
    first.display();

    std::cout << "\nCopied Prescription:\n";
    second.display();

    return 0;
}