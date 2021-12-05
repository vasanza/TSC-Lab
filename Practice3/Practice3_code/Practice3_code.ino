/*
   ****************************** TSC-Lab *******************************
   ***************************** PRACTICE 3 *****************************
   This practice is about USB DATA ACQUISITION and have 4 different cases:
    • Case 1: Ambient temperature reading using sensor 1 and 2
    • Case 2: Activation of Transistor 1 and Reading of temperature sensor 1 and 2
    
   By: Kevin E. Chica O
   More information: https://tsc-lab.blogspot.com/
*/

#include <OneWire.h>
#include <DallasTemperature.h>

//GPIO pin 4 is set as OneWire bus
OneWire ourWire1(4);
//GPIO pin 0 is set as OneWire bus
OneWire ourWire2(0);

//A variable or object is declared for our sensor 1
DallasTemperature sensors1(&ourWire1);
//A variable or object is declared for our sensor 2
DallasTemperature sensors2(&ourWire2);


//pins of transistor
int trans1 = 17;
int trans2 = 16;

//set parameters
int period = 15; //medium period in minutes
int freq_sampling = 100; // sampling time
int ciclos = 20; // sampling time

int dutyCycle1 = 0;
int dutyCycle2 = 0;

// Setting PWM properties
const int freq = 30000;
const int pwmChannell = 0;
const int pwmChannel2 = 1;
const int resolution = 8;

void setup() {
  delay(1000);
  Serial.begin(115200);
  
  sensors1.begin();   //Sensor 1 starts
  sensors2.begin();   //Sensor 2 starts

  //transistor 1
  pinMode(trans1, OUTPUT);
  //transitor 2
  pinMode(trans2, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannell, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(trans1, pwmChannell);
  ledcAttachPin(trans2, pwmChannel2);
  Serial.println("Choose any case: ");
}

void loop() {

  if (Serial.available())
  {
    String string = Serial.readStringUntil('\n');

    if (string == "case_1") {
      Serial.println("Case 1 started");
      for (int i = 1; i <= ciclos; i++) {
        //transistor 1 deactivate
        dutyCycle1 = 0;
        ledcWrite(pwmChannell, dutyCycle1);
        //transistor 2 deactivate
        dutyCycle2 = 0;
        ledcWrite(pwmChannel2, dutyCycle2);
        readData();
        readData();
      }
      Serial.println("Case 1 finished");
      Serial.println("Choose any case: ");

    }
    else if (string == "case_2") {
      Serial.println("Case 2 started");

      for (int i = 1; i <= ciclos; i++) {
        //transistor 1 activate
        dutyCycle1 = 255;
        ledcWrite(pwmChannell, dutyCycle1);
        readData();
        //transistor 1 deactivate
        dutyCycle1 = 0;
        ledcWrite(pwmChannel2, dutyCycle2);
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
    Serial.print(dutyCycle1);
    Serial.print(",");
    Serial.println(dutyCycle2);
    delay(freq_sampling);
  }

}
