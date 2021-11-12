#include <WiFi.h>

void setup(){
  Serial.begin(115200);  
  Serial.print("Dirección Mac del ESP32:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}
