/*
   ****************************** TSC-Lab *******************************
   ***************************** PRACTICE 7 *****************************
   This practice is about speed control using PWM
   By: Kevin E. Chica O
   Reviewed: Víctor Asanza
   More information: https://tsc-lab.blogspot.com/
   More examples: https://github.com/vasanza/TSC-Lab
   Dataset: http://ieee-dataport.org/4138
*/

//motor
int motor1Pin1 = 33;
int motor1Pin2 = 25;
int enable1Pin = 32;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

void motor( void *pvParameters );


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


  xTaskCreate(
    motor
    ,  "MotorDC"         // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  0                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL );           // Variable that points to the task (optional)
}

void loop() {
}

void motor( void *pvParameters ) {
  while (1) {
    // Move DC motor forward with increasing speed
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    while (dutyCycle <= 255) {
      ledcWrite(pwmChannel, dutyCycle);
      Serial.print("Forward with duty cycle: ");
      Serial.println(dutyCycle);
      dutyCycle = dutyCycle + 5;
      vTaskDelay(500);
    }
    // Stop the DC motor
    Serial.println("Motor stopped");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    vTaskDelay(4000);
    dutyCycle = 200;

     // Move DC motor backwards with increasing speed
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    while (dutyCycle <= 255) {
      ledcWrite(pwmChannel, dutyCycle);
      Serial.print("Forward with duty cycle: ");
      Serial.println(dutyCycle);
      dutyCycle = dutyCycle + 5;
      vTaskDelay(500);
    }
    // Stop the DC motor
    Serial.println("Motor stopped");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    vTaskDelay(4000);
    dutyCycle = 200;
    
  }

}
