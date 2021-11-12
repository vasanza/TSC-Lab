/*
   ****************************** TSC-Lab *******************************

   ***************************** PRACTICE 14 *****************************
   This practice is about  ThingSpeak (Http) connection

   By: Kevin E. Chica O
   Reviewed: Víctor Asanza
   More information: https://tsc-lab.blogspot.com/
   More examples: https://github.com/vasanza/TSC-Lab
   Dataset: http://ieee-dataport.org/4138
*/

#include <OneWire.h>
#include <DallasTemperature.h>

//GPIO pin 0clie is set as OneWire bus
OneWire ourWire1(0);
//GPIO pin 4 is set as OneWire bus
OneWire ourWire2(4);

//A variable or object is declared for our sensor 1
DallasTemperature sensors1(&ourWire1);
//A variable or object is declared for our sensor 2
DallasTemperature sensors2(&ourWire2);

//Status of transistors
int t1 = 0;
int t2 = 0;

//set parameters
int period=12; //medium period
int freq=20000;  // sampling time

// WiFi
#include <WiFi.h>
WiFiClient  client;
const char *ssid = "Chica Orellana"; // Enter your WiFi name
const char *password = "ManchasyBombon1998";  // Enter WiFi password

#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

unsigned long myChannelNumber = 1523638;    //your ID
const char * myWriteAPIKey = "AQ1OY0GGI3F0HCHO"; // your API-KEY


void setup() {
  delay(1000);
  Serial.begin(115200);
  sensors1.begin();   //Sensor 1 starts
  sensors2.begin();   //Sensor 2 starts

  //transistor 1
  pinMode(16, OUTPUT);
  //transitor 2
  pinMode(17, OUTPUT);
  

  //wifi
  WiFi.mode(WIFI_STA); 
  connect_wifi();
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  Serial.println("Choose any case: ");
}

void loop() {
  if (Serial.available())
  {
    String string = Serial.readStringUntil('\n');

    if (string == "case_1") {
      Serial.println("Case 1 started");
      for (int i = 1; i <= 10; i++) {
        //transistor 1 deactivate
        t1 = 0;
        //transistor 2 deactivate
        t2 = 0;
        readData();
        readData();
      }
      Serial.println("Case 1 finished");
      Serial.println("Choose any case: ");

    }
    else if (string == "case_2") {
      Serial.println("Case 2 started");

      for (int i = 1; i <= 10; i++) {
        //transistor 1 activate
        digitalWrite(16, HIGH);
        t1 = 1;
        readData();
        //transistor 1 deactivate
        t1 = 0;
        digitalWrite(16, LOW);
        readData();
      }
      Serial.println("Case 2 finished");
      Serial.println("Choose any case: ");
    }
    else if (string == "case_3") {
      Serial.println("Case 3 started");

      for (int i = 1; i <= 10; i++) {
        //transistor 2 activate
        digitalWrite(17, HIGH);
        t2 = 1;
        readData();
        //transistor 2 deactivate
        t2 = 0;
        digitalWrite(17, LOW);
        readData();
      }
      Serial.println("Case 3 finished");
      Serial.println("Choose any case: ");
    }
    else if (string == "case_4") {
      Serial.println("Case 4 started");

      for (int i = 1; i <= 10; i++) {
        //transistor 1 activate
        digitalWrite(16, HIGH);
        t1 = 1;
        //transistor 2 activate
        digitalWrite(17, HIGH);
        t2 = 1;
        readData();
        //transistor 1 deactivate
        t1 = 0;
        digitalWrite(16, LOW);
        //transistor 2 deactivate
        t2 = 0;
        digitalWrite(17, LOW);
        readData();
      }
      Serial.println("Case 4 finished");
      Serial.println("Choose any case: ");
    }

  }

}


//method to read data for 12 minute
void readData() {

  uint32_t timer = period * 60000L;      

  for ( uint32_t tStart = millis();  (millis() - tStart) < timer;  ) {
    connect_wifi();
    //The command is sent to read the temperature
    sensors1.requestTemperatures();
    //Obtain the temperature in ºC of sensor 1
    float temp1 = sensors1.getTempCByIndex(0);

    //The command is sent to read the temperature
    sensors2.requestTemperatures();
    //Obtain the temperature in ºC of sensor 2
    float temp2 = sensors2.getTempCByIndex(0);

    //print to display the temperature change
    Serial.print(temp1);
    Serial.print(",");
    Serial.print(temp2);
    Serial.print(",");
    Serial.print(t1);
    Serial.print(",");
    Serial.println(t2);
    public_ThingSpeak(temp1,temp2,t1,t2);
    delay(freq);
  }

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


void public_ThingSpeak(float temp1,float temp2, int trans1, int trans2){
  // set the fields with the values
  ThingSpeak.setField(1, (temp1));
  ThingSpeak.setField(2, (temp2));
  ThingSpeak.setField(3, (trans1));
  ThingSpeak.setField(4, (trans2));
  

 
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  
  
  //delay(20000); // Wait 20 seconds to update the channel again
  
}
