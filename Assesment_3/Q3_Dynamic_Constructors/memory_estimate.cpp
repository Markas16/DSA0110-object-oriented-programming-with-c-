#include <iostream>
#include <iomanip>

int main() {
    const int dimensions[][2] = {
        {10, 20},
        {100, 200},
        {1000, 2000}
    };

    std::cout << "SEAT MAP MEMORY ESTIMATION\n\n";

    for (const auto& dimension : dimensions) {
        long long rows = dimension[0];
        long long columns = dimension[1];

        long long seatMemory = rows * columns * sizeof(char);
        long long pointerMemory = rows * sizeof(char*);

        long long total = seatMemory + pointerMemory;

        std::cout << "Dimensions: "
                  << rows << " x " << columns << '\n';

        std::cout << "Seat memory: "
                  << seatMemory << " bytes\n";

        std::cout << "Row pointer memory: "
                  << pointerMemory << " bytes\n";

        std::cout << "Approximate total: "
                  << total << " bytes\n";

        std::cout << "Approximate total: "
                  << std::fixed << std::setprecision(2)
                  << total / 1024.0 << " KB\n\n";
    }

    std::cout << "Note: Actual allocated memory may be higher "
                 "because of allocation overhead.\n";

    return 0;
}