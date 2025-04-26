#include <iostream>
#include "BookRide.h"
#include "UserAccount.h"
using namespace std;

void adminMenu(Admin& admin) {
    admin.open();
    admin.check();
}

void userMenu(UserAccount& userAccount, BookRide& ride) {
    while (true) {
        int userChoice;
        cout << "\nUser Menu:\n";
        cout << "1. Signup User\n";
        cout << "2. Login User\n";
        cout << "3. Signup Driver\n";
        cout << "4. Login Driver\n";
        cout <<"5.Forgot Password User\n";
        cout<<"6.Forgot Password Driver\n";
        cout << "7. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> userChoice;

        switch (userChoice) {
            case 1:
                userAccount.signup("User");
                break;
            case 2: {
                if (userAccount.login("User")) {
                    cout << "Login successful.\n";
                    string Rider = userAccount.validate("User");
                    ride.bookRide(Rider);
                }
                break;
            }
            case 3:
                userAccount.signup("Driver");
                break;
            case 4:
                if(userAccount.login("Driver")){
                   cout<<"Your online.\n";
                }
                break;
            case 5:
                userAccount.forgotPassword("User");
                break;
            case 6:
                userAccount.forgotPassword("Driver");
            case 7:
                return; 
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main() {
    UserAccount userAccount;
    Admin admin;
    BookRide ride;

    while (true) {
        
        int choice;
        cout << "\nMain Menu:\n";
        cout << "1. Admin\n";
        cout << "2. User\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                adminMenu(admin);
                break;
            case 2:
                userMenu(userAccount, ride);
                break;
            case 3:
                cout << "Exiting the program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}