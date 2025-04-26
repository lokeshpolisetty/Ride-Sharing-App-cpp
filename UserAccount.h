#include <iostream>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include<cstdio>

using namespace std;

class TemporaryFileManager {
private:
    string fileName;

public:
    explicit TemporaryFileManager(const string& file) : fileName(file) {}

    ~TemporaryFileManager() {
        if (remove(fileName.c_str()) == 0) {
            cout << "Temporary file '" << fileName << "' deleted successfully.\n";
        } else {
            cout << "Error deleting temporary file '" << fileName << "'.\n";
        }
    }

    const string& getFileName() const {
        return fileName;
    }
};

TemporaryFileManager driverListManager("DriverList.txt");

class UserAccount {
protected:
    string name,username, password, confirmPassword;

public:
    void signup(const string& accountType);
    bool login(const string& accountType);
    void forgotPassword(const string& accountType);
    void saveCredentials(const string& accountType);
    string validate(const string& accountType){
        if(accountType=="User"){
            name=username;
            return name;
        }
        return "";
    }
    void updateDriverList(const string& driverName);
};

class Admin {
private:
    string username = "Admin";
    string password = "Ride";

public:
    void open();
    bool app();
    void check();
};

string getPasswordInput() {
    string input;
    char ch;

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true) {
        ch = getchar();

        if (ch == 10) {
            break;
        } else if (ch == 127) {
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
            }
        } else {
            input += ch;
            cout << "\rEnter password: ";
            for (size_t i = 0; i < input.length() - 1; ++i) {
                cout << '*';
            }
            cout << input.back();
            usleep(200000);

            cout << "\rEnter password: ";
            for (size_t i = 0; i < input.length(); ++i) {
                cout << '*';
            }
        }
    }
    cout << endl;

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return input;
} 

void UserAccount::signup(const string& accountType) {
    cout << "Enter username: ";
    cin >> username;
    cin.ignore();
    cout << "Enter password: ";
    password = getPasswordInput();
    cout << "Confirm password: ";
    confirmPassword = getPasswordInput();

    if (password == confirmPassword) {
        saveCredentials(accountType);
        cout << accountType << " signup successful.\n";
    } else {
        cout << "Passwords do not match! Signup failed.\n";
    }
}

bool UserAccount::login(const string& accountType) {
    cout << "Enter username: ";
    cin >> username;
    cin.ignore();
    cout << "Enter password: ";
    password = getPasswordInput();

    string fileName = username + "(" + accountType + ").txt";
    ifstream file(fileName);

    try {
        if (!file.is_open()) {
            throw runtime_error("No account found with the given username.");
        }

        string storedUsername, storedPassword;
        file >> storedUsername >> storedPassword;
        file.close();

        if (storedUsername == username && storedPassword == password) {
            cout << accountType << " login successful.\n";

            if (accountType == "Driver") {
                updateDriverList(username); 
            }

            return true;
        } else {
            cout << "Invalid username or password. Login unsuccessful.\n";
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    return false;
}

void UserAccount::forgotPassword(const string& accountType) {
    cout << "Enter your username: ";
    string user;
    cin >> user;

    string fileName = user + "(" + accountType + ").txt";
    ifstream file(fileName);

    try {
        if (!file.is_open()) {
            throw runtime_error("No account found with that username.");
        }

        string storedUsername, storedPassword;
        file >> storedUsername >> storedPassword;
        file.close();
        cout << "Your password is: " << storedPassword << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Admin::open() {
    ofstream file(username + ".txt");
    if (!file.is_open()) {
        cout << "Error saving credentials.\n";
        return;
    }
    file << username << '\n' << password << '\n';
    file.close();
}

bool Admin::app() {
    string admin, adminpassword;
    cout << "Enter the admin username: ";
    cin >> admin;
    cin.ignore();
    adminpassword=getPasswordInput();

    if (username == admin && password == adminpassword) {
        cout << "Your login is successful.\n";
        return true;
    } else {
        cout << "Incorrect username or password.\n";
        return false;
    }
}

void UserAccount::updateDriverList(const string& driverName) {
    try {
        ofstream driverList("DriverList.txt", ios::app);
        if (!driverList.is_open()) {
            throw runtime_error("Error saving driver to DriverList.");
        }
        driverList << driverName << endl;
        driverList.close();
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void UserAccount::saveCredentials(const string& accountType) {
    string fileName = username + "(" + accountType + ").txt";
    ofstream file(fileName);

    try {
        if (!file.is_open()) {
            throw runtime_error("Error saving credentials.");
        }
        file << username << endl << password << endl;
        file.close();
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Admin::check() {
    if (!app()) return;

    while (true) {
        int typeChoice;
        cout << "Enter\n1. to search for UserDetails\n2. to search for DriverDetails\n3. to view trip details: ";
        cin >> typeChoice;

        if (typeChoice < 1 || typeChoice > 3) {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }

        if (typeChoice == 1 || typeChoice == 2) {
            string accountType = (typeChoice == 1) ? "User" : "Driver";
            string usernameInput;
            cout << "Enter username to search in file: ";
            cin >> usernameInput;

            string fileName = usernameInput + "(" + accountType + ").txt";
            ifstream file(fileName);

            if (file.is_open()) {
                cout << "\nFile content for " << accountType << " '" << usernameInput << "':\n";
                string line;
                while (getline(file, line)) {
                    cout << line << endl;
                }
                file.close();
            } else {
                cout << "Error: Could not open file for " << accountType << " '" << usernameInput << "'.\n";
            }
        } else if (typeChoice == 3) {
            ifstream tripDetails("trip_details.txt");
            if (tripDetails.is_open()) {
                cout << "\nContents of trip details file:\n";
                string line;
                while (getline(tripDetails, line)) {
                    cout << line << endl;
                }
                tripDetails.close();
            } else {
                cout << "Error: Could not open 'trip_details.txt' file.\n";
            }
        }

        cout << "Do you want to perform another search? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'n' || choice == 'N') break;
    }
}