#include <iostream>
#include <string>
#include <stdexcept>

class Book {
private:
    std::string title;
    std::string author;
    int copies;

    static int validateCopies(int count) {
        if (count < 0) {
            throw std::invalid_argument(
                "Number of copies cannot be negative."
            );
        }
        return count;
    }

public:
    // Default constructor: empty record
    Book() : Book("", "Unknown", 0) {
    }

    // Constructor for donated book with only a title
    Book(const std::string& t) : Book(t, "Unknown", 1) {
    }

    // Parameterized constructor for full details
    Book(const std::string& t,
         const std::string& a,
         int count)
        : title(t), author(a), copies(validateCopies(count)) {
    }

    void display() const {
        std::cout << "Title: "
                  << (title.empty() ? "Empty" : title)
                  << "\nAuthor: " << author
                  << "\nCopies: " << copies
                  << "\n\n";
    }
};

int main() {
    try {
        std::cout << "BOOK RECORDS\n\n";

        Book emptyBook;
        Book donatedBook("The Alchemist");
        Book fullBook("Wings of Fire", "A.P.J. Abdul Kalam", 5);

        std::cout << "1. Default Constructor\n";
        emptyBook.display();

        std::cout << "2. Title Constructor\n";
        donatedBook.display();

        std::cout << "3. Parameterized Constructor\n";
        fullBook.display();

        std::cout << "4. Testing Negative Copies\n";

        Book invalidBook("Invalid Book", "Unknown", -5);
        invalidBook.display();
    }
    catch (const std::invalid_argument& error) {
        std::cout << "Error: " << error.what() << '\n';
    }

    return 0;
}