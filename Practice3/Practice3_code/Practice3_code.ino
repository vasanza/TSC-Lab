/*
   ****************************** TSC-Lab *******************************

   ***************************** PRACTICE 3 *****************************
   This practice is about USB DATA ACQUISITION and have 2 different cases:

    • Case 1: Ambient temperature reading using sensor 1 and 2
    • Case 2: Activation of Transistor 1 and Reading of temperature sensor 1 and 2

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
int period=12; //medium period
int freq=100;  // sampling time

void setup() {
  delay(1000);
  Serial.begin(115200);
  sensors1.begin();   //Sensor 1 starts
  sensors2.begin();   //Sensor 2 starts

  //transistor 1
  pinMode(16, OUTPUT);
  //transitor 2
  pinMode(17, OUTPUT);
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
    
  }

}


//method to read data for 12 minute
void readData() {

  uint32_t timer = period * 60000L;      

  for ( uint32_t tStart = millis();  (millis() - tStart) < timer;  ) {
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
    delay(freq);
  }

}
