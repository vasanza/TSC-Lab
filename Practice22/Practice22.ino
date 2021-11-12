/*
   ****************************** TSC-Lab *******************************

   ***************************** PRACTICE 13 *****************************
   This practice is about Wifi connection


   By: Kevin E. Chica O
   Reviewed: Víctor Asanza
   More information: https://tsc-lab.blogspot.com/
   More examples: https://github.com/vasanza/TSC-Lab
   Dataset: http://ieee-dataport.org/4138
*/

#include <WiFi.h>


// WiFi
const char *ssid = "your_ssid"; // Enter your WiFi name
const char *password = "your_password";  // Enter WiFi password



void setup() {

  // Set software serial baud to 115200;
  Serial.begin(115200);
  //wifi
  WiFi.mode(WIFI_STA);

}


void loop() {
  connect_wifi();
  delay(5000);
}


void connect_wifi() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
}
