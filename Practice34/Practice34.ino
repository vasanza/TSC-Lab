/*
   ****************************** TSC-Lab *******************************
   ***************************** PRACTICE 34 *****************************
   This practice is about use of two core esp-32
   By: Kevin E. Chica O
   More information: https://tsc-lab.blogspot.com/
*/
//temperature
#include <OneWire.h>
#include <DallasTemperature.h>
// GPIO pin 0 is set as OneWire bus
OneWire ourWire1(0);
//GPIO pin 4 is set as OneWire bus
OneWire ourWire2(4);

//A variable or object is declared for our sensor 1
DallasTemperature sensors1(&ourWire1);
//A variable or object is declared for our sensor 2
DallasTemperature sensors2(&ourWire2);

//set parameters
int period_temp = 12; //medium period
int freq_temp = 2000; // sampling time



//motor

//initial setting for data acquisition
int dutyCycleInitial = 255;
int dutyCycleFinish = 0;
int period = 13000;
int cycles = 10;




//separador library
#include <Separador.h>
Separador s;




//motor
int motor1Pin1 = 33;
int motor1Pin2 = 25;
int enable1Pin = 32;

int motor_status = 255;


// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;

//move
String move_motor = "counterclockwise";

int encoder = 27;

void motor( void *pvParameters );

void RPM( void *pvParameters );
void readTemperature( void *pvParameters );

volatile int counter = 0;


void interruption()    // Function that runs during each interrupt
{
  counter++;
}

void setup() {
  Serial.begin(115200);
  //begin temperature sensor
  sensors1.begin();   //Sensor 1 starts
  sensors2.begin();   //Sensor 2 starts


  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);



  attachInterrupt(encoder, interruption, RISING);


  xTaskCreatePinnedToCore(
    motor
    ,  "MotorDC"         // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  1                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL              // Variable that points to the task (optional)
    , 1);                // core 1


  xTaskCreatePinnedToCore(
    RPM
    ,  "RPM"              // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  1                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL              // Variable that points to the task (optional)
    , 1);                // core 1

    xTaskCreatePinnedToCore(
    readTemperature
    ,  "rTemp"              // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  1                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL              // Variable that points to the task (optional)
    , 0);                // core 0



}

void loop() {
}

void motor( void *pvParameters ) {
  while (1) {
    if (cycles > 0) {
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      ledcWrite(pwmChannel, dutyCycleInitial);

      motor_status = 255;
      vTaskDelay(period);


      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      ledcWrite(pwmChannel, dutyCycleFinish);

      motor_status = 0;
      vTaskDelay(period);
      cycles--;
    }
  }
}

void RPM( void *pvParameters ) {
  while (1) {

    vTaskDelay(999);
    Serial.print(counter * 60);
    Serial.print(",");
    Serial.println(motor_status);
    counter = 0;

  }
}



void readTemperature( void *pvParameters ) {
  while (1) {

    uint32_t timer = period_temp * 60000L;

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
      Serial.println();
      Serial.print(temp1);
      Serial.print(",");
      Serial.println(temp2);          
      delay(freq_temp);
    }

  }
}
