/*
   ****************************** TSC-Lab *******************************
   ***************************** PRACTICE 11 *****************************
   This practice is about data acquisition with square velocity input
   By: Kevin E. Chica O
   Reviewed: vasanza
   More information: https://tsc-lab.blogspot.com/
*/
//separador library
#include <Separador.h>
Separador s;

//initial setting for data acquisition
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

//Status of transistors
int t1 = 0;
int t2 = 0;
//pins of transistor
int trans1 = 17;
int trans2 = 16;

//set parameters

int dutyCycle1 = 0;
int dutyCycle2 = 0;
int band = 0;

// Setting PWM properties
const int freq = 30000;
const int pwmChannell = 0;
const int pwmChannel2 = 1;
const int resolution = 8;



void transistor( void *pvParameters );
void temperature( void *pvParameters );
void pwm( void *pvParameters );



void setup() {
  Serial.begin(115200);
  // sets the pins as outputs:
  sensors1.begin();   //Sensor 1 starts
  sensors2.begin();   //Sensor 2 starts

  //transistor 1
  pinMode(trans1, OUTPUT);
  //transitor 2
  pinMode(trans2, OUTPUT);;

  // configure LED PWM functionalitites
  ledcSetup(pwmChannell, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(trans1, pwmChannell);
  ledcAttachPin(trans2, pwmChannel2);

  xTaskCreatePinnedToCore(
    transistor
    ,  "trans"         // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  1                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL              // Variable that points to the task (optional)
    , 1);                // core 1

  xTaskCreatePinnedToCore(
    temperature
    ,  "temp"              // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  1                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL              // Variable that points to the task (optional)
    , 1);                // core 0

  xTaskCreatePinnedToCore(
    pwm
    ,  "PWM"              // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  1                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL              // Variable that points to the task (optional)
    , 1);                // core 0
}

void loop() {
}

void transistor( void *pvParameters ) {
  while (1) {

    ledcWrite(pwmChannell, dutyCycle1);
    ledcWrite(pwmChannel2, dutyCycle2);
    //vTaskDelay(period);
  }
}

// Calcula las RPM quye tiene el motor
void temperature( void *pvParameters ) {
  while (1) {
    vTaskDelay(999);
    //Serial.println(counter * 60);//255 -> 0x32 0x35 0x35
    sensors1.requestTemperatures();
    //Obtain the temperature in ºC of sensor 1
    int temp1 = sensors1.getTempCByIndex(0);

    //The command is sent to read the temperature
    sensors2.requestTemperatures();
    //Obtain the temperature in ºC of sensor 2
    int temp2 = sensors2.getTempCByIndex(0);
    //Serial.write((byte*)&temp1,4);//value temeprature 1
    //Serial.print("la banda es: ");//value temperature 1
    //Serial.println(temp1);//value temperature 1
    if (band == 0) {
      //Serial.println("t1");
      Serial.write(temp1);//value temperature 1
      
    } else if (band == 1) {
      //Serial.println("t2");
      Serial.write(temp2);//value temperature 2
    } else if (band == 2) {
      int tempProm = (temp1 + temp2) / 2;
      //Serial.println("t1t2");
      Serial.write(tempProm);//value temperature 2
      tempProm = 0;


    }
  }
}

// Read del PWM que viene desde Matlab
void pwm( void *pvParameters ) {
  while (1) {
    //Serial.println("hola");
    if (Serial.available())
    {
      /*Cases:
         t1_pwm
         t2_pwm
         t1t2_pwm1_pwm2
      */
      String string = Serial.readStringUntil('\n');
      String nameTrans = s.separa(string, '_', 0);
      int dutyCycle = s.separa(string, '_', 1).toInt();
      //dutyCycle = string.toInt();
      if (nameTrans == "t1") {
        band = 0;
        dutyCycle1 = dutyCycle;
        dutyCycle2 = 0;
      } else if (nameTrans == "t2") {
        band = 1;
        dutyCycle2 = dutyCycle;
        dutyCycle1 = 0;
      } else if (nameTrans == "t1t2") {
        band = 2;
        dutyCycle1 = dutyCycle;
        dutyCycle2 = s.separa(string, '_', 2).toInt();
      }


    }
  }
}
