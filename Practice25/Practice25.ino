#include <OneWire.h>
#include <DallasTemperature.h>

//GPIO pin 0 is set as OneWire bus
OneWire ourWire1(0);
//GPIO pin 4 is set as OneWire bus
OneWire ourWire2(4);

//A variable or object is declared for our sensor 1
DallasTemperature sensors1(&ourWire1);
//A variable or object is declared for our sensor 2
DallasTemperature sensors2(&ourWire2);



#include <WiFi.h>
#include <HTTPClient.h>


// WiFi
const char *ssid = "Chica Orellana"; // Enter your WiFi name
const char *password = "ManchasyBombon1998";  // Enter WiFi password

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.0.101:1880/update-sensor";




String temperatura1 = "";
String temperatura2 = "";
float temp1;
float temp2;




void setup() {
  //sensors
  sensors1.begin();   //Sensor 1 starts
  sensors2.begin();   //Sensor 2 starts

  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  
  //wifi
  WiFi.mode(WIFI_STA); 
  


}



void loop() {
  connect_wifi();  
  readData();
  publicData();
  delay(1000);
}







void readData() {

  //The command is sent to read the temperature
  sensors1.requestTemperatures();
  //Obtain the temperature in ºC of sensor 1
  temp1 = sensors1.getTempCByIndex(0);

  temperatura1 = String(temp1);

  //The command is sent to read the temperature
  sensors2.requestTemperatures();
  //Obtain the temperature in ºC of sensor 2
  temp2 = sensors2.getTempCByIndex(0);
  temperatura2 = String(temp2);
  //print to display the temperature change
  

}




void connect_wifi(){
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
}

void publicData(){
  WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=DS18B20&value1="+temperatura1+"&value2="+temperatura2;           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
}
