#include <iostream>
#include <stdexcept>

class SeatMap {
private:
    int rows;
    int columns;
    char** seats;

public:
    SeatMap(int r, int c)
        : rows(r), columns(c), seats(nullptr) {

        if (r < 0 || c < 0) {
            throw std::invalid_argument("Invalid dimensions");
        }

        if (rows > 0 && columns > 0) {
            seats = new char*[rows]{};

            try {
                for (int i = 0; i < rows; ++i) {
                    seats[i] = new char[columns];
                    for (int j = 0; j < columns; ++j) {
                        seats[i][j] = 'O';
                    }
                }
            }
            catch (...) {
                for (int i = 0; i < rows; ++i) {
                    delete[] seats[i];
                }
                delete[] seats;
                seats = nullptr;
                throw;
            }
        }

        std::cout << "SeatMap constructor: "
                  << rows << " x " << columns << '\n';
    }

    ~SeatMap() {
        if (seats != nullptr) {
            for (int i = 0; i < rows; ++i) {
                delete[] seats[i];
            }
            delete[] seats;
        }

        std::cout << "SeatMap destructor called.\n";
    }

    void display() const {
        std::cout << "Seat arrangement:\n";

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                std::cout << seats[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }

    void reserveSeat(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= columns) {
            std::cout << "Invalid seat position.\n";
            return;
        }

        seats[r][c] = 'X';
    }
};

int main() {
    std::cout << "1. Stack Object\n";

    SeatMap first(2, 3);
    first.reserveSeat(0, 1);
    first.display();

    std::cout << "\n2. Block Scope Object\n";

    {
        SeatMap second(2, 2);
        second.reserveSeat(1, 1);
        second.display();
        std::cout << "Leaving block scope.\n";
    }

    std::cout << "\n3. Heap Object\n";

    SeatMap* third = new SeatMap(3, 3);
    third->reserveSeat(1, 1);
    third->display();

    delete third;

    std::cout << "\n4. Zero Dimension Object\n";

    SeatMap empty(0, 0);
    empty.display();

    std::cout << "\nEnd of main function.\n";

    return 0;
}