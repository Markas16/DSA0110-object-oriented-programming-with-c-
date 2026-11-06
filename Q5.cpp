#include <iostream>
#include <string>
using namespace std;

class Token {
    static int next;
    int id;

public:
    Token() : id(++next) {}
    int get() const { return id; }
    static int issued() { return next - 100; }
};

int Token::next = 100;

class BusTicket {
    static int issued;
    static const int CAPACITY = 3;
    int seat;
    string name;

public:
    explicit BusTicket(const string& n) : seat(0), name(n) {
        if (issued < CAPACITY) {
            ++issued;
            seat = issued;
        }
    }

    static int count() {
        return issued;
    }

    void print() const {
        cout << name << ": "
             << (seat ? "Seat " + to_string(seat) : string("Waiting"))
             << '\n';
    }
};

int BusTicket::issued = 0;

void createTickets() {
    BusTicket tickets[] = {
        BusTicket("Ezhil"),
        BusTicket("Farah")
    };
    for (const BusTicket& ticket : tickets) ticket.print();
    cout << "Issued: " << BusTicket::count() << '\n';
}

int main() {
    Token counter[3];
    Token extra;
    cout << counter[0].get() << ' ' << counter[2].get() << ' '
         << extra.get() << ' ' << Token::issued() << '\n';

    BusTicket trip[4] = {
        BusTicket("Asha"), BusTicket("Bala"),
        BusTicket("Chitra"), BusTicket("Dev")
    };

    for (const BusTicket& ticket : trip) ticket.print();
    cout << "Issued: " << BusTicket::count() << '\n';

    createTickets();
    createTickets();
    return 0;
}
