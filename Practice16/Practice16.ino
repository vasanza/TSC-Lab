#include <WiFi.h>
#include <PubSubClient.h>


// WiFi
const char *ssid = "your_ssid"; // Enter your WiFi name
const char *password = "your_password";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "147.182.236.61";
const char *topic = "topicPrincipal";
const char *mqtt_username = "ideTSer";
const char *mqtt_password = "secReTex4@m#";
const int mqtt_port = 1883;



WiFiClient espClient;
PubSubClient client(espClient);



void setup() {
  
  // Set software serial baud to 115200;
  Serial.begin(115200);  
  //wifi
  WiFi.mode(WIFI_STA);  


}



void loop() {
  connect_wifi();
  connect_mqtt();  
  publicSubscribe(200);
  client.loop();
  delay(5000);
}



void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void publicSubscribe(int n) {
  // publish and subscribe
  
  
  client.publish(topic, "Mensaje enviado al servidor del TSC-Lab");
  client.subscribe(topic);
}



void connect_wifi(){
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
}

void connect_mqtt(){
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the  mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
