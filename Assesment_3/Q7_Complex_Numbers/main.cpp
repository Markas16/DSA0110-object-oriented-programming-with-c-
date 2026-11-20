#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdexcept>

const double PI = 3.14159265358979323846;

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0.0, double i = 0.0)
        : real(r), imag(i) {
    }

    Complex operator+(const Complex& other) const {
        return Complex(
            real + other.real,
            imag + other.imag
        );
    }

    Complex operator-(const Complex& other) const {
        return Complex(
            real - other.real,
            imag - other.imag
        );
    }

    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    Complex operator/(const Complex& other) const {
        double denominator =
            other.real * other.real +
            other.imag * other.imag;

        if (denominator == 0.0) {
            throw std::runtime_error(
                "Division by zero complex number."
            );
        }

        return Complex(
            (real * other.real + imag * other.imag)
                / denominator,

            (imag * other.real - real * other.imag)
                / denominator
        );
    }

    double magnitude() const {
        return std::sqrt(real * real + imag * imag);
    }

    double phase() const {
        return std::atan2(imag, real) * 180.0 / PI;
    }

    friend std::ostream& operator<<(std::ostream& out,
                                    const Complex& value) {
        out << std::fixed << std::setprecision(2)
            << value.real;

        if (value.imag >= 0) {
            out << " + " << value.imag << "i";
        } else {
            out << " - " << -value.imag << "i";
        }

        return out;
    }
};

// Series RLC impedance
Complex seriesRLC(double resistance,
                  double inductance,
                  double capacitance,
                  double frequency) {

    double omega = 2.0 * PI * frequency;

    double inductiveReactance = omega * inductance;
    double capacitiveReactance = 1.0 / (omega * capacitance);

    return Complex(
        resistance,
        inductiveReactance - capacitiveReactance
    );
}

// Parallel RLC impedance
Complex parallelRLC(double resistance,
                    double inductance,
                    double capacitance,
                    double frequency) {

    Complex zR(resistance, 0.0);

    double omega = 2.0 * PI * frequency;

    Complex zL(0.0, omega * inductance);

    Complex zC(0.0, -1.0 / (omega * capacitance));

    Complex one(1.0, 0.0);

    Complex totalAdmittance =
        one / zR + one / zL + one / zC;

    return one / totalAdmittance;
}

int main() {
    try {
        std::cout << "COMPLEX NUMBER OPERATIONS\n\n";

        Complex first(4.0, 3.0);
        Complex second(2.0, 1.0);

        std::cout << "First complex number: "
                  << first << '\n';

        std::cout << "Second complex number: "
                  << second << '\n';

        std::cout << "\nARITHMETIC OPERATIONS\n";

        std::cout << "Addition: "
                  << first + second << '\n';

        std::cout << "Subtraction: "
                  << first - second << '\n';

        std::cout << "Multiplication: "
                  << first * second << '\n';

        std::cout << "Division: "
                  << first / second << '\n';

        std::cout << "\nMAGNITUDE AND PHASE\n";

        std::cout << "Magnitude: "
                  << first.magnitude() << '\n';

        std::cout << "Phase angle: "
                  << first.phase() << " degrees\n";

        std::cout << "\nSERIES RLC CIRCUIT\n";

        double resistance = 100.0;
        double inductance = 0.1;
        double capacitance = 100e-6;
        double frequency = 50.0;

        Complex series = seriesRLC(
            resistance,
            inductance,
            capacitance,
            frequency
        );

        std::cout << "Resistance: "
                  << resistance << " ohms\n";

        std::cout << "Inductance: "
                  << inductance << " H\n";

        std::cout << "Capacitance: "
                  << capacitance << " F\n";

        std::cout << "Frequency: "
                  << frequency << " Hz\n";

        std::cout << "Series impedance: "
                  << series << " ohms\n";

        std::cout << "Magnitude: "
                  << series.magnitude() << " ohms\n";

        std::cout << "Phase: "
                  << series.phase() << " degrees\n";

        std::cout << "\nPARALLEL RLC CIRCUIT\n";

        Complex parallel = parallelRLC(
            resistance,
            inductance,
            capacitance,
            frequency
        );

        std::cout << "Parallel impedance: "
                  << parallel << " ohms\n";

        std::cout << "Magnitude: "
                  << parallel.magnitude() << " ohms\n";

        std::cout << "Phase: "
                  << parallel.phase() << " degrees\n";
    }
    catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << '\n';
    }

    return 0;
}