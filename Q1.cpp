#include <iostream>
using namespace std;

void change(int a, int& b, int* c) {
    a += 10;
    b += 10;
    if (c != nullptr) *c += 10;
}

int& bigger(int& p, int& q) {
    return (p > q) ? p : q;
}

void show(int x, int y = 5, int z = 10) {
    cout << x + y + z << ' ';
}

void applyByValue(long long fee, int percent) {
    fee -= fee * percent / 100;
}

void applyByReference(long long& fee, int percent) {
    fee -= fee * percent / 100;
}

void applyByPointer(long long* fee, int percent) {
    if (fee != nullptr) *fee -= *fee * percent / 100;
}

long long applyAndReturn(long long fee, int percent) {
    fee -= fee * percent / 100;
    return fee;
}

int main() {
    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    cout << a << ' ' << b << ' ' << c << '\n';

    bigger(a, c) = 100;
    cout << a << ' ' << c << '\n';

    show(1);
    show(1, 2);
    show(1, 2, 3);
    cout << '\n';

    long long fee1 = 45000, fee2 = 45000, fee3 = 45000;
    applyByValue(fee1, 10);
    applyByReference(fee2, 10);
    applyByPointer(&fee3, 10);

    cout << "after call by value : " << fee1 << '\n';
    cout << "after call by reference: " << fee2 << '\n';
    cout << "after call by pointer: " << fee3 << '\n';

    fee1 = applyAndReturn(fee1, 10);
    cout << "after return by value: " << fee1 << '\n';
    return 0;
}
