/*
   ****************************** TSC-Lab *******************************
   ***************************** PRACTICE 6 *****************************
   This practice is about initial setups and tests (ON/OFF) of speed control lab
   By: Kevin E. Chica O
   More information: https://tsc-lab.blogspot.com/
*/

//motor
int motor1Pin1 = 33;
int motor1Pin2 = 25;

void motor( void *pvParameters );


void setup() {
  Serial.begin(115200);
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);


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

    // Move the DC motor forward at maximum speed
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    vTaskDelay(4000);

    // Stop the DC motor
    Serial.println("Motor stopped");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    vTaskDelay(4000);

    // Move the DC motor backwards at maximum speed
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    vTaskDelay(4000);

    // Stop the DC motor
    Serial.println("Motor stopped");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    vTaskDelay(4000);
  }
  
}
