#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

using Clock = std::chrono::steady_clock;

class Prescription {
private:
    int* medicines;
    int count;

public:
    explicit Prescription(int size)
        : medicines(size > 0 ? new int[size] : nullptr),
          count(size) {

        for (int i = 0; i < count; ++i) {
            medicines[i] = i;
        }
    }

    Prescription(const Prescription& other)
        : medicines(other.count > 0
                        ? new int[other.count]
                        : nullptr),
          count(other.count) {

        std::copy(other.medicines,
                  other.medicines + count,
                  medicines);
    }

    ~Prescription() {
        delete[] medicines;
    }

    int size() const {
        return count;
    }

    int first() const {
        return count > 0 ? medicines[0] : 0;
    }
};

template <typename Function>
double measure(Function function) {
    auto start = Clock::now();

    function();

    auto end = Clock::now();

    return std::chrono::duration<double, std::milli>(
        end - start
    ).count();
}

int main() {
    const int sizes[] = {10, 1000, 100000};

    for (int size : sizes) {
        Prescription original(size);

        std::vector<int> vectorOriginal(size);

        for (int i = 0; i < size; ++i) {
            vectorOriginal[i] = i;
        }

        volatile long long checksum = 0;

        double customTime = measure([&]() {
            for (int i = 0; i < 100; ++i) {
                Prescription copy = original;
                checksum += copy.first();
            }
        });

        double vectorTime = measure([&]() {
            for (int i = 0; i < 100; ++i) {
                std::vector<int> copy = vectorOriginal;
                checksum += copy.empty() ? 0 : copy[0];
            }
        });

        std::cout << "\nMedicines: " << size << '\n';
        std::cout << "Custom deep copy: "
                  << customTime << " ms\n";

        std::cout << "Vector copy: "
                  << vectorTime << " ms\n";

        std::cout << "Checksum: " << checksum << '\n';
    }

    return 0;
}