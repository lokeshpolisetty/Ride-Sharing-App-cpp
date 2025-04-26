#include <iostream>
#include <string>
#include <curl/curl.h>        // For HTTP requests
#include "include/json.hpp"   // For JSON parsing
#include <sstream>            // For std::ostringstream

using namespace std;
using json = nlohmann::json;

// Callback function for cURL to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    response->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to URL-encode a string (required for Geocoding API)
std::string urlEncode(const std::string& value) {
    std::ostringstream encoded;
    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int)c;
        }
    }
    return encoded.str();
}

// Function to get coordinates from GoMaps Geocoding API
std::string getCoordinates(const std::string& location, const std::string& apiKey) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // URL-encode the location name
    std::string encodedLocation = urlEncode(location);

    // Initialize cURL
    curl = curl_easy_init();
    if (curl) {
        // Construct the Maps Geocoding API URL
        std::string url = "Mention_Your_API_url" + encodedLocation + "&key=" + apiKey;

        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return "";
        }
        
        // Clean up
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Error: Failed to initialize cURL." << std::endl;
        return "";
    }

    // Parse JSON response to extract latitude and longitude
    try {
        json jsonData = json::parse(readBuffer);
        if (!jsonData["results"].empty()) {
            auto locationData = jsonData["results"][0]["geometry"]["location"];
            std::string coordinates = to_string(locationData["lat"]) + "," + to_string(locationData["lng"]);
            return coordinates;
        } else {
            return "";
        }
    } catch (json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return "";
    }
}

// Function to get distance between two locations from GoMaps Distance Matrix API
std::string getDistance(const std::string& originCoords, const std::string& destinationCoords, const std::string& apiKey) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Initialize cURL
    curl = curl_easy_init();
    if (curl) {
        // Construct the Maps Distance Matrix API URL
        std::string url = "Add_diatance_matrix_url" + originCoords +
                          "&destinations=" + destinationCoords + "&key=" + apiKey;

        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return "Error: API request failed.";
        }
        
        // Clean up
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Error: Failed to initialize cURL." << std::endl;
        return "Error: cURL initialization failed.";
    }

    // Parse JSON response
    try {
        json jsonData = json::parse(readBuffer);
        if (jsonData["rows"][0]["elements"][0]["status"] == "OK") {
            return jsonData["rows"][0]["elements"][0]["distance"]["text"];
        } else {
            return "Error: Unable to find distance.";
        }
    } catch (json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return "Error: JSON parsing failed.";
    }
}
