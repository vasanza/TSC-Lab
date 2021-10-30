/*
   ****************************** TSC-Lab *******************************

   ***************************** PRACTICE 17_B *****************************
   This practice is about MQTT Server (MYDATA-LAB)

   By: Kevin E. Chica O
   Reviewed: Víctor Asanza
   More information: https://tsc-lab.blogspot.com/
   More examples: https://github.com/vasanza/TSC-Lab
   Dataset: http://ieee-dataport.org/4138
*/



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

//Status of transistors
int t1 = 0;
int t2 = 0;

//set parameters
int period = 12; //medium period
int freq = 5000; // sampling time


//include librarys for date and time
#include <NTPClient.h>
#include <WiFiUdp.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate = "";
String dayStamp = "";
String timeStamp = "";

#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_MAX_PACKET_SIZE 256;


// WiFi
const char *ssid = "Chica Orellana"; // Enter your WiFi name
const char *password = "ManchasyBombon1998";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "147.182.236.61";
const char *topic = "topicPrincipal";
const char *mqtt_username = "ideTSer";
const char *mqtt_password = "secReTex4@m#";
const int mqtt_port = 1883;

// variables
String mac = "24:62:AB:FC:A3:E8";


String temperatura1 = "";
String temperatura2 = "";
String transistor1 = "";
String transistor2 = "";
float temp1;
float temp2;

WiFiClient espClient;
PubSubClient client(espClient);


#include <ArduinoJson.h>
char buffer[330] = "";
StaticJsonDocument<330> doc;

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
  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone
  timeClient.setTimeOffset(-18000);
  connect_wifi();
  connect_mqtt();
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
    connect_mqtt();
    //The command is sent to read the temperature
    sensors1.requestTemperatures();
    //Obtain the temperature in ºC of sensor 1
    float temp1 = sensors1.getTempCByIndex(0);
    temperatura1 = String(temp1);
    //The command is sent to read the temperature
    sensors2.requestTemperatures();
    //Obtain the temperature in ºC of sensor 2
    float temp2 = sensors2.getTempCByIndex(0);
    temperatura2 = String(temp2);
    //print to display the temperature change
    /*Serial.print(temp1);
      Serial.print(",");
      Serial.print(temp2);
      Serial.print(",");
      Serial.print(t1);
      Serial.print(",");
      Serial.println(t2);*/
    transistor1 = String(t1);
    transistor2 = String(t2);
    date_time();
    SerializeObject();
    publicSubscribe(330);
    client.loop();
    delay(freq);
    //delay(15000);
  }

}

void SerializeObject()
{


  doc["modelo"] = "TSC-Lab";

  //temperature 1
  doc["sensores"][0]["nombre"] = "temperatura1" ;
  doc["sensores"][0]["valor"] = temperatura1 ;
  doc["sensores"][0]["unidadMedicion"] = "C" ;

  //temperature 2
  doc["sensores"][1]["nombre"] = "temperatura2" ;
  doc["sensores"][1]["valor"] = temperatura2 ;
  doc["sensores"][1]["unidadMedicion"] = "C" ;

  //transistor 1
  doc["sensores"][2]["nombre"] = "transistor1" ;
  doc["sensores"][2]["valor"] = transistor1 ;
  doc["sensores"][2]["unidadMedicion"] = "" ;

  //transistor 2
  doc["sensores"][3]["nombre"] = "transistor2" ;
  doc["sensores"][3]["valor"] = transistor2 ;
  doc["sensores"][3]["unidadMedicion"] = "" ;

  //doc.clear();
  doc.garbageCollect();
  serializeJson(doc, buffer);

}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void publicSubscribe(int n) {
  // publish and subscribe

  Serial.println("Publicando...");
  char kevin[n];
  String todo = mac + "#" + dayStamp + "#" + timeStamp + "#" + buffer;
  //char prueba[n]=mac + "#" + dayStamp + "#" + timeStamp + "#" + buffer;
  todo.toCharArray(kevin, n);  
  Serial.println(sizeof(kevin));
  client.publish(topic, kevin);
  client.subscribe(topic);
}


void date_time() {
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);

  // Extract time
  timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);

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

void connect_mqtt() {
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  client.setBufferSize(512);
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the  mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
