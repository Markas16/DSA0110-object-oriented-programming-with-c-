#include <iostream>

class Book {
private:
    int copies;

public:
    explicit Book(int count) : copies(count) {
    }

    void display() const {
        std::cout << "Number of copies: " << copies << '\n';
    }
};

int main() {
    Book first(5);  // Direct initialization works

    // Book second = 10;
    // Uncommenting this line produces a compilation error
    // because the constructor is explicit.

    first.display();

    return 0;
}