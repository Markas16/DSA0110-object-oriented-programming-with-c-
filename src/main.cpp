#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include "fare_system.hpp"

using namespace MetroSystem;

void displayMenu() {
    std::cout << "\n======================================================\n";
    std::cout << "  CITY METRO SMART-CARD SYSTEM (DSA01 Assignment 1)\n";
    std::cout << "  Student: Markas | Register No: 192421250\n";
    std::cout << "======================================================\n";
    std::cout << " 1. Check Smart-Card Balance\n";
    std::cout << " 2. Tap & Travel (Record a Metro Journey)\n";
    std::cout << " 3. Recharge Smart-Card Balance\n";
    std::cout << " 4. View Journey Audit Log\n";
    std::cout << " 5. Exit\n";
    std::cout << "------------------------------------------------------\n";
    std::cout << "Select option [1-5]: ";
}

int main() {
    SmartCard card(192421250ULL, 10000LL); // Initial balance Rs 100.00 (10,000 paise)

    std::cout << "Welcome to the City Metro Automated Fare Gate System!\n";
    std::cout << "Smart-Card initialized for Register No: 192421250\n";
    std::cout << "Initial Prepaid Balance: " << FareEngine::formatRupees(card.getBalancePaise()) << "\n";

    int choice = 0;
    while (true) {
        displayMenu();
        if (!(std::cin >> choice)) {
            // End of stream or bad token
            break;
        }

        if (choice == 1) {
            std::cout << "\n>>> Card ID: #" << card.getCardId() << "\n";
            std::cout << ">>> Current Balance: " << FareEngine::formatRupees(card.getBalancePaise()) 
                      << " (" << card.getBalancePaise() << " paise)\n";
            std::cout << ">>> Minimum Required Balance: Rs 50.00\n";
        } else if (choice == 2) {
            std::string zoneStr;
            int hour = 0;
            char weekendChar = 'n';

            std::cout << "\nEnter Destination Zone (Inner, Middle, Outer, Airport): ";
            std::cin >> zoneStr;
            Zone zone = stringToZone(zoneStr);
            if (zone == Zone::Unknown) {
                std::cout << "[Error] Unknown zone entered! Valid zones: Inner, Middle, Outer, Airport.\n";
                continue;
            }

            std::cout << "Enter Travel Hour [0 - 23]: ";
            if (!(std::cin >> hour) || hour < 0 || hour > 23) {
                std::cout << "[Error] Invalid hour! Must be between 0 and 23.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            std::cout << "Is it weekend travel? (y/n): ";
            std::cin >> weekendChar;
            bool isWeekend = (weekendChar == 'y' || weekendChar == 'Y');

            int64_t expectedFare = FareEngine::calculateFare(zone, hour, isWeekend);
            std::cout << "Calculated Fare: " << FareEngine::formatRupees(expectedFare) << "\n";

            bool success = card.processTrip(zone, hour, isWeekend);
            if (success) {
                std::cout << ">>> GATES OPEN! Journey approved. Remaining Balance: " 
                          << FareEngine::formatRupees(card.getBalancePaise()) << "\n";
            } else {
                std::cout << ">>> GATES LOCKED! Journey denied. Card must maintain at least Rs 50.00 post-trip.\n";
            }
        } else if (choice == 3) {
            int64_t amountPaise = 0;
            std::cout << "\nEnter recharge amount in paise (e.g. 5000 for Rs 50.00): ";
            if (std::cin >> amountPaise) {
                if (card.recharge(amountPaise)) {
                    std::cout << ">>> Recharge Successful! New Balance: " 
                              << FareEngine::formatRupees(card.getBalancePaise()) << "\n";
                } else {
                    std::cout << ">>> Recharge Failed: amount must be strictly positive.\n";
                }
            } else {
                std::cout << "[Error] Invalid input for recharge amount.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } else if (choice == 4) {
            card.printReport();
        } else if (choice == 5) {
            std::cout << "\nThank you for using City Metro Smart-Card System. Safe travels!\n";
            break;
        } else {
            std::cout << "[Error] Invalid option! Please select between 1 and 5.\n";
        }
    }

    return 0;
}
