#include <iostream>
#include <iomanip>
using namespace std;

// Original design: shifting the array when it is full.
class ShiftingHistory {
    static const int CAPACITY = 10;
    double data[CAPACITY];
    int count;

public:
    ShiftingHistory() : count(0) {}

    void add(double value) {
        if (count < CAPACITY) {
            data[count++] = value;
        } else {
            for (int i = 1; i < CAPACITY; ++i)
                data[i - 1] = data[i];
            data[CAPACITY - 1] = value;
        }
    }

    void print() const {
        for (int i = 0; i < count; ++i)
            cout << data[i] << ' ';
        cout << '\n';
    }
};

// Redesigned solution: circular buffer; insertion is O(1).
class CircularHistory {
    static const int CAPACITY = 10;
    double data[CAPACITY];
    int count;
    int next;

public:
    CircularHistory() : count(0), next(0) {}

    void add(double value) {
        data[next] = value;
        next = (next + 1) % CAPACITY;
        if (count < CAPACITY) ++count;
    }

    void print() const {
        int start = (next - count + CAPACITY) % CAPACITY;
        for (int i = 0; i < count; ++i)
            cout << data[(start + i) % CAPACITY] << ' ';
        cout << '\n';
    }
};

int main() {
    ShiftingHistory original;
    CircularHistory redesigned;

    for (int i = 1; i <= 15; ++i) {
        original.add(i * 100.0);
        redesigned.add(i * 100.0);
    }

    cout << fixed << setprecision(0);
    cout << "Original (last 10): ";
    original.print();

    cout << "Redesigned (last 10): ";
    redesigned.print();

    return 0;
}
