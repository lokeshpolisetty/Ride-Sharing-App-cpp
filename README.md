# Ride Sharing App (C++)

This project is a simple ride sharing application implemented in C++. It supports:

- **Admin Functionalities:** Managing user and driver details and viewing trip information.
- **User Functionalities:** User/driver signup, login, and ride booking.
- **Ride Booking:** Displays available drivers, calculates fares with discounts, and processes payments (via cash or online).
- **Distance Calculation:** Uses a Maps API (via cURL) combined with the [nlohmann/json](https://github.com/nlohmann/json) library for geocoding and distance matrix operations.
- **File Handling:** Stores credentials, driver lists, and trip details in text files.

## Requirements

- **C++17 or later**
- [cURL](https://curl.se/) library
- [nlohmann/json](https://github.com/nlohmann/json) header-only JSON library

> **Note:** The JSON library is included as a header file (`include/json.hpp`) in this project. If you plan to use or update the library independently, ensure that you download the latest version from its [GitHub repository](https://github.com/nlohmann/json).

## Setup and Installation

1. **Clone or Download the Project:**
   - Ensure that the project directory contains the following key files:
     - `Ride_Booking_App.cpp`
     - `UserAccount.h`
     - `BookRide.h`
     - `distance.h`
     - `include/json.hpp` *(nlohmann/json header-only library)*
     - `path.txt` *(Build instructions and API key configuration)*

2. **Install Dependencies on Mac:**
   - **cURL:** You can install cURL using Homebrew if not already available:
     ```bash
     brew install curl
     ```
   - **JSON Library:** Since the JSON parser is header-only and already included in the repository (under the `include` folder), no separate installation is necessary. However, ensure that the include path is correctly set when compiling.

3. **Build Instructions:**
   - Open the Terminal and navigate to the project directory:
     ```bash
     cd "/Users/Desktop/Ride Sharing App(c++)/"
     ```
   - Compile the project with the following command. The `-I` flag should point to the directory containing `json.hpp` if it is not in your system include paths.
     ```bash
     g++ -std=c++17 Ride_Booking_App.cpp -o Ride_booking_App -I/usr/local/include -L/usr/local/lib -lcurl
     ```
     > **Tip:** If you are using the included version of nlohmann/json, you might update the include path to point to your local `include` folder:
     > ```bash
     > g++ -std=c++17 Ride_Booking_App.cpp -o Ride_booking_App -I"./include" -L/usr/local/lib -lcurl
     > ```

4. **Run the Application:**
   - Execute the compiled binary:
     ```bash
     ./Ride_booking_App
     ```

## API Key and Maps API Setup

- The project uses a Maps API for geolocation services. The API key is defined in the source files (e.g., in `distance.h` and `path.txt`).
- **Configuration:**
  - Update the placeholder URL and API key in your source code if necessary.
  - Ensure that your API key has the appropriate permissions for geocoding and distance matrix requests.

## Project File Structure

- **Ride_Booking_App.cpp:** Main application entry point.
- **UserAccount.h:** Classes for user and admin account management.
- **BookRide.h:** Classes for ride booking logic, payment processing, and fare calculation.
- **distance.h:** Functions using cURL and nlohmann/json to interact with the Maps API.
- **include/json.hpp:** Header-only JSON parser (nlohmann/json).
- **path.txt:** Contains build instructions, API key, and endpoint URLs.

## Additional Notes

- **JSON Library Setup:**  
  The [nlohmann/json](https://github.com/nlohmann/json) library is header-only and requires no additional linking. Simply include `json.hpp` in your project as done in `distance.h`. If you later wish to update the JSON library, replace this header with the latest version from its GitHub repository.
  
- **cURL and API Communication:**  
  The project uses cURL to send HTTP requests to the Maps API. Ensure you have the cURL development package installed and correctly linked during compilation.
  
- **Platform Specifics (Mac):**  
  The provided build instructions and terminal commands are specific to macOS. Adjust paths accordingly for different environments.
