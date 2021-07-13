/*
   ****************************** TSC-Lab *******************************
   ***************************** PRACTICE 8 *****************************
   This practice is about motor direction control and Speed control
   By: Kevin E. Chica O
   Reviewed: Víctor Asanza
   More information: https://tsc-lab.blogspot.com/
   More examples: https://github.com/vasanza/TSC-Lab
   Dataset: http://ieee-dataport.org/4138
*/


//separador library
#include <Separador.h>
Separador s;

//motor
int motor1Pin1 = 33;
int motor1Pin2 = 25;
int enable1Pin = 32;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 0;

//move
String move_motor = "counterclockwise";

void motor( void *pvParameters );
void comands( void *pvParameters );


void setup() {
  Serial.begin(115200);
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);

  xTaskCreatePinnedToCore(
    motor
    ,  "MotorDC"         // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  1                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL              // Variable that points to the task (optional)
    , 1);                // core 1


  xTaskCreatePinnedToCore(
     comands
    ,  "Comands"         // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  1                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL              // Variable that points to the task (optional)
    , 1);                //core 1



}

void loop() {
}

void motor( void *pvParameters ) {
  while (1) {
    if (move_motor == "clockwise") {
      // Move the DC motor forward at maximum speed
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
      ledcWrite(pwmChannel, dutyCycle);
    } else if (move_motor == "counterclockwise") {
      // Stop the DC motor
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      ledcWrite(pwmChannel, dutyCycle);
    }

  }

}



void comands(void *pvParameters) {
  while (1) {
    if (Serial.available()) {
      String string = Serial.readStringUntil('\n');
      String pwm = s.separa(string, '_', 0);
      if (string == "clockwise") {
        Serial.println("Motor move on clockwise");
        move_motor = "clockwise";
      } else if (string == "counterclockwise") {
        Serial.println("Motor move on counterclockwise");
        move_motor = "counterclockwise";
      }
      else if (pwm == "pwm") {
        dutyCycle = s.separa(string, '_', 1).toInt();

      }

    }
  }

}
