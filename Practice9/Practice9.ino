/*
   ****************************** TSC-Lab *******************************
   ***************************** PRACTICE 9 *****************************
   This practice is about initial setups and tests (ON/OFF) of speed control lab
   By: Kevin E. Chica O
   More information: https://tsc-lab.blogspot.com/
*/
//tasking
TaskHandle_t TaskEnviar_Handler;

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

//encoder
int LED_B0 = 27;

int min_max_delay_ms[] = {500, 1000};
int delay_cnt = 10;// Ancho de banda [2Hz - 1KHz]

int delay_led = 250;
float Valor_RPM = 0;
float Valor_Hz = 0;
int cnt = 1;
float temp = 0;
bool anterior = LOW;
int band = 0;

void motor( void *pvParameters );
void comands( void *pvParameters );
void enviar( void *pvParameters );
void RPM( void *pvParameters );

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

  xTaskCreatePinnedToCore(
    enviar
    ,  "envioIOT"             // Descriptive name of the function (MAX 8 characters)
    ,  2048                   // Size required in STACK memory
    ,  NULL                  // INITIAL parameter to receive (void *)
    ,  1                     // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  &TaskEnviar_Handler   // Variable that points to the task (optional)
    , 0);                     //core 0
  xTaskCreatePinnedToCore(
    RPM
    ,  "RPM"              // Descriptive name of the function (MAX 8 characters)
    ,  2048              // Size required in STACK memory
    ,  NULL              // INITIAL parameter to receive (void *)
    ,  1                 // Priority, priority = 3 (configMAX_PRIORITIES - 1) is the highest, priority = 0 is the lowest.
    ,  NULL              // Variable that points to the task (optional)
    , 0);                // core 0

  vTaskSuspend(TaskEnviar_Handler);

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

void enviar( void *pvParameters ) {
  for (;;) {
    temp = cnt / delay_cnt;
    vTaskDelay(1 / portTICK_PERIOD_MS); //<--------------------------------------------------------------
    Valor_Hz = 2 * 1000 / temp; // Por que usamos dos flancos positivos
    vTaskDelay(1 / portTICK_PERIOD_MS); //<--------------------------------------------------------------
    Valor_RPM = 60 * Valor_Hz; // Calculo para los 60Segundos

    vTaskDelay(1 / portTICK_PERIOD_MS); //<--------------------------------------------------------------
    Serial.println("Valor del cnt es: " + String(cnt ));
    vTaskDelay(1 / portTICK_PERIOD_MS); //<--------------------------------------------------------------
    Serial.println("Valor de frecuencia en Hz es: " + String(Valor_Hz));
    vTaskDelay(1 / portTICK_PERIOD_MS); //<--------------------------------------------------------------
    Serial.println("Valor de las RPM es: " + String(Valor_RPM ));

    band = 0;
    cnt = 0;
    vTaskSuspend(TaskEnviar_Handler);
  }
}

void RPM( void *pvParameters ) {
  while (1) {

    if (anterior == LOW && digitalRead(LED_B0) == HIGH ) {
      band += 1; //cambia cuando existe un flanco positivo
    }
    anterior = digitalRead(LED_B0);

    if (band == 1) { //En el primer flanco positivo
      vTaskDelay(delay_cnt  / portTICK_PERIOD_MS);
      cnt += 1;
    }

    if (band == 2) { //En el segundo flanco positivo
      vTaskResume(TaskEnviar_Handler);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
}
