#include <iostream>
#include <cstring>
#include <string>
#include <chrono>
#include <cstddef>

class Text {
private:
    std::size_t length;
    char* data;

public:
    explicit Text(const char* value = "")
        : length(std::strlen(value)), data(new char[length + 1]) {

        std::memcpy(data, value, length + 1);
    }

    Text(const Text& other)
        : length(other.length), data(new char[length + 1]) {

        std::memcpy(data, other.data, length + 1);
    }

    Text& operator=(const Text& other) {
        if (this != &other) {
            char* newData = new char[other.length + 1];

            std::memcpy(newData, other.data, other.length + 1);

            delete[] data;

            data = newData;
            length = other.length;
        }

        return *this;
    }

    ~Text() {
        delete[] data;
    }

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

    std::size_t size() const {
        return length;
    }
};

int main() {
    const int iterations = 100000;

    using Clock = std::chrono::steady_clock;

    std::cout << "TEXT AND STRING BENCHMARK\n";
    std::cout << "Iterations: " << iterations << "\n\n";

    Text customText("A");
    Text addition("B");

    auto customStart = Clock::now();

    for (int i = 0; i < iterations; ++i) {
        customText += addition;
    }

    auto customEnd = Clock::now();

    std::string standardText = "A";

    auto stringStart = Clock::now();

    for (int i = 0; i < iterations; ++i) {
        standardText += "B";
    }

    auto stringEnd = Clock::now();

    double customTime =
        std::chrono::duration<double, std::milli>(
            customEnd - customStart
        ).count();

    double stringTime =
        std::chrono::duration<double, std::milli>(
            stringEnd - stringStart
        ).count();

    std::cout << "Custom Text length: "
              << customText.size() << '\n';

    std::cout << "std::string length: "
              << standardText.size() << '\n';

    std::cout << "Custom Text time: "
              << customTime << " ms\n";

    std::cout << "std::string time: "
              << stringTime << " ms\n";

    return 0;
}