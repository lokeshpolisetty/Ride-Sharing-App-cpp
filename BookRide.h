#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <stdexcept>
#include <thread>
#include <chrono>
#include<regex>
#include "distance.h"
using namespace std;

class Distance {
public:
    string origin,destination;
    string distance();
    string originplace();
    string destinationplace();
};

string Distance::originplace(){

    cout << "Enter your current location (area, city): ";
    getline(cin, origin);

    return origin;
}

string Distance::destinationplace(){

    cout << "Enter your destination location (area, city): ";
    getline(cin, destination);

    return destination;
}
string Distance::distance() {
    string apiKey = "AlzaSyFBMKJ3oMjt6EMSQhzUXfkb3iWsIeiYspW";

    origin=originplace();
    destination=destinationplace();

    string originCoords = getCoordinates(origin, apiKey);
    string destinationCoords = getCoordinates(destination, apiKey);

    if (originCoords.empty() || destinationCoords.empty()) {
        cerr << "Error: Could not retrieve coordinates for one or both locations." << endl;
        return "0"; 
    } else {
        string distance = getDistance(originCoords, destinationCoords, apiKey);
        cout << "Distance: " << distance << endl;
        return distance;
    }
    return "0";
}

class Amount {
private:
    int bike_baseprice = 20;
    int auto_baseprice = 40;
    int car_baseprice = 60;

public:
    template <typename T>
int calculateFare(T distance, int vehicle) {
    int price = 0;

    switch (vehicle) {
        case 1: price = bike_baseprice + distance * 9; break;
        case 2: price = auto_baseprice + distance * 17; break;
        case 3: price = car_baseprice + distance * 26; break;
        default: throw invalid_argument("Invalid vehicle type");
    }

    cout << "Actual Price: ₹" << price << endl;

    double discount = 0.0;
    if (price > 500) {
        discount = 0.25; 
    } else if (price > 250) {
        discount = 0.15; 
    } else if (price > 100) {
        discount = 0.10; 
    }

    if(discount > 0.0) {
        int discountAmount = static_cast<int>(price*discount);
        int discountedPrice = price - discountAmount;

        cout << "You are eligible for a discount of ₹" << discountAmount
             << " (" << discount * 100 << "% off).\n";
        cout << "Do you want to apply the coupon? (y/n): ";

        char userResponse;
        cin >> userResponse;

        if (userResponse == 'y' || userResponse == 'Y') {
            price = discountedPrice;
            cout << "Coupon applied! Total Price after Discount: ₹" << price << endl;
        } else {
            cout << "Coupon not applied. Total Price remains: ₹" << price << endl;
        }
    }else{
        cout << "No offers available for the current price.\n";
    }

    return price;
 }
};

class IRideRequester {
public:
    virtual void requestRide() = 0;
    virtual ~IRideRequester() = default;
};

class IDriverServices {
public:
    virtual void startTrip() = 0;
    virtual void endTrip() = 0;
    virtual ~IDriverServices() = default;
};

class Rider : public IRideRequester {
public:
    void requestRide() override {
        cout << "Rider is requesting a ride.\n";
    }
};

class Driver : public IDriverServices {
private:
    string driverName;

public:
    explicit Driver(const string& name) : driverName(name) {}

    void startTrip() override {
        std::cout << "Driver " << driverName << " has started the trip.\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout <<"Driver has ended the trip.\n";
    }

    void endTrip() override {
        cout << driverName << " has ended the trip.\n";
    }

    string getName() const {
        return driverName;
    }
};

class PaymentMethod {
public:
    virtual void processPayment(int amount) = 0; 
};

class PaymentProcess : public PaymentMethod {
public:
    void processPayment(int amount) override {
        cout << "User pays " << amount << " Rupees in cash to the driver.\n";
        this_thread::sleep_for(std::chrono::seconds(2));
        cout << "User has paid the amount.\n";
        this_thread::sleep_for(std::chrono::seconds(1));
        cout << "Trip completed.\n";
    }

    void processOnlinePayment(int amount) {
        string bankDetails, accountNumber;
        int cvv;

        cout << "Enter Bank Name: ";
        cin >> bankDetails;
        while (true) {
            cout << "Enter Account Number (format: 1234 5678 9012 3456): ";
            cin.ignore();
            getline(cin, accountNumber);

            regex accountNumberPattern("^\\d{4} \\d{4} \\d{4} \\d{4}$");
            if (regex_match(accountNumber, accountNumberPattern)) {
                break;
            } else {
                cout << "Invalid account number. Please enter in the correct format.\n";
            }
        }

        while (true) {
            cout << "Enter CVV (3 digits): ";
            cin >> cvv;

            if (cvv >= 100 && cvv <= 999) {
                break;
            } else {
                cout << "Invalid CVV. Please enter a 3-digit CVV.\n";
            }
        }

        cout << "\nProcessing online payment of " << amount << " Rupees...\n";
        this_thread::sleep_for(chrono::seconds(3));
        cout << "Payment Successful!\n";
        this_thread::sleep_for(chrono::seconds(1));
        cout << "Trip completed.\n";
    }
};

class BookRide : public Amount, public Distance, public PaymentProcess {
private:
    int vehicleChoice;
    string space;
    int paymentChoice;

public:
    void bookRide(const string& Rider);
    void tripDetails(const string& riderName, const string& driverName, const string& origin, const string& destination,int paymentChoice, int price, int distance);
    void saveTripDetails(const string& riderName, const string& driverName, const string& origin, const string& destination, int paymentChoice, int price, int distance);
};

void BookRide::bookRide(const string& Rider) {
    while (true) {
        space = distance(); 

        cout << "Enter vehicle type to start Ride:\n";
        cout << "1. Bike\n2. Auto\n3. Car\n";
        cin >> vehicleChoice;
        cin.ignore();

        int distanceValue = stoi(space);
        int price = calculateFare(distanceValue, vehicleChoice);

        vector<string> drivers;
        ifstream driverList("DriverList.txt");
        if (driverList.is_open()) {
            string line;
            while (getline(driverList, line)) {
                drivers.push_back(line);
            }
            driverList.close();
        } else {
            cout << "Error: Could not open DriverList.txt file!\n";
            return;
        }

        if (drivers.empty()) {
            cout << "No drivers are currently available.\n";
            char tryAgain;
            cout << "Do you want to try booking the ride again? (y/n): ";
            cin >> tryAgain;
            cin.ignore();

            if (tryAgain == 'y' || tryAgain == 'Y') {
                continue;
            } else {
                cout << "Exiting the booking process. Thank you!\n";
                return;
            }
        }

        for (auto& driverName : drivers) {
            char response;
            cout << "Requesting Driver...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            cout << "Driver " << driverName << " is available. Accept ride? (y/n): ";
            cin >> response;
            cin.ignore();

            if (response == 'y' || response == 'Y') {
                cout << "Driver started to your location.\n";
                std::this_thread::sleep_for(std::chrono::seconds(3));
                cout << "Driver reached the location.\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
                cout << "Rider arrived at the location.\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));

                Driver driver(driverName);
                driver.startTrip();

                cout << "\nChoose Payment Method:\n";
                cout << "1. Cash\n";
                cout << "2. Online\n";
                cout << "Enter your choice: ";
                cin >> paymentChoice;
                cin.ignore(); 

                PaymentProcess payment;

                switch (paymentChoice) {
                case 1:
                    payment.processPayment(price);
                    break;
                case 2:
                    payment.processOnlinePayment(price);
                    break;
                default:
                    cout << "Invalid payment method choice. Please try again.\n";
                }

                string Ridername = Rider;

                tripDetails(Ridername, driverName, origin, destination, paymentChoice, price, distanceValue);
                saveTripDetails(Ridername, driverName, origin, destination, paymentChoice, price, distanceValue);
                return;
            } else {
                cout << "Driver " << driverName << " has been rejected. Searching for another driver...\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        cout << "All drivers are currently unavailable.\n";
        char tryAgain;
        cout << "Do you want to try booking the ride again? (y/n): ";
        cin >> tryAgain;
        cin.ignore();

        if (tryAgain == 'y' || tryAgain == 'Y') {
            continue;
        } else {
            cout << "Exiting the booking process. Thank you!\n";
            return;
        }
    }
}

void BookRide::tripDetails(const string& riderName, const string& driverName, const string& origin, const string& destination,int paymentChoice, int price, int distance) {
    string riderFileName = riderName + "(User).txt";
    string driverFileName = driverName + "(Driver).txt";
    
    string paymentMethod = (paymentChoice == 1) ? "Cash" : "Online";

    string tripInfo = "Origin: " + origin + "\nDestination: " + destination + "\nDistance: " + to_string(distance) + " km\nPaymentChoice: "+ paymentMethod+"\nFare: ₹" + to_string(price) + "\n";

    try {
        ofstream riderFile(riderFileName, ios::app);
        if (!riderFile.is_open()) {
            throw runtime_error("Error opening rider's file: " + riderFileName);
        }
        riderFile << "Trip Details:\n" << tripInfo << "--------------------------\n";
        riderFile.close();

        ofstream driverFile(driverFileName, ios::app);
        if (!driverFile.is_open()) {
            throw runtime_error("Error opening driver's file: " + driverFileName);
        }
        driverFile << "Trip Details:\n" << tripInfo << "--------------------------\n";
        driverFile.close();

        cout << "Trip details saved successfully.\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void BookRide::saveTripDetails(const string& riderName, const string& driverName, const string& origin, const string& destination, int paymentChoice, int price, int distance) {

    ofstream tripDetails("trip_details.txt", ios::app);
    if (tripDetails.is_open()) {
        string paymentMethod = (paymentChoice == 1) ? "Cash" : "Online";

        tripDetails << "Rider: " << riderName << ", Driver: " << driverName 
                    << ", Origin: " << origin << ", Destination: " << destination
                    << ", Distance: " << distance << " km, Price: ₹" << price 
                    << ", Payment Method: " << paymentMethod << "\n";

        tripDetails.close();
        cout << "Trip details saved successfully.\n";
    } else {
        cout << "Error: Unable to save trip details.\n";
    }
}