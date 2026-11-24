#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>

/**
 * ============================================================================
 * Course: Object Oriented Programming with C++ (DSA01)
 * Assignment 4 - Task 6: Vehicle Rental Fleet Management System
 * Bloom's Level: K5 – Evaluate
 * Compiler: g++ -std=c++17 -Wall -Wextra
 * ============================================================================
 */

// Member Class: Date (Composition)
class Date {
private:
    int day;
    int month;
    int year;

public:
    Date(int d = 1, int m = 1, int y = 2026) : day(d), month(m), year(y) {}

    std::string toString() const {
        std::string dStr = (day < 10 ? "0" : "") + std::to_string(day);
        std::string mStr = (month < 10 ? "0" : "") + std::to_string(month);
        return dStr + "/" + mStr + "/" + std::to_string(year);
    }
};

// Abstract Base Class: Vehicle
class Vehicle {
protected:
    std::string regNo;
    double dailyRate;
    bool isRented;
    std::string renterName;
    int bookedDays;

public:
    Vehicle(const std::string& reg, double rate)
        : regNo(reg), dailyRate(rate), isRented(false), renterName(""), bookedDays(0) {}

    virtual ~Vehicle() = default; // Essential virtual destructor

    // Pure virtual functions defining common polymorphic interface
    virtual double calculateCharge(int actualDays, double returnFuelOrKWh) const = 0;
    virtual std::string getType() const = 0;

    // Renting functionality
    virtual bool rentVehicle(const std::string& customer, int days) {
        if (isRented) {
            std::cout << "[ERROR] Vehicle " << regNo << " is already rented to " << renterName << "!\n";
            return false;
        }
        isRented = true;
        renterName = customer;
        bookedDays = days;
        std::cout << "[RENTED] " << getType() << " [" << regNo << "] rented to " 
                  << renterName << " for " << bookedDays << " days.\n";
        return true;
    }

    // Returning functionality with late penalty fee calculation
    virtual double returnVehicle(int actualDays, double returnFuelOrKWh) {
        if (!isRented) {
            std::cout << "[ERROR] Vehicle " << regNo << " is not currently rented!\n";
            return 0.0;
        }

        double baseAndEnergyCharge = calculateCharge(actualDays, returnFuelOrKWh);
        double lateFee = 0.0;

        // Late fee calculation: 50% surcharge per day beyond booked duration
        if (actualDays > bookedDays) {
            int extraDays = actualDays - bookedDays;
            lateFee = extraDays * (dailyRate * 1.50);
            std::cout << "  -> Late return! " << extraDays << " overdue days charged at 1.5x daily rate = Rs. " 
                      << std::fixed << std::setprecision(2) << lateFee << '\n';
        }

        double totalAmount = baseAndEnergyCharge + lateFee;
        std::cout << "[RETURNED] " << getType() << " [" << regNo << "] returned by " << renterName 
                  << " | Actual Days: " << actualDays 
                  << " | Total Bill: Rs. " << std::fixed << std::setprecision(2) << totalAmount << '\n';

        // Reset rental state
        isRented = false;
        renterName = "";
        bookedDays = 0;
        return totalAmount;
    }

    virtual void display() const {
        std::cout << "Reg: " << std::left << std::setw(12) << regNo
                  << " | Type: " << std::setw(14) << getType()
                  << " | Daily: Rs. " << std::right << std::setw(7) << std::fixed << std::setprecision(2) << dailyRate
                  << " | Status: " << (isRented ? "RENTED (" + renterName + ")" : "AVAILABLE");
    }

    const std::string& getRegNo() const { return regNo; }
    bool getIsRented() const { return isRented; }
};

// Derived Class 1: Bicycle (Non-motorized)
class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& reg, double rate)
        : Vehicle(reg, rate) {}

    std::string getType() const override { return "Bicycle"; }

    // Bicycles have no fuel charge
    double calculateCharge(int actualDays, double /*returnFuelOrKWh*/) const override {
        return dailyRate * actualDays;
    }

    void display() const override {
        Vehicle::display();
        std::cout << " | Eco-Friendly (No Fuel)\n";
    }
};

// Intermediate Class: MotorVehicle
class MotorVehicle : public Vehicle {
protected:
    double fuelLevelPercent; // 0.0 to 100.0%
    Date serviceDueDate;     // Member class (Composition)
    static constexpr double FUEL_PRICE_PER_PERCENT = 1.20; // Refill cost per 1% deficit

public:
    MotorVehicle(const std::string& reg, double rate, double fuel, const Date& serviceDate)
        : Vehicle(reg, rate), fuelLevelPercent(fuel), serviceDueDate(serviceDate) {}

    virtual double calculateFuelCharge(double returnedFuelPercent) const {
        if (returnedFuelPercent < 100.0) {
            double deficit = 100.0 - returnedFuelPercent;
            return deficit * FUEL_PRICE_PER_PERCENT;
        }
        return 0.0;
    }

    void display() const override {
        Vehicle::display();
        std::cout << " | Fuel: " << std::right << std::setw(5) << std::fixed << std::setprecision(1) << fuelLevelPercent 
                  << "% | Next Service: " << serviceDueDate.toString();
    }
};

// Derived Class 2: TwoWheeler (Motorcycle / Scooter)
class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(const std::string& reg, double rate, double fuel, const Date& serviceDate)
        : MotorVehicle(reg, rate, fuel, serviceDate) {}

    std::string getType() const override { return "Two-Wheeler"; }

    double calculateCharge(int actualDays, double returnFuelPercent) const override {
        double rental = dailyRate * actualDays;
        double fuelRefill = calculateFuelCharge(returnFuelPercent);
        return rental + fuelRefill;
    }

    void display() const override {
        MotorVehicle::display();
        std::cout << " | Helmet Included\n";
    }
};

// Derived Class 3: Car (Passenger Vehicle)
class Car : public MotorVehicle {
protected:
    int seatingCapacity;

public:
    Car(const std::string& reg, double rate, double fuel, const Date& serviceDate, int seats)
        : MotorVehicle(reg, rate, fuel, serviceDate), seatingCapacity(seats) {}

    std::string getType() const override { return "Car"; }

    double calculateCharge(int actualDays, double returnFuelPercent) const override {
        double rental = dailyRate * actualDays;
        double fuelRefill = calculateFuelCharge(returnFuelPercent);
        // Surcharge of Rs. 150/day if seating capacity exceeds 5
        double seatSurcharge = (seatingCapacity > 5) ? (150.0 * actualDays) : 0.0;
        return rental + fuelRefill + seatSurcharge;
    }

    void display() const override {
        MotorVehicle::display();
        std::cout << " | Seats: " << seatingCapacity << '\n';
    }
};

// Derived Class 4: ElectricCar (Zero Emission Motor Vehicle)
class ElectricCar : public Vehicle {
private:
    double batteryCapacityKWh;
    double currentKWh;
    Date serviceDueDate;
    static constexpr double KWH_RECHARGE_RATE = 12.50; // Rs. 12.50 per kWh recharge

public:
    ElectricCar(const std::string& reg, double rate, double batteryKWh, double curKWh, const Date& serviceDate)
        : Vehicle(reg, rate), batteryCapacityKWh(batteryKWh), currentKWh(curKWh), serviceDueDate(serviceDate) {}

    std::string getType() const override { return "Electric Car"; }

    // Electric cars charge per kWh consumed rather than fuel litres
    double calculateCharge(int actualDays, double returnKWh) const override {
        double rental = dailyRate * actualDays;
        double rechargeFee = 0.0;
        if (returnKWh < batteryCapacityKWh) {
            double kwhDeficit = batteryCapacityKWh - returnKWh;
            rechargeFee = kwhDeficit * KWH_RECHARGE_RATE;
        }
        return rental + rechargeFee;
    }

    void display() const override {
        Vehicle::display();
        std::cout << " | Battery: " << batteryCapacityKWh << " kWh (" 
                  << std::fixed << std::setprecision(1) << currentKWh << " kWh left) | Service: " 
                  << serviceDueDate.toString() << '\n';
    }
};

// ============================================================================
// EXTENSION: Adding a New Vehicle Type WITHOUT Changing Any Existing Classes!
// Demonstrating the Open/Closed Principle (OCP)
// ============================================================================
class ElectricBus : public Vehicle {
private:
    int passengerCapacity;
    double batteryKWh;
    static constexpr double BUS_KWH_RATE = 15.00;
    static constexpr double COMMERCIAL_PERMIT_TAX_PER_DAY = 500.0;

public:
    ElectricBus(const std::string& reg, double rate, int capacity, double battery)
        : Vehicle(reg, rate), passengerCapacity(capacity), batteryKWh(battery) {}

    std::string getType() const override { return "Electric Bus"; }

    double calculateCharge(int actualDays, double returnKWh) const override {
        double baseRent = dailyRate * actualDays;
        double permitTax = COMMERCIAL_PERMIT_TAX_PER_DAY * actualDays;
        double recharge = 0.0;
        if (returnKWh < batteryKWh) {
            recharge = (batteryKWh - returnKWh) * BUS_KWH_RATE;
        }
        return baseRent + permitTax + recharge;
    }

    void display() const override {
        Vehicle::display();
        std::cout << " | Capacity: " << passengerCapacity << " passengers | Pack: " 
                  << batteryKWh << " kWh [Heavy Commercial EV]\n";
    }
};

// Fleet Management System Class
class FleetManager {
private:
    std::vector<std::unique_ptr<Vehicle>> fleet;

public:
    void addVehicle(std::unique_ptr<Vehicle> v) {
        fleet.push_back(std::move(v));
    }

    Vehicle* findVehicle(const std::string& reg) {
        for (const auto& v : fleet) {
            if (v->getRegNo() == reg) return v.get();
        }
        return nullptr;
    }

    void listFleet() const {
        std::cout << "\n--------------------------------------------------------------------------------------------------\n";
        std::cout << "                                  COMPLETE FLEET INVENTORY                                        \n";
        std::cout << "--------------------------------------------------------------------------------------------------\n";
        for (const auto& v : fleet) {
            v->display();
        }
        std::cout << "--------------------------------------------------------------------------------------------------\n";
    }
};

int main() {
    std::cout << "==================================================================================================\n";
    std::cout << "                       SMART VEHICLE RENTAL FLEET MANAGEMENT SYSTEM                               \n";
    std::cout << "==================================================================================================\n";

    FleetManager fleet;

    // Register diverse vehicles
    fleet.addVehicle(std::make_unique<Bicycle>("BIKE-101", 150.0));
    fleet.addVehicle(std::make_unique<TwoWheeler>("TN-09-AB-2024", 450.0, 100.0, Date(15, 6, 2026)));
    fleet.addVehicle(std::make_unique<Car>("TN-01-XY-5555", 2200.0, 100.0, Date(20, 8, 2026), 7)); // 7-seater SUV
    fleet.addVehicle(std::make_unique<ElectricCar>("TN-07-EV-9999", 3000.0, 60.0, 60.0, Date(10, 12, 2026)));

    // STAGE 5 EXTENSION: Adding ElectricBus seamlessly!
    fleet.addVehicle(std::make_unique<ElectricBus>("TN-22-BUS-01", 8000.0, 42, 250.0));

    // 1. Initial Fleet Listing
    std::cout << "\n[PHASE 1] Initial Fleet Status:\n";
    fleet.listFleet();

    // 2. Renting Demonstration
    std::cout << "\n[PHASE 2] Renting Operations:\n";
    Vehicle* bike = fleet.findVehicle("BIKE-101");
    if (bike) bike->rentVehicle("Rajesh Kumar", 3);

    Vehicle* car = fleet.findVehicle("TN-01-XY-5555");
    if (car) car->rentVehicle("Dr. Malathi", 4);

    Vehicle* ev = fleet.findVehicle("TN-07-EV-9999");
    if (ev) ev->rentVehicle("Vikram Seth", 2);

    Vehicle* bus = fleet.findVehicle("TN-22-BUS-01");
    if (bus) bus->rentVehicle("SRM Tours & Travels", 5);

    // Attempting to double rent (Constraint check)
    std::cout << "\n[Constraint Check] Attempting to rent already booked vehicle:\n";
    if (bike) bike->rentVehicle("Unauthorized Caller", 1);

    // 3. Return Operations with Normal and Late Fees
    std::cout << "\n[PHASE 3] Vehicle Return and Billing Calculation:\n";
    
    // Bike returned on time (3 days, 0 fuel)
    if (bike) bike->returnVehicle(3, 0.0);

    // Car returned 2 days LATE (6 actual days vs 4 booked, returned with 70% fuel -> 30% deficit)
    if (car) car->returnVehicle(6, 70.0);

    // EV returned on time (2 days, returned with 35 kWh remaining -> 25 kWh recharge deficit)
    if (ev) ev->returnVehicle(2, 35.0);

    // Electric Bus returned 1 day late (6 actual days, returned with 180 kWh -> 70 kWh recharge deficit)
    if (bus) bus->returnVehicle(6, 180.0);

    // 4. Final Fleet Status
    std::cout << "\n[PHASE 4] Final Fleet Status After Returns:\n";
    fleet.listFleet();

    return 0;
}
