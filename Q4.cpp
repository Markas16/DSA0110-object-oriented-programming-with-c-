#include <iostream>
#include <iomanip>
using namespace std;

class SavingsAccount {
private:
    long long accountNumber;
    double balance;
    double transactions[10];
    int count;

    void addTransaction(double amount) {
        if (count < 10) {
            transactions[count++] = amount;
        } else {
            for (int i = 1; i < 10; ++i) transactions[i - 1] = transactions[i];
            transactions[9] = amount;
        }
    }

public:
    SavingsAccount(long long number, double initialBalance)
        : accountNumber(number), balance(initialBalance), count(0) {}

    void deposit(double amount);
    bool withdraw(double amount);
    void miniStatement() const;
    double getBalance() const { return balance; }
};

void SavingsAccount::deposit(double amount) {
    if (amount <= 0) return;
    balance += amount;
    addTransaction(amount);
}

bool SavingsAccount::withdraw(double amount) {
    if (amount <= 0 || amount > balance) return false;
    balance -= amount;
    addTransaction(-amount);
    return true;
}

void SavingsAccount::miniStatement() const {
    cout << "Account: " << accountNumber << '\n';
    cout << fixed << setprecision(2);
    cout << "Balance: " << balance << '\n';
    cout << "Last transactions:\n";
    for (int i = 0; i < count; ++i) {
        cout << transactions[i] << '\n';
    }
}

int main() {
    SavingsAccount account(10001, 5000.0);
    account.deposit(1000);
    account.withdraw(500);
    account.deposit(250);
    account.withdraw(100);
    account.deposit(750);
    account.withdraw(200);
    account.deposit(300);
    account.withdraw(150);
    account.deposit(400);
    account.withdraw(50);
    account.deposit(900);
    account.miniStatement();
    return 0;
}
