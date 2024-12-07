String receivedData = "";  // Variable to store incoming serial data
int gsrValue = 0;          // Variable for GSR value
int emgValue = 0;          // Variable for EMG value


void setup() {
  Serial.begin(9600);  // Initialize Serial communication
}

void loop() {
  if (Serial.available()) {
    receivedData = Serial.readStringUntil('\n');  // Read the string until newline
    parseData(receivedData);                      // Parse the received string
  }

  // Debugging output to Serial Monitor
  Serial.print("GSR: ");
  Serial.print(gsrValue > 0 ? gsrValue : 0);  // Ensure valid values are printed
  Serial.print(" | EMG: ");
  Serial.println(emgValue > 0 ? emgValue : 0);

}

void parseData(String data) {
  // Check if the received data contains a comma
  int commaIndex = data.indexOf(',');

  if (commaIndex != -1) {
    // If comma exists, split into GSR and EMG values
    String gsrString = data.substring(0, commaIndex);   // Extract GSR part
    String emgString = data.substring(commaIndex + 1);  // Extract EMG part

    // Trim the extracted strings
    gsrString.trim();
    emgString.trim();

    // Convert the strings to integers
    gsrValue = gsrString.toInt();
    emgValue = emgString.toInt();
  } else {
    // If no comma, reset the values (invalid data received)
    gsrValue = 0;
    emgValue = 0;
  }
}
