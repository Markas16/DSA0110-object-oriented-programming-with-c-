#include <iostream>
#include <string>
using namespace std;

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};

double charge(double weightKg, bool express = false, bool insured = false) {
    double amount = weightKg * 50.0;
    if (express) amount += 30.0;
    if (insured) amount += 20.0;
    return amount;
}

double charge(int weightGrams) {
    return charge(weightGrams / 1000.0, false, false);
}

double charge(const Parcel& parcel) {
    return charge(parcel.weightKg, parcel.express, parcel.insured);
}

int main() {
    cout << "charge(2): " << charge(2) << '\n';
    cout << "charge(2.0): " << charge(2.0) << '\n';
    cout << "charge(2.0f): " << charge(static_cast<double>(2.0f)) << '\n';
    cout << "charge(2000): " << charge(2000) << '\n';
    cout << "charge(2.0, true): " << charge(2.0, true) << '\n';

    Parcel p{2.5, true, true};
    cout << "charge(Parcel): " << charge(p) << '\n';
    return 0;
}
