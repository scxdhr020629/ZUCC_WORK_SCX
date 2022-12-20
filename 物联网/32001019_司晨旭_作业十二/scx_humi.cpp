#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DHT.h"


const char *ssid = "scxwifi";
const char *password = "cxcx629326";


const char *mqtt_broker = "121.5.180.148";
const char *topic = "toSCX/1019";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;


#define DHTPIN 5
#define DHTTYPE DHT11   // DHT 11
unsigned long previousMillis = 0;

WiFiClient espClient;
// PubSubClient client(espClient);
// DHT dht(5,DHTTYPE);
PubSubClient client(espClient);
DHT dht(5,DHT11);
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();


  if ((char)payload[0] == '1') {
     digitalWrite(BUILTIN_LED, LOW);


   } else {
    digitalWrite(BUILTIN_LED, HIGH);
  }

}


void setup() {

    Serial.begin(115200);
 
   WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
   }
     Serial.println("Connected to the WiFi network");
  
     client.setServer(mqtt_broker, mqtt_port);
     client.setCallback(callback);
  
   while (!client.connected()) {
     String client_id = "esp8266-client-";
     client_id += String(WiFi.macAddress());
     Serial.println("Connecting to public emqx mqtt broker.....");
       if (client.connect("ESP8266Client")) {
       Serial.println("Public emqx mqtt broker connected");
       } else {
       Serial.print("failed with state ");
       Serial.print(client.state());
       delay(2000);
       }
 }

 dht.begin();
}

void loop() {
   client.loop();
   unsigned long currentMillis = millis();

   if (currentMillis - previousMillis >= 5000) {
     previousMillis = currentMillis;
   	float temp = dht.readTemperature();
 	float humi = dht.readHumidity();

     DynamicJsonDocument data(256);
  
//       data["xuehao"] = "32001019";
//       data["name"] = "SCX";
       data["temp"] = temp;
       data["humi"] = humi;

         char json_string[256];
         serializeJson(data, json_string);

         Serial.println(json_string);
         client.publish(topic, json_string, false);
}
}
