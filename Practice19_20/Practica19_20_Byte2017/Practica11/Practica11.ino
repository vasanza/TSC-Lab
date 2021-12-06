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
//pins of transistor
int trans1 = 17;
int trans2 = 16;

//set parameters

int dutyCycle1 = 0;
int dutyCycle2 = 0;
int tempProm =0;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
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
  ledcSetup(pwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(trans1, pwmChannel);
  ledcAttachPin(trans2, pwmChannel);

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

    ledcWrite(pwmChannel, dutyCycle1);
    ledcWrite(pwmChannel, dutyCycle2);
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

    //t1_pwm
    Serial.write(temp1);//value temperature 1

    //t2_pwm
    //Serial.write(temp2);//value temperature 2
    
    //t1t2_pwm1_pwm2
    //tempProm = (temp1 + temp2)/2;
    //Serial.write(tempProm);//value temperature 2

  }
}

// Read del PWM que viene desde Matlab
void pwm( void *pvParameters ) {
  while (1) {
    //Serial.println("hola");

       //Serial.println("hola");
    if (Serial.available())
    {
       dutyCycle1 =Serial.read();//Version 2017: t1_pwm
       //dutyCycle2 =Serial.read();//Version 2017: t2_pwm
    }
   } 
}
