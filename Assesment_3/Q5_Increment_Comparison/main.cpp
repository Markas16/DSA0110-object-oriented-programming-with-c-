#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

class Time {
private:
    int seconds;

public:
    explicit Time(int totalSeconds = 0) : seconds(totalSeconds) {
        if (seconds < 0) {
            throw std::invalid_argument("Time cannot be negative.");
        }
    }

    Time& operator++() {
        ++seconds;
        return *this;
    }

    Time operator++(int) {
        Time old = *this;
        ++seconds;
        return old;
    }

    bool operator==(const Time& other) const {
        return seconds == other.seconds;
    }

    bool operator<(const Time& other) const {
        return seconds < other.seconds;
    }

    friend std::ostream& operator<<(std::ostream& out,
                                    const Time& time) {
        int hours = time.seconds / 3600;
        int minutes = (time.seconds % 3600) / 60;
        int secs = time.seconds % 60;

        out << hours << ':';

        if (minutes < 10) out << '0';
        out << minutes << ':';

        if (secs < 10) out << '0';
        out << secs;

        return out;
    }
};

class Playlist {
private:
    std::vector<std::string> songs;

public:
    void addSong(const std::string& song) {
        songs.push_back(song);
    }

    std::string& operator[](int index) {
        if (index < 0 || index >= static_cast<int>(songs.size())) {
            throw std::out_of_range("Playlist index out of range.");
        }

        return songs[index];
    }

    const std::string& operator[](int index) const {
        if (index < 0 || index >= static_cast<int>(songs.size())) {
            throw std::out_of_range("Playlist index out of range.");
        }

        return songs[index];
    }

    int size() const {
        return static_cast<int>(songs.size());
    }
};

int main() {
    std::cout << "TIME OPERATOR OVERLOADING\n\n";

    Time t(3599);

    std::cout << "Initial time: " << t << '\n';

    Time old = t++;
    std::cout << "Postfix returned value: " << old << '\n';
    std::cout << "After postfix: " << t << '\n';

    Time& updated = ++t;
    std::cout << "Prefix returned value: " << updated << '\n';
    std::cout << "After prefix: " << t << '\n';

    std::cout << "\nTIME ROLLOVER TEST\n";

    Time rollover(3599);

    std::cout << "Before increment: " << rollover << '\n';
    ++rollover;
    std::cout << "After increment: " << rollover << '\n';

    std::cout << "\nCOMPARISON OPERATORS\n";

    Time first(3600);
    Time second(3600);
    Time third(4000);

    std::cout << "first == second: "
              << (first == second) << '\n';

    std::cout << "first < third: "
              << (first < third) << '\n';

    std::cout << "\nPLAYLIST TEST\n";

    Playlist playlist;

    playlist.addSong("Song A");
    playlist.addSong("Song B");
    playlist.addSong("Song C");

    std::cout << "First song: " << playlist[0] << '\n';

    playlist[1] = "Updated Song B";

    std::cout << "Updated second song: " << playlist[1] << '\n';

    const Playlist& constPlaylist = playlist;

    std::cout << "Const playlist third song: "
              << constPlaylist[2] << '\n';

    std::cout << "\nINVALID INDEX TESTS\n";

    try {
        std::cout << playlist[-1] << '\n';
    }
    catch (const std::out_of_range& error) {
        std::cout << "Negative index: " << error.what() << '\n';
    }

    try {
        std::cout << playlist[playlist.size()] << '\n';
    }
    catch (const std::out_of_range& error) {
        std::cout << "End index: " << error.what() << '\n';
    }

    return 0;
}