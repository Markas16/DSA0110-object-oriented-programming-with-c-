#include <iostream>

class Book {
private:
    int copies;
    int shelfCode;

public:
    Book() : shelfCode(101), copies(5) {
    }

    void display() const {
        std::cout << "Copies: " << copies << '\n';
        std::cout << "Shelf Code: " << shelfCode << '\n';
    }
};

int main() {
    Book b;
    b.display();

    return 0;
}