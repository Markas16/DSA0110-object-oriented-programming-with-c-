#include <iostream>

class LeakySeatMap {
private:
    int rows;
    char** seats;

public:
    explicit LeakySeatMap(int rowCount)
        : rows(rowCount), seats(new char*[rowCount]{}) {

        for (int i = 0; i < rows; ++i) {
            seats[i] = new char[20]{};
        }

        std::cout << "Seat map created: "
                  << rows << " rows.\n";
    }

    // Intentionally no destructor.
    // This program demonstrates missing memory cleanup.
};

int main() {
    LeakySeatMap event(10);

    std::cout << "Program ending without releasing allocated memory.\n";

    return 0;
}