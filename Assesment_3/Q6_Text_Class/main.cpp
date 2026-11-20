#include <iostream>
#include <cstring>
#include <stdexcept>
#include <cstddef>

class Text {
private:
    std::size_t length;
    char* data;

public:
    // Default and parameterized constructor
    explicit Text(const char* value = "")
        : length(0), data(nullptr) {

        if (value == nullptr) {
            throw std::invalid_argument("Text cannot be null.");
        }

        length = std::strlen(value);
        data = new char[length + 1];

        std::memcpy(data, value, length + 1);
    }

    // Deep copy constructor
    Text(const Text& other)
        : length(other.length), data(new char[other.length + 1]) {

        std::memcpy(data, other.data, length + 1);

        std::cout << "Copy constructor called.\n";
    }

    // Copy assignment operator
    Text& operator=(const Text& other) {
        if (this != &other) {
            char* newData = new char[other.length + 1];

            std::memcpy(newData, other.data, other.length + 1);

            delete[] data;

            data = newData;
            length = other.length;
        }

        std::cout << "Copy assignment called.\n";

        return *this;
    }

    // Destructor
    ~Text() {
        delete[] data;
    }

    // Concatenation assignment
    Text& operator+=(const Text& other) {
        char* newData = new char[length + other.length + 1];

        std::memcpy(newData, data, length);
        std::memcpy(newData + length, other.data,
                    other.length + 1);

        delete[] data;

        data = newData;
        length += other.length;

        return *this;
    }

    // Concatenation operator
    Text operator+(const Text& other) const {
        Text result(*this);
        result += other;
        return result;
    }

    // Equality operator
    bool operator==(const Text& other) const {
        return std::strcmp(data, other.data) == 0;
    }

    // Inequality operator
    bool operator!=(const Text& other) const {
        return !(*this == other);
    }

    // Output operator
    friend std::ostream& operator<<(std::ostream& out,
                                    const Text& text) {
        out << text.data;
        return out;
    }

    std::size_t size() const {
        return length;
    }
};

int main() {
    std::cout << "DYNAMIC TEXT CLASS\n\n";

    // Constructor
    Text first("Hello");
    Text second(" World");

    std::cout << "First text: " << first << '\n';
    std::cout << "Second text: " << second << '\n';

    // Copy constructor
    std::cout << "\n1. Deep Copy Constructor\n";

    Text copied = first;

    std::cout << "Copied text: " << copied << '\n';

    // Modify the original through concatenation
    Text extra(" C++");
    first += extra;

    std::cout << "Modified original: " << first << '\n';
    std::cout << "Copied text remains: " << copied << '\n';

    // Copy assignment
    std::cout << "\n2. Copy Assignment\n";

    Text assigned;
    assigned = second;

    std::cout << "Assigned text: " << assigned << '\n';

    // Self assignment
    std::cout << "\n3. Self Assignment\n";

    assigned = assigned;

    std::cout << "After self assignment: " << assigned << '\n';

    // + operator
    std::cout << "\n4. Addition Operator\n";

    Text combined = first + second;

    std::cout << "Combined text: " << combined << '\n';

    // Comparison operators
    std::cout << "\n5. Comparison Operators\n";

    Text a("FASTGO");
    Text b("FASTGO");
    Text c("Business");

    std::cout << "a == b: " << (a == b) << '\n';
    std::cout << "a != c: " << (a != c) << '\n';

    // Empty text
    std::cout << "\n6. Empty Text\n";

    Text empty;

    std::cout << "Empty text: [" << empty << "]\n";
    std::cout << "Empty text length: " << empty.size() << '\n';

    // Long text
    std::cout << "\n7. Long Text\n";

    Text longText(
        "Dynamic memory allocation and deep copying "
        "in C++ programming."
    );

    std::cout << longText << '\n';
    std::cout << "Length: " << longText.size() << '\n';

    std::cout << "\nEnd of program.\n";

    return 0;
}