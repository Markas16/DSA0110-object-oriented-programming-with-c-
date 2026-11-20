#include <iostream>

class Book {
private:
    int copies;

public:
    Book(int count) : copies(count) {
    }

    void display() const {
        std::cout << "Copies: " << copies << '\n';
    }
};

int main() {
    Book books[3];

    return 0;
}