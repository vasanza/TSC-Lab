/*
   ****************************** TSC-Lab *******************************
   ***************************** PRACTICE 1 *****************************
   By: Kevin E. Chica O
   Reviewed: Víctor Asanza
   More information: https://tsc-lab.blogspot.com/
   More examples: https://github.com/vasanza/TSC-Lab
   Dataset: http://ieee-dataport.org/4138
*/

#include <OneWire.h>
#include <DallasTemperature.h>

//GPIO pin 4 is set as OneWire bus
OneWire ourWire1(4);
//GPIO pin 0 is set as OneWire bus
OneWire ourWire2(0);
//pins of transistor
int trans1 = 17;
int trans2 = 16;

//A variable or object is declared for our sensor 1
DallasTemperature sensors1(&ourWire1);
//A variable or object is declared for our sensor 2
DallasTemperature sensors2(&ourWire2);

void setup() {
  delay(1000);
  Serial.begin(115200);
  sensors1.begin();   //Sensor 1 starts
  sensors2.begin();   //Sensor 2 starts

  //transistor
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
}

void loop() {

  if (Serial.available())
  {
    String string = Serial.readStringUntil('\n');
    if (string == "t1_on") {
      digitalWrite(trans1, HIGH);
      Serial.println("Transistor 1 on");
    }
    if (string == "t1_off") {
      digitalWrite(trans1, LOW);
      Serial.println("Transistor 1 off");
    }
    if (string == "t2_on") {
      digitalWrite(trans2, HIGH);
      Serial.println("Transistor 2 on");
    }
    if (string == "t2_off") {
      digitalWrite(trans2, LOW);
      Serial.println("Transistor 2 off");
    }

  }

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
  Serial.println(temp2);
  delay(100);
}
