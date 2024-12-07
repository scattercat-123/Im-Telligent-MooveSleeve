#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "EMGFilters.h"

#define TIMING_DEBUG 1
#define SensorInputPin A0

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "HarvestStudent"
#define WIFI_PASSWORD "StudenT@HIS1"

/* 2. Define the API Key */
#define API_KEY "AIzaSyBsZCSRWnmWPfRN1AB3CRJo8iy1zWBhKtg"

/* 3. Define the RTDB URL */
#define DATABASE_URL "im-telligent-1fcfd-default-rtdb.firebaseio.com" 

/* 4. Define the user Email and password that already registered or added in your project */
#define USER_EMAIL "replyatharv@gmail.com"
#define USER_PASSWORD "Atharv@23"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;
String receivedData = "";  // Variable to store incoming serial data
int gsrValue = 0;          // Variable for GSR value
int emgValue = 0;

void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the API key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign-in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long-running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will be controlled by your code or third-party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

void loop() {
  if (Serial.available()) {
    receivedData = Serial.readStringUntil('\n');  // Read the string until newline
    parseData(receivedData); // Call the parseData function
  }

  if (Firebase.ready()) {
    // Create the timestamp for unique database path
    String timestamp = String(millis());

    // Prepare data paths
    String gsrPath = "/sensors/GSR";  // Constant path for GSR
    String emgPath = "/sensors/EMG";  // Constant path for EMG

    // Upload GSR data
    if (gsrValue > 0) {
      if (Firebase.setInt(fbdo, gsrPath, gsrValue)) {
        Serial.println("GSR data sent to Firebase.");
      } else {
        Serial.print("Failed to send GSR data: ");
        Serial.println(fbdo.errorReason());
      }
    }

    // Upload EMG data
    if (emgValue > 0) {
      if (Firebase.setInt(fbdo, emgPath, emgValue)) {
        Serial.println("EMG data sent to Firebase.");
      } else {
        Serial.print("Failed to send EMG data: ");
        Serial.println(fbdo.errorReason());
      }
    }
  }

  // Update every 500ms (adjust this delay as needed)
  delay(250);  
}


void parseData(String data) {
  // Debug: Check the data received
  // Ensure that the received data contains a comma
  int commaIndex = data.indexOf(',');

  if (commaIndex != -1) {
    // If comma exists, split into GSR and EMG values
    String emgString = data.substring(0, commaIndex);   // Extract GSR part
    String gsrString = data.substring(commaIndex + 1);  // Extract EMG part

    // Trim the extracted strings to remove unwanted spaces
    gsrString.trim();
    emgString.trim();

    // Convert the strings to integers  
    gsrValue = gsrString.toInt();
    emgValue = emgString.toInt();

    // Debug: Print parsed values
    Serial.print("Parsed GSR: ");
    Serial.println(gsrValue);
    Serial.print("Parsed EMG: ");
    Serial.println(emgValue);
  } else {
    // If no comma is found, reset the values
    gsrValue = 0;
    emgValue = 0;
    Serial.println("Invalid data received");
  }
}
