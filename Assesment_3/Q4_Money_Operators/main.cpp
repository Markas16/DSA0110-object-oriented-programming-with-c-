#include <iostream>
#include <iomanip>
#include <stdexcept>

class Money {
private:
    long long paise;

public:
    explicit Money(long long p = 0) : paise(p) {
        if (p < 0) {
            throw std::invalid_argument("Money cannot be negative.");
        }
    }

    Money operator+(const Money& other) const {
        return Money(paise + other.paise);
    }

    bool operator==(const Money& other) const {
        return paise == other.paise;
    }

    friend std::ostream& operator<<(std::ostream& out,
                                    const Money& money) {
        out << "Rs. " << money.paise / 100
            << '.' << std::setw(2) << std::setfill('0')
            << money.paise % 100;
        return out;
    }

    friend std::istream& operator>>(std::istream& in,
                                    Money& money) {
        long long rupees;
        int cents;

        std::cout << "Enter rupees and paise separately: ";

        if (in >> rupees >> cents) {
            if (rupees < 0 || cents < 0 || cents > 99) {
                in.setstate(std::ios::failbit);
            } else {
                money.paise = rupees * 100 + cents;
            }
        }

        return in;
    }
};

int main() {
    std::cout << "MONEY OPERATOR OVERLOADING\n\n";

    Money first(1250);
    Money second(275);

    std::cout << "First amount: " << first << '\n';
    std::cout << "Second amount: " << second << '\n';

    Money total = first + second;

    std::cout << "\nAddition result: " << total << '\n';

    Money third(1250);

    std::cout << "\nEquality comparison:\n";

    if (first == third) {
        std::cout << "First and third amounts are equal.\n";
    } else {
        std::cout << "Amounts are not equal.\n";
    }

    std::cout << "\nPaise carry demonstration:\n";

    Money amount1(199);
    Money amount2(2);

    std::cout << amount1 << " + " << amount2
              << " = " << amount1 + amount2 << '\n';

    std::cout << "\nInput operator demonstration:\n";

    Money entered;

    if (std::cin >> entered) {
        std::cout << "Entered amount: " << entered << '\n';
    } else {
        std::cout << "Invalid money input.\n";
    }

    return 0;
}